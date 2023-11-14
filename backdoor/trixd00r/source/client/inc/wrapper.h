/*******************************************************************************
 *                ____                     _ __                                *
 *     ___  __ __/ / /__ ___ ______ ______(_) /___ __                          *
 *    / _ \/ // / / (_-</ -_) __/ // / __/ / __/ // /                          *
 *   /_//_/\_,_/_/_/___/\__/\__/\_,_/_/ /_/\__/\_, /                           *
 *                                            /___/ nullsecurity team          *
 *                                                                             *
 * trixd00r - Advanced and invisible TCP/IP based userland backdoor            *
 *                                                                             *
 * FILE                                                                        *
 * client/inc/wrapper.h                                                        *
 *                                                                             *
 * DATE                                                                        *
 * 02/10/2012                                                                  *
 *                                                                             *
 * DESCRIPTION                                                                 *
 * trixd00r is an advanced and invisible userland backdoor based on TCP/IP for *
 * UNIX systems. It consists of a server and a client. The server sits and     *
 * waits for magic packets using a sniffer. If a magic packet arrives, it will *
 * bind a shell over TCP or UDP on the given port or connecting back to the    *
 * client again over TCP or UDP. The client is used to send magic packets to   *
 * trigger the server and get a shell.                                         *
 *                                                                             *
 * COPYRIGHT                                                                   *
 * Read docs/COPYING.                                                          *
 *                                                                             *
 * AUTHOR                                                                      *
 * noptrix - http://www.nullsecurity.net/                                      *
 *                                                                             *
 ******************************************************************************/

#ifndef __WRAPER_H__
#define __WRAPER_H__


#define _BSD_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pcap.h>


/* memory */
void *xmalloc(size_t);
void *xmemset(void *, int, size_t);
void *xmemcpy(void *, const void *, size_t);
void *alloc_buff(unsigned int);


/* file */
void xclose(int);
int xdup2(int, int);
void xselect(int, fd_set *, fd_set *, fd_set *, struct timeval *);
void xpipe(int *);


/* net */
struct hostent *xgethostbyname(const char *);
int xsocket(int, int, int);
void xconnect(int, const struct sockaddr *, socklen_t);
void xsendto(int, const void *, size_t, int, const struct sockaddr *, socklen_t);
ssize_t xrecv(int, void *, size_t, int);
ssize_t xwrite(int, const void *, size_t);
ssize_t xread(int, void *, size_t);
void xsetsockopt(int, int, int, const void *, socklen_t);
void xinet_ntop(int, const void *, char *, socklen_t);
void xbind(int, const struct sockaddr *, socklen_t);
void xlisten(int, int);
int xaccept(int, struct sockaddr *, socklen_t *);



/* misc */
void xsystem(const char *);


/* own */
char *convert_host(char *);

#endif

/* EOF */
