// process.cpp
#include "processRSS.h"
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// Platform detection
#if defined(_WIN64)
#define USE_WINDOWS_API 1
#elif defined(__APPLE__)
#define OS_APPLE 1
#elif defined(__linux__)
#define USE_PROC_MEM 1
#else
#error "Unsupported platform for process memory utility"
#endif
#if OS_APPLE // use mach function
#include <mach/mach_init.h>
#include <mach/task.h>
#include <mach/task_info.h> // For TASK_VM_INFO
#elif USE_PROC_MEM
#include <unistd.h>
#elif USE_WINDOWS_API
#include <psapi.h>
#include <windows.h>
#endif



using namespace std;
namespace process {
/*****************************************************************************
 * Internal helper to fetch both current and peak RSS in bytes with a single query.
 *****************************************************************************/
static MemRSS FetchMemoryStats() {
#ifdef USE_PROC_MEM
  {
    unsigned long rss_kb = 0;
    unsigned long hwm_kb = 0;
    bool found_rss = false;
    bool found_hwm = false;
    FILE *file = fopen("/proc/self/status", "r");
    if (!file) {
      std::cerr << "Linux detected but /proc not available. Will report 0." << std::endl;
      return {0, 0};
    }
    char line[128];
    while (fgets(line, sizeof(line), file)) {
      if (sscanf(line, "VmRSS: %lu", &rss_kb) == 1) {
        found_rss = true;
      } else if (sscanf(line, "VmHWM: %lu", &hwm_kb) == 1) {
        found_hwm = true;
      }
      if (found_rss && found_hwm) {
        break;
      }
    }
    (void)fclose(file);
    if (!found_rss || !found_hwm) {
      return {0, 0};
    }
    return {rss_kb * 1024, hwm_kb * 1024}; // Convert kB to bytes
  }
#elif OS_APPLE
  // Use Mach functions.
  task_vm_info_data_t vmInfo;
  mach_msg_type_number_t count = TASK_VM_INFO_COUNT;
  if (task_info(mach_task_self(), TASK_VM_INFO, (task_info_t)&vmInfo, &count) == KERN_SUCCESS) {
    return {vmInfo.resident_size, vmInfo.resident_size_peak};
  } else {
    std::cerr << "Apple OS detected but kernel call failed. Will report 0." << std::endl;
    return {0, 0};
  }
#elif USE_WINDOWS_API
  PROCESS_MEMORY_COUNTERS pmc;
  if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
    return {pmc.WorkingSetSize, pmc.PeakWorkingSetSize};
  }
  return {0, 0};
#else
  std::cerr << "Unsupported OS. Will report 0." << std::endl;
  return {0, 0};
#endif
}


/**
 * Returns current and peak resident set size (RSS) in bytes.
 * RSS is the portion of memory occupied by a process that is held in main memory (RAM).
 * This is process-wide RSS (not per-thread or heap-only).
 * The measurement is platform-specific and may return {0,0} on unsupported systems or errors.
 */
MemRSS getResidentMemory() {
  static std::atomic<bool> memAvailable(true);
  if (memAvailable) {
    MemRSS stats = FetchMemoryStats();
    if (stats.current == 0 && stats.peak == 0) {
      memAvailable = false;
    }
    return stats;
  }
  return {0, 0};
}

/*****************************************************************************
 * Helper to convert bytes to human-readable string (e.g., "34.3 MB").
 * Avoids fractions <1 by choosing appropriate unit.
 *****************************************************************************/
static string humanReadable(size_t bytes) {
  if (bytes == 0) {
    return "0 B";
  }
  if (bytes < 1024) {
    return to_string(bytes) + " B";
  }
  double val = static_cast<double>(bytes) / 1024.0;
  string unit = "KB";
  if (val >= 1024.0) {
    val /= 1024.0;
    unit = "MB";
  }
  if (val >= 1024.0) {
    val /= 1024.0;
    unit = "GB";
  }
  stringstream ss;
  int precision = (val < 10.0) ? 2 : (val < 100.0) ? 1 : 0;
  ss << fixed << setprecision(precision) << val;
  string str = ss.str();
  // Strip trailing .0 or .00
  size_t dot_pos = str.find('.');
  if (dot_pos != string::npos) {
    size_t trailing_zero_pos = str.find_last_not_of('0');
    if (trailing_zero_pos == dot_pos) {
      str = str.substr(0, dot_pos);
    } else if (trailing_zero_pos < str.size() - 1) {
      str = str.substr(0, trailing_zero_pos + 1);
    }
  }
  return str + " " + unit;
}

std::ostream &operator<<(std::ostream &os, const MemRSS &m) {
  os << "Resident: " << humanReadable(m.current) << ", Peak: " << humanReadable(m.peak);
  return os;
}
} // namespace process