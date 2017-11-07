#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <time.h>

float starttime;
struct timespec ts;

#define timer_start()	\
{\
	clock_gettime(CLOCK_MONOTONIC, &ts);\
	starttime = (float)ts.tv_sec + (float)ts.tv_nsec / 1000000000.0f;\
}
#define timer_stop(A)\
{\
	clock_gettime(CLOCK_MONOTONIC, &ts);\
	float stoptime = (float)ts.tv_sec + (float)ts.tv_nsec / 1000000000.0f;\
	A = stoptime - starttime;\
}

#endif

