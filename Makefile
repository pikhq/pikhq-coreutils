CFLAGS += -std=c99
CPPFLAGS += -Iinclude

PROGS = basename/basename cal/cal cat/cat cksum/cksum
PIKHQBOX_SOURCES = basename/basename.c cal/cal.c cat/cat.c cksum/cksum.c

.PHONY: all clean

all: ${PROGS}

pikhqbox.c: ${PIKHQBOX_SOURCES}

clean:
	rm -f ${PROGS} pikhqbox
