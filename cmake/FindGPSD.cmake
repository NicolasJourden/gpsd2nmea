# - Find gpsd
# Find the gpsd headers and libraries.
#
# This module defines the following variables:
#
#   GPSD_INCLUDE_DIRS - *.h, etc.
#   GPSD_LIBRARIES    - The libs.
#   GPSD_FOUND        - If found set to yes.
#

find_path(LIBGPS_INCLUDE_DIR
  NAMES libgpsmm.h
  PATHS_SUFFIXES
    /usr/local/include
    /usr/include
  PATHS
    ${CMAKE_FIND_ROOT_PATH}
)

find_library(LIBGPS_LIBRARY
  NAMES gps
  PATHS_SUFFIXES
    /usr/lib/x86_64-linux-gnu
    /usr/lib
    /usr/local/lib
  PATHS
    ${CMAKE_FIND_ROOT_PATH}
)

# Copy the results to the output variables.
if(LIBGPS_LIBRARY AND LIBGPS_INCLUDE_DIR)
  set (LIBGPS_FOUND "YES")
  set(LIBGPS_INCLUDE_DIRS "${LIBGPS_INCLUDE_DIR}")
  set(LIBGPS_LIBRARIES ${LIBGPS_LIBRARY})
endif()

message(STATUS " LIBGPS STATUS")
message(STATUS " LIBGPS_LIBRARIES ${LIBGPS_LIBRARIES}")
message(STATUS " LIBGPS_INCLUDE ${LIBGPS_INCLUDE_DIRS}")
