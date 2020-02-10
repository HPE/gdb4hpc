/*********************************************************************************\
 * dbg_sci_be.c
 *
 * © 2011-2013,2020 Cray Inc.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "bitset.h"
#include "dbg_sci.h"
#include "dbg_set_sync.h"
#include "dbg_soc.h"
#include "dbgevent.h"
#include "dbgsrv_defs.h"
#include "defs.h"
#include "list.h"

struct sci_be_type {
    int rank;             // rankd of backend
    char rank_str[1024];  // ("%d %d", rankd size)
    bitset* rank_bs;      // bitset
    char* rank_bs_str;

    int num_of_be;
};
typedef struct sci_be_type sci_be_type;

static sci_be_type* sci_be = NULL;
static char sci_be_msg_header = DBG_SCI_MSG_SINGLE;

static sci_group_t waiting_group;
static int waiting_seq = -1;
static char* waiting_msg = NULL;

void (*dbgsrv_callback)(char* msg, sci_group_t group, int seq) = NULL;

sci_be_type* new_sci_be_type(int id) {
    sci_be_type* sci = (sci_be_type*)Alloc(sizeof(sci_be_type));

    sci->rank = id;
    sci->rank_bs = NULL;
    sci->rank_bs_str = NULL;

    return sci;
}

void free_sci_be_type(sci_be_type* sci) {
    if (sci == NULL) return;

    if (sci->rank_bs != NULL) bitset_free(sci->rank_bs);

    Free(sci->rank_bs_str);

    Free(sci);
}

char* result = NULL;

void sci_be_handler(void* user_param, sci_group_t group, void* buffer,
                    int size) {
    int res;

    waiting_group = group;
    res = sci_unpack_fe_msg(buffer, size, &waiting_seq, &waiting_msg);
    if (res < 0) {
        fprintf(stderr, "ERROR: sci_be_handler received invalid messge %s\n",
                buffer);
        waiting_msg = NULL;
    }
    DVPRINT("INFO: sci_be_handler recieved msg[%s]. seq[%d], size[%d]\n",
            waiting_msg, waiting_seq, size);
}

/*------------------------------------------ ---------------------*/
/*----------------------APIs start from here----------------------*/
/*------------------------------------------- --------------------*/

void sci_be_initialize(char* job_key, char* host_name, char* host_port,
                       char* client_id) {
    int id;

    DVPRINT("enterring sci_be_initialize\n");
    DVPRINT("JOB KEY: %s\n", job_key);
    DVPRINT("PARENT HOST: %s\n", host_name);
    DVPRINT("PARENT PORT: %s\n", host_port);
    DVPRINT("CLIENT ID: %s\n", client_id);

    // environment variables of SCI with default values
    setenv(SCI_USE_EXTLAUNCHER, USE_EXTLAUNCHER_VALUE, 1);
    setenv(SCI_PARENT_ID, PARENT_ID_VALUE, 1);
    setenv(SCI_EMBED_AGENT, EMBED_AGENT_VALUE, 1);

    // environment variables of SCI with the values determined by SCI FE
    setenv(SCI_JOB_KEY, job_key, 1);
    setenv(SCI_PARENT_HOSTNAME, host_name, 1);
    setenv(SCI_PARENT_PORT, host_port, 1);
    setenv(SCI_CLIENT_ID, client_id, 1);

    sscanf(client_id, "%d", &id);
    sci_be = new_sci_be_type(id);

    DVPRINT("leaving sci_be_initialize\n");
}

/*
** create SCI BE
*/
int sci_be_create() {
    sci_info_t info;
    int res;

    bzero(&info, sizeof(info));
    info.type = SCI_BACK_END;
    info.be_info.mode = SCI_POLLING;
    info.be_info.hndlr = (SCI_msg_hndlr*)&sci_be_handler;
    info.be_info.param = NULL;
    info.disable_sshauth == 1;

    res = SCI_Initialize(&info);
    if (res != SCI_SUCCESS) {
        fprintf(stderr,
                "ERROR: sci_be_create SCI_Initialize() returns ERROR[%d]\n",
                res);
        return -1;
    }

    res = SCI_Query(BACKEND_ID, &sci_be->rank);
    if (res != SCI_SUCCESS) {
        fprintf(stderr,
                "ERROR: SCI_Query in sci_be_create returned an ERROR[%d]\n",
                res);
        return -1;
    }

    //	return 0;
    return sci_be->rank;
}

///////////////////////////////

