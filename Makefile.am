ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS=-Wall -Werror -g
AM_CFLAGS+=-D DEBUG
AM_CFLAGS+=-D WARNING
# Uncomment if performance when running with -DDEBUG is an issue
# Warning, if uncommented, issuing calls to debug functions during options
# parsing might result in debug flag incorrectly set to 0 for some files
#AM_CFLAGS+=-D CACHE_DEBUG_FLAG



SUBDIRS=

bin_PROGRAMS=
bin_PROGRAMS+=phase1
bin_PROGRAMS+=phase2


COMMON=debug.h debug.c util.h util.c getelf.c display.c

EXTRA_DIST=filtre.pl
phase1_SOURCES=$(COMMON) phase1.c 
phase2_SOURCES=$(COMMON) phase2.c writeelf.c fusionelf.c

EXTRA_DIST+=

