#include "thread_timer.h"

std::atomic<size_t> pr::thread_timer::total_cpu_time_ms{0};