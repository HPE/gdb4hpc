/*********************************************************************************\
 * interface of MRNet for Guard
 *
 * Copyright 2011-2013,2020 Cray Inc.
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

#ifndef _GUARD_SCI_TYPES_H
#define _GUARD_SCI_TYPES_H

#include "bitset.h"
#include "sci.h"

/* Environment variables of SCI */
#define SCI_PARENT_ID "SCI_PARENT_ID"
#define SCI_PARENT_HOSTNAME "SCI_PARENT_HOSTNAME"
#define SCI_PARENT_PORT "SCI_PARENT_PORT"
#define SCI_CLIENT_ID "SCI_CLIENT_ID"
#define SCI_JOB_KEY "SCI_JOB_KEY"
#define SCI_USE_EXTLAUNCHER "SCI_USE_EXTLAUNCHER"
#define SCI_LISTENER_PORT "SCI_LISTENER_PORT"
#define SCI_REMOTE_SHELL "SCI_REMOTE_SHELL"
#define SCI_EMBED_AGENT "SCI_EMBED_AGENT"

#define SCI_DAEMON_PATH "SCI_DAEMON_PATH"  // scid
#define SCI_AGENT_PATH "SCI_AGENT_PATH"
#define SCI_LIB_PATH "SCI_LIB_PATH"
#define SCI_DAEMON_LOG_DIRECTORY "SCI_DAEMON_LOG_DIRECTORY"

/*Default values */
#define PARENT_ID_VALUE "-1"
//#define	USE_EXTLAUNCHER_VALUE	"yes"
#define USE_EXTLAUNCHER_VALUE "no"
//#define REMOTE_SHELL_VALUE		"true"
#define REMOTE_SHELL_VALUE "false"
#define EMBED_AGENT_VALUE "yes"
//#define EMBED_AGENT_VALUE		"no"

#define DEFAULT_LISTENER_PORT "56789"

/* Configuration */
#define GUARD_DBGSRV_LOG_DIR "CRAY_DBG_LOG_DIR"
#define GUARD_SCI_CFG_DIRECTORY "GUARD_SCI_CFG_DIRECTORY"
#define MACHINES_FILE "machines_file.dat"
#define ALPS_APRUNID_FILE "apid.dat"  // TODO remove it
#define GUARD_SCI_FILTER_PATH "GUARD_SCI_FILTER_PATH"
#define GUARD_SCI_BE_PATH "GUARD_SCI_BE_PATH"

#define GUARD_SCI_BE_JOBKEY "GUARD_SCI_BE_JOBKEY"
#define GUARD_SCI_BE_HOSTNAME "GUARD_SCI_BE_HOSTNAME"
#define GUARD_SCI_BE_HOSTPORT "GUARD_SCI_BE_HOSTPORT"
#define GUARD_SCI_BE_ARCH "GUARD_SCI_BE_ARCH"
#define GUARD_SCI_BE_PROTO "GUARD_SCI_BE_PROTO"
#define GUARD_SCI_BE_DBGBE "GUARD_SCI_BE_DBGBE"
#define GUARD_SCI_BE_PROGRAM "GUARD_SCI_BE_PROGRAM"
#define GUARD_SCI_BE_LOG_DIR "GUARD_SCI_BE_LOG_DIR"

#define CCDB_LIBRARY_DIR "CCDB_LIB_DIR"
#define AGGREGATION_FILTER "libdbgaggsci.so"
#define DBG_SCI_FILTER_CMD 1
#define DBG_SCI_FILTER_EVENT 2

#define NUM_FE_BUFS 2
#define NUM_BE_BUFS 7

/*--------------------------------------------------------------------------*/
/*------ -------- -The format of replies sent by dbgsrv  -------------------*/
/*CONCAT:
 * |Header[1]|NumOfMsg[4]|LenOfSingleMsg[4]|SEQ[4]|TYPE[4]|LenOfBS_String[r]|BS_String|LenOfMsg[r]|Msg|
 */
/*SINGlE: |Header[1]|SEQ[4]|TYPE[4]|LenOfBS_String[4]|BS_String|LenOfMsg[4]|Msg|
 */
/*--------------------------------------------------------------------------*/

enum DBG_SCI_MSG_HEADER { DBG_SCI_MSG_SINGLE, DBG_SCI_MSG_CONCAT };
typedef enum DBG_SCI_MSG_HEADER DBG_SCI_MSG_HEADER;

enum DBG_SCI_BE_MSG_TYPE {
    DBG_SCI_BE_MSG_REPLY,
    DBG_SCI_BE_MSG_EVENT,
    DBG_SCI_BE_MSG_IO
};
typedef enum DBG_SCI_BE_MSG_TYPE DBG_SCI_BE_MSG_TYPE;

typedef void* SCI_Handler;

extern int sci_get_polling_fd(int* polling_fd);

extern int sci_pack_fe_msg(int* seq, char* msg, int* sizes, void** bufs);
extern int sci_unpack_fe_msg(void* buffer, int size, int* seq, char** msg);
extern int sci_pack_be_msg(char* header, int* bs_str_len, char* bs_str,
                           int* type, int* seq, int* msg_len, char* msg,
                           int* sizes, void** bufs);
extern int sci_unpack_be_msg_single(void* buffer, int size, bitset** bs,
                                    int* type, int* seq, char** msg);

#endif /* _GUARD_SCI_TYPES_H */
