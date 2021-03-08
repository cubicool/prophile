#include "prophile.h"

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

int main(int argc, char** argv) {
	// prophile_t pro = prophile_create(PROPHILE_DEFAULTS);

	prophile_t pro = prophile_create(
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

	prophile_destroy(pro);

	return 0;
}
