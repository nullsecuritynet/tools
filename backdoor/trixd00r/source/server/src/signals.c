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
 * server/src/signals.c                                                        *
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

#include "signals.h"
#include "error.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


/* signal() predates POSIX, therefore we use wrapper for sigaction().
 * ripped from UNPv1. thx to W. R. STEVENS (R.I.P.) */
sigfunc *xsignal(int signo, sigfunc *func)
{
    struct sigaction act, oact;

    
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (signo == SIGALRM) {
        #ifdef SA_INTERRUPT
            /* SUN OS */
            act.sa_flags |= SA_INTERRUPT;
        #endif
    } else {
        #ifdef SA_RESTART
            /* BSD */
            act.sa_flags |= SA_RESTART;
        #endif
    }
    if (sigaction(signo, &act, &oact) < 0) {
        return SIG_ERR;
    }

    return oact.sa_handler;
}


/* we don't want to have zombies */
void sig_chld()
{
   pid_t pid = 0;
   int stat = 0;


   if ((pid = waitpid(-1, &stat, WNOHANG)) < 0) {
      if (errno != ECHILD) {
         print_error(ERR_GEN);
      }
   }

   return;
}


/* we only want to interrupt the connect() - to get EINTR and then assign
 * ETIMEDOUT to errno */
void connect_alarm()
{
    return;
}


/* cleanup for SIGINT/SIGTERM */
void cleanup()
{
    return;
}

/* EOF */
