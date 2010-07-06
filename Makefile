CFLAGS += -std=c99
CPPFLAGS += -Iinclude

PROGS = basename/basename cat/cat cksum/cksum

.PHONY: all clean

all: ${PROGS}

clean:
	rm -f ${PROGS}
