/*********************************************************************************\
 * <bitset_convert.cpp>
 *
 * Copyright 2012-2013,2021 Hewlett Packard Enterprise Development LP.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 *********************************************************************************/
#include "bitset_convert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include <sstream>

#include "dense_bitset.h"
#include "sparse_bitset.h"
#include "lrunle_bitset.h"

namespace bitset {

std::unique_ptr<DenseBitset> createDenseFrom(const SparseBitset &sparse) {
	auto dense = std::make_unique<DenseBitset>(sparse.sizeInBits());

	if (sparse.sizeInRanges() == 0) {
		// empty bitset, we're done
		return dense;
	}

	for (auto it = sparse.getSetBitsIterator(); *it < sparse.sizeInBits(); ++it) {
		dense->set(*it);
	}

	return dense;
}

std::unique_ptr<DenseBitset> createDenseFrom(const LRunLEBitset &lrun) {
	auto dense = std::make_unique<DenseBitset>(lrun.sizeInBits());

	for (size_t bit : lrun) {
		dense->set(bit);
	}

	return dense;
}

} // bitset
