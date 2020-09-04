/*********************************************************************************\
 * mrn_topo.c
 *
 * Copyright 2012-2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "Topology_C.h"
#include "common_tools_be.h"
#include "dbg.h"
#include "dbg_mrn.h"
#include "dbgsrv_defs.h"
#include "defs.h"
#include "mrn_cfg.h"

// line format string: "%-7d %" MAXADDRLEN_FMT "s %-5d %-6d\n"
#define ATTACH_FILE_LINE_LEN (8 + MAXADDRLEN + 1 + 6 + 7)
#define NUM_TIMEOUT_RETRY 1000

/*
** This file describes functions that are used on the compute node to interface
** with the mrnet config file.
*/

int ProcRankToMRNRank(int procRank) { return (procRank + ATTACHBE_RANK_SKEW); }

int get_myparent_info(int id, char *host, char *par_addr, unsigned *par_port,
                      unsigned *par_rank) {
    FILE *conn;
    int dbgsrv_id;
    char connections[MAXPATHLEN];
    char addr[MAXADDRLEN + 1];
    unsigned port;
    unsigned rank;
    int found = 0;
    int retval;

#ifdef ALPS
    // file was shipped via CTI
    char* fileDir = cti_be_getFileDir();
    snprintf(connections, MAXPATHLEN - 1, "%s/%s", fileDir,
             MRNET_ATTACH_CONNECTIONS);
    free(fileDir);

    if ((conn = fopen(connections, "r")) == (FILE *)NULL) {
        PrintOutError("ERROR: get_myparent_info cannot open %s!\n",
                      connections);
        return -1;
    }

    DVPRINT("Reading backend attach info from %s\n", connections);
#else
    int retry = 0;
    char *dir = getenv(GUARD_CFG_DIRECTORY);

    if (dir == NULL) {
        PrintOutError(
            "ERROR: get_myparent_info cannot find %s! Please set up MRNet "
            "configurations!",
            GUARD_CFG_DIRECTORY);
        return -1;
    }
    snprintf(connections, MAXPATHLEN - 1, "%s/%s", dir,
             MRNET_ATTACH_CONNECTIONS);

    conn = fopen(connections, "r");

    while (conn == NULL) {
        if (retry > NUM_TIMEOUT_RETRY) break;

        PrintOutError("INFO: get_myparent_info cannot open %s!\n", connections);
        sleep(1);

        conn = fopen(connections, "r");
        retry++;
    }

    if (conn == NULL) {
        PrintOutError("ERROR: get_myparent_info cannot open %s!\n",
                      connections);
        return -1;
    }
#endif

    // advance the file stream to our current rank's position in the file
    // this will greatly reduce read times for large numbers of runs
#ifdef ALPS
    if (fseek(conn, id * ATTACH_FILE_LINE_LEN, SEEK_SET)) {
        perror("fseek");
    }
#endif

    do {
        retval = fscanf(conn, "%d %s %d %d", &dbgsrv_id, addr, &port, &rank);
        addr[MAXADDRLEN] = '\0';
        DVPRINT("INFO: get_myparent_info read %d:%s:%d:%d\n", dbgsrv_id, addr,
                port, rank);
        if (id == dbgsrv_id) {
            found = 1;
            break;
        }
    } while (retval != EOF);

    fclose(conn);

    if (found) {
        sprintf(par_addr, "%s", addr);
        *par_port = port;
        *par_rank = rank;

        return 0;
    }

    return -1;
}
