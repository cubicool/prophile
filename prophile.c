// Some inspiration for this code came from the following:
//
// http://www.songho.ca/misc/timer/timer.html
// https://gist.github.com/ngryman/6482577

#include "prophile.h"

#include <stdlib.h>
#include <stdarg.h>

#ifdef _MSC_VER
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

typedef struct _prophile_sample_t* prophile_sample_t;

// TODO: As long as this struct matches the layout of the prophile_callback_data_t, it might
// be possible to simply copy the necessary bytes, and add the prophile_callback_mode_t, etc
// as needed?
struct _prophile_sample_t {
	prophile_tick_t start;
	prophile_tick_t stop;
	const void* data;
	prophile_sample_t next;
};

struct _prophile_t {
	prophile_unit_t units;
	prophile_callback_t callback;
	prophile_sample_t samples;

	// We keep the structure here, attached the prophile_t context, so that each thread can
	// have its own copy without worrying about funky static behavior.
	prophile_callback_data_t cb_data;
};

prophile_t prophile_create(prophile_opt_t opt, ...) {
	prophile_t pro = (prophile_t)(calloc(1, sizeof(struct _prophile_t)));
	prophile_opt_t o = opt;

	va_list args;

	va_start(args, opt);

	while(o) {
		if(o == PROPHILE_UNITS) {
			pro->units = va_arg(args, prophile_unit_t);
		}

		else if(o == PROPHILE_CALLBACK) {
			pro->callback = va_arg(args, prophile_callback_t);
		}

		else {
			// TODO: Handle error better...

			break;
		}

		o = va_arg(args, prophile_opt_t);
	}

	va_end(args);

	return pro;
}

void prophile_destroy(prophile_t pro) {
	if(!pro) return;

	free(pro);
}

// Private helper function for invoking the prophile_callback_t, if any.
static void _prophile_callback(prophile_t pro, prophile_sample_t s, prophile_callback_mode_t m) {
	if(pro->callback) {
		pro->cb_data.mode = m;
		pro->cb_data.start = s->start;
		pro->cb_data.stop = s->stop;
		pro->cb_data.data = s->data;

		pro->callback(&pro->cb_data);

		// TODO: Should pro->cb_data be reset to a 0-state?
	}
}

void prophile_start(prophile_t pro, const void* data) {
	prophile_sample_t s = (prophile_sample_t)(calloc(1, sizeof(struct _prophile_sample_t)));

	s->start = prophile_tick(pro->units);
	s->data = data;
	s->next = pro->samples;

	_prophile_callback(pro, s, PROPHILE_START);

	pro->samples = s;
}

prophile_tick_t prophile_stop(prophile_t pro) {
	prophile_sample_t s = pro->samples;
	pro->samples = pro->samples->next;

	s->stop = prophile_tick(pro->units);

	prophile_tick_t r = s->stop - s->start;

	_prophile_callback(pro, s, PROPHILE_STOP);

	free(s);

	return r;
}

// Sleep for specified number of microseconds (1/1000000).
// TODO: Make this accept prophile_unit_t!
void prophile_sleep(unsigned int usec) {
#ifdef _MSC_VER

	LARGE_INTEGER due = { .QuadPart = -(10 * (__int64)(usec)) };
	HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);

	SetWaitableTimer(timer, &due, 0, NULL, NULL, FALSE);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);

#else

	usleep(usec);

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

	if(u == PROPHILE_NSEC) {
		t = tick.QuadPart * (1.0e9 / (prophile_tick_t)(FREQ.QuadPart));
	}

	else if(u == PROPHILE_USEC) {
		t = tick.QuadPart * (1.0e6 / (prophile_tick_t)(FREQ.QuadPart));
	}

	else if(u == PROPHILE_MSEC) {
		t = tick.QuadPart * (1.0e3 / (prophile_tick_t)(FREQ.QuadPart));
	}

	else if(u == PROPHILE_SEC) {
		t = tick.QuadPart * (1.0 / (prophile_tick_t)(FREQ.QuadPart));
	}

	else {
		// TODO: Handle errors better...
	}

#else

	struct timeval tick = {0, 0};

	gettimeofday(&tick, NULL);

	if(u == PROPHILE_NSEC) {
		t = tick.tv_sec * 1000000000.0;
		t += tick.tv_usec * 1000.0;
	}

	else if(u == PROPHILE_USEC) {
		t = tick.tv_sec * 1000.0;
		t += tick.tv_usec;
	}

	else if(u == PROPHILE_MSEC) {
		t = tick.tv_sec * 1000.0;
		t += tick.tv_usec / 1000.0;
	}

	else if(u == PROPHILE_SEC) {
		t = tick.tv_sec;
		t += tick.tv_usec / 1000000.0;
	}

	else {
		// TODO: Handle errors better...
	}

#endif

	return t;
}
