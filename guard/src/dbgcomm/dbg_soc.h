/*********************************************************************************\
 * socket utility routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#ifndef _DBG_SOC_H
#define _DBG_SOC_H

#include <sys/types.h>

struct socprt {
    SOCKET soc_fd;  /* socket file descriptor */
    int soc_port;   /* socket port number */
    SOCKET soc_ofd; /* originating file descriptor */
};

typedef struct socprt SOCPRT;

struct socclient {
    SOCKET cl_fd; /* socket file descriptor */
};

typedef struct socclient SOCCLIENT;

extern int CreateSoc(SOCPRT **, void (*)(SOCKET, SOCPRT *));
extern SOCPRT *soctcp_create();
extern fd_set soc_fdset;
extern int soc_cnt;
extern int soc_getreqset();
extern SOCCLIENT *socclnt_create(char *, int);
extern int socclnt_call(SOCCLIENT *, char *, char **, int *, struct timeval *);
extern void socclnt_timeout(struct timeval *);
extern void socclnt_destroy(SOCCLIENT *);
extern int soc_print(void);
extern int soc_getreqset(fd_set *);
extern int soc_register(SOCKET, SOCPRT *, void (*)());
extern int soc_unregister(SOCPRT *);
extern int soc_destroy(SOCPRT *);
extern int soc_sendreply(SOCPRT *, char *);
extern int soc_write(SOCKET, char *, int);
extern int soc_read(SOCKET, char **);

#endif /* ! _DBG_SOC_H */
