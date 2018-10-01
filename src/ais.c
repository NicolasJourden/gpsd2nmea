#include "gpsd2nmea.h"

/**
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Nicolas JOURDEN <nicolas.jourden@laposte.net>

   This part is largely inspired from gpsd and its pseudo AIS encoder.
 */

static void ais_addbits(unsigned char *bits, unsigned int start, unsigned int len, uint64_t data)
{
  unsigned int  l;
  unsigned int  pos;
  uint64_t      mask;
  unsigned int  mask1;

  mask  = 0x1;
  pos   = (start+len-1) / 6;
  mask1 = 0x20;
  mask1 = mask1 >> ((start+len-1) % 6);

  if (len == 0) { return; }

  for(l=0;l<len;l++) {
    if (data & mask) {
      bits[pos] |= mask1;
    }
    mask  <<= 1;
    mask1 <<= 1;
    if (mask1 == 0x40) {
      pos   -= 1;
      mask1  = 0x1;
    }
  }
  return;
}

static void ais_addchar(unsigned char *bits, unsigned int start, unsigned int len, const char *data)
{ 
  unsigned int l;
  unsigned int flag;
  static unsigned char contab1[] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
  0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
  0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
  
  for(l=0,flag=0;l<len;l++) {
    unsigned char a, b;
    a = (unsigned char) data[l];
    if (a == (unsigned char)'\0') {
      flag = 1;
    }
    if (flag == 0) {
      b = contab1[a & 0x7f];
    } else {
      b = (unsigned char) '\0';
    }
    ais_addbits(bits, start+6*l, 6, (uint64_t)b);
  }
  return;
}

static void ais_binary_to_ascii(unsigned char *bits, unsigned int len)
{
  unsigned int l;
  static unsigned char convtab[] = {"0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVW`abcdefghijklmnopqrstuvw"};

  if (len == 0) {
    bits[0] = '\0';
    return;
  }

  for (l=0;l<len;l+=6) {
    bits[l/6] = convtab[bits[l/6] & 0x3f];
  }
  return;
}

unsigned int ais_binary_encode(t_gpsd2nmea_ais_msg19 * ais, unsigned char *bits)
{
  unsigned int len = 0;
  ais_addbits(bits,   0,  6, (uint64_t)ais->type);
  ais_addbits(bits,   6,  2, (uint64_t)ais->repeat);
  ais_addbits(bits,   8, 30, (uint64_t)ais->mmsi);
  ais_addbits(bits,  38,  8, (uint64_t)ais->reserved);
  ais_addbits(bits,  46, 10, (uint64_t)ais->speed);
  ais_addbits(bits,  56,  1, (uint64_t)ais->accuracy);
  ais_addbits(bits,  57, 28, (uint64_t)ais->longitude);
  ais_addbits(bits,  85, 27, (uint64_t)ais->latitude);
  ais_addbits(bits, 112, 12, (uint64_t)ais->course);
  ais_addbits(bits, 124,  9, (uint64_t)ais->heading);
  ais_addbits(bits, 133,  6, (uint64_t)ais->second);
  ais_addbits(bits, 139,  4, (uint64_t)ais->regional);
  ais_addchar(bits, 143, 20,           ais->shipname);
  ais_addbits(bits, 263,  8, (uint64_t)ais->shiptype);
  ais_addbits(bits, 271,  9, (uint64_t)ais->to_bow);
  ais_addbits(bits, 280,  9, (uint64_t)ais->to_stern);
  ais_addbits(bits, 289,  6, (uint64_t)ais->to_port);
  ais_addbits(bits, 295,  6, (uint64_t)ais->to_starboard);
  ais_addbits(bits, 301,  4, (uint64_t)ais->epfd);
  ais_addbits(bits, 305,  1, (uint64_t)ais->raim);
  ais_addbits(bits, 306,  1, (uint64_t)ais->dte);
  ais_addbits(bits, 307,  1, (uint64_t)ais->assigned);
  ais_addbits(bits, 308,  4, (uint64_t)ais->spare);
  len = 312;  
  
  ais_binary_to_ascii(bits, len);
  return len;
}

int gpsd2nmea_setAISStr(char * pBuffer, struct gps_data_t * gpsdata, int pMMSI, const char * pShipName)
{
  unsigned char ais_encoded[t_gpsd2nmea_endoded_msg19_size+1];
  t_gpsd2nmea_ais_msg19 ais;
  memset(&ais, 0x00, t_gpsd2nmea_ais_msg19_size);
  memset(ais_encoded, 0x00, t_gpsd2nmea_endoded_msg19_size+1);
  memset(pBuffer, 0x00, GPSD2NMEA_BUFFER_SIZE);

  // Set time: seconds:
  struct tm date;
  time_t gps_date = gpsdata->fix.time;
  memcpy(&date, localtime(&gps_date), sizeof(struct tm));
  
  // Put data:
  ais.type = 19; // message type.
  ais.repeat = 2; // do not repeat.
  ais.mmsi = pMMSI;
  ais.reserved = 0; // none.
  ais.speed = (uint16_t) ((gpsdata->fix.speed * 1.94384) * 10);
  ais.accuracy = 0; // > 10 meters.
  ais.longitude = (uint32_t) (gpsdata->fix.longitude * 600000.00);
  ais.latitude =  (uint32_t) (gpsdata->fix.latitude * 600000.00);
  ais.course = (uint16_t) gpsdata->fix.track / 0.1;
  ais.heading =  511; // (uint16_t) gpsdata->fix.track;
  ais.second = date.tm_sec;
  ais.regional = 0; // none.
  memset(ais.shipname, ' ', AIS_SHIPNAME_MAXLEN);
  memcpy(ais.shipname, pShipName, (strlen(pShipName) < 20 ? strlen(pShipName) : 20 ));
  for(int i = 0; i< AIS_SHIPNAME_MAXLEN; i++) {
    ais.shipname[i] = toupper(ais.shipname[i]);
  }
  ais.shiptype = 41; // TO CHECK
  ais.to_bow = 1; // fixed dimension in meters.
  ais.to_stern = 1;
  ais.to_port = 1;
  ais.to_starboard = 1;
  ais.epfd = 6; // Integrated navigation system
  ais.raim = 0; // Receiver Autonomous Integrity Monitoring = not use.
  ais.dte = 1;
  ais.assigned = 0;
  ais.spare = 0;  
  
  // Encode the message:
  unsigned int val = ais_binary_encode(&ais, ais_encoded);

  // Make the sentence:
  sprintf(pBuffer, GPSD2NMEA_AIS_SENTENCE, (char *) ais_encoded);

  // Append CRC:
  int length = strlen(pBuffer);
  int CRC = gpsd2nmea_getCRC(pBuffer, length);
  sprintf(pBuffer+length, "%02X\r\n", CRC);
  
  return strlen(pBuffer);
}
