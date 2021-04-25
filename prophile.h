// Prophile (cubicool@gmail.com)

#ifndef PROPHILE_H
#define PROPHILE_H 1

#if defined(_MSC_VER)
	#define PROPHILE_API
	// #define PROPHILE_API __declspec(dllimport)
	// #define PROPHILE_API __declspec(dllexport)
#else
	#define PROPHILE_API
#endif

#include <stdint.h>
#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef uint64_t prophile_tick_t;

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
	PROPHILE_INDEX,
	PROPHILE_DATA,
	PROPHILE_STATUS
} prophile_opt_t;

// The default units used internally by Prophile are microseconds (PROPHILE_USEC), as
// these most closely correspond to the low-level native values of each supported OS.
// This option will determine what "units" are being indicated with each prophile_tick_t.
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

	size_t index;

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

#define prophile_get_unit(pro) prophile_get(pro, PROPHILE_UNIT).unit
#define prophile_get_callback(pro) prophile_get(pro, PROPHILE_CALLBACK).callback
#define prophile_get_start(pro) prophile_get(pro, PROPHILE_START).tick
#define prophile_get_stop(pro) prophile_get(pro, PROPHILE_STOP).tick
#define prophile_get_duration(pro) prophile_get(pro, PROPHILE_DURATION).tick
#define prophile_get_index(pro) prophile_get(pro, PROPHILE_INDEX).index
#define prophile_get_data(pro) prophile_get(pro, PROPHILE_DATA).data
#define prophile_get_status(pro) prophile_get(pro, PROPHILE_STATUS).opt

#define prophile_set_unit(pro, u) prophile_set(pro, PROPHILE_UNIT, {.unit = u})
#define prophile_set_callback(pro, cb) prophile_set(pro, PROPHILE_CALLBACK, {.callback = cb})
#define prophile_set_data(pro) prophile_set(pro, PROPHILE_DATA, {.data = d})

PROPHILE_API prophile_tick_t prophile_tick(prophile_unit_t unit);

// https://en.wikipedia.org/wiki/Time_Stamp_Counter
//
// This returns the number of "CPU cycles"; they don't actually correspond directly to any
// measurement, like nanoseconds. You can divide this number by your CPU frequency (MHz) to get
// something sensible.
//
// TODO: Should there be an option "hz" argument? Perhaps some way to query it?
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
