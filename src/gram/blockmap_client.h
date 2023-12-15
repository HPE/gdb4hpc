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
#include "blockmap.h"

// see dbgcomm/blockmap.h for the struct

const std::vector<std::shared_ptr<BlockMap>> GetBlockMapList();
extern BlockMap *FindBlockMap(const std::string& blockmap);
extern bool VerifyBlockMap(BlockMap*);
extern BlockMap *NewBlockMap(const std::string& name);
void RegisterBlockMap(std::shared_ptr<BlockMap> blockmap);

// remove `name` from internal blockmap list. returns true if `name` existed and was
// removed, `false` if it never existed in the first place.
bool UnregisterBlockMap(const std::string& name);

extern void FinishBlockMap(void);

void PrintWithBlockMap(AIFStream* aif, BlockMap* bm, std::ostream& out);
    
extern std::shared_ptr<ais::DecompIterator> MakeBlockMapIterator(BlockMap *bm,
                                                                 const std::string& idx);

