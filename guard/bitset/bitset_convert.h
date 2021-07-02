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

#include "bitset.h"
#include "dense_bitset.h"
#include "sparse_bitset.h"

// MRNet's behaviour is to package bitsets up as sparse bitsets, send them over
// the network, and then change them to dense bitsets once they get to the
// client side.

namespace bitset {

std::unique_ptr<DenseBitset> createDenseFrom(const SparseBitset &bitset);

} // bitset
