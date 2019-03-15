/*********************************************************************************\
 * vector.c - Functions relating to maintaining the shape/size of a vector.
 *
 * © 2014-2019 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
#include "vector.h"

// external error printing function
extern void	PrintOutError(char *, ...);

vectorLikeDimension_t * newVectorLikeDimension(void){
		vectorLikeDimension_t *      vec_dim;
        
        // create the object
        if ((vec_dim = (vectorLikeDimension_t*)malloc(sizeof(vectorLikeDimension_t))) == (void *)0)
        {
                printf("newVectorLikeDimension: malloc error!\n");
                return (vectorLikeDimension_t *)NULL;
        }
        memset(vec_dim, 0, sizeof(vectorLikeDimension_t));     // clear it to NULL
        vec_dim->myPointer = vec_dim;
        // allocate the default dimdef size
        if ((vec_dim->dimdef = (vectorLikeDimension_t*)malloc(sizeof(vectorLikeDimension_t) * DIMDEF_SIZE)) == (void *)0)
        {
                printf("newArrayDimension: malloc error!\n");
                freeVectorLikeDimension(vec_dim);
                return (vectorLikeDimension_t *)NULL;
        }
        memset(vec_dim->dimdef, 0, sizeof(vectorLikeDimension_t) * DIMDEF_SIZE);     // clear it to NULL
        // set the hidden capacity value
		vec_dim->_capacity = DIMDEF_SIZE;
		
        if ((vec_dim->sizes = (unsigned int*)malloc(sizeof(unsigned int) * DIMDEF_SIZE)) == (void *)0)
        {
                printf("newArrayDimension: malloc error!\n");
                freeVectorLikeDimension(vec_dim);
                return (vectorLikeDimension_t *)NULL;
        }	
        memset(vec_dim->sizes, 0, sizeof(unsigned int)*DIMDEF_SIZE);
        vec_dim->sizesSize = 0;
        
        return vec_dim;


}


//TODO: HANDLE Vectors of Arrays
int addVectorLikeDimensionDef(vectorLikeDimension_t * myArray, int* atCoords,int atSize, unsigned int size){
        // sanity check
        if (myArray == (vectorLikeDimension_t *)NULL)
        {
                PrintOutError("addVectorLikeDimensionDef: null args!\n");
                return 1;
        }
        
        myArray->sizes = (unsigned int*)realloc(myArray->sizes, (myArray->sizesSize+1)*sizeof(unsigned int));
        myArray->sizes[myArray->sizesSize++] = 0;
        
		//Traverse the vectorLikeDimension structure using atcoords until
		//we get to the node we want
		int i;
		vectorLikeDimension_t * curr = myArray;
		if(atCoords){
			for(i=0; i<atSize; i++){
				if(curr->dimdef){
					curr = &curr->dimdef[*atCoords];
				}
				++atCoords;
			}
		}
		
		//We have arrived at the node we're interested in. Set all values appropriately.
		curr->dim = size;
		curr->_size = size;
		
		//Malloc dimdef and memset it to zero based on size
		curr->dimdef = (vectorLikeDimension_t*)realloc(curr->dimdef,  size*sizeof(vectorLikeDimension_t));
		memset(curr->dimdef,0,size*sizeof(vectorLikeDimension_t));

		
		//Create new nodes at each allocated spot in dimdef and set parents
		for(i=0; i<size; i++){
			curr->dimdef[i] = *newVectorLikeDimension();
			curr->dimdef[i].parent = curr;
		}
        return 0;
		


}

vectorLikeDimension_t * getVectorLikeDimensionStruct(vectorLikeDimension_t * myArray, int * atCoords, int atCoordsSize){
	vectorLikeDimension_t * curr = myArray;
	
	//Error if either argument is null
	if(!myArray || !atCoords){
		PrintOutError("getVectorLikeDimensionStruct: null args!\n");
		return NULL;
	}
	
	//Traverse the node structure to get to the nide we want
	int i;
	for(i=0; i<atCoordsSize; i++){
		if(curr->dimdef){
			curr = &curr->dimdef[atCoords[i]];
		}
	}
	
	
	//We have the node we want, so return it
	return curr;

}

unsigned int getVectorLikeDimension(vectorLikeDimension_t * myArray, int * atCoords, int atCoordsSize){
	vectorLikeDimension_t * curr = myArray;
	
	//Error if either argument is null
	if(!myArray || !atCoords){
		PrintOutError("getVectorLikeDimension: null args!\n");
		return 0;
	}
	
	//Traverse the node structure to get to the node we want
	int i;
	for(i=0; i<atCoordsSize; i++){
		if(curr->dimdef){
			curr = &curr->dimdef[atCoords[i]];
		}
	}
	
	//We have the node we want, so return its dimension
	return curr->dim;

}

AIF * recDummyAIF(vectorLikeDimension_t * myArray, vectorLikeDimension_t * shape, AIF * ac, int depth){
	
	//Error if either argument is null
	if(!shape || !ac){
		PrintOutError("createDummyAIF: null args!\n");
		return NULL;
	}
	
	//Base case is when the size of the shape is zero.
	//Return the representative element (ac) that can be used to initialize empty arrays.
	if(shape->_size==0)
	{
		shape->aif = ac;
		PrintOutError("createDummyAIF: base case, returning ac (the representative element)\n");
		return ac;	
	}
	
	//If not the base case, get AIF of all children first
	vectorLikeDimension_t * child = NULL;
	int i;
	for(i=0; i<shape->_size; i++)
	{
		child = &shape->dimdef[i];
		child->aif = recDummyAIF(myArray, child, ac, depth+1);
	}
	PrintOutError("createDummyAIF: Done creating children AIF\n");
	
	//If the node has a parent, ask that parent for how big to make the AIF array
	if(shape->dimdef && shape->_size > 0){
		shape->aif = EmptyArrayToAIF(0,myArray->sizes[depth]-1,shape->dimdef[0].aif);
		PrintOutError("createDummyAIF: Done creating shape's AIF. Shape is an internal node\n");	
	}
	
	//return the resulting AIF
	return shape->aif;
	

}

AIF * createDummyAIF(vectorLikeDimension_t * shape, AIF * ac){
	return recDummyAIF(shape, shape, ac,0);
}

void printVectorLikeDimension(vectorLikeDimension_t * shape){	
	int i;
	fprintf(stderr, "Dim: %d  [", shape->dim);
	for(i=0; i<shape->_size; i++){
		printVectorLikeDimension(&shape->dimdef[i]);
		if(i<shape->_size-1)
			fprintf(stderr, " , ");
	}
	fprintf(stderr, "]");
	
}

void freeVectorLikeDimension(vectorLikeDimension_t * shape){
	if(!shape)
		return;
	vectorLikeDimension_t * child = NULL;

	int i;
	for(i=0; i<shape->_size; i++)
	{
		child = &shape->dimdef[i];
		freeVectorLikeDimension(child);
	}
	
	free(shape->dimdef);
	free(shape->sizes);
	free(shape->myPointer);

}

void connectAIFArrays(vectorLikeDimension_t * shape){
	if(!shape)
		return;
	vectorLikeDimension_t * child = NULL;

	//using the node structure add the aif of every one of my children to my AIF array
	int i;
	for(i=0; i<shape->_size; i++)
	{
		child = &shape->dimdef[i];
		connectAIFArrays(child);
		AIFAddArrayElement(shape->aif, i, child->aif);
	}

}
