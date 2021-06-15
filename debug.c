// SegFault & Debug handler - By Nik_Doof

#include "dhsd.h"
#include <stdio.h>
#include <unistd.h>

void han_segv()
{
#ifdef USE_SYSLOG
   closelog();
#endif
   close(sockfd);
   pdebug("SegFault in DHSD...exiting safely...");
   //return 1;
   exit(0);
}

void pdebug(char *dmsg)
{
#ifdef DEBUG
   printf("DhsD Debug: %s\n",dmsg);
#endif
}  

