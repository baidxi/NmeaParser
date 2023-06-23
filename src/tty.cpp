//
// Created by juno on 23-6-22.
//

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <termios.h>
#include <sys/epoll.h>
#include <future>

#include "tty.h"

tty::tty(const std::string &device, uint32_t bitrate) :
	mWhat(-1),
	mIsReady(false),
	fd(-1),
	epoll_fd(-1),
	running(true) {
  termios term{0};

  fd = ::open(device.c_str(), O_NONBLOCK | O_RDONLY);
  if (fd < 0) {
	mWhat = errno;
	return;
  }

  if (!tcgetattr(fd, &term)) {
	term.c_lflag = 0;
	cfsetspeed(&term, bitrate);
  } else {
	mWhat = errno;
	return;
  }

  if (tcsetattr(fd, TCSANOW, &term)) {
	mWhat = errno;
	return;
  }

  loop_run();
}

bool tty::is_open() const {
  return mIsReady;
}

const char *tty::what() const {
  return ::strerror(mWhat);
}

void tty::loop_run() {
  epoll_event ev{0};
  epoll_fd = epoll_create(5);
  if (epoll_fd < 0) {
	mWhat = errno;
	return;
  }

  ev.data.fd = fd;
  ev.events = EPOLL_EVENTS::EPOLLET | EPOLL_EVENTS::EPOLLIN;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev)) {
	mWhat = errno;
	return;
  }

  std::thread th([this] {
	epoll_event events[5];
	char buf[128];
	int ret;
	size_t size = 0;
	while (running) {
	  ret = epoll_wait(epoll_fd, events, 5, -1);
	  for (int i = 0; i < ret; ++i) {
		if (events[i].events & EPOLL_EVENTS::EPOLLIN) {
		  char buf1[128]{0};
		  size_t sz = read(events[i].data.fd, buf1, 128);
		  for (int j = 0; j < sz; ++j) {
			buf[size++] = buf1[j];
			if (buf1[j] == '$') {
			  if ((size - 1) > 0) {
				buf[size - 1] = '\0';
				if (buf[size - 2] == '\n')
				  buf[size - 2] = '\0';

				size = 0;

				if (!checksum(buf))
				  continue;

				auto msg = std::make_shared<std::string>(buf);

				buf[size++] = buf1[j];
				if (msg != nullptr)
				  on_message(msg);
			  }
			}
		  }
		}
	  }
	}
  });

  th.detach();
  mIsReady = true;
}

tty::~tty() {
  running = false;
  mIsReady = false;
  ::close(fd);
}
