/******************************************************************************\
 * vector.h - Header file for the vector interface.
 *
 * © 2014-2019 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

/* This interface is used to store information about the size and shape
 * of an array. This is useful when we wish to pre-allocate an aif array
 * using type information before we create each individual aif entry.
 */
 
#ifndef _VECTOR_H
#define _VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

// default size of the dimdef list
// note that the size of this is automatically increased when calling
// addArrayDimensionDef is there is not enough room in the list for the
// new dimension definition. 
#define DIMDEF_SIZE             5
#include "aif.h"

//used for returning shape of stl containers

struct vectorLikeDimension
{
    unsigned int    	  			dim;            // dimension of array
    unsigned int * 					sizes;
    int 							sizesSize;
	AIF * 							aif;   //actual AIF array being pointed to
	struct vectorLikeDimension * 	parent;
    struct vectorLikeDimension *  	dimdef;         // indicies in each dimension
	int								biggestOfChildren;
    int               				_size;           // *hidden* allocated size of the dimdef array
	int 							_capacity;  	//*hidden* total size of array for smart resize
	struct vectorLikeDimension *    myPointer;

};

typedef struct vectorLikeDimension vectorLikeDimension_t;

/**
	newVecorLikeDimension: 
	Create a new vectorLikeDimension_t object.
**/
vectorLikeDimension_t *      newVectorLikeDimension(void);

/**
	addVectorLikeDimension: 
	Add a new dimension at the specified coordinates with the specified size.
	@param myArray: The vectorLikeDimension_t to which the new dimension will be added
	@param atCoords: int array of coordinates specifying where to add the new dimension (should be NULL to reference the root)
	@param atSize: Size of the atCoords array
	@param size: unsigned int representing the size of the dimension to be added
	@return 0 on success, 1 on error
**/
int 					addVectorLikeDimensionDef(vectorLikeDimension_t * myArray, int* atCoords,int atSize, unsigned int size);

/**
	getVectorLikeDimension: 
	Get the size of the dimension at the specified coordinates.
	@param myArray: The vectorLikeDimension_t from which the dimension will be gotten
	@param atCoords: int array of coordinates specifying where to get the dimension (should be NULL to reference the root)
	@param atCoordsSize: Size of the atCoords array
	@return unsigned int representing the dimension of myArray at coordinates atCoords
**/
unsigned int 			getVectorLikeDimension(vectorLikeDimension_t * myArray, int * atCoords, int atCoordsSize);

/**
	getVectorLikeDimensionStruct: 
	Get the vectorLikeDimension_t object of the dimension at the specified coordinates.
	@param myArray: The vectorLikeDimension_t from which the dimension will be gotten
	@param atCoords: int array of coordinates specifying where to get the dimension (should be NULL to reference the root)
	@param atCoordsSize: Size of the atCoords array
	@return vectorLikeDimension_t struct pointer representing the dimension of myArray at coordinates atCoords
**/
vectorLikeDimension_t * getVectorLikeDimensionStruct(vectorLikeDimension_t * myArray, int * atCoords, int atCoordsSize);

/**
	freeVectorLikeDimension: 
	Free a vectorLikeDimension_t object.
	@param myArray: The vectorLikeDimension_t object to be freed.
**/
void                    freeVectorLikeDimension(vectorLikeDimension_t * myArray);

/**
	createDummyAIF: 
	Create empty AIF arrays based on the specified dimension object and the specified representative element.
	@param shape: the vectorLikeDimension_t object in which to store the AIF arrays
	@param ac: AIF pointer to the representative element used to determine the size of the array in the base case
**/
AIF * 					createDummyAIF(vectorLikeDimension_t * shape, AIF * ac);

/**
	printVectorLikeDimension: 
	Print out a pretty representation of a vectorLikeDimension_t object to stderr
	@param shape: the vectorLikeDimension_t object to print out
**/
void 					printVectorLikeDimension(vectorLikeDimension_t * shape);

/**
	connectAIFArrays: 
	Use the dimension structure to connect the AIF arrays together.
	@param shape: the vectorLikeDimension_t object to connect up
**/
void 					connectAIFArrays(vectorLikeDimension_t * shape);

#ifdef __cplusplus
}
#endif

#endif /* _VECTOR_H */
