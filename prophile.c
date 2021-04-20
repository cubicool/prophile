// Some inspiration for this code came from the following:
//
// http://www.songho.ca/misc/timer/timer.html
// https://gist.github.com/ngryman/6482577
//
// TODO: Investigate using the following instead:
//
// https://raw.githubusercontent.com/nothings/stb/master/stb_ds.h

#include "prophile.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

#ifdef _MSC_VER
#include <windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif

typedef struct _prophile_timer_t* prophile_timer_t;

struct _prophile_timer_t {
	prophile_tick_t start;
	prophile_tick_t stop;
	prophile_timer_t next;

	const void* data;
};

struct _prophile_t {
	prophile_unit_t unit;
	prophile_callback_t callback;
	prophile_timer_t timer;
};

prophile_t prophile_create(prophile_opt_t opt, ...) {
	prophile_t pro = (prophile_t)(calloc(1, sizeof(struct _prophile_t)));
	prophile_opt_t o = opt;

	va_list args;

	va_start(args, opt);

	while(o) {
		if(o == PROPHILE_UNIT) {
			pro->unit = va_arg(args, prophile_unit_t);
		}

		else if(o == PROPHILE_CALLBACK) {
			pro->callback = va_arg(args, prophile_callback_t);
		}

		// A bad option was given, so just stop trying and go with whatever has
		// already been set (if anything).
		else break;

		o = va_arg(args, prophile_opt_t);
	}

	va_end(args);

	return pro;
}

void prophile_destroy(prophile_t pro) {
	if(!pro) return;

	free(pro);
}

void prophile_start(prophile_t pro, const void* data) {
	prophile_timer_t t = (prophile_timer_t)(calloc(1, sizeof(struct _prophile_timer_t)));

	t->next = pro->timer;
	t->data = data;

	pro->timer = t;

	// Set the start immediately before calling the callback.
	t->start = prophile_tick(pro->unit);

	if(pro->callback) pro->callback(pro);
}

prophile_tick_t prophile_stop(prophile_t pro) {
	if(!pro || !pro->timer) return 0;

	pro->timer->stop = prophile_tick(pro->unit);

	if(pro->callback) pro->callback(pro);

	prophile_timer_t t = pro->timer;
	prophile_tick_t r = t->stop - t->start;

	pro->timer = pro->timer->next;

	free(t);

	return r;
}

prophile_val_t prophile_get(const prophile_t pro, prophile_opt_t o) {
	prophile_val_t v = { .data = NULL };

	if(o == PROPHILE_UNIT) v.unit = pro->unit;

	else if(o == PROPHILE_CALLBACK) v.callback = pro->callback;

	else if(o == PROPHILE_START && pro->timer) v.tick = pro->timer->start;

	else if(o == PROPHILE_STOP && pro->timer) v.tick = pro->timer->stop;

	else if(o == PROPHILE_DURATION && pro->timer && pro->timer->stop) {
		v.tick = pro->timer->stop - pro->timer->start;
	}

	else if(o == PROPHILE_DATA && pro->timer) v.data = pro->timer->data;

	else if(o == PROPHILE_STATUS) {
		v.opt = PROPHILE_NULL;

		if(pro->timer && !pro->timer->stop) v.opt = PROPHILE_START;

		else v.opt = PROPHILE_STOP;
	}

	return v;
}

void prophile_set(prophile_t pro, prophile_opt_t opt, prophile_val_t val) {
}

prophile_tick_t prophile_tick_rdtsc() {
#if defined _WIN32 || defined __CYGWIN__
	return int64_t(__rdtsc());

#elif defined __i386 || defined _M_IX86
	uint32_t eax;
	uint32_t edx;

	asm volatile("rdtsc" : "=a" (eax), "=d" (edx));

	return (uint64_t(edx) << 32) + uint64_t(eax);

#elif defined __x86_64__ || defined _M_X64
	uint64_t rax;
	uint64_t rdx;

	asm volatile("rdtsc" : "=a" (rax), "=d" (rdx));

	return (int64_t)((rdx << 32) + rax);

#else
	#error "prophile_tick_rdtsc undefined for this platform"
#endif
}

