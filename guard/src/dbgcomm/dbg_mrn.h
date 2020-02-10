/*********************************************************************************\
 * interface of MRNet for Guard
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

#ifndef _GUARD_MRNET_TYPES_H
#define _GUARD_MRNET_TYPES_H

#include "MRNet_C.h"
#include "bitset.h"
#include "list.h"
#include "mrn_cfg.h"

#define MRNET_BLOCKING 1
#define MRNET_UNBLOCKING 0
#define ATTACHBE_RANK_SKEW (1024 * 1024 * 16)

typedef enum {
    DBG_MRNet_PRO_COMMAND = FirstApplicationTag,
    DBG_MRNet_PRO_IO,
    DBG_MRNet_PRO_EVENT
} DBG_MRNet_PROTOCOL;

typedef enum {
    IO_MESSAGE,
    EVENT_MESSAGE,
    NULL_MESSAGE,
    ERROR_MESSAGE
} dbg_mrnet_event_id;

enum mrn_dbgcmd { MRNCMD_SETEVENTSTREAM };

extern void DbgSRVMRNSetParam(const char* msg);

#endif /* _GUARD_MRNET_TYPES_H */
