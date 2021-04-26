// vimrun! ./prophile-cpp

#include "prophile.hpp"

#include <cstdio>
#include <cinttypes>

#if 0
#include <stdint.h>
#include <stdio.h>

static void main_callback(prophile_callback_data_t* cbd) {
	if(cbd->mode == PROPHILE_START) {
		printf("START [%s]\n", (const char*)(cbd->data));
	}

	else {
		printf("STOP  [%s]: %f\n", (const char*)(cbd->data), cbd->stop - cbd->start);
	}
}
#endif

int main(int argc, char** argv) {
	/* prophile_t pro = prophile_create(
		PROPHILE_UNITS, PROPHILE_MSEC,
		PROPHILE_CALLBACK, main_callback,
		NULL
	);

	for(uint32_t i = 0; i < 3; i++) {
		prophile_start(pro, "prophile_sleep(100000)");
		prophile_sleep(100000);

		for(uint32_t ii = 0; ii < 2; ii++) {
			prophile_start(pro, "prophile_sleep(50000)");
			prophile_sleep(50000);
			printf("RETURNED: %f\n", prophile_stop(pro));
		}

		printf("RETURNED: %f\n", prophile_stop(pro));
	}

	prophile_destroy(pro); */

	/* auto pro = prophile::Prophile();

	for(auto i = 0; i < 3; i++) {
		pro.start("prophile_sleep(100000)");
		// pro.sleep(100000);

		prophile::sleep(prophile::Unit::USEC, 100000);

		for(auto ii = 0; ii < 2; ii++) {
			pro.start("prophile_sleep(50000)");
			// pro.sleep(50000);

			prophile::sleep(prophile::Unit::USEC, 50000);

			std::printf("RETURNED: %f\n", pro.stop());
		}

		std::printf("RETURNED: %f\n", pro.stop());
	}

	// std::printf("%f\n", prophile::tick(prophile::unit_t::PROPHILE_MSEC)); */

	auto p = prophile::Prophile();

	p.start();

	prophile::sleep(prophile::Unit::NSEC, 500000000);

	std::printf("%s\n", prophile_unit(prophile_get_unit(p.ptr())));
	std::printf(PROPHILE_FMT "\n", p.stop());

	std::printf(PROPHILE_FMT "\n", prophile::tick(prophile::Unit::NSEC));
	std::printf(PROPHILE_FMT "\n", prophile::tick(prophile::Unit::USEC));
	std::printf(PROPHILE_FMT "\n", prophile::tick(prophile::Unit::MSEC));
	std::printf(PROPHILE_FMT "\n", prophile::tick(prophile::Unit::SEC));

	return 0;
}
