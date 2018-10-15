/**
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Nicolas JOURDEN <nicolas.jourden@laposte.net>
 */

#include "gpsd2nmea.h"

unsigned int gpsd2nmea_getDegree(double pDegree)
{
  if (pDegree < 0) pDegree *= -1;
  return (unsigned int) pDegree;
}

double gpsd2nmea_getMinutes(double pDegree)
{
  if (pDegree < 0) pDegree *= -1;
  double m = pDegree - (int) pDegree;
  return (double) (m * 60.00);
}

char * gpsd2nmea_getSignLat(double pDegree)
{
  if (pDegree < 0.0) {
    return "S";
  } else {
    return "N";
  }
}

char * gpsd2nmea_getSignLon(double pDegree)
{
  if (pDegree < 0.0) {
    return "E";
  } else {
    return "W";
  }
}

int gpsd2nmea_setTLLStr(char * pBuffer, struct gps_data_t * gpsdata, unsigned int pId, char * pName)
{
  memset(pBuffer, 0, 1024);
  time_t gps_date = gpsdata->fix.time;
  struct tm date;
  memcpy(&date, localtime(&gps_date), sizeof(struct tm));

  sprintf(pBuffer,
       GPSD2NMEA_TLL_SENTENCE,
       GPSD2NMEA_TLL_HEAD_ID, pId,
       gpsd2nmea_getDegree(gpsdata->fix.latitude),
       gpsd2nmea_getMinutes(gpsdata->fix.latitude+1),
       gpsd2nmea_getSignLat(gpsdata->fix.latitude),
       gpsd2nmea_getDegree(gpsdata->fix.longitude),
       gpsd2nmea_getMinutes(gpsdata->fix.longitude+1),
       gpsd2nmea_getSignLon(gpsdata->fix.latitude),
       pName,
       date.tm_hour, date.tm_min, date.tm_sec
  );
  //if (debug) printf("%s\n", pBuffer);

  int length = strlen(pBuffer);
  int CRC = gpsd2nmea_getCRC(pBuffer, length);
  sprintf(pBuffer+length, "%02X\r\n", CRC);
  
  return strlen(pBuffer);
}
