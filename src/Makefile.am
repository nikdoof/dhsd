prefix = /usr

EXTRA_DIST = config.h.in stamp-h.in

sbin_PROGRAMS = dhsd
dhsd_SOURCES = main.c dhsd.h rconfig.c pdetach.c updateip.c devcheck.c base64.c debug.c utils.c

if DEBUG
  DBG = -DDEBUG=1
  CFLAGS = @CFLAGS@ $(DBG)
else
  DBG =
  CFLAGS = @CFLAGS@ 
endif




