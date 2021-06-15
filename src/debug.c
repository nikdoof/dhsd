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

void han_segv()
{
#ifdef HAVE_SYSLOG_H
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

