/**
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Nicolas JOURDEN <nicolas.jourden@laposte.net>
 */

#include "../src/gpsd2nmea.h"

#include "../src/crc.c"
#include "../src/ais.c"

int main ( int argc, char * *argv ) {
  
  // Test 1:
  char buffer1[GPSD2NMEA_BUFFER_SIZE];
  struct gps_data_t gpsdata1;
  char * name1 = "TESTTESTTESTTESTTEST";
  unsigned int mmsi1 = 316001245;
  gpsdata1.fix.latitude = 49.200283;
  gpsdata1.fix.longitude = -123.877748;
  gpsdata1.fix.time = 7;
  gpsdata1.fix.track = 235.0; // 
  gpsdata1.fix.speed = 10.08311; // m/s
  gpsd2nmea_setAISStr_msg19(buffer1, &gpsdata1, mmsi1, name1);
  printf ("%s\n", buffer1);
//  assert (strcmp(buffer, "!AIVDM,1,1,,A,14eG;o@034o8sd<L9i:a;WF>062D,0*7D\r\n") == 0x00);

  // Test 2:
  char buffer2[GPSD2NMEA_BUFFER_SIZE];
  struct gps_data_t gpsdata2; //
  char * name2 = "meLiTa";
  unsigned int mmsi2 = 238125441;
  gpsdata2.fix.latitude = 45.4696608;
  gpsdata2.fix.longitude = -73.5857573;
  gpsdata2.fix.time = 10;
  gpsdata2.fix.track = 180.0;
  gpsdata2.fix.speed = 0.01;

  gpsd2nmea_setAISStr_msg19(buffer2, &gpsdata2, mmsi2, name2);
  printf ("%s\n", buffer2);
//  assert (strcmp(buffer, "!AIVDM,1,1,,B,177KQJ5000G?tO`K>RA1wUbN0TKH,0*5C\r\n") == 0x00);

  return 0;
}
