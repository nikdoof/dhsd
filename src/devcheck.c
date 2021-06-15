/*
   DHSD devcheck.c - Device checker
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

#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "dhsd.h"


int devcheck(char *dev)
{
   int fd;
   int strl;
   struct sockaddr *sa;
   struct sockaddr_in *sin;
   struct ifreq ifr;

   fd = socket(PF_INET,SOCK_STREAM,0);
   if (fd == -1) {
      pdebug("socket() failed");
      return -1;
   }
       
   /* Copy the interface name into the buffer */
   strncpy(ifr.ifr_name,dev,IFNAMSIZ);
        
   if (ioctl(fd,SIOCGIFADDR,&ifr)) {
      pdebug("device is not up/online");
      oldaddr = 0; // Kludges r' us?
      return -1;
   }
   /* Now the buffer will contain the information we requested */
   sa = (struct sockaddr *)&(ifr.ifr_addr);
   if (sa->sa_family == AF_INET) {
      sin = (struct sockaddr_in*) sa;
      if (inet_addr(inet_ntoa(sin->sin_addr)) != oldaddr) {
   	 oldaddr = inet_addr(inet_ntoa(sin->sin_addr));
         strl = strlen(inet_ntoa(sin->sin_addr)); 
         strncpy(coldaddr,inet_ntoa(sin->sin_addr),strl);
         close(fd);
         return 1;
      } else {
         close(fd);
         return 0;
      }
   } else {
      pdebug("Unknown Address Family");
   }

   close(fd);
   return -1;
}
