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

void sig_hup()
{
#ifdef USE_SYSLOG
  syslog(LOG_NOTICE,"HUP recived, reloading config");
#endif
  pdebug("Reloading config");
  if (rconfig(dhsd_config)) {
#ifdef USE_SYSLOG
    syslog(LOG_NOTICE,"Config file cant be found, or invalid...exiting");
#endif
    pdebug("Config not found!");
    exit(0);
  }
}

int main(int argc, char **argv)
{
  int i, res, doext;

#ifndef DEBUG
  pdetach();
  signal(SIGSEGV,han_segv);
#endif
  signal(SIGINT,cleanup);
  signal(SIGKILL,cleanup);
  signal(SIGHUP,sig_hup);

#ifdef HAVE_SYSLOG_H
  openlog("dhsd",LOG_PID,LOG_DAEMON);
  syslog(LOG_NOTICE,"DHSD %s loaded...",VERSION);
#endif

  /* Loop here for multiple argvs */

  if(argv[1]) {
    strncpy(dhsd_config,argv[1],strlen(argv[1]));
  } else {
    strncpy(dhsd_config,DHSD_DEF_CONFIG,strlen(DHSD_DEF_CONFIG));
  }
  if (rconfig(dhsd_config)) {
#ifdef USE_SYSLOG
    syslog(LOG_NOTICE,"Config file cant be found, or invalid...exiting");
#endif
    pdebug("Config not found!");
    exit(0);
  }  

  do {
    if (devcheck(config.device)) {
#ifdef HAVE_SYSLOG_H  
      syslog(LOG_NOTICE,"device ip has changed, update initiated...");
#endif

      i = 0;

      do {
        while(updateip(coldaddr,config.host[i])) {;
          sleep(SLEEP_TIME);
        };
        i++;
      } while(i != config.noofhosts);
    
    }
    doext = 0;
    sleep(SLEEP_TIME);
  } while(!doext); 
  
  cleanup();
  return 0;
}
