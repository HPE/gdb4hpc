/*********************************************************************************\
 * <Topology.cpp>
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

#include <string.h>

#include <vector>

#include "MRNet_type_wrapper.h"
#include "mrnet/NetworkTopology.h"
#include "mrnet/Types.h"

using namespace MRN;

extern "C" unsigned int topology_get_nodes_number(MRNet_Topology *topology) {
    NetworkTopology *pTopology = (NetworkTopology *)topology;
    return pTopology->get_NumNodes();
}

extern "C" int topology_print_leaves(MRNet_Topology *topology, char *file) {
    std::vector<NetworkTopology::Node *> leaves;
    NetworkTopology *pTopology = (NetworkTopology *)topology;

    pTopology->get_Leaves(leaves);
    unsigned num_leaves = leaves.size();

    // fprintf(*err_stream_ptr, "INFO: mrnet::topology_print_leaves [%p] has %d
    // leavs!\n", topology, num_leaves);

    if (num_leaves <= 0) {
        return 0;
    }

    FILE *pfile = fopen(file, "w");
    if (pfile == NULL) {
        // fprintf(*err_stream_ptr, "ERROR: cannot open %s to print MRNet
        // leaves!\n", file);
        return 0;
    }

    for (unsigned index = 0; index < num_leaves; ++index) {
        fprintf(pfile, "%s\t%d\t%d\n", leaves[index]->get_HostName().c_str(),
                leaves[index]->get_Port(), leaves[index]->get_Rank());

        /*fprintf(*err_stream_ptr, "%s\t%d\t%d\n",
                                leaves[index]->get_HostName().c_str(),
                                leaves[index]->get_Port(),
                                leaves[index]->get_Rank());*/
    }

    fclose(pfile);
    return num_leaves;
}

/* Guard initialization ensures that each physical node has only one leafnode*/
extern "C" int topology_get_leaves(MRNet_Topology *topology,
                                   MRNet_Node_t **nodes) {
    std::vector<NetworkTopology::Node *> leaves;
    NetworkTopology *pTopology = (NetworkTopology *)topology;

    pTopology->get_Leaves(leaves);
    unsigned num_leaves = leaves.size();
    if (num_leaves <= 0) {
        *nodes = NULL;
        return 0;
    }

    MRNet_Node_t *pNode =
        (MRNet_Node_t *)malloc(num_leaves * sizeof(MRNet_Node_t));
    if (pNode == NULL) {
        // fprintf(*err_stream_ptr, "ERROR: cannot allocate enough memory for
        // leaves!\n");
        return 0;
    }

    for (unsigned index = 0; index < num_leaves; ++index) {
        pNode[index].hostname =
            strndup(leaves[index]->get_HostName().c_str(), 1024);
        pNode[index].port = leaves[index]->get_Port();
        pNode[index].rank = leaves[index]->get_Rank();
    }

    *nodes = pNode;
    return num_leaves;
}

extern "C" void release_nodes(MRNet_Node_t *nodes, int num) {
    if (nodes == NULL || num < 1) return;

    for (int index = 0; index < num; ++index) {
        if (nodes[index].hostname != NULL) free(nodes[index].hostname);
    }

    free(nodes);
}

extern "C" void topology_print(MRNet_Topology *topology, FILE *file) {
    NetworkTopology *pTopology = (NetworkTopology *)topology;
    pTopology->print(file);
}

/*
    void print_TopologyFile( const char * filename ) const;
    void print_DOTGraph( const char * filename ) const;
    void print( FILE * ) const;


    unsigned int get_NumNodes() const;
    void get_Leaves( std::vector< Node * > &leaves ) const;
*/
