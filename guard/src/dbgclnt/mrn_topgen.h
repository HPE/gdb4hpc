#ifndef _GUARD_MRNET_TOPGEN_H
#define _GUARD_MRNET_TOPGEN_H

extern int create_mrnet_topology(int num_be, char *root, char **hosts, int attaching);
extern int get_apid(int *nid);
extern int create_parents_info(MRNet_Topology *topology, int nproc);
extern int clear_mrnetcfg_files();
extern int wait_for_creating_mrnetcfg_files();
extern int get_mrn_filter_timeout();

#endif /*_GUARD_MRNET_TOPGEN_H*/