int sci_be_create_debug() {
    int my_rank = -10;
    sci_info_t info;
    int res;

    bzero(&info, sizeof(info));
    info.type = SCI_BACK_END;
    info.be_info.mode = SCI_POLLING;
    info.be_info.hndlr = (SCI_msg_hndlr*)&sci_be_handler;
    info.be_info.param = NULL;

    res = SCI_Initialize(&info);
    if (res != SCI_SUCCESS) {
        fprintf(stderr,
                "ERROR: sci_be_create SCI_Initialize() returns ERROR[%d]\n",
                res);
        return -2;
    }

    res = SCI_Query(BACKEND_ID, &my_rank);
    if (res != SCI_SUCCESS) {
        fprintf(stderr,
                "ERROR: SCI_Query in sci_be_create returned an ERROR[%d]\n",
                res);
        return -1;
    }

    return my_rank;
}
///////////////////////////////

void sci_be_set_size(int size) {
    DVPRINT("enterring sci_be_set_size. size[%d]\n", size);

    sci_be->num_of_be = size;

    sci_be->rank_bs = bitset_new(size);
    bitset_set(sci_be->rank_bs, sci_be->rank);

    sci_be->rank_bs_str = bitset_to_str(sci_be->rank_bs);

    sprintf(sci_be->rank_str, "%d %d", sci_be->rank, sci_be->num_of_be);

    DVPRINT("leaving sci_be_set_size.\n");
}

int sci_be_send_reply(sci_group_t group, int seq, char* msg) {
    int res;
    int bs_str_len, msg_len;
    void* bufs[NUM_BE_BUFS];
    int sizes[NUM_BE_BUFS];
    int type = DBG_SCI_BE_MSG_REPLY;

    DVPRINT("[INFO] sci_be_send_reply is sending reply[%s], be_id[%d]\n", msg,
            sci_be->rank);

    //	sci_pack_be_msg(&(sci_be->rank), &type, &seq, msg, &sizes, &bufs);
    bs_str_len = strlen(sci_be->rank_bs_str);
    msg_len = strlen(msg);
    sci_pack_be_msg(&sci_be_msg_header, &bs_str_len, sci_be->rank_bs_str, &type,
                    &seq, &msg_len, msg, sizes, bufs);

    //    res = SCI_Upload(SCI_FILTER_NULL, group, NUM_BE_BUFS, bufs, sizes);
    res = SCI_Upload(DBG_SCI_FILTER_CMD, group, NUM_BE_BUFS, bufs, sizes);
    if (res != SCI_SUCCESS) {
        fprintf(stderr, "ERROR: SCI_Upload returned an error[%d]\n", res);
        return -1;
    }

    return 0;
}

int sci_be_send_event(char* msg) {
    int seq = -1;
    int res;
    int bs_str_len, msg_len;
    void* bufs[NUM_BE_BUFS];
    int sizes[NUM_BE_BUFS];
    int type = DBG_SCI_BE_MSG_EVENT;

    DVPRINT("[INFO] sci_be_send_event is sending event[%s], be_id[%d]\n", msg,
            sci_be->rank);

    //	sci_pack_be_msg(&(sci_be->rank), &type, &seq, msg, &sizes, &bufs);
    bs_str_len = strlen(sci_be->rank_bs_str);
    msg_len = strlen(msg);
    sci_pack_be_msg(&sci_be_msg_header, &bs_str_len, sci_be->rank_bs_str, &type,
                    &seq, &msg_len, msg, sizes, bufs);

    res = SCI_Upload(DBG_SCI_FILTER_EVENT, SCI_GROUP_ALL, NUM_BE_BUFS, bufs,
                     sizes);
    if (res != SCI_SUCCESS) {
        fprintf(stderr, "ERROR: SCI_Upload returned an error[%d]\n", res);
        return -1;
    }

    return -1;
}

int sci_be_send_io(char* msg) { return -1; }

int sci_be_handle_msg() {
    char* msg;
    sci_group_t group;
    int seq;
    if (waiting_msg != NULL) {
        msg = waiting_msg;
        seq = waiting_seq;
        group = waiting_group;
        waiting_msg = NULL;
        DVPRINT("INFO: sci_be_handle_msg msg[%s]\n", msg);

        dbgsrv_callback(msg, group, seq);
        return 0;
    }

    return -1;
}

int sci_be_recv(bitset* target, char** msg, int* source) {
    int res;
    res = SCI_Poll(50);
    if (res != SCI_SUCCESS) {
        fprintf(stderr, "ERROR: sci_be_recv returned an ERROR[%d]\n", res);
        return -1;
    }

    if (waiting_msg == NULL) {
        return -1;
    }

    return 0;
}

void sci_be_set_callback(void (*callback)()) {
    dbgsrv_callback = callback;
    return;
}

int sci_be_shutdown() { SCI_Terminate(); }
