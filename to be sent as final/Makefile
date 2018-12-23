# =========================================================================
# Makefile for the (de)cipher/segment problem
#
# To custimize it, add/replace the dependencies after the shifts/segment
# targets. You can also add other rules.
#
# WARNING: the compilation/linking flag cannot be edited !
# =========================================================================
CC=gcc
LD=gcc
CFLAGS=--std=c99 --pedantic -Wall -W -Wmissing-prototypes -DNDEBUG
LDFLAGS=-lm

all: cipher decipher shifts segment

.c .o:
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f *.o


cipher: cipher.o IOHandler.o Matrix.o
	$(LD) -o $@ $^ $(LDFLAGS)

decipher: decipher.o IOHandler.o Matrix.o
	$(LD) -o $@ $^ $(LDFLAGS)

# TODO: edit
shifts: mainShift.o shifts.o IOHandler.o
	$(LD) -o $@ $^ $(LDFLAGS)

# TODO: edit
segment: mainSegment.o segment.o HashTable.o maxSegmentHandling.o 
	$(LD) -o $@ $^ $(LDFLAGS)
