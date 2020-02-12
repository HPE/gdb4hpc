/*********************************************************************************\
 * array.c - Functions relating to maintaining the shape/size of an array.
 *
 * Copyright 2011-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// external error printing function
extern void PrintOutError(const char *, ...);

/* static prototypes */
static int resizeArrayDimension(arrayDimension_t *);

// create a new arrayDimension_t object
arrayDimension_t *newArrayDimension() {
    arrayDimension_t *arr_dim;

    // create the object
    if ((arr_dim = (arrayDimension_t *)malloc(sizeof(arrayDimension_t))) ==
        (void *)0) {
        PrintOutError("newArrayDimension: malloc error!\n");
        return (arrayDimension_t *)NULL;
    }
    memset(arr_dim, 0, sizeof(arrayDimension_t));  // clear it to NULL

    // allocate the default dimdef size
    if ((arr_dim->dimdef = (unsigned int *)malloc(sizeof(unsigned int) *
                                                  DIMDEF_SIZE)) == (void *)0) {
        PrintOutError("newArrayDimension: malloc error!\n");
        freeArrayDimension(arr_dim);
        return (arrayDimension_t *)NULL;
    }
    memset(arr_dim->dimdef, 0,
           sizeof(unsigned int) * DIMDEF_SIZE);  // clear it to NULL

    // allocate the default dimdef size for the min range
    if ((arr_dim->dimmin = (int *)malloc(sizeof(int) * DIMDEF_SIZE)) ==
        (void *)0) {
        PrintOutError("newArrayDimension: malloc error!\n");
        freeArrayDimension(arr_dim);
        return (arrayDimension_t *)NULL;
    }
    memset(arr_dim->dimmin, 0, sizeof(int) * DIMDEF_SIZE);  // clear it to NULL

    // allocate the default dimdef size for the max range
    if ((arr_dim->dimmax = (int *)malloc(sizeof(int) * DIMDEF_SIZE)) ==
        (void *)0) {
        PrintOutError("newArrayDimension: malloc error!\n");
        freeArrayDimension(arr_dim);
        return (arrayDimension_t *)NULL;
    }
    memset(arr_dim->dimmax, 0, sizeof(int) * DIMDEF_SIZE);  // clear it to NULL

    // set the hidden len value
    arr_dim->_len = DIMDEF_SIZE;

    return arr_dim;
}

// resize an arrayDimension_t object
static int resizeArrayDimension(arrayDimension_t *myArray) {
    unsigned int *temp;
    int *temp_range;

    // sanity check
    if (myArray == (arrayDimension_t *)NULL) {
        PrintOutError("resizeArrayDimension: null arg!\n");
        return 1;
    }

    // resize the dimdef
    if ((temp = (unsigned int *)realloc(
             myArray->dimdef,
             sizeof(unsigned int) * (myArray->_len + DIMDEF_SIZE))) ==
        (void *)0) {
        PrintOutError("resizeArrayDimension: realloc error!\n");
        return 1;
    }
    // null the new memory starting at the first entry after the old _len
    // note that offsetting to _len in the new array will be this member.
    memset(&temp[myArray->_len], 0, sizeof(unsigned int) * DIMDEF_SIZE);

    myArray->dimdef = temp;

    // resize the dimmin
    if ((temp_range = (int *)realloc(
             myArray->dimmin, sizeof(int) * (myArray->_len + DIMDEF_SIZE))) ==
        (void *)0) {
        PrintOutError("resizeArrayDimension: realloc error!\n");
        return 1;
    }
    // null the new memory starting at the first entry after the old _len
    // note that offsetting to _len in the new array will be this member.
    memset(&temp_range[myArray->_len], 0, sizeof(int) * DIMDEF_SIZE);

    myArray->dimmin = temp_range;

    // resize the dimmax
    if ((temp_range = (int *)realloc(
             myArray->dimmax, sizeof(int) * (myArray->_len + DIMDEF_SIZE))) ==
        (void *)0) {
        PrintOutError("resizeArrayDimension: realloc error!\n");
        return 1;
    }
    // null the new memory starting at the first entry after the old _len
    // note that offsetting to _len in the new array will be this member.
    memset(&temp_range[myArray->_len], 0, sizeof(int) * DIMDEF_SIZE);

    myArray->dimmax = temp_range;

    // increase the private len variable
    myArray->_len += DIMDEF_SIZE;

    return 0;
}

