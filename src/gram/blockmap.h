/*********************************************************************************\
 * Block Map routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020-2023 Hewlett Packard Enterprise Development LP.
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

#pragma once

#include "dbg_client.h"
#include "dbgblockmap.h"

// see dbgcomm/dbgblockmap.h for the struct

const std::vector<std::shared_ptr<g_block_map_t>> GetBlockMapList();
extern g_block_map_t *FindBlockMap(const std::string& blockmap);
extern bool VerifyBlockMap(g_block_map_t*);
extern g_block_map_t *NewBlockMap(const std::string& name);
void RegisterBlockMap(std::shared_ptr<g_block_map_t> blockmap);

extern std::string GetBlockMapMsg(g_block_map_t *);
extern void FinishBlockMap(void);
extern bool BlockMapIsDegen(g_block_map_t *);
extern AIFStream* construct_blockmap_data(g_block_map_t *bm, const std::string& idx,
                                          rec_set_t* rec_set,
                                          rep_set_t* rep_set);
void PrintWithBlockMap(AIFStream* aif, g_block_map_t* bm, std::ostream& out);
extern size_t BlockMapGetElementsPerRank(g_block_map_t*);
    
extern std::shared_ptr<ais::DecompIterator> MakeBlockMapIterator(g_block_map_t *bm,
                                                                 const std::string& idx);

