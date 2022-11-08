# vim:set fileencoding=utf8 fileformat=unix filetype=makefile tabstop=2 noexpandtab:
# $Id: Makefile,v 1.4 2022/02/23 09:37:56 ralph Exp $

CC=/usr/bin/gcc
CFLAGS=-c -Wall -O2 -s
RM=/bin/rm
LDFLAGS=
SOURCES=tainted.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=tainted

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	strip $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(EXECUTABLE) $(OBJECTS)

## should run everywhere!
static: $(EXECUTABLE)_static $(OBJECTS)

$(EXECUTABLE)_static:
	$(CC) --static $(LDFLAGS) $(OBJECTS) -o $@
	strip $@

changelog:
	gitchangelog > ChangeLog.txt
	git commit -a -s -m "chg: Updated Changelog (by Makefile)"
	cat ChangeLog.txt
