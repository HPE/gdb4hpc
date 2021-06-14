/*********************************************************************************\
 * Debug server commands dispatcher definitions.
 *
 * Copyright (c) 1996-2012 by Monash Uni.
 *
 * Copyright 2013,2020-2021 Hewlett Packard Enterprise Development LP.
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

#ifndef _DBGSRV_CMDS_H
#define _DBGSRV_CMDS_H

class dbgcomm_info;
std::unique_ptr<char, decltype(&std::free)> dbgsrv_dispatch_cmds(const char *msg, const dbgcomm_info *ci);

#endif /*_DBGSRV_CMDS_H*/
