/**
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Nicolas JOURDEN <nicolas.jourden@laposte.net>
 */

#include "../src/gpsd2nmea.h"

#include "../src/udp.c"

int main ( int argc, char * *argv ) {

  char * gga = "$INGGA,140742.45,3822.549763,N,00902.839254,W,2,10,0.9,6.29,M,49.82,M,2.0,0136*49\r\n";

  gpsd2nmea_sendUDP(gga, strlen(gga), "127.0.0.1", 1234);

  return 0;
}
