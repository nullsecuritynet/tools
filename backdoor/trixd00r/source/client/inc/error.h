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
 * client/inc/error.h                                                          *
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


#ifndef __ERROR_H__
#define __ERROR_H__


#include "trixd00r.h"


/* beginning of error messages */
#define ERROR           "[-] ERROR"
#define WARNING         "[-] WARNING"


/* fatal error messages */
#define ERR_MSG_GEN             ERROR
#define ERR_MSG_PKT_TYPE        ERROR ":"RED" invalid packet type"NORM
#define ERR_MSG_SHELL_MODE      ERROR ":"RED" invalid shell mode"NORM
#define ERR_MSG_HOST            ERROR ":"RED" invalid host"NORM
#define ERR_MSG_IP              ERROR ":"RED" invalid ip address"NORM
#define ERR_MSG_PORT            ERROR ":"RED" invalid port number"NORM
#define ERR_MSG_ARGC            ERROR ":"RED" use -H for usage and help"NORM
#define ERR_MSG_ARGS            ERROR ":"RED" you fucked up, mount /dev/brain"NORM
#define ERR_MSG_UID             ERROR ":"RED" you must be root"NORM
#define ERR_MSG_PKT_SIZE        ERROR ":"RED" packet size larger than 65535"NORM
#define ERR_MSG_NET             ERROR
#define ERR_MSG_CLOSE           ERROR


/* call error function */
#define __CALL_ERROR_FUNC(code)  error[code].fptr(error[code].msg);


/* our own error codes */
enum {
    ERR_GEN = 0, ERR_PKT_TYPE, ERR_SHELL_MODE, ERR_HOST, ERR_IP, ERR_PORT,
    ERR_ARGC, ERR_ARGS, ERR_UID, ERR_PKT_SIZE, ERR_NET, ERR_CLOSE
};


/* error handling */
typedef struct {
    unsigned char code;             /* relevant error code */
    const char *msg;                /* relevant error message */
    void (*fptr)(const char *);     /* ptr to error-functions */
} error_t;


void print_error(unsigned char);


#endif

/* EOF */
