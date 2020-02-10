/*********************************************************************************\
 * socket utility routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013,2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "aif.h"
#include "dbg.h"
#include "defs.h"
#include "useful.h"

#define DSPCOUNT 512 /* dispatcher table size */

struct dspelem {
    SOCPRT *sp;         /* socket information */
    void (*dispatch)(); /* dispatch function */
};

struct dspelem dsptable[DSPCOUNT]; /* dispatch table */
int dspcount = 0;                  /* dispatch counter */

fd_set soc_fdset;
int soc_cnt;

/* Default timeout can be changed using soccclnt_timeout() */
static struct timeval SOCTIMEOUT = {25, 0};
static int use_soctimeout = 0;

/*
** Replicate RPC behaviour. Once timeout is set here, the timeout
** parameter passed to socclnt_call() will be ignored in all future
** calls.
*/
void socclnt_timeout(struct timeval *tv) {
    SOCTIMEOUT = *tv;
    use_soctimeout = 1;
}

/*
** Create and listen on a socket
*/
int CreateSoc(SOCPRT **sp, void (*clnt)(SOCKET fd, SOCPRT *)) {
    SOCPRT *s;

    if ((s = soctcp_create()) == NULL) {
        PrintOutError("soctcp_create error");
        return -1;
    }

    if (soc_register(s->soc_fd, s, *clnt) < 0) {
        PrintOutError("soc_register error");
        return -1;
    }

    *sp = s;

    return 0;
}

SOCPRT *soctcp_create() {
    int status;
    int sd;
    int port;
    socklen_t slen;
    struct sockaddr_in sname;
    SOCPRT *sp;

    sp = (SOCPRT *)Alloc(sizeof(SOCPRT));
    sp->soc_fd = INVALID_SOCKET;
    sp->soc_port = -1;
    sp->soc_ofd = INVALID_SOCKET;

    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        PrintOutError("socket error");
        return NULL;
    }

    memset(&sname, 0, sizeof(sname));
    sname.sin_family = PF_INET;
    sname.sin_port = htons(0);
    sname.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((status = bind(sd, (struct sockaddr *)&sname, sizeof(sname))) ==
        SOCKET_ERROR) {
        PrintOutError("bind error");
        CLOSE_SOCKET(sd);
        return NULL;
    }

    slen = sizeof(sname);

    if (getsockname(sd, (struct sockaddr *)&sname, &slen) == SOCKET_ERROR) {
        PrintOutError("getsockname error");
        CLOSE_SOCKET(sd);
        return NULL;
    }

    port = (int)ntohs(sname.sin_port);

    // rwm: Use a generous size queue for accept's (5 -> 200)
    if (listen(sd, 200) == SOCKET_ERROR) {
        PrintOutError("listen error");
        CLOSE_SOCKET(sd);
        return NULL;
    }

    sp->soc_fd = sd;
    sp->soc_port = port;

    return sp;
}

int soc_print() {
    printf("soc_print %d fd %d\n", dspcount, dsptable[0].sp->soc_fd);
    return 0;
}

int soc_getreqset(fd_set *fds) {
    int count;

    for (count = 0; count < dspcount; count++) {
        if (FD_ISSET(dsptable[count].sp->soc_fd, fds)) {
            dsptable[count].dispatch(dsptable[count].sp->soc_fd,
                                     dsptable[count].sp);
        }
    }

    return 0;
}

int soc_register(SOCKET fd, SOCPRT *sp, void (*dispatch)()) {
    if (dspcount >= DSPCOUNT) {
        return -1;
    }

    if (dspcount == 0) {
        FD_ZERO(&soc_fdset);
    }

    dsptable[dspcount].sp = sp;
    dsptable[dspcount].dispatch = dispatch;

    dspcount++;
    soc_cnt++;

    FD_SET(fd, &soc_fdset);

    return 0;
}

int soc_unregister(SOCPRT *sp) {
    int count;

    for (count = 0; count < dspcount; count++) {
        if (dsptable[count].sp == sp) {
            FD_CLR(dsptable[count].sp->soc_fd, &soc_fdset);
            dspcount--;
            soc_cnt--;
            dsptable[count] = dsptable[dspcount];
            break;
        }
    }

    return 0;
}

int soc_destroy(SOCPRT *sp) {
    CLOSE_SOCKET(sp->soc_fd);
    Free(sp);
    return 0;
}

int soc_sendreply(SOCPRT *sp, char *s) {
    int n;
    int len;

    len = strlen(s);

    n = soc_write(sp->soc_fd, s, len);

    if (n < len) {
        return -1;
    }

    return 0;
}

