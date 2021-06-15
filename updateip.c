#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "dhsd.h"

int sconnect(char *ip)
{
   int isock;
   struct sockaddr_in srvaddr;
   
   isock = socket(AF_INET, SOCK_STREAM, 0);
   srvaddr.sin_family = AF_INET;
   srvaddr.sin_port = htons(DHS_PORT);
   srvaddr.sin_addr.s_addr = inet_addr(ip);
   bzero(&(srvaddr.sin_zero),8);

   if (connect(isock, (struct sockaddr *)&srvaddr,sizeof(struct sockaddr)) == -1) {
     perror("connect");
     close(isock);
     return -1;
   }
   return isock;
}

int updateip(char *ipaddr, char *dhsdom, char *dhshost)
{
  char post_data[255];
  char post_head[64];
  char auth_head[128];
  char chostname[50];
  char tmpdatabuf[8192];
  char databuf[4*8192];
  char outbuf[8192];
  char b64str[255];
  int ibytes;

  encbasic(b64str,USR_LOGIN,USR_PASSWD);
  sprintf(chostname,"%s.%s",dhshost,dhsdom);

  sprintf(auth_head,"Authorization: Basic %s\r\n",b64str);
 
  sprintf(post_data,"%stype=4&updatetype=Online&hostscmd=edit&hostscmdstage=2&ip=%s&mx=%s&domain=%s&hostname=%s",DHS_ADDR,ipaddr,chostname,dhsdom,dhshost);
  sprintf(post_head,"GET %s HTTP/1.0\r\n",post_data);
 
  sockfd = sconnect(DHS_IP);
  if (sockfd == -1) {
    pdebug("sconnect() failed");
    return -1;
  } 

  sprintf(outbuf,"%s%s\r\n\r\n",post_head,auth_head);

  send(sockfd,outbuf,strlen(outbuf),0);

  do {
    ibytes = recv(sockfd, tmpdatabuf, 8192, 0); 
    if (ibytes > 0) {
      tmpdatabuf[ibytes] = '\0';
      strncat(databuf,tmpdatabuf,strlen(tmpdatabuf));
    }
  } while (ibytes > 0);
  pdebug(tmpdatabuf);

  //close(sockfd);

  pdebug("socket closed");

  /* Some error checking code could be handy... */

  return 0;
}
 
