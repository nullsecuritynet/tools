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
 * client/src/error.c                                                          *
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

#include "error.h"


static void error_fatal(const char *);
/*static void error_warn(const char *);*/
static void sys_error_fatal(const char *);
static void sys_error_warn(const char *);


/* error codes, messages and related functions */
error_t error[] = {
    { ERR_GEN,              ERR_MSG_GEN,            error_fatal },
    { ERR_PKT_TYPE,         ERR_MSG_PKT_TYPE,       error_fatal },
    { ERR_SHELL_MODE,       ERR_MSG_SHELL_MODE,     error_fatal },
    { ERR_HOST,             ERR_MSG_HOST,           error_fatal },
    { ERR_IP,               ERR_MSG_IP,             error_fatal },
    { ERR_PORT,             ERR_MSG_PORT,           error_fatal },
    { ERR_ARGC,             ERR_MSG_ARGC,           error_fatal },
    { ERR_ARGS,             ERR_MSG_ARGS,           error_fatal },
    { ERR_UID,              ERR_MSG_UID,            error_fatal },
    { ERR_PKT_SIZE,         ERR_MSG_PKT_SIZE,       error_fatal },
    { ERR_NET,              ERR_MSG_NET,            sys_error_fatal },
    { ERR_CLOSE,            ERR_MSG_CLOSE,          sys_error_warn }
};


/* generate error messages */
static void error_fatal(const char *msg)
{
    fprintf(stderr, "%s\n", msg);

    __EXIT_FAILURE;

    return;
}


/* generate warning messages
static void error_warn(const char *msg)
{
    fprintf(stderr, "%s\n", msg);

    return;
}*/


/* generate system error messages */
static void sys_error_fatal(const char *msg)
{
    perror(msg);
    
    __EXIT_FAILURE;

    return;
}


/* generate system warning messaes */
static void sys_error_warn(const char *msg)
{
    perror(msg);

    return;
}


/* call one of our error routines (see above) */
void print_error(unsigned char code)
{
    __CALL_ERROR_FUNC(code);
    
    return;
}

/* EOF */