prophile_tick_t prophile_tick(prophile_unit_t u) {
	prophile_tick_t t = 0;

#ifdef _MSC_VER
	static LARGE_INTEGER FREQ = { .QuadPart = 0 };

	LARGE_INTEGER tick = { .QuadPart = 0 };

	// Set the Frequency only once; it doesn't change.
	if(!FREQ.QuadPart) QueryPerformanceFrequency(&FREQ);

	QueryPerformanceCounter(&tick);

	if(u == PROPHILE_NSEC) t = tick.QuadPart * (1.0e9 / (prophile_tick_t)(FREQ.QuadPart));

	else if(u == PROPHILE_USEC) t = tick.QuadPart * (1.0e6 / (prophile_tick_t)(FREQ.QuadPart));

	else if(u == PROPHILE_MSEC) t = tick.QuadPart * (1.0e3 / (prophile_tick_t)(FREQ.QuadPart));

	else if(u == PROPHILE_SEC) t = tick.QuadPart * (1.0 / (prophile_tick_t)(FREQ.QuadPart));

#else
	// #if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0) && defined(_POSIX_MONOTONIC_CLOCK)

	/* // This is the implementation using gettimeofday().
	struct timeval tick = {0, 0};

	gettimeofday(&tick, NULL);

	if(u == PROPHILE_NSEC) {
		t = tick.tv_sec * 1000000000.0;
		t += tick.tv_usec * 1000.0;
	}

	else if(u == PROPHILE_USEC) {
		t = tick.tv_sec * 1000000.0;
		t += tick.tv_usec;
	}

	else if(u == PROPHILE_MSEC) {
		t = tick.tv_sec * 1000.0;
		t += tick.tv_usec / 1000.0;
	}

	else if(u == PROPHILE_SEC) {
		t = tick.tv_sec;
		t += tick.tv_usec / 1000000.0;
	} */

	// This is the implementation using clock_gettime().
	struct timespec tick = {0, 0};

	clock_gettime(CLOCK_MONOTONIC, &tick);

	if(u == PROPHILE_NSEC) t = (tick.tv_sec * 1000000000.0) + tick.tv_nsec;

	else if(u == PROPHILE_USEC) t = (tick.tv_sec * 1000000.0) + (tick.tv_nsec / 1000.0);

	else if(u == PROPHILE_MSEC) t = (tick.tv_sec * 1000.0) + (tick.tv_nsec / 1000000.0);

	else if(u == PROPHILE_SEC) t = tick.tv_sec + (tick.tv_nsec / 1000000000.0);
#endif

	return t;
}

// https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-setwaitabletimer
void prophile_sleep(prophile_unit_t unit, prophile_tick_t usec) {
#ifdef _MSC_VER
	// The "due" units are 100ns apiece; the negative value indicates a relative future offset time,
	// rather than an absolute (positive) time.
	LARGE_INTEGER due = { .QuadPart = -(10 * (__int64)(usec)) };
	HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);

	SetWaitableTimer(timer, &due, 0, NULL, NULL, FALSE);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);

#else
	// TODO: Convert to using nanosleep() instead, as usleep() was rendered
	// obsolete in POSIX-1.2001.
	usleep(usec);
#endif
}

const char* prophile_opt(prophile_opt_t o) {
	if(o == PROPHILE_UNIT) return "PROPHILE_UNIT";

	else if(o == PROPHILE_CALLBACK) return "PROPHILE_CALLBACK";

	else if(o == PROPHILE_START) return "PROPHILE_START";

	else if(o == PROPHILE_STOP) return "PROPHILE_STOP";

	else if(o == PROPHILE_DURATION) return "PROPHILE_DURATION";

	else if(o == PROPHILE_DATA) return "PROPHILE_DATA";

	else if(o == PROPHILE_STATUS) return "PROPHILE_STATUS";

	else return "PROPHILE_NULL";
}

const char* prophile_unit(prophile_unit_t u) {
	if(u == PROPHILE_NSEC) return "PROPHILE_NSEC";

	else if(u == PROPHILE_USEC) return "PROPHILE_USEC";

	else if(u == PROPHILE_MSEC) return "PROPHILE_MSEC";

	else return "PROPHILE_SEC";
}
