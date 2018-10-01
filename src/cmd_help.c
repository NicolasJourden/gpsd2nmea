/**
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Nicolas JOURDEN <nicolas.jourden@laposte.net>
 */

#include "gpsd2nmea.h"

void gpsd2nmea_displayUsage(const struct option * pLong_opts, int pSize)
{
  puts("gpsd2nmea has the following options: ");

  for (int i = 0; i< pSize; i++) {
    printf("\t-%c\t%s\n", pLong_opts[i].val, pLong_opts[i].name);
  }
}
