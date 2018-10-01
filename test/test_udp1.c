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

  char * str1 = "Hello\n";

  gpsd2nmea_sendUDP(str1, strlen(str1), "127.0.0.1", 1234);

  return 0;
}
