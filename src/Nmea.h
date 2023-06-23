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
