//
// Created by juno on 23-6-22.
//
#include <chrono>
#include <thread>
#include <algorithm>
#include "GPS.h"

GPS::GPS(const std::string &device):
tty(device, 9600),
nmea(nullptr)
{
	nmea = new Nmea();
}

GPS::~GPS()
{
	tty::~tty();
	delete nmea;
}

int GPS::on_message(std::shared_ptr<std::string> &msg)
{
	auto lock = std::lock_guard<std::mutex>(mutex);

	mMsgs.push(msg);

	return 0;
}

void GPS::run()
{
	auto ms = std::chrono::duration<double, std::milli>(200);
	while(is_open()) {
		if (mMsgs.empty()) {
			std::this_thread::sleep_for(ms);
			continue;
		}
		auto msg = mMsgs.front();

		nmea->parse(msg);

		auto lock = std::lock_guard<std::mutex>(mutex);
		mMsgs.pop();
	}

	fprintf(stdout, "%s\n", what());
}
bool GPS::checksum(const std::string &str)
{
	int sum = 0;
	size_t start = str.find('*') + 1;

	for (char ch : str) {
		if (ch == '$')
			continue;
		if (ch == '*')
			break;
		if (ch != '\n') {
			sum ^= ch;
		}
	}

	return sum == strtol(str.substr(start).c_str(), nullptr, 16);
}
