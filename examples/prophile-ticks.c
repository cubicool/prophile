#include "prophile.h"

#include <stdio.h>

void tick_test(prophile_unit_t u) {
	prophile_tick_t start = prophile_tick(u);

	// Do some work here; we'll just sleep.
	prophile_sleep(100000);

	prophile_tick_t stop = prophile_tick(u);

	// At least you didn't have to worry about QueryPerformanceTimer or gettimeofday. :)
	printf("start = %fms\n", start);
	printf("stop  = %fms\n", stop);
	printf("stop - start = %fms\n", stop - start);
}

int main(int argc, char** argv) {
	printf("PROPHILE_NSEC\n"); tick_test(PROPHILE_NSEC);
	printf("PROPHILE_USEC\n"); tick_test(PROPHILE_USEC);
	printf("PROPHILE_MSEC\n"); tick_test(PROPHILE_MSEC);
	printf("PROPHILE_SEC\n"); tick_test(PROPHILE_SEC);

	return 0;
}
