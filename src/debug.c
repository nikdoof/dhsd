/* 
   DHSD debug.c - Debug handler
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

#include "dhsd.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void han_segv()
{

   /* TODO: add a method of debugging...dunno how! */

#ifdef HAVE_SYSLOG_H
   syslog(LOG_NOTICE,"Segmentation fault...exiting safely...");
   closelog();
#endif
   close(sockfd);
   pdebug("SegFault in DHSD...exiting safely...");
   
   signal(SIGSEGV,SIG_DFL);
}

void pdebug(char *dmsg)
{
#ifdef DEBUG
   
#ifndef HAVE_SYSLOG_H
printf("DHSD Debug: %s\n",dmsg);
#else
syslog(LOG_DEBUG,"DHSD Debug: %s",dmsg);
#endif

#endif
}  

