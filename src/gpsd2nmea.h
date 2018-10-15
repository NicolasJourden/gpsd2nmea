/**
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Nicolas JOURDEN <nicolas.jourden@laposte.net>
 */

#ifndef __GPSD2NMEA__

#define __GPSD2NMEA__		"1"

#include <assert.h>
#include <gps.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define GPSD_SERVER_IP			"127.0.0.1"
#define GPSD_SERVER_PORT		"2947"
#define GPSD_CONFIG			"?WATCH={\"enable\":true,\"json\":true,\"nmea\":false}\r\n"

#define GPSD2NMEA_TLL_SERVER_IP		"192.168.0.1"
#define GPSD2NMEA_TLL_SERVER_PORT	1234
#define GPSD2NMEA_TLL_HEAD_ID		"GP"
#define GPSD2NMEA_TLL_SENTENCE		"$%sTLL,%02d,%02d%02.5f,%s,%03d0%02.5f,%s,%s,%02d%02d%02d.00,T,,*"
#define GPSD2NMEA_AIS_SENTENCE		"!AIVDM,1,1,,A,%s,0*"
#define GPSD2NMEA_POLL_DELAY		(60*1000)
#define GPSD2NMEA_BUFFER_SIZE		2048
#define GPSD2NMEA_USAGE			"h?dD:H:P:t:p:N:i:a"

void gpsd2nmea_displayUsage(const struct option * pLong_opts, int pSize);

int gpsd2nmea_getCRC(const char * pBuffer, int len);

int gpsd2nmea_sendUDP(const char * pBuffer, int len, char * pAddress, int pPort);

int gpsd2nmea_setTLLStr(char * pBuffer, struct gps_data_t * gpsdata, unsigned int pId, char * pName);

int gpsd2nmea_setAISStr_msg19(char * pBuffer, struct gps_data_t * gpsdata, unsigned int pId, const char * pName);

int gpsd2nmea_setAISStr_msg1(char * pBuffer, struct gps_data_t * gpsdata, unsigned int pId);

unsigned int gpsd2nmea_getDegree(double pDegree);
double gpsd2nmea_getMinutes(double pDegree);

char * gpsd2nmea_getSignLat(double pDegree);
char * gpsd2nmea_getSignLon(double pDegree);

typedef struct {
    // Common elements:
    uint8_t type;
    uint8_t repeat;
    uint32_t mmsi;
    uint16_t speed;
    uint8_t accuracy;
    int32_t longitude;
    int32_t latitude;  
    uint16_t course;
    uint16_t heading;
    uint8_t second;
    uint8_t spare;
    uint8_t raim;

    // Message type 19:
    uint8_t reserved;
    uint8_t regional;
    uint8_t shipname[20];
    uint8_t shiptype;
    uint16_t to_bow;
    uint16_t to_stern;
    uint16_t to_port;
    uint16_t to_starboard;
    uint8_t epfd;
    uint8_t dte;
    uint8_t assigned;

    // Message type 1:
    uint8_t navigation_status;
    int8_t rate_of_turn;
    uint8_t maneuver;
    uint32_t radio;
} t_gpsd2nmea_ais_msg;
#define t_gpsd2nmea_ais_msg_size	(sizeof(t_gpsd2nmea_ais_msg))
#define t_gpsd2nmea_endoded_msg19_size	(312/6)
#define t_gpsd2nmea_endoded_msg1_size	(168/6)

#endif
