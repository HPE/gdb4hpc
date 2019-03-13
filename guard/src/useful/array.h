/******************************************************************************\
 * array.h - Header file for the array interface.
 *
 * © 2011-2013 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
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
#define DIMDEF_SIZE             5
#include "aif.h"
//

// used for returning shape of arrays
typedef struct
{
        unsigned int    dim;            // dimension of array
        unsigned int *  dimdef;         // indicies in each dimension
        int *           dimmin;         // min range value in each dimension
        int *           dimmax;         // max range value in each dimension
        int             _len;           // *hidden* allocated size of the dimdef array
} arrayDimension_t;

/* arrayDimension_t operator prototypes */

arrayDimension_t *      newArrayDimension(void);
int                     addArrayDimensionDef(arrayDimension_t *, unsigned int, int, int);
unsigned int            getArrayDimension(arrayDimension_t *, int);
int                     getArrayDimensionRangeMin(arrayDimension_t *, int);
int                     getArrayDimensionRangeMax(arrayDimension_t *, int);
void                    freeArrayDimension(arrayDimension_t *);

#ifdef __cplusplus
}
#endif

#endif /* _ARRAY_H */
