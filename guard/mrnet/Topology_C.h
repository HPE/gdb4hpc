#ifndef _GUARD_MRNET_TOPOLOGY_H
#define _GUARD_MRNET_TOPOLOGY_H

#include <stdlib.h>
#include <stdio.h>
#include "MRNet_C.h"

unsigned int topology_get_nodes_number(MRNet_Topology *topology);
int topology_get_leaves(MRNet_Topology *topology, MRNet_Node_t **nodes);
int topology_print_leaves(MRNet_Topology *topology, char *file);
void topology_print(MRNet_Topology *topology, FILE *file);
void release_nodes(MRNet_Node_t* nodes, int num);

#endif /* _GUARD_MRNET_TOPOLOGY_H */
