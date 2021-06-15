
#ifndef __DHSD_H__
#define __DHSD_H__

#include "config.h"

#define DHSD_VERSION "v0.01 Beta"                
#define DHSD_INT_VERSION 0001

#define DHS_IP "209.249.50.99"
#define DHS_PORT 80
#define DHS_ADDR "/nic/hosts?"

#ifdef USE_SYSLOG
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

// base64.c
void encbasic( char *d, const char *u, const char *p );

#endif
