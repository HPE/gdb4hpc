/*********************************************************************************\
 * array.c - Functions relating to maintaining the shape/size of an array.
 *
 * Copyright 2011-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#include "array.h"
#include <climits>

// external error printing function
extern void PrintOutError(const char *, ...);

ArrayDimension::ArrayDimension() 
    : m_is_fortran{false}
{}

int ArrayDimension::numDimensions() const {
    // outside of this class, the returned value is stored and used as an int instead
    // of a size_t, so we return it as an int, despite size()'s return type of size_t. 
    // we do want to at least make sure no nasty casting ever happens though.
    // hopefully, no one will ever write a program with a 4294967295 dimensional array...
    if (m_dimensions.size() > INT_MAX) {
        PrintOutError("Warning: casting a size_t > INT_MAX to an int\n");
    }
    // let's be explicit about it
    return static_cast<int>(m_dimensions.size());
}

// add a dimension to the object
int ArrayDimension::addDimensionDef(int min_range, int max_range) {
    // assign the new integer representation of the definition string into the
    // list
    m_dimensions.push_back({min_range, max_range});

    return 0;
}

// get the dimension of the domain at the specified index
int ArrayDimension::getArrayDimension(int index) const {
    // sanity check
    if (index <= 0 || index > numDimensions()) {
        PrintOutError("getArrayDimension: index %d boundary error!\n", index);
        return -1;
    }

    auto [min, max] = getArrayDimensionRange(index);
    return max - min + 1;
}

// get the min range for the specified dimension
int ArrayDimension::getArrayDimensionRangeMin(int index) const {
    // sanity check
    if (index <= 0 || index > numDimensions()) {
        PrintOutError("getArrayDimensionRangeMin: index %d boundary error!\n",
                      index);
        return -1;
    }

    // we parsed the string in reverse when creating this dimensional info
    // so we should return the proper index that the caller asks for
    return m_dimensions[numDimensions() - index].minimum;
}

// get the max range for the specified dimension
int ArrayDimension::getArrayDimensionRangeMax(int index) const {
    // sanity check
    if (index <= 0 || index > numDimensions()) {
        PrintOutError("getArrayDimensionRangeMax: index %d boundary error!\n",
                      index);
        return -1;
    }

    // we parsed the string in reverse when creating this dimensional info
    // so we should return the proper index that the caller asks for
    return m_dimensions[numDimensions() - index].maximum;
}

std::pair<int, int>
ArrayDimension::getArrayDimensionRange(int index) const {
    // sanity check
    if (index <= 0 || index > numDimensions()) {
        PrintOutError("getArrayDimensionRange: index %d boundary error!\n",
                      index);
        return {-1, -1};
    }

    // we parsed the string in reverse when creating this dimensional info
    // so we should return the proper index that the caller asks for
    return {
        m_dimensions[numDimensions() - index].minimum,
        m_dimensions[numDimensions() - index].maximum
    };
}

bool ArrayDimension::isFortran() const {
    return m_is_fortran;
}

void ArrayDimension::setIsFortran(bool is_fortran) {
    m_is_fortran = is_fortran;
}
