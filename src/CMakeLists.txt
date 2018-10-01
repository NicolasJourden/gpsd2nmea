file (GLOB SRC_C crc.c)
file (GLOB SRC_H *.h)
add_executable (gpsd2nmea 
  main.c
  cmd_help.c
  tll.c
  ais.c
  crc.c
  udp.c
)
target_link_libraries (gpsd2nmea ${LIBGPS_LIBRARIES})
