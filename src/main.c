/**
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Nicolas JOURDEN <nicolas.jourden@laposte.net>
 */

#include "gpsd2nmea.h"

int main(int argc, char* argv[])
{
  char * gpsd_server_address = GPSD_SERVER_IP;
  char * gpsd_server_port = GPSD_SERVER_PORT;
  char * tll_server_address = GPSD2NMEA_TLL_SERVER_IP;
  unsigned int tll_server_port = (unsigned int) GPSD2NMEA_TLL_SERVER_PORT;
  unsigned int delay = (unsigned int) GPSD2NMEA_POLL_DELAY;
  unsigned int id = 1;
  unsigned int ais = 0;
  unsigned int debug = 0;
  int len = 0;
  char * name = "TEST";

  static const struct option long_opts[] = {
    {"Help (this message).", no_argument, NULL, '?' },
    {"Debug.", no_argument, NULL, 'd' },
    {"Delay between each message in second.", required_argument, NULL, 'D' },
    {"gpsd server address (default: 127.0.0.1).", optional_argument, NULL, 'H' },
    {"gpsd server port (default: 2947).", optional_argument, NULL, 'P' },
    {"Target UDP server address (default: 192.168.0.1).", optional_argument, NULL, 't' },
    {"Target UDP server port (default: 1234).", optional_argument, NULL, 'p' },
    {"TLL target name / AIS ship name (default: TEST).", optional_argument, NULL, 'N' },
    {"Id / MMSI (default 1).", optional_argument, NULL, 'i' },
    {"AIS output (value of 1 or 19 for message type), without TLL is outputed (defaukt).", no_argument, NULL, 'a' }
  };
  int long_index = 0;

  int c;
  char buffer[GPSD2NMEA_BUFFER_SIZE];
  int ret = 0;
  int errno = 0;
  struct gps_data_t gpsdata;

  // Read params:
  while (( c = getopt_long(argc, argv, GPSD2NMEA_USAGE, long_opts, &long_index)) )
  {
    if (c == -1) break;

    switch (c)
    {
      case 'd':
        debug = 1;
        break;
      case 'D':
        delay = (unsigned int) atoi(optarg);
        break;
      case 'H':
        gpsd_server_address = optarg;
        break;
      case 'P':
        gpsd_server_port = optarg;
        break;
      case 't':
        tll_server_address = optarg;
        break;
      case 'p':
        tll_server_port = (unsigned int) atoi(optarg);
        break;
      case 'N':
        name = optarg;
        break;
      case 'i':
        id = (unsigned int) atoi(optarg);
        break;
      case 'a':
        ais = (unsigned int) atoi(optarg);
        break;
      case ':':
        fprintf(stderr, "%s: option `-%c' requires an argument\n", argv[0], optopt);
        gpsd2nmea_displayUsage(long_opts,10);
        return 1;
      case '?':
      case 'h':
        gpsd2nmea_displayUsage(long_opts,10);
        return 0;
        break;
      default:
        return 1;
    }
  }

  // Loops:
  while (1)
  {
init:
    ret = 0;
    errno = 0;
    memset(&gpsdata, 0, sizeof(gpsdata));

    // Conect to GPS:
    ret = gps_open(gpsd_server_address, gpsd_server_port, &gpsdata);
    if (ret && debug) printf ("Cannot connect to %s:%s\n", gpsd_server_address, gpsd_server_port);

    // Ask for a stream:
    gps_stream(&gpsdata, WATCH_ENABLE | WATCH_JSON | WATCH_NEWSTYLE, NULL);
    gps_send(&gpsdata, GPSD_CONFIG);

    // Wait for data:
    while (1)
    {
      if (!gps_waiting(&gpsdata, 500)) {
        sleep(1);
        continue;
      }

      if (gps_read(&gpsdata) == -1) {
        errno++;
        continue;
      }

      if (errno > 5000) {
        goto init;
      }

      if (gpsdata.set & LATLON_SET) {
        if (debug) printf ("Lat %f / lon  %f / Time %f.\n", gpsdata.fix.latitude, gpsdata.fix.longitude, gpsdata.fix.time);
        break;
      }
    }

    // Close stream:
    gps_stream(&gpsdata, WATCH_DISABLE, NULL);
    gps_close(&gpsdata);

    // Prepare the string:
    if (ais == 1) {
      len = gpsd2nmea_setAISStr_msg1(buffer, &gpsdata, id);
    }
    else if (ais == 19) {
      len = gpsd2nmea_setAISStr_msg19(buffer, &gpsdata, id, name);
    }
    else {
      len = gpsd2nmea_setTLLStr(buffer, &gpsdata, id, name);
    }
    //if (debug) printf("%s\n", buffer);

    // Send via UDP:
    if (debug) printf("Sending to %s:%d - %s", tll_server_address, tll_server_port, buffer);
    gpsd2nmea_sendUDP(buffer, len+1, tll_server_address, tll_server_port);

    // Pause:
    sleep(delay);
  }

  return 1;
}
