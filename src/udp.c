/**
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Nicolas JOURDEN <nicolas.jourden@laposte.net>
 */

#include "gpsd2nmea.h"

int gpsd2nmea_sendUDP(const char * pBuffer, int len, char * pAddress, int pPort)
{ 
  int sockfd;
  struct sockaddr_in server_addr;

  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
  {
    return -1;
  }
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(pPort);
  inet_pton(AF_INET, pAddress, &(server_addr.sin_addr.s_addr));

  ssize_t r;
  r = sendto(sockfd, pBuffer, len, MSG_DONTWAIT | MSG_DONTROUTE, (const struct sockaddr *) &server_addr, sizeof(server_addr));

  close(sockfd);

  return 0;
}
