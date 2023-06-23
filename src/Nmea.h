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
  char time[32];
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
  uint32_t elevation;
  uint32_t azimuth;
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
  char time[32];
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

struct gsa {
  uint8_t select_mode;
  uint8_t mode;
  uint8_t id[12];
  double pdop;
  double hdop;
  double vdop;
};

struct rmc {
  char time[32];
  char status;
  double lat;
  char ns;
  double lon;
  char ew;
  double speed;
  double deg;
  uint16_t date;
  double magnetic_variation;
  char variation_ew;
};

struct zda {
  uint8_t local_min_desc;
  uint8_t local_zone_desc;
  uint8_t day;
  uint8_t mon;
  uint16_t year;
  char time[32];
};

class Nmea {
  std::map<std::string, std::function<void(std::shared_ptr<std::string> &, Nmea *nmea)>> processor_map;
  std::map<std::string, void *> data;
public:
  Nmea();
  ~Nmea();
  int parse(std::shared_ptr<std::string> &msg);
  void *get(const std::string &name);
};

#endif //GPS_UTILS_SRC_NMEA_H
