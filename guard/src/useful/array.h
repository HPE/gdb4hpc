/******************************************************************************\
 * array.h - Header file for the array interface.
 *
 * Copyright 2011-2020 Hewlett Packard Enterprise Development LP.
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

#ifndef _ARRAY_H
#define _ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

// default size of the dimdef list
// note that the size of this is automatically increased when calling
// addArrayDimensionDef is there is not enough room in the list for the
// new dimension definition.
#define DIMDEF_SIZE 5
#include "aif.h"
//

// used for returning shape of arrays
typedef struct {
    unsigned int dim;      // dimension of array
    unsigned int *dimDef;  // indicies in each dimension
    int *dimMin;           // min range value in each dimension
    int *dimMax;           // max range value in each dimension
    bool isFortran;        // array is indexed in fortran
    int _len;              // *hidden* allocated size of the dimdef array
} arrayDimension_t;

/* arrayDimension_t operator prototypes */

arrayDimension_t *newArrayDimension(void);
arrayDimension_t *dupArrayDimension(arrayDimension_t *);
int addArrayDimensionDef(arrayDimension_t *, unsigned int, int, int);
unsigned int getArrayDimension(arrayDimension_t *, int);
int getArrayDimensionRangeMin(arrayDimension_t *, int);
int getArrayDimensionRangeMax(arrayDimension_t *, int);
void freeArrayDimension(arrayDimension_t *);

#ifdef __cplusplus
}
#endif

#endif /* _ARRAY_H */
