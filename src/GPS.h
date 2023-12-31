//
// Created by juno on 23-6-22.
//

#ifndef GPS_UTILS_SRC_GPS_H
#define GPS_UTILS_SRC_GPS_H

#include <queue>
#include <mutex>

#include "tty.h"
#include "Nmea.h"

class GPS : tty {
  Nmea *nmea;
  std::queue<std::shared_ptr<std::string>> mMsgs;
  std::mutex mutex;
public:
  int on_message(std::shared_ptr<std::string> &msg) override;
  bool checksum(const std::string &str) override;
  explicit GPS(const std::string &device);
  bool is_active() {
	return tty::is_active();
  }
  void *get(const char *name) {
	return nmea->get(name);
  }
  void run();
  ~GPS() override;
};

#endif //GPS_UTILS_SRC_GPS_H
