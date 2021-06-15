/*
   DHSD dhsd.h - Source header
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

#ifndef __DHSD_H__
#define __DHSD_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define DHSD_CONFIG "/etc/dhsd.conf"

#define DHS_IP "209.249.50.99"
#define DHS_PORT 80
#define DHS_ADDR "/nic/hosts?"

#define SLEEP_TIME 10

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif

// pdetach.c - by dyfet@sourceforge.net
void pdetach(void);

// dprintf.c - by dfx@sourceforge.net
void dprintf(int fd, char *format, ...);

// devcheck.c
char *oldaddr;
int devcheck(char *dev);

// updateip.c
int sockfd;
int updateip(char *ipaddr, char *dhsdom, char *dhshost);

// debug.c
void han_segv();
void pdebug(char *dmsg);

// base64.c - Snarfed from DHSdynupdate
void encbasic( char *d, const char *u, const char *p );

// rconfig.c
char dhs_username[10];
char dhs_passwd[10];
char dhs_host[10];
char dhs_domain[10];
char conf_dev[10];
int rconfig(char *filename);

#endif
