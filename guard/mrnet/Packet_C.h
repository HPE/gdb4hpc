/*********************************************************************************\
 * <Packet_C.h>
 *
 * © 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _GUARD_MRNET_PACKET_H
#define _GUARD_MRNET_PACKET_H

#include "MRNet_C.h"

#ifdef __cplusplus
extern "C" {
#endif

int packet_unpack(MRNet_Packet* packet, const char* ifmt, ...);
void packet_delete(MRNet_Packet* packet);

#ifdef __cplusplus
}
#endif

#endif /* _GUARD_MRNET_PACKET_H */
