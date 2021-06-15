#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "dhsd.h"

void cleanup()              
{
#ifdef USE_SYSLOG
  closelog();
#endif
  close(sockfd);
  exit(0);
}


int main(void)
{
  int doext;
  char verstring[255];
  int res;

  printf("DhsD %s\n\n",DHSD_VERSION);
  printf("Detaching to daemon...\n");
#ifndef DEBUG
  pdetach();
#endif
  signal(SIGINT,cleanup);
  signal(SIGKILL,cleanup);
  signal(SIGSEGV,han_segv);

#ifdef USE_SYSLOG
  verstring[0] = '\0';
  openlog("dhsd",LOG_PID,LOG_DAEMON);
  strncat(verstring, "DhsD ",5);
  strncat(verstring, DHSD_VERSION,strlen(DHSD_VERSION));
  strncat(verstring, " loaded...\0",10);
  syslog(LOG_NOTICE,verstring);
#endif

  do {
    pdebug("loop...");
    if (devcheck(USR_DEV) == 1) {
#ifdef USE_SYSLOG     
      syslog(LOG_NOTICE,"device ip has changed, update initiated...");
#endif
      do {
       res = updateip(oldaddr,USR_DOM,USR_HOST);
       if (res != 0) {
          sleep(SLEEP_TIME);
       }
      } while(res != 0);  
#ifdef USE_SYSLOG
      syslog(LOG_NOTICE,"update complete");
#endif
    }
    doext = 0;
    sleep(SLEEP_TIME);
  } while(doext == 0); 
  
  cleanup();
  return 0;
}
