# -*- Makefile -*-
#
# $Id: Makefile 801 2005-09-08 00:39:46Z dsevilla $
#
CFLAGS=-O2 -ansi -pedantic -Wall -g

#ALL = $(patsubst %.c,%,$(wildcard *.c))

ALL= slr

all: $(ALL)

clean:
	-rm -rf *.o $(ALL)  core *~

.PHONY: clean all
