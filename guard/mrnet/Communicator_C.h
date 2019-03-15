/*********************************************************************************\
 * <Communicator_C.h>
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



#ifndef _GUARD_MRNET_COMMUNICATOR_H
#define _GUARD_MRNET_COMMUNICATOR_H

#include "MRNet_C.h"

#ifdef __cplusplus
extern "C" {
#endif

int communicator_get_endpoints_size(MRNet_Communicator* communicator);

#ifdef __cplusplus
}
#endif

#endif /* _GUARD_MRNET_COMMUNICATOR_H */
