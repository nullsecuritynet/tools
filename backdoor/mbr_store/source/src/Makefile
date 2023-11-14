# mbr_store src/ Makefile

BINDIR		= ../bin/
INCLUDES    = -I../include
CFLAGS		= -O2 -std=c99 -Wall -Wextra -pedantic
LINKFLAGS	= -lnettle

all: mbr_store

mbr_store: mbr_store.c optget.c
	$(CC) *.c -o $(BINDIR)mbr_store $(CFLAGS) $(LINKFLAGS) $(INCLUDES)

clean:
	rm -f mbr_store
