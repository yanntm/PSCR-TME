#include <cstddef> // for size_t
#include <iostream>
#include <atomic>
#ifdef _WIN32
#include <windows.h>
#elif defined(__unix__) || defined(__APPLE__) || defined(__linux__)
#include <time.h>
#else
#error "Unsupported platform for thread_timer"
#endif

namespace pr {

/**
 * A simple cross-platform utility to profile thread CPU *usage*.
 * It does not measure wall-clock time, but CPU time (user + system) consumed by the thread.
 * Unfortunately, this is not part of standard C++, so we honor platform-specific implementations:
 * For POSIX (Linux, macOS, etc.), it uses timespec and clock_gettime with CLOCK_THREAD_CPUTIME_ID
 * to measure per-thread user + system CPU time.
 * For Windows, it uses GetThreadTimes to fetch kernel (system) and user mode times, summing them.
 * This allows RAII-style timing in multithreaded code without relying on C++11 chrono.  
 * Use it stack-allocated per thread; it's not thread-safe for sharing.  
 * Note: Returns 0 on internal errors (e.g., GetThreadTimes or clock_gettime failure)
 * 
 * Profiling aggregation: This class maintains a global atomic counter of total CPU time measured
 * across all timer instances. Each call to getElapsedms contributes its elapsed time to the global total.
 * Use reset() to restart timing for loops or repeated measurements. 
 * Do not nest timers in the same thread for this metric to be accurate.
 * Example usage:
 * \code
 * void someThreadFunction() {
 *   thread_timer timer;
 *   // ... do some work ...
 *   std::cout << "Elapsed CPU time: " << timer.getElapsedms() << " ms\n";
 * }
 * \endcode
 * Typically in main, you can report the total CPU time across all threads at the end:
 * \code
 *   std::cout << "Total CPU time: " << thread_timer::getTotalCpuTimeMs() << " ms\n";
 * \endcode
 * */
class thread_timer {
private:
  static std::atomic<size_t> total_cpu_time_ms;
#ifdef _WIN32
  FILETIME kernel_start;
  FILETIME user_start;
#else
  timespec start_time;
#endif
public:
  static size_t getTotalCpuTimeMs() {
    return total_cpu_time_ms.load(std::memory_order_relaxed);
  }
  void reset() {
#ifdef _WIN32
    FILETIME creation, exit_time;
    if (!GetThreadTimes(GetCurrentThread(), &creation, &exit_time, &kernel_start, &user_start)) {
      // Zero out on failure
      kernel_start = {0};
      user_start = {0};
    }
#else
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_time) != 0) {
      // Zero out on failure
      start_time = {0, 0};
    }
#endif
  }
  thread_timer() { reset(); }
  /**
   * Returns the CPU time (user + system) in milliseconds for the current thread
   * since the creation of this timer instance (or last reset).
   * This object should be stack-allocated by the owning thread and not shared across threads.
   * Returns 0 on error (e.g., GetThreadTimes/clock_gettime failure) or if truly idle.
   * 
   * Note: This method contributes the elapsed time to the global total on each call.
   */
  size_t getElapsedms() const {
#ifdef _WIN32
    FILETIME creation, exit_time, kernel_end, user_end;
    if (!GetThreadTimes(GetCurrentThread(), &creation, &exit_time, &kernel_end, &user_end)) {
      return 0; // Failure
    }
    ULARGE_INTEGER k_start, u_start, k_end, u_end;
    k_start.LowPart = kernel_start.dwLowDateTime;
    k_start.HighPart = kernel_start.dwHighDateTime;
    u_start.LowPart = user_start.dwLowDateTime;
    u_start.HighPart = user_start.dwHighDateTime;
    k_end.LowPart = kernel_end.dwLowDateTime;
    k_end.HighPart = kernel_end.dwHighDateTime;
    u_end.LowPart = user_end.dwLowDateTime;
    u_end.HighPart = user_end.dwHighDateTime;
    ULONGLONG total_100ns =
        (k_end.QuadPart - k_start.QuadPart) + (u_end.QuadPart - u_start.QuadPart);
    size_t elapsed = static_cast<size_t>(total_100ns / 10000); // 100-ns to ms
#else
    timespec end_time;
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end_time) != 0) {
      return 0; // Failure
    }
    long long secs = end_time.tv_sec - start_time.tv_sec;
    long long nsecs = end_time.tv_nsec - start_time.tv_nsec;
    if (nsecs < 0) {
      secs -= 1;
      nsecs += 1000000000LL;
    }
    long long total_nsecs = secs * 1000000000LL + nsecs;
    size_t elapsed = static_cast<size_t>(total_nsecs / 1000000); // ns to ms
#endif
    total_cpu_time_ms.fetch_add(elapsed, std::memory_order_relaxed);
    return elapsed;
  }
  friend std::ostream &operator<<(std::ostream &os, const thread_timer &t) {
    os << t.getElapsedms();
    return os;
  }
};

} // namespace pr