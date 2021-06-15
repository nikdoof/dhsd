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
#include <stdlib.h>
#include <string.h>
#include "dhsd.h"

void parseline(char *strbuf)
{
  int strl;
  int i = 0;
  char cmd[20];
  char arg[20];

  bzero(cmd,sizeof(cmd));
  bzero(arg,sizeof(arg));

  //filtercr(strbuf);
  strbuf[strlen(strbuf)-1] = '\0';

  splitstr(strbuf, '=', cmd, arg);

  if(!strcasecmp("USER",cmd)) { 
    strncat(config.username, arg, strlen(arg));
    return; 
  }
  if(!strcasecmp("PASS",cmd)) { 
    strncat(config.passwd, arg, strlen(arg)); 
    return; 
  }
  if(!strcasecmp("HOST",cmd)) {
    strncat(config.host[config.noofhosts], arg, strlen(arg));
    config.noofhosts++; 
    return;
  }
  if(!strcasecmp("DEV",cmd)) {
    strncat(config.device, arg, strlen(arg));
    return;
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
    filefd = fopen(DHSD_DEF_CONFIG,"r");
  }
  if (!filefd) {
    return -1;
  }
  /* Zero the strings */
  bzero(config.username,sizeof(config.username));
  bzero(config.passwd,sizeof(config.passwd));
  bzero(config.host,sizeof(config.host));
  bzero(config.device,sizeof(config.device));
  config.noofhosts = 0;

  /* Start parsing the file */

  while (fgets(strbuf, 255, filefd) != NULL) {
    if (strbuf) {
      parseline(strbuf);
    }
  }

  fclose(filefd);

  if(config.username && config.passwd && config.device && config.noofhosts) { 
    return 0;
  } else {
    return -1;
  }

}
  
