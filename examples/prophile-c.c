// vimrun! ./prophile-c

#include "prophile.h"

#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

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
		" - start: %" PRIu64 "\n"
		" - stop: %" PRIu64 "\n"
		" - duration: %" PRIu64 "\n"
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

	printf("start    = %" PRIu64 "\n", start);
	printf("stop     = %" PRIu64 "\n", stop);
	printf("duration = %" PRIu64 "\n", stop - start);
}
#endif

int main0(int argc, char** argv) {
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

			printf("delta: %" PRIu64 "us\n", prophile_stop(pro));
		}

		printf("delta: %" PRIu64 "us\n", prophile_stop(pro));
	}
#endif

	prophile_destroy(pro);
#endif

	prophile_tick_t rdtsc0 = prophile_tick_rdtsc();

	for(uint32_t i = 0; i < 10; i++) {
		prophile_tick_t rdtsc1 = prophile_tick_rdtsc();

		printf("rdtsc %u: %" PRIu64 "\n", i, rdtsc1 - rdtsc0);

		rdtsc0 = rdtsc1;
	}

	return 0;
}

int main1(int argc, char** argv) {
	prophile_tick_t start = prophile_tick(PROPHILE_NSEC);

	prophile_sleep(PROPHILE_NSEC, 2300000000);

	prophile_tick_t stop = prophile_tick(PROPHILE_NSEC);

	printf("Slept for: %" PRIu64 "ns\n", stop - start);

	return 0;
}

#include <time.h>
#include <errno.h>
#include <string.h>

int main2(int argc, char** argv) {
#ifdef _MSC_VER
	return 0;

#else
	uint64_t ns = 1500000000;

	struct timespec ts = {
		.tv_sec = ns / 1000000000,
		.tv_nsec = ns % 1000000000
	};

	printf("ns       = %" PRIu64 "\n", ns);
	printf("tv_sec   = %" PRIu64 "\n", ns / 1000000000);
	printf("tv_nsec  = %" PRIu64 "\n", ns % 1000000000);

	// prophile_tick_t start = prophile_tick_rdtsc();
	prophile_tick_t start = prophile_tick(PROPHILE_NSEC);

	if(nanosleep(&ts, NULL)) {
		printf("Error (%d): %s\n", errno, strerror(errno));

		return 1;
	}

	// prophile_tick_t stop = prophile_tick_rdtsc();
	prophile_tick_t stop = prophile_tick(PROPHILE_NSEC);

	printf("start    = %" PRIu64 "\n", start);
	printf("stop     = %" PRIu64 "\n", stop);
	printf("duration = %" PRIu64 "\n", stop - start);

	return 0;
#endif
}

int main(int argc, char** argv) {
	return main1(argc, argv);
}
