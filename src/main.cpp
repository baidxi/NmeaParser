#include <iostream>
#include <thread>

#include "GPS.h"
#include "ui.h"

int main() {
  auto gps = new GPS("/dev/ttyUSB0");

  std::thread th([=](){
	gps->run();
  });

  th.detach();

  ui *gui = new ui(800, 480);
  gui->style_init();

  gui->loop_run(gps);

  return 0;
}
