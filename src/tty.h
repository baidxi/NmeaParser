//
// Created by juno on 23-6-22.
//

#ifndef GPS_UTILS_SRC_TTY_H
#define GPS_UTILS_SRC_TTY_H

#include <string>
#include <memory>
#include <queue>

class tty {
  int fd;
  int mWhat;
  bool mIsReady;
  int epoll_fd;
  void loop_run();
  bool running;
public:
  explicit tty(const std::string &device, uint32_t bitrate);
  [[nodiscard]] bool is_open() const;
  [[nodiscard]] const char *what() const;
  virtual ~tty();
protected:
  virtual int on_message(std::shared_ptr<std::string> &msg) = 0;
  virtual bool checksum(const std::string &str) = 0;
};

#endif //GPS_UTILS_SRC_TTY_H
