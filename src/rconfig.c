/*
   DHSD rconfig.c - Config file parser
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "dhsd.h"

void parseline(char *strbuf)
{
  int strl;
  int i = 0;
  char cmd[10];
  char arg[10];

  bzero(cmd,10);
  bzero(arg,10);

  strl = strlen(strbuf);

  if (strl > 0) {
    if (strbuf[0] == '#') return;
    if (strbuf[0] == '\n') return;
    while (i < strl) {
      if (strbuf[i] == '=') {
        strncpy(cmd, &strbuf[0], i);
        strncpy(arg, &strbuf[i+1], strlen(strbuf)-i-1);
        cmd[strlen(cmd)] = '\0';
        arg[strlen(arg)-1] = '\0';
        break;
      }
      i++;
    };  

    if(strcmp("username",cmd) == 0) { strncat(dhs_username, arg, strlen(arg)); return; }
    if(strcmp("passwd",cmd) == 0) { strncat(dhs_passwd, arg, strlen(arg)); return; }
    if(strcmp("host",cmd) == 0) { strncat(dhs_host, arg, strlen(arg)); return; }
    if(strcmp("domain",cmd) == 0) { strncat(dhs_domain, arg, strlen(arg)); return; }
    if(strcmp("device",cmd) == 0) { strncat(conf_dev, arg, strlen(arg)); return; }
  }
} 

int rconfig(char *filename)
{
  FILE *filefd;
  char strbuf[255];  

  if (filename) 
  {
    filefd = fopen(filename,"r");
  } else {
    filefd = fopen(DHSD_CONFIG,"r");
  }
  if (filefd == 0) {
    return -1;
  }
  /* Zero the strings */
  bzero(dhs_username,10);
  bzero(dhs_passwd,10);
  bzero(dhs_host,10);
  bzero(dhs_domain,10);
  bzero(conf_dev,4);

  /* Start parsing the file */

  while (fgets(strbuf, 255, filefd)) {
    if (strbuf) {
      parseline(strbuf);
    }
  }

  fclose(filefd);

  return 0;
}
  
