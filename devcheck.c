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
   struct sockaddr *sa;
   struct sockaddr_in *sin;
   struct ifreq ifr;

   char *toldaddr;

   toldaddr = oldaddr;

   fd = socket(PF_INET,SOCK_STREAM,0);
   if (fd == -1) {
      pdebug("socket() failed");
      return -1;
   }
       
   /* Copy the interface name into the buffer */
   strncpy(ifr.ifr_name,dev,IFNAMSIZ);
        
   if (ioctl(fd,SIOCGIFADDR,&ifr) == -1) {
      pdebug("device ioctl failed");
      oldaddr = '\0'; // Hackaratus?
      return -1;
   }
   /* Now the buffer will contain the information we requested */
   sa = (struct sockaddr *)&(ifr.ifr_addr);
   if (sa->sa_family == AF_INET) {
      sin = (struct sockaddr_in*) sa;
      pdebug(toldaddr);
      if (inet_ntoa(sin->sin_addr) != toldaddr) {
         oldaddr = inet_ntoa(sin->sin_addr); 
         close(fd);
         return 1;
      } else {
         pdebug(inet_ntoa(sin->sin_addr));
         close(fd);
         return 0;
      }
   } else {
      printf(": Unknown Address Family (%d)\n",sa->sa_family);
   }

   close(fd);
   return -1;
}
