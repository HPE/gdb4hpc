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

// external error printing function
extern void PrintOutError(const char *, ...);

ArrayDimension::ArrayDimension() 
    : isFortran{false}
{}

// add a dimension to the object
int ArrayDimension::addDimensionDef(size_t size, unsigned int min_range, unsigned int max_range) {
    // assign the new integer representation of the definition string into the
    // list
    m_dimensions.push_back({size, min_range, max_range});

    return 0;
}

// get the dimension of the domain at the specified index
int ArrayDimension::getArrayDimension(unsigned int index) const {
    // sanity check
    if (index <= 0 || index > m_dimensions.size()) {
        PrintOutError("getArrayDimension: index %d boundary error!\n", index);
        return -1;
    }

    // we parsed the string in reverse when creating this dimensional info
    // so we should return the proper index that the caller asks for
    return std::get<0>(m_dimensions[m_dimensions.size() - index]);
}

// get the min range for the specified dimension
int ArrayDimension::getArrayDimensionRangeMin(unsigned int index) const {
    // sanity check
    if (index <= 0 || index > m_dimensions.size()) {
        PrintOutError("getArrayDimensionRangeMin: index %d boundary error!\n",
                      index);
        return -1;
    }

    // we parsed the string in reverse when creating this dimensional info
    // so we should return the proper index that the caller asks for
    return std::get<1>(m_dimensions[m_dimensions.size() - index]);
}

// get the max range for the specified dimension
int ArrayDimension::getArrayDimensionRangeMax(unsigned int index) const {
    // sanity check
    if (index <= 0 || index > m_dimensions.size()) {
        PrintOutError("getArrayDimensionRangeMax: index %d boundary error!\n",
                      index);
        return -1;
    }

    // we parsed the string in reverse when creating this dimensional info
    // so we should return the proper index that the caller asks for
    return std::get<2>(m_dimensions[m_dimensions.size() - index]);
}

size_t ArrayDimension::numDimensions() const {
    return m_dimensions.size();
}
