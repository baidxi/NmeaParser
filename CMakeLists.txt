cmake_minimum_required(VERSION 3.25)
project(gps_utils)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE "./src/*.h" )
add_subdirectory(./src/lvgl)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-O3")

find_package(SDL2 REQUIRED SDL2)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(gps_utils
        src/main.cpp src/tty.cpp src/GPS.cpp
        src/Nmea.cpp src/mouse_cursor_icon.c
        ${SOURCES} ${INCLUDES} src/ui.cpp src/ui.h)
add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)
target_link_libraries(gps_utils PRIVATE lvgl ${SDL2_LIBRARIES})

target_compile_options(lvgl PRIVATE -Werror -Werror=float-conversion)
