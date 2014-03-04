/*********************************************************************************\
 * <smart_bitset.c>
 *
 * © 2012-2013 Cray Inc.  All Rights Reserved.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "smart_bitset.h"
#include "conversion.h"
#include "sparse_bitset.h"
#include "bitset.h"

smart_bitset*	
smart_bitset_new_no_space()
{
	smart_bitset *	sb;

	sb = malloc(sizeof(smart_bitset));

	sb->sb_dense = NULL;
	sb->sb_sparse = NULL;

	return sb;
}

smart_bitset*	
smart_bitset_new(int num_bits)
{
	smart_bitset *	sb;

	sb = malloc(sizeof(smart_bitset));

	sb->sb_dense = NULL;
	sb->sb_sparse = sparse_bitset_new(num_bits);

//	sb->sb_dense = bitset_new(num_bits);
//	sb->sb_sparse = NULL;

	return sb;
}

void			
smart_bitset_free(smart_bitset *sb)
{
	if(sb == NULL)
		return;

	if(sb->sb_dense != NULL)
	{
		bitset_free(sb->sb_dense);
		sb->sb_dense = NULL;
	}

	if(sb->sb_sparse != NULL)
	{
		sparse_bitset_free(sb->sb_sparse);
		sb->sb_sparse = NULL;
	}

	free(sb);

	sb = NULL;
}

void		
smart_bitset_clear(smart_bitset *sb)
{
	if(sb == NULL)
		return;

	if(sb->sb_dense != NULL)
		bitset_clear(sb->sb_dense);

	if(sb->sb_sparse != NULL)
		sparse_bitset_clear(sb->sb_sparse);
}

void
smart_bitset_copy(smart_bitset *sb1, smart_bitset *sb2)
{
	if(sb1 == NULL || sb2 == NULL)
		return;

	if(sb1->sb_dense != NULL && sb2->sb_dense != NULL)
		bitset_copy(sb1->sb_dense, sb2->sb_dense);
	else if(sb1->sb_sparse != NULL && sb2->sb_sparse != NULL)
		sparse_bitset_copy(sb1->sb_sparse, sb2->sb_sparse);
	else {
		/*TODO*/
	}
}

int			
smart_bitset_is_sparse(smart_bitset *sb)
{
	if(sb != NULL && sb->sb_sparse != NULL)
		return 1;

	return 0;
}

int				
smart_bitset_is_dense(smart_bitset *sb)
{
	if(sb != NULL && sb->sb_dense != NULL)
		return 1;

	return 0;
}

smart_bitset*
smart_bitset_dup(smart_bitset* sb)
{
	smart_bitset *nb = smart_bitset_new_no_space();

	if(sb->sb_dense != NULL)
		nb->sb_dense = bitset_dup(sb->sb_dense);

	if(sb->sb_sparse != NULL)
		nb->sb_sparse = sparse_bitset_dup(sb->sb_sparse);

	return nb;
}

void			
smart_bitset_set(smart_bitset *sb, int n)
{
	if(sb == NULL)
		return;

	if(sb->sb_dense != NULL)
		bitset_set(sb->sb_dense, n);

	if(sb->sb_sparse != NULL)
		sparse_bitset_set(sb->sb_sparse, n);
}

smart_bitset*	
smart_bitset_or(smart_bitset *sb1, smart_bitset *sb2)
{
	smart_bitset* sb = smart_bitset_dup(sb1);
	smart_bitset_oreq(sb, sb2);

	return sb;
}

void			
smart_bitset_oreq(smart_bitset *sb1, smart_bitset *sb2)
{
	if(sb1 == NULL || sb2 == NULL)
		return;

	if(sb1->sb_dense != NULL && sb2->sb_dense != NULL)
		bitset_oreq(sb1->sb_dense, sb2->sb_dense);
	else if(sb1->sb_sparse != NULL && sb2->sb_sparse != NULL)
		sparse_bitset_oreq(sb1->sb_sparse, sb2->sb_sparse);
	else if(sb1->sb_dense != NULL && sb2->sb_sparse != NULL){
		smart_bitset_convert(sb2);
		bitset_oreq(sb1->sb_dense, sb2->sb_dense);
	} else if(sb1->sb_sparse != NULL && sb2->sb_dense != NULL){
		smart_bitset_convert(sb1);
		bitset_oreq(sb1->sb_dense, sb2->sb_dense);
	}
}

int				
smart_bitset_convert(smart_bitset *sb)
{
	if(sb == NULL || sb->sb_sparse == NULL || sb->sb_dense != NULL)
		return -1;

	sparse_bitset* sparse = sb->sb_sparse;
	sb->sb_dense = bitset_new(sparse->sb_num_bits);
	
	int num_ranges = sparse->sb_num_ranges;
	sparse_range* ranges = sparse->sb_ranges;
	if(ranges == NULL || num_ranges <= 0)
		return 0;
	
	int i, j;
	for(i = 0; i < num_ranges; i++)
		for(j = ranges[i].sr_start; j <= ranges[i].sr_end; j++)
			bitset_set(sb->sb_dense, j);

	return 0;
}

char*		 
smart_bitset_to_set(sparse_bitset *b)
{
	return NULL;
}

smart_bitset*	
set_to_smart_bitset(char *set)
{
	return NULL;
}

char*		 	
smart_bitset_serialize(smart_bitset *sb)
{
	char* 	content;
	int 	format;
	int		num_bits;

	if(sb == NULL || (sb->sb_dense == NULL && sb->sb_sparse == NULL))
		return strdup("0;0;{}");

	if(sb->sb_dense != NULL)
	{
		content = bitset_serialize(sb->sb_dense);
		format = BT_FORMAT_DENSE;
		num_bits = sb->sb_dense->bs_nbits;
	}
	else if(sb->sb_sparse != NULL)
	{
		content = sparse_bitset_serialize(sb->sb_sparse);
		format = BT_FORMAT_SPARSE;
		num_bits = sb->sb_sparse->sb_num_bits;
	}

	char* res = (char *)malloc(strlen(content)+16);
	sprintf(res, "%x;%x;%s", format, num_bits, content);

	return res;
}

smart_bitset*	
smart_bitset_deserialize(char *set)
{
	bitset* dense = NULL;
	sparse_bitset* sparse = NULL;
	smart_bitset* smart = NULL;

	int index, start;
	int format = -1, size = -1, res;

	//get format
	start = 0;
	for (index = start; set[index] != ';' && set[index] != '\0'; index++);
	res = atoi_hex(set, start, index - 1, &format);
	if(res < 0)
		return NULL;

	//get size
	start = index + 1;
	for (index = start; set[index] != ';' && set[index] != '\0'; index++);
	res = atoi_hex(set, start, index - 1, &size);
	if(res < 0 || size < 1)
		return NULL;

	int str_len = strlen(set);
	switch (format)
	{
		case BT_FORMAT_DENSE:
			dense = bitset_deserialize(&set[index+1]);
			break;
		case BT_FORMAT_SPARSE:
			sparse = sparse_bitset_deserialize(size, &set[index+1], str_len-index-1);
			break;
		default:
			return NULL;
			break;
	}

	smart = smart_bitset_new_no_space();
	smart->sb_dense = dense;
	smart->sb_sparse = sparse;
	
	return smart;
}
