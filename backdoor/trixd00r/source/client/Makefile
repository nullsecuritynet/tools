################################################################################
#                 ____                     _ __                                #
#      ___  __ __/ / /__ ___ ______ ______(_) /___ __                          #
#     / _ \/ // / / (_-</ -_) __/ // / __/ / __/ // /                          #
#    /_//_/\_,_/_/_/___/\__/\__/\_,_/_/ /_/\__/\_, /                           #
#                                             /___/ nullsecurity team          #
#                                                                              #
# trixd00r - Advanced and invisible TCP/IP based userland backdoor             #
#                                                                              #
# FILE                                                                         #
# client/Makefile                                                              #
#                                                                              #
# DATE                                                                         #
# 02/10/2012                                                                   #
#                                                                              #
# DESCRIPTION                                                                  #
# trixd00r is an advanced and invisible userland backdoor based on TCP/IP for  #
# UNIX systems. It consists of a server and a client. The Server sits and      #
# waits for magic packets using a sniffer (pcap). If a magic packet arrives,   #
# it will bind a shell over TCP or UDP on the given port or connecting back to #
# the client again over TCP or UDP. The client is used to send magic packets   #
# to trigger the server and get a shell.                                       #
#                                                                              #
# COPYRIGHT                                                                    #
# Read docs/COPYING.                                                           #
#                                                                              #
# AUTHOR                                                                       #
# noptrix - http://www.nullsecurity.net/                                       #
#                                                                              #
################################################################################

.EXPORT_ALL_VARIABLES:
	@echo "[-] ERROR: you need GNU make!"

CC = gcc
RM = rm -rf
MV = mv
STRIP = strip

INSTDIR = /usr/local/bin

INCDIR = client/inc
SRCDIR = client/src

CFLAGS = -W -Wall -Wextra -O2 -pedantic -I $(INCDIR)

OBJS = $(SRCDIR)/trixd00r.o $(SRCDIR)/help.o $(SRCDIR)/error.o \
	   $(SRCDIR)/checks.o $(SRCDIR)/controller.o $(SRCDIR)/packet.o \
	   $(SRCDIR)/shell.o $(SRCDIR)/wrapper.o $(SRCDIR)/signals.o

$(SRCDIR)%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

trixd00r: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	$(STRIP) trixd00r

clean:
	$(RM) trixd00r $(SRCDIR)/*.o *.o *.core core

# EOF
