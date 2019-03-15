/*********************************************************************************\
 * <MRNet_C.h>
 *
 * © 2010-2019 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/


#ifndef _GUARD_MRNET_H
#define _GUARD_MRNET_H

#include "mrnet_lightweight/Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
** NOTE: Any change here should also be reflected in MRNet_type_wrapper.h.
** 
** This header file is meant to be seen by the c source files interfacing with
** this wrapper.
**
** The MRNet_type_wrapper.h header file is meant to only be used by the cpp
** wrappers.
**
*/

/*Network class*/
struct MRNet_Network;
typedef struct MRNet_Network MRNet_Network;

/*NetworkTopology class*/
struct MRNet_Topology;
typedef struct MRNet_Topology MRNet_Topology;

/*Communicator class*/
struct MRNet_Communicator;
typedef struct MRNet_Communicator MRNet_Communicator;

/*Packet class*/
struct MRNet_Packet;
typedef struct MRNet_Packet MRNet_Packet;

/*Packet class*/
struct MRNet_Stream;
typedef struct MRNet_Stream MRNet_Stream;

typedef enum {
	MRNet_EVENT_ERROR = 0,
	MRNet_EVENT_DATA,
	MRNet_EVENT_TOPOLOGY,
	MRNet_EVENT_UNKNOWN
} MRNEventType;

struct MRNet_Node
{
	char 	*hostname;
	Port 	port;
	Rank 	rank;
};
typedef struct MRNet_Node MRNet_Node_t;

#ifdef __cplusplus
}
#endif

#endif /*_GUARD_MRNET_H*/
