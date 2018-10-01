/**
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Nicolas JOURDEN <nicolas.jourden@laposte.net>
 */

#include "../src/gpsd2nmea.h"

#include "../src/crc.c"
#include "../src/tll.c"

int main ( int argc, char * *argv ) {
  char buffer[GPSD2NMEA_BUFFER_SIZE];
  struct gps_data_t gpsdata;
  gpsdata.fix.latitude = 9.009;
  gpsdata.fix.longitude = 90.900;
  gpsdata.fix.time = 0;
  
  gpsd2nmea_setTLLStr(buffer, &gpsdata, 1, "Goldorak");
  printf ("%s\n", buffer);
  assert (strcmp(buffer, "$GPTLL,01,090.54000,N,090054.00000,W,Goldorak,010000.00,T,,*27\r\n") == 0x00);

  gpsd2nmea_setTLLStr(buffer, &gpsdata, 5, "Goldorak");
  printf ("%s\n", buffer);
  assert (strcmp(buffer, "$GPTLL,05,090.54000,N,090054.00000,W,Goldorak,010000.00,T,,*23\r\n") == 0x00);

  gpsd2nmea_setTLLStr(buffer, &gpsdata, 99, "Aktarus");
  printf ("%s\n", buffer);
  assert (strcmp(buffer, "$GPTLL,99,090.54000,N,090054.00000,W,Aktarus,010000.00,T,,*5A\r\n") == 0x00);

  gpsd2nmea_setTLLStr(buffer, &gpsdata, 23, "Meli");
  printf ("%s\n", buffer);
  assert (strcmp(buffer, "$GPTLL,23,090.54000,N,090054.00000,W,Meli,010000.00,T,,*3D\r\n") == 0x00);
  
  return 0;
}
