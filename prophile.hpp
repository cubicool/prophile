// Prophile (cubicool@gmail.com)

#ifndef PROPHILE_HPP
#define PROPHILE_HPP 1

#include "prophile.h"

#include <variant>

namespace prophile {

using tick_t = prophile_tick_t;
using unit_t = prophile_unit_t;
// using opt_t = prophile_opt_t;
// using val_t = prophile_val_t;

using val_t = std::variant<
	prophile_tick_t,
	prophile_opt_t,
	prophile_unit_t,
	prophile_callback_t,
	const void*
>;

inline void sleep(unsigned int usec) {
	prophile_sleep(usec);
}

inline tick_t tick(unit_t u) {
	return prophile_tick(u);
}

class prophile {
public:
	// enum class unit_t {
	// 	USEC = PROPHILE_USEC,
	// 	NSEC = PROPHILE_NSEC,
	// 	MSEC = PROPHILE_MSEC,
	// 	SEC = PROPHILE_SEC
	// };

	prophile() {
		_pro = prophile_create(PROPHILE_DEFAULTS);
	}

	~prophile() {
		if(_pro) prophile_destroy(_pro);
	}

	inline void start(const void* data) {
		prophile_start(_pro, data);
	}

	inline tick_t stop() {
		return prophile_stop(_pro);
	}

	// static void sleep(unsigned int usec) {
	// 	prophile_sleep(usec);
	// }
	//
	// static tick_t tick(unit_t u) {
	// 	return prophile_tick(u);
	// }

protected:
	prophile_t _pro = nullptr;
};

}

#endif
