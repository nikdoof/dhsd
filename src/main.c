/*
   DHSD main.c - Main exec procedure
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
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "dhsd.h"

void cleanup()              
{
#ifdef HAVE_SYSLOG_H
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

  printf("DhsD %s\n\n",VERSION);
  printf("Detaching to daemon...\n");
#ifndef DEBUG
  pdetach();
#endif
  signal(SIGINT,cleanup);
  signal(SIGKILL,cleanup);
#ifndef DEBUG
  signal(SIGSEGV,han_segv);
#endif

#ifdef HAVE_SYSLOG_H
  verstring[0] = '\0';
  openlog("dhsd",LOG_PID,LOG_DAEMON);
  sprintf(verstring, "DhsD %s loaded...",VERSION);
  syslog(LOG_NOTICE,verstring);
#endif

  res = rconfig(DHSD_CONFIG);
  if (res == -1) {
#ifdef USE_SYSLOG
    syslog(LOG_ERR,"Config file cant be found, or invalid...exiting");
#endif
    pdebug("Config not found!");
    exit(0);
  }  

  do {
    if (devcheck(conf_dev) == 1) {
#ifdef HAVE_SYSLOG_H  
      syslog(LOG_NOTICE,"device ip has changed, update initiated...");
#endif
      do {
       res = updateip(oldaddr,dhs_domain,dhs_host);
       if (res != 0) {
          sleep(SLEEP_TIME);
       }
      } while(res != 0);  
#ifdef HAVE_SYSLOG_H
      syslog(LOG_NOTICE,"update complete");
#endif
    }
    doext = 0;
    sleep(SLEEP_TIME);
  } while(doext == 0); 
  
  cleanup();
  return 0;
}
