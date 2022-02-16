# vim:set fileencoding=utf8 fileformat=unix filetype=makefile tabstop=2 noexpandtab:
# $Id: Makefile,v 1.2 2022/02/16 20:20:14 ralph Exp $

C=/usr/bin/gcc
CFLAGS=-c -Wall -O2 -s
RM=/bin/rm
LDFLAGS=
SOURCES=tainted.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=tainted

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(EXECUTABLE) $(OBJECTS)
