cmake_minimum_required(VERSION 2.8.0)

project(gpsd2nmea)
enable_testing ()
option(BUILD_TESTING "Build tests" ON)
set(PROJECT_NAME "gpsd2nmea")
set(PROJECT_VENDOR "Nicolas JOURDEN")
set(PROJECT_COPYRIGHT "GPLv3")
set(PROJECT_VERSION_MAJ "0")
set(PROJECT_VERSION_MIN "1")


list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(GPSD)

add_subdirectory(src)

if (BUILD_TESTING)
  add_subdirectory(test)
endif (BUILD_TESTING)
