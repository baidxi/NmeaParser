//
// Created by juno on 23-6-22.
//

#ifndef GPS_UTILS_SRC_NMEA_H
#define GPS_UTILS_SRC_NMEA_H

#include <memory>
#include <string>
#include <map>
#include <functional>

struct gll {
	double lat;
	char ns;
	double lon;
	char ew;
	char time[12];
	char status;
};

struct vtg {
	double deg1;
	double deg2;
	double speed;
	double kph;
};

struct gsv_info {
	uint32_t sat_num;
	uint32_t deg1;
	uint32_t deg2;
	uint32_t snr;
};

struct gsv_group {
	uint8_t num_info;
	gsv_info infos[4];
};

struct gsv {
	uint8_t sat_count;
	uint8_t group_count;
	gsv_group group[4];
};

struct gga {
	char time[12];
	double lat;
	char ns;
	double lon;
	char ew;
	uint8_t quality;
	uint8_t sat_num;
	double horizontal;
	double ant_alt;
	char ant_units;
	double geoidal;
	char geoiadl_units;
	double age;
	uint16_t id;
};

class Nmea {
	std::map<std::string, std::function<void(std::shared_ptr<std::string> &, Nmea *nmea)>> processor_map;
	std::map<std::string, void *> data;
public:
	Nmea();
	~Nmea();
	int parse(std::shared_ptr<std::string> &msg);
	void *get_data(const std::string &name);
};

#endif //GPS_UTILS_SRC_NMEA_H
