/*********************************************************************************\
 * mrn_topo.h
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


#ifndef _MRN_TOPO_H
#define _MRN_TOPO_H

extern int ProcRankToMRNRank(int);
extern int get_myparent_info(int id, char *host, char *par_ipaddr, unsigned *par_port, unsigned *par_rank);

#endif /*_MRN_TOPO_H*/
