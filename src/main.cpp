#include <iostream>
#include "GPS.h"

int main() {
	auto gps = new GPS("/dev/ttyUSB0");

	gps->run();

	return 0;
}
