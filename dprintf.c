#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#define BUFLEN 4096

void dprintf(int fd, char *format, ...)
{
   va_list arglist;
   char buffer[BUFLEN];

   va_start(arglist, format);
   vsnprintf(buffer, BUFLEN, format, arglist);
   va_end(arglist);

   write(fd, buffer, strlen(buffer));
}
