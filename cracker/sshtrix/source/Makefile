###############################################################################
# sshtrix - a very fast multithreaded SSH login cracker                       #
#                                                                             #
# Makefile                                                                    #
#                                                                             #
# Copyright (C) 2011 noptrix - http://www.noptrix.net/                        #
#                                                                             #
# This program is free software: you can redistribute it and/or modify        #
# it under the terms of the GNU General Public License as published by        #
# the Free Software Foundation, either version 3 of the License, or           #
# (at your option) any later version.                                         #
#                                                                             #
# This program is distributed in the hope that it will be useful,             #
# but WITHOUT ANY WARRANTY; without even the implied warranty of              #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                #
# GNU General Public License for more details.                                #
#                                                                             #
# You should have received a copy of the GNU General Public License           #
# along with this program. If not, see <http://www.gnu.org/licenses/>.        #
#                                                                             #
###############################################################################


.EXPORT_ALL_VARIABLES:
	@echo "[-] ERROR: you need gnu make!"


CC = gcc
RM = rm -rf
MV = mv
CP = cp
MKDIR = mkdir


DISTDIR = /usr/local
INCDIR = inc
SRCDIR = src


CFLAGS = -W -Wall -Wextra -O2 -std=c99 -pedantic -I $(INCDIR)


OBJS = $(SRCDIR)/sshtrix.o $(SRCDIR)/file.o $(SRCDIR)/help.o $(SRCDIR)/error.o \
	   $(SRCDIR)/checks.o $(SRCDIR)/init.o $(SRCDIR)/ssh.o $(SRCDIR)/signals.o \
	   $(SRCDIR)/wrapper.o $(SRCDIR)/job.o $(SRCDIR)/report.o \
	   $(SRCDIR)/wordlist.o


$(SRCDIR)%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)


default:
	@echo "+-----------------------------------+"
	@echo "| sshtrix - http://www.noptrix.net/ |"
	@echo "+-----------------------------------+"
	@echo
	@echo "[-] ERROR: type one of the following:"
	@echo
	@echo "[+] make sshtrix     - build sshtrix"
	@echo "[+] make install     - install sshtrix" 
	@echo "[+] make clean       - clean all"
	@echo
	@echo "[!] NOTES"
	@echo "- you need libssh (http://www.libssh.org/) version >= 0.5.0"
	@echo "- if you want to choose another prefix, set DISTDIR env"


sshtrix: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) -pthread -lssh


install:
	$(MKDIR) -p $(DISTDIR)/share/sshtrix
	$(MKDIR) -p $(DISTDIR)/share/man/man1
	$(MV) sshtrix $(DISTDIR)/bin
	$(CP) -r doc/ $(DISTDIR)/share/sshtrix/
	$(CP) doc/sshtrix.1 $(DISTDIR)/share/man/man1/


clean:
	$(RM) sshtrix $(SRCDIR)/*.o *.o sshtrix.core core

# EOF
