/**
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Nicolas JOURDEN <nicolas.jourden@laposte.net>
 */

#include "../src/gpsd2nmea.h"

#include "../src/crc.c"

int main ( int argc, char * *argv ) {
  char * nmea1 = "GPTLL,47,3822.93922,N,00902.81424,W,lisa,154400.00,T,,*08";
  int crc1 = gpsd2nmea_getCRC(nmea1, strlen(nmea1));
  printf ("0x%02X = 0x08\n", crc1);
  assert (crc1 == 0x08);

  char * nmea2 = "GPTLL,55,3822.67494,N,00902.97300,W,carol,155100.00,T,,*66";
  int crc2 = gpsd2nmea_getCRC(nmea2, strlen(nmea2));
  printf ("0x%02X = 0x66\n", crc2);
  assert (crc2 == 0x66);

  char * nmea3 = "INZDA,095507.45,07,09,2018,,*7C";
  int crc3 = gpsd2nmea_getCRC(nmea3, strlen(nmea3));
  printf ("0x%02X = 0x7C\n", crc3);
  assert (crc3 == 0x7C);

  char * nmea4 = "INGGA,140742.45,3822.549763,N,00902.839254,W,2,10,0.9,6.29,M,49.82,M,2.0,0136*49";
  int crc4 = gpsd2nmea_getCRC(nmea4, strlen(nmea4));
  printf ("0x%02X = 0x49\n", crc4);
  assert (crc4 == 0x49);
  
  return 0;
}
