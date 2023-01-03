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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

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

#include "blockmap_util.h"
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
void ParseDBGExpr(const char *expr, char *sliceinfo, char *var,
                  char *args) {
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

void ParseHashArgs(char *args, int *hash_func_id, int *lower, int *upper,
                   char *hash_info) {
    char *tok;
    char buf[BUFSIZ] = "";

    tok = strtok(args, "#");
    strcpy(hash_info, tok);
    tok = strtok(NULL, "#");
    if (tok == NULL) {
        strcpy(buf, hash_info);
        strcpy(hash_info, "");
    } else
        strcpy(buf, tok);

    // now parse the hash args
    tok = strtok(buf, ",");
    *hash_func_id = atoi(tok);
    tok = strtok(NULL, ",");
    *lower = atoi(tok);
    tok = strtok(NULL, ",");
    *upper = atoi(tok);

    return;
}

void ParseCountelemArgs(char *args, double *lower, double *upper) {
    char *tok;

    // now parse the countelem args
    tok = strtok(args, ",");
    *lower = atof(tok);
    tok = strtok(NULL, ",");
    *upper = atof(tok);

    return;
}

void ParseHistogramArgs(char *args, int *numbins, double *start, double *end) {
    char *tok;

    // now parse the histogram args
    tok = strtok(args, ",");
    *numbins = atoi(tok);
    tok = strtok(NULL, ",");
    *start = atof(tok);
    tok = strtok(NULL, ",");
    *end = atof(tok);

    return;
}

void ParseBlockMapArgs(char *args, blockmap_info *bm) {
    int i;
    char *tok;

    tok = strtok(args, ",");
    strcpy(bm->isym, tok);
    tok = strtok(NULL, ",");
    bm->num_procs = atoi(tok);
    tok = strtok(NULL, ",");
    bm->rank = atoi(tok);

    for (i = 0; i < bm->rank; i++) {
        tok = strtok(NULL, ",");
        bm->m_distribute[i] = atoi(tok);
    }

    for (i = 0; i < bm->rank; i++) {
        tok = strtok(NULL, ",");
        bm->m_blockcyclic_val[i] = atoi(tok);
    }

    for (i = 0; i < bm->rank; i++) {
        tok = strtok(NULL, ",");
        bm->m_data[i] = atoi(tok);
    }

    for (i = 0; i < bm->rank; i++) {
        tok = strtok(NULL, ",");
        bm->m_procs[i] = atoi(tok);
    }

    for (i = 0; i < bm->rank; i++) {
        tok = strtok(NULL, ",");
        bm->m_dim_order[i] = atoi(tok);
    }

    return;
}

// blockmap_string =
// $i_sym,num_procs,rank,<distribute>,<blockcyclic_val>,<data>,<procs_grid>,<dim_order>
// P2P args = <owner>-<sender>-<blockmap_string1>-<blockmap_string2>
// example:
// a-a-$b,256,3,152,165,165,0,0,0,64,64,64,1,64,4,2,3,1-$a,128,3,152,165,165,0,0,0,64,64,64,1,64,4,2,3,1
void ParseP2PArgs(char *args, char *my_isym, int *is_sender,
                  blockmap_info *my_bm, blockmap_info *o_bm) {
    char *tok;
    char buf1[BUFSIZ] = "";
    char buf2[BUFSIZ] = "";
    char buf3[BUFSIZ] = "";
    char sender[BUFSIZ] = "";

    // get owner proc_set symbol
    tok = strtok(args, "-");
    strcpy(my_isym, tok);
    tok = strtok(NULL, "-");

    // get sender symbol
    strcpy(sender, tok);
    // check if you are the sender
    if (strcmp(my_isym, sender) == 0) *is_sender = 1;

    tok = strtok(NULL, "-");
    if (NULL != strstr(tok, my_isym))
        strcpy(buf1, tok);
    else
        strcpy(buf2, tok);

    tok = strtok(NULL, "-");
    if (NULL != strstr(tok, my_isym))
        strcpy(buf1, tok);
    else
        strcpy(buf2, tok);

    tok = strtok(NULL, "-");
    strcpy(buf3, tok);

    // get blockmap info for my_bm
    ParseBlockMapArgs(buf1, my_bm);
    // get blockmap info for my_bm
    ParseBlockMapArgs(buf2, o_bm);

    tok = strtok(buf3, ",");
    my_bm->a_lower = atof(tok);
    tok = strtok(NULL, ",");
    my_bm->a_upper = atof(tok);

    return;
}

/* this function takes in data size for a process, block size, the index, the
 * rank and return sclice info for that process
 */
int _get_slice_info(int *data_size, int *block_size, char *sliceinfo, int index,
                    int rank, int isFortran) {
    int i;
    char tmpstr[BUFSIZ] = "";
    int *start = NULL;
    int *end = NULL;

    start = (int *)Alloc(sizeof(int) * rank);
    end = (int *)Alloc(sizeof(int) * rank);

    for (i = rank - 1; i >= 0; i--) {
        if (index == 0) {
            start[i] = 0;
            end[i] = block_size[i] - 1;
        } else {
            start[i] = block_size[i] * index;
            index = start[i] / data_size[i];
            start[i] = start[i] % data_size[i];
            end[i] = start[i] + block_size[i] - 1;
        }
    }

    // store the slicing info
    strcpy(sliceinfo, "");
    for (i = 0; i < rank; i++) {
        if (i < rank - 1)
            sprintf(tmpstr, "%d,%d,", start[i] + isFortran, end[i] + isFortran);
        else
            sprintf(tmpstr, "%d,%d", start[i] + isFortran, end[i] + isFortran);
        strcat(sliceinfo, tmpstr);
    }

    Free(start);
    Free(end);

    return 0;
}

int _get_slice_destination(int rank, int *global_start_loc, int *data_size,
                           int *proc_grid, int *order_map) {
    int des = 0;
    int i, carry = 1;

    for (i = 0; i < rank; i++) {
        int tmp = global_start_loc[order_map[i]] / data_size[order_map[i]];
        if (i > 0 && tmp > 0) {
            des += tmp * carry;
            carry = 1;
        } else
            des += tmp;
        carry *= proc_grid[order_map[i]];
    }

    return des;
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

// Minh
// now we map the index in hash_bound to data_bound_max
uint32_t _perform_hash(char *datastr, char *fds, int *hash_bound,
                       int *data_bound, int *index, int num_elem_to_hash,
                       int rank, int hashfunc) {
    int *data_indices = (int *)Alloc(num_elem_to_hash * sizeof(int));
    int *local_index = (int *)Alloc(rank * sizeof(int));
    int i, j, tmp, start_point;
    char *tok, elem[BUFSIZ] = "";
    char *cp_datastr = strdup(datastr);
    uint32_t result = 0;

    // work out the first start point for this index first
    start_point = _get_data_index(rank, data_bound, index);

    tmp = 0;
    for (i = 0; i < num_elem_to_hash; i++) {
        // translate i into local_index
        int tmp2 = i;
        for (j = rank - 1; j >= 0; j--) {
            local_index[j] = tmp2 % hash_bound[j];
            tmp2 = tmp2 / hash_bound[j];
        }
        data_indices[tmp] = _get_data_index(rank, data_bound, local_index);
        // correct data_indices using the index
        data_indices[tmp] += start_point;
        tmp++;
    }

    // reset i, j;
    i = 0;
    j = 0;
    // check the type, if type is Fortran COMPLEX, count "," twice
    if (strstr(fds, "COMPLEX") != NULL) {
        tok = strtok(cp_datastr, ")");
        TrimStrWithTarget(tok, '(');
    } else
        tok = strtok(cp_datastr, ",");
    while (tok != NULL) {
        strcpy(elem, tok);
        if (strlen(elem) > 0) {
            if (i == data_indices[j]) {
                j++;
                if (j >= num_elem_to_hash) break;
            }
            i++;
        }
        // check the type, if type is Fortran COMPLEX, count "," twice
        if (strstr(fds, "COMPLEX") != NULL) {
            tok = strtok(NULL, ")");
            TrimStrWithTarget(tok, ',');
            TrimStrWithTarget(tok, '(');
        } else
            tok = strtok(NULL, ",");
    }

    if (data_indices) Free(data_indices);
    if (local_index) Free(local_index);
    if (cp_datastr) Free(cp_datastr);

    return result;
}

int ReceiverSetSocketPort(void) {
    int true_bool = 1;
    struct sockaddr_in server_addr;
    int port = my_rank + 10000;

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &true_bool,
                   sizeof(int)) == -1) {
        DVPRINT("in SliceSocketReceive: Setsockopt\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock_fd, (struct sockaddr *)&server_addr,
             sizeof(struct sockaddr)) == -1) {
        DVPRINT("in SliceSocketReceive: Unable to bind\n");
        return -1;
    }

    return 0;
}

