// Prophile (cubicool@gmail.com)

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
extern "C" {
#endif

// This is our main "context" instance. Most the Prophile API will accept this as the
// first argument, abstracting the nitty-gritty details and improving support for usage
// in multi-threaded situations.
typedef struct _prophile_t* prophile_t;

// Options used when creating the prophile_t context.
typedef enum _prophile_opt_t {
	PROPHILE_DEFAULTS,
	PROPHILE_UNITS,
	PROPHILE_CALLBACK
	// PROPHILE_CALLBACK_PRINTF
	// PROPHILE_SAMPLES
	// PROPHILE_INDEX
} prophile_opt_t;

// The default units used internally by Prophile are microseconds (PROPHILE_USEC), as
// these most closely correspond to the low-level native values of each supported OS.
// This option will determine what "units" are being indicated with each prophile_tick_t.
//
// TODO: Make sure that any corresponding code checks in this same order.
typedef enum _prophile_unit_t {
	// Microseconds (1/1000000)
	PROPHILE_USEC,
	// Nanoseconds (1/1000000000)
	PROPHILE_NSEC,
	// Milliseconds (1/1000)
	PROPHILE_MSEC,
	// Seconds
	PROPHILE_SEC
	// TODO: PROPHILE_NATIVE
} prophile_unit_t;

// Currently, a single callback is used for both STARTING and STOPPING a measurement.
// Perhaps it the future it may make more sense to have them be individually set.
typedef enum _prophile_callback_mode_t {
	PROPHILE_START,
	PROPHILE_STOP
} prophile_callback_mode_t;

// Although the FINAL value is stored in a double, the actual UNITS this value will
// represent are determined by the prophile_t "unit" option.
typedef double prophile_tick_t;

// It's easier to pass this entire structure to the prophile_callback_t function than
// it is to have to specify EACH of the possible fields in the signature.
typedef struct _prophile_callback_data_t {
	prophile_callback_mode_t mode;
	prophile_tick_t start;
	prophile_tick_t stop;
	const void* data;
	// TODO: It would probably be really handy to know what "level" of the stack our
	// measurement is being taken on, particularly when support for relative timing
	// is added; that is, measurements based on some relationship to the parent and
	// sibling samples.
	// size_t index;
} prophile_callback_data_t;

// If set on the prophile_t context during creation, the desired callback must match
// the signature indicated by the prophile_callback_t type.
typedef void (*prophile_callback_t)(prophile_callback_data_t*);

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

// This is a handy routine that internally implements OS-level, precise "sleeping" for
// the specified amount of microseconds.
//
// TODO: Change the first argument to prophile_unit_t, rather than JUST microseconds.
PROPHILE_API void prophile_sleep(unsigned int usec);

// There may be cases where the user doesn't want to be bothered with prophile_t context
// creation and simply wants an easy, reliable way to fetch timing information from the
// OS. In those cases, the return value is in units specified in the function argument.
//
// You probably won't use this, but, what do I know?
PROPHILE_API prophile_tick_t prophile_tick(prophile_unit_t u);

// TODO: Find some good way to help output formatting; both with the prophile_unit_t as
// some string value AND using good printf flags (e.g., "12.6f").
// PROPHILE_API const char* prophile_units(...)
// PROPHILE_API void prophile_sprintf(...)

// TODO: For the below to be valid, prophile_tick_t would need to need to know the INITIAL
// prophile_unit_t; maybe prophile_tick_t should be a structure?
// PROPHILE_API prophile_tick_t prophile_unit(prophile_unit_t u, prophile_tick_t t);

#ifdef  __cplusplus
}
#endif

#endif
