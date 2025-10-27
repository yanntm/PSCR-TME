#include "util/rsleep.h"
#include "util/mtrand.h"
#include <time.h>

void pr::randsleep() {
  struct timespec tosleep;
  tosleep.tv_sec = 0;
  // 300 millions de ns = 0.3 secondes
  // 1 milliard de ns = 1 seconde
  tosleep.tv_nsec = pr::mtrand(300000000, 1000000000);
  struct timespec remain;
  while (nanosleep(&tosleep, &remain) != 0) {
    tosleep = remain;
  }
}
