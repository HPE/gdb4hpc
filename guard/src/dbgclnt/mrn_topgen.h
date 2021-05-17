/*********************************************************************************\
 * mrn_topgen.h
 *
 * Copyright 2010-2013,2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett
 * Packard Enterprise Development LP., no part of this work or its content may be
 * used, reproduced or disclosed in any form.
 *
 *********************************************************************************/

#pragma once

#include "mrnet/NetworkTopology.h"

int create_mrnet_topology(int num_be, char *root, char **hosts, int attaching);
int get_apid(int *nid);
int create_parents_info(MRN::NetworkTopology *topology, int nproc);
int clear_mrnetcfg_files();
int wait_for_creating_mrnetcfg_files();
int get_mrn_filter_timeout();
