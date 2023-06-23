cmake_minimum_required(VERSION 3.25)
project(gps_utils)

set(CMAKE_CXX_STANDARD 17)

add_executable(gps_utils src/main.cpp src/tty.cpp src/tty.h src/GPS.cpp src/GPS.h src/Nmea.cpp src/Nmea.h)
