CFLAGS += -std=c99
CPPFLAGS += -Iinclude

all: cat/cat cksum/cksum

clean:
	rm -f cat/cat cksum/cksum

