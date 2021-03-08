#include "prophile.h"

#include <stdio.h>

int main(int argc, char** argv) {
	prophile_tick_t start = prophile_tick(PROPHILE_USEC);

	// Do some work here; we'll just sleep.
	prophile_sleep(300000);

	prophile_tick_t stop = prophile_tick(PROPHILE_USEC);

	// At least you didn't have to worry about QueryPerformanceTimer or gettimeofday. :)
	printf("Slept for %fms\n", stop - start);

	return 0;
}
