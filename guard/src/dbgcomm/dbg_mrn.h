/* interface of MRNet for Guard */
#ifndef _GUARD_MRNET_TYPES_H
#define _GUARD_MRNET_TYPES_H

#include "list.h"
#include "mrn_cfg.h"

#include "bitset.h"
#include "MRNet_C.h"

#define MRNET_BLOCKING 		1
#define MRNET_UNBLOCKING 	0
#define ATTACHBE_RANK_SKEW	(1024*1024*16)

typedef enum 
{ 
	DBG_MRNet_PRO_COMMAND=FirstApplicationTag,
	DBG_MRNet_PRO_IO,
	DBG_MRNet_PRO_EVENT
}	DBG_MRNet_PROTOCOL;

typedef enum
{
	IO_MESSAGE,
	EVENT_MESSAGE,
	NULL_MESSAGE,
	ERROR_MESSAGE
}	dbg_mrnet_event_id;

#endif /* _GUARD_MRNET_TYPES_H */