void _fix_fortran_blockmap_info(blockmap_info *my_bm) {
    int i, j;
    int tmpbuf[10];

    // fix value of other blockmap attributes
    j = 0;
    for (i = my_bm->rank - 1; 0 <= i; --i) {
        tmpbuf[i] = my_bm->m_data[j];
        j++;
    }
    for (i = 0; i < my_bm->rank; i++) my_bm->m_data[i] = tmpbuf[i];

    j = 0;
    for (i = my_bm->rank - 1; 0 <= i; --i) {
        tmpbuf[i] = my_bm->m_distribute[j];
        j++;
    }
    for (i = 0; i < my_bm->rank; i++) my_bm->m_distribute[i] = tmpbuf[i];

    j = 0;
    for (i = my_bm->rank - 1; 0 <= i; --i) {
        tmpbuf[i] = my_bm->m_procs[j];
        j++;
    }
    for (i = 0; i < my_bm->rank; i++) my_bm->m_procs[i] = tmpbuf[i];

    j = 0;
    for (i = my_bm->rank - 1; 0 <= i; --i) {
        tmpbuf[i] = my_bm->m_blockcyclic_val[j];
        j++;
    }
    for (i = 0; i < my_bm->rank; i++) my_bm->m_blockcyclic_val[i] = tmpbuf[i];

    j = 0;
    for (i = my_bm->rank - 1; 0 <= i; --i) {
        tmpbuf[i] = my_bm->m_dim_order[j];
        j++;
    }
    for (i = 0; i < my_bm->rank; i++) my_bm->m_dim_order[i] = tmpbuf[i];

    _get_order_map(my_bm->rank, my_bm->m_dim_order, my_bm->order_map);

    return;
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

