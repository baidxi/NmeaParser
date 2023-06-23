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

		if (*msg->c_str() == '$') {
			nmea->parse(msg);
		}

		auto lock = std::lock_guard<std::mutex>(mutex);
		mMsgs.pop();

		gsv *v =(gsv *) nmea->get_data("GSV");
		gsv_group *group;
		gsv_info *info;

		fprintf(stderr, "sat count:%d \n", v->sat_count);

		for (int i = 0; i < v->group_count; i++) {
			group = v->group + i;
			for (int j = 0; j < group->num_info; ++j) {
				info = group->infos + j;
				fprintf(stderr, "sat num:%d deg1 = %d deg2 = %d SNR:%d\n",
						info->sat_num, info->deg1, info->deg2, info->snr);
			}
		}

	}

	fprintf(stdout, "%s\n", what());
}
