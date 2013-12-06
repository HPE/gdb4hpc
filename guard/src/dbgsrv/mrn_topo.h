#ifndef _MRN_TOPO_H
#define _MRN_TOPO_H

extern int ProcRankToMRNRank(int);
extern int get_myparent_info(int id, char *host, char *par_host, unsigned *par_port, unsigned *par_rank);

#endif /*_MRN_TOPO_H*/
