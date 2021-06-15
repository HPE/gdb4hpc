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

// used for returning shape of arrays
class ArrayDimension {
public:
    ArrayDimension();

    int numDimensions() const;
    int addDimensionDef(int min_range, int max_range);
    int getArrayDimension(int index) const;
    int getArrayDimensionRangeMin(int index) const;
    int getArrayDimensionRangeMax(int index) const;
    // returns (min, max) for the given index
    std::pair<int, int> getArrayDimensionRange(int index) const;
    
    bool isFortran() const;
    void setIsFortran(bool is_fortran);
private:
    struct Range {
        int minimum;
        int maximum;
    };
    std::vector<Range> m_dimensions;
    
    // true if array is indexed in fortran. not used internally by the class.
    bool m_is_fortran;
};
