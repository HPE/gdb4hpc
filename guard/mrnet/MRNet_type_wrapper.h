/*********************************************************************************\
 * <MRNet_type_wrapper.h>
 *
 * © 2012-2013 Cray Inc.  All Rights Reserved.
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

#ifndef _MRNET_TYPE_WRAPPER_H
#define _MRNET_TYPE_WRAPPER_H

#include "mrnet/Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
** NOTE: Any change here should also be reflected in MRNet_C.h.
**
** This header file is meant to be seen by the cpp wrapper source.
** The MRNet_C.h header file is meant to be seen by the c user that is
** interfacing with these wrappers.
**
*/

using namespace MRN;

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

struct MRNet_Node {
    char *hostname;
    Port port;
    Rank rank;
};
typedef struct MRNet_Node MRNet_Node_t;

// external FILE streams for printing
// FIXME: This should be using a standard getter/setter like every other library
//        on the face of the planet. This is a major cleanup task that should
//        be done. All error printing in this library is disabled for now.
// extern FILE **			err_stream_ptr;

#ifdef __cplusplus
}
#endif

#endif /* _MRNET_TYPE_WRAPPER_H */
