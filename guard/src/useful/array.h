/******************************************************************************\
 * array.h - Header file for the array interface.
 *
 * Copyright 2011-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

/* This interface is used to store information about the size and shape
 * of an array. This is useful when we wish to pre-allocate an aif array
 * using type information before we create each individual aif entry.
 */

#pragma once

#include <vector>
#include <tuple>
#include "aif.h"

// used for returning shape of arrays
class ArrayDimension {
public:
    ArrayDimension();

    size_t numDimensions() const;
    int addDimensionDef(size_t size, unsigned int min_range, unsigned int max_range);
    int getArrayDimension(unsigned int index) const;
    int getArrayDimensionRangeMin(unsigned int index) const;
    int getArrayDimensionRangeMax(unsigned int index) const;
    
    bool isFortran;            // array is indexed in fortran
private:
    // tuple is (dimension size, range minimum, range maximum)
    std::vector<std::tuple<int, int, int>> m_dimensions;
};
