// Prophile (cubicool@gmail.com)

#ifndef PROPHILE_HPP
#define PROPHILE_HPP 1

#include "prophile.h"

#include <functional>
#include <variant>

namespace prophile {

class Prophile;

using Tick = prophile_tick_t;
// using Callback = std::function<void(const Prophile&)>;
using Callback = prophile_callback_t;

enum class Option {
	UNIT = PROPHILE_UNIT,
	CALLBACK = PROPHILE_CALLBACK,
	START = PROPHILE_START,
	STOP = PROPHILE_STOP,
	DURATION = PROPHILE_DURATION,
	INDEX = PROPHILE_INDEX,
	DATA = PROPHILE_DATA,
	STATUS = PROPHILE_STATUS
};

enum class Unit {
	NSEC = PROPHILE_NSEC,
	USEC = PROPHILE_USEC,
	MSEC = PROPHILE_MSEC,
	SEC = PROPHILE_SEC
};

struct Value: public std::variant<Tick, Option, Unit, Callback, void*> {
	constexpr auto tick() const { return std::get<Tick>(*this); }
	constexpr auto option() const { return std::get<Option>(*this); }
	constexpr auto unit() const { return std::get<Unit>(*this); }
	constexpr auto callback() const { return std::get<Callback>(*this); }
	constexpr auto data() const { return std::get<void*>(*this); }
};

inline void sleep(Unit unit, Tick tick) {
	prophile_sleep(static_cast<prophile_unit_t>(unit), tick);
}

inline auto tick(Unit u) {
	return prophile_tick(static_cast<prophile_unit_t>(u));
}

class Prophile {
public:
	Prophile() {
		_pro = prophile_create(PROPHILE_NULL);
	}

	~Prophile() {
		if(_pro) prophile_destroy(_pro);
	}

	inline void start(void* data = nullptr) {
		prophile_start(_pro, data);
	}

	inline auto stop() {
		return prophile_stop(_pro);
	}

	inline void sleep(Tick tick) const {
		prophile_sleep(prophile_get_unit(_pro), tick);
	}

	inline auto tick() const {
		return prophile_tick(prophile_get_unit(_pro));
	}

	inline auto ptr() {
		return _pro;
	}

	inline auto ptr() const {
		return _pro;
	}

protected:
	prophile_t _pro = nullptr;
};

}

#endif
