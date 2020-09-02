/*********************************************************************************\
 * mrn_topgen.h
 *
 * Copyright 2010-2013,2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#ifndef _GUARD_MRNET_TOPGEN_H
#define _GUARD_MRNET_TOPGEN_H

extern int create_mrnet_topology(int num_be, char *root, char **hosts,
                                 int attaching);
extern int get_apid(int *nid);
extern int create_parents_info(MRNet_Topology *topology, int nproc);
extern int clear_mrnetcfg_files();
extern int wait_for_creating_mrnetcfg_files();
extern int get_mrn_filter_timeout();

#endif /*_GUARD_MRNET_TOPGEN_H*/
