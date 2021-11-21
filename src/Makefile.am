PKG_CONFIG ?= pkg-config

AM_CFLAGS = -I$(top_srcdir)/src `$(PKG_CONFIG) --cflags jack` -pthread $(SAMPLERATE_CFLAGS)
jclient_LDFLAGS = `$(PKG_CONFIG) --libs jack` $(SAMPLERATE_LIBS)

bin_PROGRAMS = jclient
jclient_SOURCES = jclient.c jclient.h utils.c utils.h

AM_CPPFLAGS = -Wall -O3
