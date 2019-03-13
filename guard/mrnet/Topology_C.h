/*********************************************************************************\
 * <Topology_C.h>
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
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


#ifndef _GUARD_MRNET_TOPOLOGY_H
#define _GUARD_MRNET_TOPOLOGY_H

#include <stdlib.h>
#include <stdio.h>
#include "MRNet_C.h"

#ifdef __cplusplus
extern "C" {
#endif

unsigned int topology_get_nodes_number(MRNet_Topology *topology);
int topology_get_leaves(MRNet_Topology *topology, MRNet_Node_t **nodes);
int topology_print_leaves(MRNet_Topology *topology, char *file);
void topology_print(MRNet_Topology *topology, FILE *file);
void release_nodes(MRNet_Node_t* nodes, int num);

#ifdef __cplusplus
}
#endif

#endif /* _GUARD_MRNET_TOPOLOGY_H */
