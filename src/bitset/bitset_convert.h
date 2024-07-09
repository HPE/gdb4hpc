/*********************************************************************************\
 * <bitset_convert.h>
 *
 * Copyright 2012-2013,2021 Hewlett Packard Enterprise Development LP.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 *********************************************************************************/
#pragma once

#include "config.h"

#include "bitset.h"
#include "dense_bitset.h"

#ifdef MRNET
#include "sparse_bitset.h"
#endif

#ifdef ZMQNET
#include "lrunle_bitset.h"
#endif

namespace bitset {

#ifdef MRNET
std::unique_ptr<DenseBitset> createDenseFrom(const SparseBitset &bitset);
#endif

#ifdef ZMQNET
std::unique_ptr<DenseBitset> createDenseFrom(const LRunLEBitset &bitset);
#endif

} // bitset
