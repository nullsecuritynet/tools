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
 * server/inc/controller.h                                                     *
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


#include "trixd00rd.h"
#include "sniffer.h"


/* some boolean options */
#define DAEMON      1       /* daemonize trixd00rd */
#define BANNER_ON   1       /* send welcome and bye banner */
#define BANNER_OFF  0       /* do not send welcome and byb banner */
#define SSL         1       /* use ssl */
#define VERBOSE     1       /* verbose mode */
#define QUIET       0       /* quiet mode */


/* free used memory for trixd00rd */
void free_trixd00rd(ctrl_t *);


/* ignore or handle some signals */
void install_signals();


/* allocate buffer for each struct and fill in with zeros */
ctrl_t *alloc_structs();


/* set default values for ctrl_t */
ctrl_t *set_ctrl_defaults(ctrl_t *);


/* begin of trixd00rd */
void start_trixd00rd(ctrl_t *);


/* end of trixd00rd */
void end_trixd00rd(ctrl_t *);

#endif

/* EOF */
