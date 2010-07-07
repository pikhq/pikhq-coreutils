CFLAGS += -std=gnu99
CPPFLAGS += -Iinclude

PROGS = base64/base64 basename/basename cal/cal cat/cat cksum/cksum
PIKHQBOX_SOURCES = base64/base64.c basename/basename.c cal/cal.c cat/cat.c cksum/cksum.c

.PHONY: all clean

all: ${PROGS}

pikhqbox: pikhqbox.o

pikhqbox.o: ${PIKHQBOX_SOURCES}

clean:
	rm -f ${PROGS} pikhqbox pikhqbox.o
