/*********************************************************************************\
 * dbgsrv_util.c
 *
 * Copyright 2011-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#include "config.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "dbg.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_libmi2aif.h"
#include "dbgsrv_p2p.h"
#include "dbgsrv_util.h"
#include "useful.h"

extern int sock_fd;
extern int my_rank;
extern appInfo_t other_app;

// this function parse the big dbg expression into gdb_var name, reduction
// function_id and a list of args for such function
void ParseDBGExpr(const char *expr, char *sliceinfo, char *var, char *) {
    char *tok;
    char buf[BUFSIZ];
    char sub_expr[BUFSIZ] = "";
    char tmp[100] = "";

    strcpy(buf, expr);

    // look for ``
    if (NULL != strstr(buf, "`")) {
        char sub_expr2[BUFSIZ] = "";

        tok = strtok(buf, "`");
        strcpy(sub_expr, tok);
        tok = strtok(NULL, "`");
        strcpy(sub_expr2, tok);

        if (NULL != strstr(sub_expr2, "slice")) {
            char tmp2[BUFSIZ] = "";

            sub_expr2[strlen(sub_expr2) - 1] = '\0';
            tok = strtok(sub_expr2, "(");
            tok = strtok(NULL, "(");
            strcpy(tmp2, tok);

            tok = strtok(tmp2, ")");
            strcpy(sliceinfo, tok);
            tok = strtok(NULL, ")");
            strcpy(var, tok);
        } else
            strcpy(var, sub_expr2);
    } else if (NULL != strstr(buf, "slice")) {
        buf[strlen(buf) - 1] = '\0';
        tok = strtok(buf, "(");
        tok = strtok(NULL, "(");

        strcpy(tmp, tok);
        tok = strtok(tmp, ")");
        strcpy(sliceinfo, tok);
        tok = strtok(NULL, ")");
        strcpy(var, tok);
    }

    return;
}

int _get_data_index(int rank, int *data, int *index) {
    int i, j;
    int tmp, result = 0;

    for (i = 0; i < rank; i++) {
        tmp = 1;
        if (i == rank - 1)
            result += index[i];
        else {
            for (j = rank - 1; j > i; j--) tmp *= data[j];
            tmp *= index[i];
            result += tmp;
        }
    }
    return result;
}

int ReceiverSetSocketPort(void) {
    int true_bool = 1;
    struct sockaddr_in server_addr;
    int port = my_rank + 10000;

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &true_bool,
                   sizeof(int)) == -1) {
        DVPRINT("in ReceiverSetSocketPort: setsockopt\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock_fd, (struct sockaddr *)&server_addr,
             sizeof(struct sockaddr)) == -1) {
        DVPRINT("in ReceiverSetSocketPort: Unable to bind\n");
        return -1;
    }

    return 0;
}

// Get the size of the virtual data segment of the current process
// in megabytes.
unsigned long GetProcSize() {
    if (!debug_flag)
         return 0l;
    FILE* stat = fopen("/proc/self/status","r");
    if (!stat) {
        DVPRINT("memory: could not open /proc/self/status\n");
        return 0;
    }
    char line[128];
    while (fgets(line,128,stat)) {
        int size;
        if (sscanf(line, "VmData: %d kB", &size) == 1) {
            fclose(stat);
            return size >> 10;  // return in megabytes
        }
    }
    DVPRINT("memory: could not find size in /proc/self/status\n");
    fclose(stat);
    return 0;
}

