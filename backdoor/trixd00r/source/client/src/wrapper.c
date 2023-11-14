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
 * client/src/wrapper.c                                                        *
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


ssize_t xrecv(int sockfd, void *buf, size_t len, int flags)
{

    ssize_t nreceived = 0;
    char *p = buf;


    while (len > 0) {
        nreceived = recv(sockfd, p, len, flags);
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


ssize_t xwrite(int sockfd, const void *buff, size_t len)
{
    size_t nleft = 0;
    ssize_t nwritten = 0;
    const char *ptr = NULL;

    
    ptr = buff;
    nleft = len;

    while (nleft > 0) {
        if ((nwritten = write(sockfd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR) {
                nwritten = 0;
            } else {
                return -1;
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }

    return len;
}


ssize_t xread(int sockfd, void *buf, size_t len)
{
    size_t nleft = 0;
    ssize_t nread = 0;
    char *ptr = NULL;


    ptr = buf;
    nleft = len;

    while (nleft > 0) {
        if ((nread = read(sockfd, ptr, nleft)) < 0) {
            if (errno == EINTR) {
                nread = 0;
            } else {
                return -1;
            }
        } else if (nread == 0) {
            break;
        }
        nleft -= nread;
        ptr += nread;
    }

    return (len - nleft);
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


void xsystem(const char *cmd)
{
    int x = 0;


    x = system(cmd);

    if (x == -1) {
        print_error(ERR_GEN);
    }

    return;
}

/* EOF */
