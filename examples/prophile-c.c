// vimrun! ./prophile-c

#include "prophile.h"

#include <stdint.h>
#include <stdio.h>

#if 0
	PROPHILE_NULL,
	PROPHILE_UNIT,
	PROPHILE_CALLBACK,
	PROPHILE_START,
	PROPHILE_STOP,
	PROPHILE_DATA,
	PROPHILE_STATUS
#endif

static void main_callback(prophile_t pro) {
	printf(
		"main_callback {\n"
		" - unit: %s\n"
		" - callback: %p\n"
		" - start: %f\n"
		" - stop: %f\n"
		" - duration: %f\n"
		" - data: '%s'\n"
		" - status: %s\n"
		"}\n",
		prophile_unit(prophile_get(pro, PROPHILE_UNIT).unit),
		prophile_get(pro, PROPHILE_CALLBACK).callback,
		prophile_get(pro, PROPHILE_START).tick,
		prophile_get(pro, PROPHILE_STOP).tick,
		prophile_get(pro, PROPHILE_DURATION).tick,
		(const char*)(prophile_get(pro, PROPHILE_DATA).data),
		prophile_opt(prophile_get(pro, PROPHILE_STATUS).opt)
	);

#if 0
	const char* d = prophile_get(pro, PROPHILE_DATA).data;

	if(prophile_get(pro, PROPHILE_STATUS).opt == PROPHILE_START) printf(">> cb/START [%s]\n", d);

	else printf(
		">> cb/STOP  [%s]: %f\n",
		d,
		prophile_get(pro, PROPHILE_STOP).tick - prophile_get(pro, PROPHILE_START).tick
	);
#endif
}

#if 1
static void sleep_for(unsigned int usec) {
	printf("sleep = %u =========================================\n", usec);

	prophile_tick_t start = prophile_tick(PROPHILE_USEC);

	prophile_sleep(PROPHILE_USEC, usec);

	prophile_tick_t stop = prophile_tick(PROPHILE_USEC);

	printf("start    = %f\n", start);
	printf("stop     = %f\n", stop);
	printf("duration = %f\n", stop - start);
}
#endif

int main(int argc, char** argv) {
	sleep_for(300000);
	sleep_for(600000);

#if 0
	for(uint32_t i = 0; i < 2; i++) {
		sleep_for(500000); {
			sleep_for(100000);
			sleep_for(100000);
			sleep_for(100000);
		}
	}
#endif

#if 1
	prophile_t pro = prophile_create(
		PROPHILE_UNIT, PROPHILE_USEC,
		PROPHILE_CALLBACK, main_callback,
		NULL
	);

#if 0
	printf("prophile_start/stop 300000 =================\n");
	prophile_start(pro, "prophile_sleep(300000)");
	prophile_sleep(PROPHILE_USEC, 300000);
	printf("RETURNED: %f\n", prophile_stop(pro));

	printf("prophile_start/stop 600000 ================\n");
	prophile_start(pro, "prophile_sleep(600000)");
	prophile_sleep(PROPHILE_USEC, 600000);
	printf("RETURNED: %f\n", prophile_stop(pro));
#endif

#if 0
	prophile_start(pro, "prophile_sleep(100000)"); {
		prophile_start(pro, "prophile_sleep(50000)"); {
			prophile_sleep(PROPHILE_USEC, 50000);

			printf("RETURNED: %f\n", prophile_stop(pro));
		}

		printf("RETURNED: %f\n", prophile_stop(pro));
	}
#endif

#if 1
	for(uint32_t i = 0; i < 3; i++) {
		prophile_start(pro, "prophile_sleep(100000)");
		prophile_sleep(PROPHILE_USEC, 100000);

		for(uint32_t ii = 0; ii < 2; ii++) {
			prophile_start(pro, "prophile_sleep(50000)");
			prophile_sleep(PROPHILE_USEC, 50000);

			printf("delta: %fus\n", prophile_stop(pro));
		}

		printf("delta: %fus\n", prophile_stop(pro));
	}
#endif

	prophile_destroy(pro);
#endif

	prophile_tick_t rdtsc0 = prophile_tick_rdtsc();

	for(uint32_t i = 0; i < 10; i++) {
		prophile_tick_t rdtsc1 = prophile_tick_rdtsc();

		printf("rdtsc %u: %f\n", i, rdtsc1 - rdtsc0);

		rdtsc0 = rdtsc1;
	}

	return 0;
}
