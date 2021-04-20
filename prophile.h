// Prophile (cubicool@gmail.com)

// TODO: Define SCOPE (dos and donts)
// TODO: Should there be a PAUSE?
// TODO: Throttle? (Try to force FPS)
// TODO: tick_as(unit_t)
// TODO: get/set; way to return entire sample? get mode? USE UNION RETURN
// TODO: Introduce some kind of mutex (and example) to show prophile_t thread support
// TODO: prophile_value(pro); this means we need to keep LAST diff before stop() deletes it
// TODO: prophile_start_n/stop_n macro; will wrap code and collect average
// TODO: inject LD_PRELOAD style to trace functions

#ifndef PROPHILE_H
#define PROPHILE_H 1

#if defined(_MSC_VER)
	#define PROPHILE_API
	// TODO: When do these REALLY need to be set anymore?
	// #define PROPHILE_API __declspec(dllimport)
	// #define PROPHILE_API __declspec(dllexport)
#else
	#define PROPHILE_API
#endif

#ifdef  __cplusplus
// TODO: Something here to encourage the use of prophile.hpp!
extern "C" {
#endif

// While the API returns a double to the user, the time values used internally are generally
// uint64_t or uint32_t. There are potential overflow issues in extreme conditions, but nothing that
// any "typical" use is likely to encounter.
typedef double prophile_tick_t;

// This is our main "context" instance. Most of the Prophile API will accept this as the
// first argument, abstracting the nitty-gritty details and improving support for usage
// in multi-threaded situations.
typedef struct _prophile_t* prophile_t;

typedef enum {
	PROPHILE_NULL,
	PROPHILE_UNIT,
	PROPHILE_CALLBACK,
	PROPHILE_START,
	PROPHILE_STOP,
	PROPHILE_DURATION,
	PROPHILE_DATA,
	PROPHILE_STATUS
	// TODO: Future...
	// PROPHILE_INDEX
} prophile_opt_t;

// The default units used internally by Prophile are microseconds (PROPHILE_USEC), as
// these most closely correspond to the low-level native values of each supported OS.
// This option will determine what "units" are being indicated with each prophile_tick_t.
//
// TODO: Make sure that any corresponding code checks in this same order.
typedef enum {
	// Nanoseconds (1/1000000000)
	PROPHILE_NSEC,
	// Microseconds (1/1000000)
	PROPHILE_USEC,
	// Milliseconds (1/1000)
	PROPHILE_MSEC,
	// Seconds
	PROPHILE_SEC
} prophile_unit_t;

// TODO: Precisely describe the behavior of when the callback is called and when the START/STOP
// tick values are ACTUALLY recorded!
typedef void (*prophile_callback_t)(prophile_t);

typedef union {
	prophile_tick_t tick;
	prophile_opt_t opt;
	prophile_unit_t unit;
	prophile_callback_t callback;

	const void* data;
} prophile_val_t;

// Standard create/destroy pair for the opaque prophile_t instance.
//
// TODO: Demonstrate how the prophile_create can be called in various ways, AND how
// the va_list of variable arguments is terminated in those cases.
PROPHILE_API prophile_t prophile_create(prophile_opt_t opt, ...);
PROPHILE_API void prophile_destroy(prophile_t pro);

// These are the main API entrypoints into Prophile. Internally, each measurement is
// implemented as a stack of values, requiring that each prophile_start call--no matter
// how deeply nested--have a corresponding prophile_stop. If a prophile_callback_t has
// been specified, it will be called as result of each of these actions.
//
// The "data" argument can be any kind of closure data the user requires; from a simple
// string to something more complex. NOTE: The caller manages the memory!
PROPHILE_API void prophile_start(prophile_t pro, const void* data);
PROPHILE_API prophile_tick_t prophile_stop(prophile_t pro);

PROPHILE_API prophile_val_t prophile_get(const prophile_t pro, prophile_opt_t opt);
PROPHILE_API void prophile_set(prophile_t pro, prophile_opt_t opt, prophile_val_t val);

PROPHILE_API prophile_tick_t prophile_tick(prophile_unit_t unit);

// https://en.wikipedia.org/wiki/Time_Stamp_Counter
PROPHILE_API prophile_tick_t prophile_tick_rdtsc();

// This is a handy routine that internally implements OS-level, precise "sleeping" for
// the specified amount of time (using whatever prophile_unit_t is set).
PROPHILE_API void prophile_sleep(prophile_unit_t unit, prophile_tick_t tick);

// These functions return the string names of the corresponding enumerations.
PROPHILE_API const char* prophile_opt(prophile_opt_t opt);
PROPHILE_API const char* prophile_unit(prophile_unit_t unit);

#ifdef  __cplusplus
}
#endif

#endif
