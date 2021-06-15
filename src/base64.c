/* Original codes shamelessly snarfed from Peter Mac Millan =/
   Few code cleanups using better, more secure functions    */
  
#include <string.h>
#include <stdio.h>
  
static const unsigned char BaseTable[] =
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "abcdefghijklmnopqrstuvwxyz"
                           "0123456789+/";


void benc( unsigned char *dst, unsigned char *s )
{
   int n = strlen( s );
   int n3byt      = n / 3; 
   int k          = n3byt * 3; 
   int nrest      = n % 3;
   int i          = 0;
   int dstlen     = 0;

   while ( i < k )
   {
      dst[dstlen++] = BaseTable[(( s[i]   & 0xFC) >> 2)];
      dst[dstlen++] = BaseTable[(((s[i]   & 0x03) << 4) | ((s[i+1] & 0xF0) >> 4))];
      dst[dstlen++] = BaseTable[(((s[i+1] & 0x0F) << 2) | ((s[i+2] & 0xC0) >> 6))];
      dst[dstlen++] = BaseTable[(  s[i+2] & 0x3F)];

      i += 3;
   }
        
   if (nrest==2)
   {
      dst[dstlen++] = BaseTable[(( s[k] & 0xFC)   >> 2)];
      dst[dstlen++] = BaseTable[(((s[k] & 0x03)   << 4) | ((s[k+1] & 0xF0) >> 4))];
      dst[dstlen++] = BaseTable[(( s[k+1] & 0x0F) << 2)]; 
   }
   else if (nrest==1)
   {
      dst[dstlen++] = BaseTable[((s[k] & 0xFC) >> 2)];
      dst[dstlen++] = BaseTable[((s[k] & 0x03) << 4)];
   }

   while (dstlen%3)
         dst[dstlen++] = '=';

   dst[dstlen] = '\0';
}

void encbasic( char *d, const char *u, const char *p )
{
   char _buf[4*1024];
   
   sprintf( _buf, "%s:%s", u, p);
   benc( d, _buf);
}