SOCCLIENT *socclnt_create(char *chost, int cport) {
    SOCCLIENT *cp;
    SOCKET sd;
    struct hostent *host;
    long int haddr;
    struct sockaddr_in scket;
    int i;
    int connected = 0;

    cp = (SOCCLIENT *)Alloc(sizeof(SOCCLIENT));
    cp->cl_fd = INVALID_SOCKET;

    host = gethostbyname(chost);

    if (host == (struct hostent *)NULL) {
        PrintOutError("could not find host \"%s\"\n", chost);
        return NULL;
    }

    haddr = ((host->h_addr[0] & 0xff) << 24) |
            ((host->h_addr[1] & 0xff) << 16) | ((host->h_addr[2] & 0xff) << 8) |
            ((host->h_addr[3] & 0xff) << 0);

    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("socket");
        return NULL;
    }

    memset(&scket, 0, sizeof(scket));
    scket.sin_family = PF_INET;
    scket.sin_port = htons((u_short)cport);
    scket.sin_addr.s_addr = htonl(haddr);

    /* FIXME (donny): a workaround to solve
    "SIOCSIFFLAGS: Cannot assign requested address" problem on the Cray platform
    */
    for (i = 0; i < 15; i++) {
        if (connect(sd, (struct sockaddr *)&scket, sizeof(scket)) ==
            SOCKET_ERROR) {
            sleep(5);
        } else {
            connected = 1;
            break;
        }
    }

    if (connected == 0) {
        perror("connect");
        CLOSE_SOCKET(sd);
        return NULL;
    }

    cp->cl_fd = sd;

    return cp;
}

int socclnt_call(SOCCLIENT *cl, char *request, char **result, int *status,
                 struct timeval *timeout) {
    int n;
    int size;
    fd_set fds;
    int st;
    struct timeval tv;

    /*
    ** There doesn't seem to be a standard as to timeout being
    ** modified by select. Make a copy just in case.
    */
    if (!use_soctimeout && timeout != NULL)
        tv = *timeout;
    else
        tv = SOCTIMEOUT;

    size = strlen(request);
    n = soc_write(cl->cl_fd, request, size);
    if (n < size) {
        return -1;
    }

    FD_ZERO(&fds);
    FD_SET(cl->cl_fd, &fds);

    for (;;) {
        st = select(cl->cl_fd + 1, &fds, NULL, NULL, timeout);

        switch (st) {
            case INVALID_SOCKET:
                if (errno == EINTR) continue;

                perror("select");
                return -1;

            case 0:
                PrintOutError("select timed out\n");
                return -1;

            default:
                if (!FD_ISSET(cl->cl_fd, &fds)) {
                    PrintOutError("select on bad socket\n");
                    return -1;
                }
                break;
        }

        break;
    }

    *result = NULL;
    n = soc_read(cl->cl_fd, result);
    if (n <= 0) {
        *status = -1;
        return -1;
    }

    *status = 0;
    return 0;
}

void socclnt_destroy(SOCCLIENT *cp) {
    if (cp->cl_fd >= 0) {
        CLOSE_SOCKET(cp->cl_fd);
    }

    Free(cp);
}

int soc_write(SOCKET fd, char *ptr, int nbytes) {
    int nleft, nwritten;

    DPRINT("soc_write: %d %d::: %s", fd, nbytes, ptr);

    nleft = nbytes;
    while (nleft > 0) {
        nwritten = send(fd, ptr, nleft, 0);
        if (nwritten <= 0) {
            return nwritten;
        }

        nleft -= nwritten;
        ptr += nwritten;
    }
    return (nbytes - nleft);
}

int soc_read(SOCKET fd, char **result) {
    int size;
    int count;
    char *pbuf;
    char ch;
    int n;

    size = BUFSIZ;
    count = 0;

    pbuf = (char *)Alloc(size);

    do {
        n = recv(fd, &ch, 1, 0);

        if (n <= 0 || ch == '\n') {
            break;
        }

        pbuf[count] = ch;
        count++;

        if (count >= size - 1) {
            size = 2 * size;
            *result = (char *)Alloc(size);
            memcpy(*result, pbuf, count);
            Free(pbuf);
            pbuf = *result;
        }
    } while (1);

    pbuf[count] = '\0';
    *result = pbuf;

    DPRINT("soc_read: %d %d::: %s\n", fd, count, *result);

    return (count);
}
