/**
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Nicolas JOURDEN <nicolas.jourden@laposte.net>
 */

#include "gpsd2nmea.h"

int gpsd2nmea_getCRC(const char * pBuffer, int len)
{
  int checksum = 0;

  for (int i=0; i <= len; i++)
  {
    switch (pBuffer[i])
    {
      case '!':
      case '$':
        continue;
      case '*':
        goto end;
      default:
        if (checksum == 0)
        {
          checksum = pBuffer[i];
        }
        else
        {
          checksum ^= pBuffer[i];
        }
    }
  }
end:
  return checksum;
}
