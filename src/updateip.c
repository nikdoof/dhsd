/*
   DHSD updateip.c - tcp/http update code
   Copyright (C) 2000 Andrew Williams
    
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
    
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
   
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "dhsd.h"

int sconnect(char *ip)
{
   int isock;
   struct sockaddr_in srvaddr;
   
   isock = socket(AF_INET, SOCK_STREAM, 0);
   srvaddr.sin_family = AF_INET;
   srvaddr.sin_port = htons(DHS_PORT);
   srvaddr.sin_addr.s_addr = inet_addr(ip);
   bzero(&(srvaddr.sin_zero),8);

   if (connect(isock, (struct sockaddr *)&srvaddr,sizeof(struct sockaddr)) == -1) {
     perror("connect");
     close(isock);
     return -1;
   }
   return isock;
}

int updateip(char *ipaddr, char *dhsdom, char *dhshost)
{
  char post_data[128];
  char post_head[128];
  char auth_head[96];
  char chostname[64];
  char tmpdatabuf[128];
  char databuf[8192];
  char outbuf[2048];
  char b64str[64];
  int ibytes;

  encbasic(b64str,dhs_username,dhs_passwd);
  sprintf(chostname,"%s.%s",dhshost,dhsdom);

  sprintf(auth_head,"Authorization: Basic %s\r\n",b64str);
 
  sprintf(post_data,"%stype=4&updatetype=Online&hostscmd=edit&hostscmdstage=2&ip=%s&mx=%s&domain=%s&hostname=%s",DHS_ADDR,ipaddr,chostname,dhsdom,dhshost);
  sprintf(post_head,"GET %s HTTP/1.0\r\n",post_data);

  sockfd = sconnect(DHS_IP);
  if (sockfd == -1) {
    pdebug("sconnect() failed");
    return -1;
  } 

  sprintf(outbuf,"%s%s\r\n\r\n",post_head,auth_head);

  //pdebug(outbuf);

  send(sockfd,outbuf,strlen(outbuf),0);

  do {
    ibytes = recv(sockfd, tmpdatabuf, 128, 0); 
    if (ibytes > 0) {
      tmpdatabuf[ibytes] = '\0';
      strncat(databuf,tmpdatabuf,strlen(tmpdatabuf));
    }
  } while (ibytes > 0);
  pdebug(databuf);

  fclose(sockfd);

  /* Some error checking code could be handy... */

  return 0;
}
 