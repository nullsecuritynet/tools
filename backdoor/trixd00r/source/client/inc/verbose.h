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
 * client/inc/verbose.h                                                        *
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

#define __IF_VERBOSE_CTRL   if (ctrl->verbose)
#define __IF_VERBOSE_PKT    if (packet->verbose)

#define __VERBOSE_ARGS  __IF_VERBOSE_CTRL \
    printf("[+] "BLUE"checking arguments"NORM"\n");

#define __VERBOSE_BUILD __IF_VERBOSE_CTRL \
    printf("[+] "BLUE"building magic packet"NORM"\n");

#define __VERBOSE_SEND  __IF_VERBOSE_CTRL \
    printf("[+] "BLUE"sending magic packet"NORM"\n");

#define __VERBOSE_CONNECTED(host,port)  __IF_VERBOSE_PKT \
    printf("--- "YELLOW"connected to %s:%u"NORM"\n", host, port);

#define __VERBOSE_GOT_CONNECTION(host)  __IF_VERBOSE_PKT \
    printf("--- "YELLOW"got a connection from %s"NORM"\n", host);

#define __VERBOSE_GOT_SHELL __IF_VERBOSE_PKT \
    printf("--- "YELLOW"w00t, we've got a shell"NORM"\n");

#define __VERBOSE_END   __IF_VERBOSE_CTRL \
    printf("[+] ending trixd00r"NORM"\n");

#endif

/* EOF */
