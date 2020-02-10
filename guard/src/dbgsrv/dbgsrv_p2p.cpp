/*********************************************************************************\
 * dbgsrv_p2p.c
 *
 * © 2012-2013,2020 Cray Inc.
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

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "cmdtoken.h"
#include "dbgsrv_p2p.h"
#include "defs.h"
#include "expr.h"
#include "useful.h"

/* this function takes a procset name and load the netinfo from file to
  *appInfo_t
 **/
int LoadAppInfo(appInfo_t *myapp) {
    char filename[BUFSIZ];
    char nid[BUFSIZ];
    char line[BUFSIZ];
    int rank, port;
    FILE *netinfo_fd;
    const char *envDir;

    DPRINT("In LoadAppInfo\n");

    if ((envDir = getenv("CRAY_DBG_LOG_DIR")) == (char *)NULL) {
        // user didn't set the environment variable, so lets
        // default to tmp
        envDir = "/tmp";
    }
    sprintf(filename, "%s/dbglog_netinfo_%s", envDir, myapp->id);

    netinfo_fd = fopen(filename, "r");
    if (netinfo_fd == (FILE *)NULL) {
        // do something here maybe
        return 0;
    }

    if (myapp->rank != NULL) {
        while (fscanf(netinfo_fd, "%s", line) != EOF) {
            char *tok;
            tok = strtok(line, ":");
            rank = atoi(tok);
            tok = strtok(NULL, ":");
            strcpy(nid, tok);
            tok = strtok(NULL, ":");
            port = atoi(tok);
            // create a new rankInfoEntry_t
            myapp->rank[rank]->nid = strdup(nid);
            myapp->rank[rank]->port = port;
        }
    }

    fclose(netinfo_fd);

    return 1;
}

int UnloadAppInfo(appInfo_t *myapp) {
    int i;
    for (i = 0; i < myapp->nrank; i++) {
        Free(myapp->rank[i]->nid);
        Free(myapp->rank[i]);
    }
    Free(myapp->rank);

    return 0;
}