// add a dimension to the object
int addArrayDimensionDef(arrayDimension_t *myArray, unsigned int size,
                         int min_range, int max_range) {
    // sanity check
    if (myArray == (arrayDimension_t *)NULL) {
        PrintOutError("addArrayDimensionDef: null args!\n");
        return 1;
    }

    // ensure we have enough room in the dimdef array for a new entry
    if (myArray->_len <= int(myArray->dim) + 1) {
        if (resizeArrayDimension(myArray)) {
            PrintOutError(
                "addArrayDimensionDef: resizeArrayDimension failed!\n");
            return 1;
        }
    }

    // assign the new integer representation of the definition string into the
    // list
    myArray->dimdef[myArray->dim] = size;
    myArray->dimmin[myArray->dim] = min_range;
    myArray->dimmax[myArray->dim] = max_range;

    // increment the dim counter
    ++myArray->dim;

    return 0;
}

// get the dimension of the domain at the specified index
unsigned int getArrayDimension(arrayDimension_t *myArray, int index) {
    // sanity check
    if (myArray == (arrayDimension_t *)NULL) {
        PrintOutError("getArrayDimension: null args!\n");
        return -1;
    }

    if (index <= 0 || index > int(myArray->dim)) {
        PrintOutError("getArrayDimension: index %d boundary error!\n", index);
        return -1;
    }

    // we parsed the string in reverse when creating this dimensional info
    // so we should return the proper index that the caller asks for
    return myArray->dimdef[myArray->dim - index];
}

// get the min range for the specified dimension
int getArrayDimensionRangeMin(arrayDimension_t *myArray, int index) {
    // sanity check
    if (myArray == (arrayDimension_t *)NULL) {
        PrintOutError("getArrayDimensionRangeMin: null args!\n");
        return -1;
    }

    if (index <= 0 || index > int(myArray->dim)) {
        PrintOutError("getArrayDimensionRangeMin: index %d boundary error!\n",
                      index);
        return -1;
    }

    // we parsed the string in reverse when creating this dimensional info
    // so we should return the proper index that the caller asks for
    return myArray->dimmin[myArray->dim - index];
}

// get the max range for the specified dimension
int getArrayDimensionRangeMax(arrayDimension_t *myArray, int index) {
    // sanity check
    if (myArray == (arrayDimension_t *)NULL) {
        PrintOutError("getArrayDimensionRangeMax: null args!\n");
        return -1;
    }

    if (index <= 0 || index > int(myArray->dim)) {
        PrintOutError("getArrayDimensionRangeMax: index %d boundary error!\n",
                      index);
        return -1;
    }

    // we parsed the string in reverse when creating this dimensional info
    // so we should return the proper index that the caller asks for
    return myArray->dimmax[myArray->dim - index];
}

// free a arrayDimension_t object
void freeArrayDimension(arrayDimension_t *myArray) {
    // sanity check
    if (myArray == (arrayDimension_t *)NULL) {
        PrintOutError("freeArrayDimension: null arg!\n");
        return;
    }

    // free the dimdef array
    if (myArray->dimdef != (unsigned int *)NULL) {
        free(myArray->dimdef);
    }

    // free the dimmin array
    if (myArray->dimmin != (int *)NULL) {
        free(myArray->dimmin);
    }

    // free the dimmin array
    if (myArray->dimmax != (int *)NULL) {
        free(myArray->dimmax);
    }

    // free the arrayDimension_t object
    free(myArray);
}
