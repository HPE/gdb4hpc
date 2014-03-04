/*********************************************************************************\
 * <sparse_bitset.c>
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

#include "sparse_bitset.h"
#include "varstr.h"
#include "conversion.h"

int
binary_search(sparse_range* ranges, int size, int value)
{
	int pos;

	if(ranges == NULL || size == 0)
		return -1;	

	int low = 0;
	int high = size-1;
	int mid = 0;
	int found = 0;
	while(!found && low < high)
	{
		mid = (high + low)/2;
		if(value < ranges[mid].sr_start)
			high = mid-1;
		else if(value > ranges[mid].sr_end)
			low = mid+1;
		else if(value >= ranges[mid].sr_start && value <= ranges[mid].sr_end)
		{
			found = 1;
			pos = mid;
			break;
		}
	}
	
	if(found == 0)
	{
		if(high < 0)
			pos = -1;
		else if(low >= size)
			pos = size;	
		else if(value > ranges[mid].sr_end)
			pos = mid+1;
		else
			pos = mid;
	}

	return pos;
}

int 
count_char(sparse_range* range)
{
	int res = 0;
	if(range == NULL || range->sr_start < 0 || range->sr_end < 0 || range->sr_start > range->sr_end)
		return res;

	int	count;
	int start = range->sr_start, end = range->sr_end;

	count = 1;
	while(start >= 16)
	{
		start = start >> 4;
		count++;
	}

	if(start != end)
	{
		count++;
		while(end >= 16)
		{
			end = end >> 4;
			count++;
		}

		count++; // '-'
	}
	
	return count;
}

void
sparse_bitset_extend_size(sparse_bitset* sb, int size)
{
	sparse_range* old;
	int new_size;

	if(sb == NULL)
		return;

	if(sb->sb_size <= 0)
	{
	    sb->sb_ranges = (sparse_range*)malloc(sizeof(sparse_range) * SB_INITIAL_NUM_RANGES);
	    sb->sb_size = SB_INITIAL_NUM_RANGES;
	} else {
		old = sb->sb_ranges;	
		if (size > 0)
			new_size = size;
		else 
			new_size = sb->sb_size * 2;
		sb->sb_ranges = (sparse_range*)malloc(sizeof(sparse_range)*new_size);
		memcpy(sb->sb_ranges, old, sb->sb_size*sizeof(sparse_range));
	    sb->sb_size = new_size;
		free(old);
	}
}

sparse_bitset*	
sparse_bitset_new(int num_bits)
{
	sparse_bitset *	sb;

	sb = malloc(sizeof(sparse_bitset));

	sb->sb_num_bits = num_bits;
	sb->sb_num_ranges = 0;
	sb->sb_ranges = (sparse_range *)malloc(sizeof(sparse_range) * SB_INITIAL_NUM_RANGES);
	memset(sb->sb_ranges, 0, sizeof(sparse_range) * SB_INITIAL_NUM_RANGES);
	sb->sb_size = SB_INITIAL_NUM_RANGES;

	return sb;
}

sparse_bitset*	
sparse_bitset_new_no_ranges(int num_bits)
{
	sparse_bitset *	sb;

	sb = malloc(sizeof(sparse_bitset));

	sb->sb_num_bits = num_bits;
	sb->sb_num_ranges = 0;
	sb->sb_ranges = NULL;
	sb->sb_size = 0;

	return sb;
}

void			
sparse_bitset_free(sparse_bitset *sb)
{
	if(sb == NULL)
		return;

	free(sb->sb_ranges);
	free(sb);

	sb = NULL;
}

void		
sparse_bitset_clear(sparse_bitset *sb)
{
	if(sb != NULL)
		sb->sb_num_ranges = 0;
}

void
sparse_bitset_copy(sparse_bitset *sb1, sparse_bitset *sb2)
{
	if(sb1!= NULL && sb1->sb_ranges != NULL && sb2 != NULL && sb2->sb_ranges != NULL)
	{
		if(sb1->sb_size < sb2->sb_num_ranges)
			sparse_bitset_extend_size(sb1, sb2->sb_size);
		memcpy(sb1->sb_ranges, sb2->sb_ranges, sb2->sb_num_ranges*sizeof(sparse_range));
	}
}

sparse_bitset*
sparse_bitset_dup(sparse_bitset* sb)
{
	sparse_bitset *nb = sparse_bitset_new_no_ranges(sb->sb_num_bits);

	nb->sb_num_ranges = sb->sb_num_ranges;
	nb->sb_ranges = (sparse_range*)malloc(sizeof(sparse_range)*sb->sb_num_ranges);
	nb->sb_size = sb->sb_num_ranges;
	sparse_bitset_copy(nb, sb);

	return nb;
}

void			
sparse_bitset_set(sparse_bitset *sb, int n)
{
	int index;
	int pos;
	sparse_range* ranges = NULL;

	if(sb == NULL || sb->sb_ranges == NULL || sb->sb_num_ranges < 0)
		return;

	ranges = sb->sb_ranges;

	for(index = 0; index < sb->sb_num_ranges; index++)
	{
		if( ranges[index].sr_start <= n && n <= ranges[index].sr_end) // the bit "n" has been set
			return;
		else if( n < ranges[index].sr_start)
			break;
	}

	pos = index;

	if(pos > 0 && ranges[pos-1].sr_end == n-1) 
	{
		ranges[pos-1].sr_end = n;
		if(pos < sb->sb_num_ranges && ranges[pos-1].sr_end+1 == ranges[pos].sr_start)
		{
			ranges[pos-1].sr_end = ranges[pos].sr_end;

			for(index = pos; index < sb->sb_num_ranges-1; ++index)
			{
				ranges[index].sr_start = ranges[index+1].sr_start;
				ranges[index].sr_end = ranges[index+1].sr_end;
			}
			--sb->sb_num_ranges;
		}
	}
	else if(pos < sb->sb_num_ranges-1 && ranges[pos].sr_start == n+1)
	{
		ranges[pos].sr_start = n;
	} 
	else 
	{
		if( sb->sb_size < (sb->sb_num_ranges + 1))
		{
			sparse_bitset_extend_size(sb, -1);
			ranges = sb->sb_ranges;
		}

		for(index = sb->sb_num_ranges; index > pos; index--)
		{
			ranges[index].sr_start = ranges[index-1].sr_start;
			ranges[index].sr_end = ranges[index-1].sr_end;
		}

		ranges[pos].sr_start = n;	
		ranges[pos].sr_end = n;	
		++sb->sb_num_ranges;
	}
}

void			
sparse_bitset_set_range(sparse_bitset *sb, int start, int end)
{
	if(sb == NULL || sb->sb_num_ranges < 0 || start < 0 || end < start)
		return;

	int num_ranges = sb->sb_num_ranges; 
	sparse_range* ranges = sb->sb_ranges;

	int index, start_pos = num_ranges, end_pos = num_ranges; 
	for(index = 0; index < num_ranges; index++)
	{
		if( start <= ranges[index].sr_start || start <= ranges[index].sr_end) 
			start_pos = index;

		if( end <= ranges[index].sr_start || end <= ranges[index].sr_end) 
			end_pos = index;
	}

	int real_start = start, real_end = end, num_shrink;
	if(start_pos == end_pos)
	{
		if((start_pos == num_ranges) || (end+1 < ranges[end_pos].sr_start))//add a new range at 'start_pos'
		{
			if(num_ranges+1 > sb->sb_size)
				sparse_bitset_extend_size(sb, -1);

			for(index = sb->sb_num_ranges; index > start_pos; index--)
			{
				ranges[index].sr_start = ranges[index-1].sr_start;
				ranges[index].sr_end = ranges[index-1].sr_end;
			}

			sb->sb_ranges[start_pos].sr_start = start;
			sb->sb_ranges[start_pos].sr_end = end;

			sb->sb_num_ranges++;
		}
		else if(start < ranges[end_pos].sr_start) // merge it with the 'end_pos'
				ranges[start_pos].sr_start = start;
	} else {
		num_shrink = (end_pos - start_pos) - 1;

		if(start_pos != num_ranges && real_start > ranges[start_pos].sr_start)
			real_start = ranges[start_pos].sr_start; 

		if(end_pos != num_ranges && ranges[end_pos].sr_start-1 <= real_end && real_end < ranges[end_pos].sr_end)
		{
			real_end = ranges[end_pos].sr_end;
			num_shrink++;
		}

		ranges[start_pos].sr_start = real_start;
		ranges[start_pos].sr_end = real_end;
		
		if(num_shrink > 0)
		{
			for(index = start_pos+num_shrink+1; index < num_ranges; index++)
			{
				ranges[index-num_shrink].sr_start = ranges[index].sr_start;
				ranges[index-num_shrink].sr_end = ranges[index].sr_end;
			}
		}	
		
		sb->sb_num_ranges -= num_shrink;
	}
}

sparse_bitset*	
sparse_bitset_or(sparse_bitset *sb1, sparse_bitset *sb2)
{
	sparse_bitset* sb = sparse_bitset_dup(sb1);
	sparse_bitset_oreq(sb, sb2);

	return sb;
}

void			
sparse_bitset_oreq(sparse_bitset *sb1, sparse_bitset *sb2)
{
	int res_num_bits = (sb1->sb_num_bits > sb2->sb_num_bits)? sb1->sb_num_bits : sb2->sb_num_bits;

	int start, end;
	int index1, index2, index, res_index;
	int num_ranges1 = sb1->sb_num_ranges; 
	int num_ranges2 = sb2->sb_num_ranges; 
	sparse_range*	ranges1= sb1->sb_ranges;
	sparse_range*	ranges2= sb2->sb_ranges;
	int num_ranges = num_ranges1 + num_ranges2;

	int res_size = sb1->sb_size + sb2->sb_size;
	sparse_range* res_ranges = (sparse_range*)malloc(sizeof(sparse_range)*res_size);
	/*TODO: optimizting memory usage. Re-use sb1->sb_ranges*/

	index1 = 0;
	index2 = 0;
	res_index = -1;
	for(index = 0; index < num_ranges; index++)
	{
		if(index1 >= num_ranges1 || ( index2 < num_ranges2 && (ranges1[index1].sr_start > ranges2[index2].sr_start) ) )
		{
			start = ranges2[index2].sr_start;
			end = ranges2[index2].sr_end;
			index2++;
		} 
		else
		{
			start = ranges1[index1].sr_start;
			end = ranges1[index1].sr_end;
			index1++;
		}

		if(res_index < 0 || res_ranges[res_index].sr_end+1 < start) 
		{
			res_index++;
			if(res_index >= res_size)
			{
				printf("Impossible Error! num_ranges[%d], res_size[%d], res_index[%d]\n", num_ranges, res_size, res_index);
			}
			res_ranges[res_index].sr_start = start;
			res_ranges[res_index].sr_end = end;
		} 
		else if ((res_ranges[res_index].sr_end + 1) >= start) 
		{
			if(res_ranges[res_index].sr_end < end)
				res_ranges[res_index].sr_end = end; 
		} 
	}

	sb1->sb_num_bits = res_num_bits;
	free(sb1->sb_ranges);
	sb1->sb_num_ranges = res_index + 1;
	sb1->sb_ranges = res_ranges;
	sb1->sb_size = res_size;
}

int				
sparse_bitset_convert(sparse_bitset *sb)
{
	if(sb == NULL || sb->sb_num_ranges < 64)
		return -1;

	int count = 0, index;
	for(index = 0; index < sb->sb_num_ranges; index++)
		count += count_char(sb->sb_ranges + index);

	count += sb->sb_num_ranges - 1; // the number of ','

	if(count > (sb->sb_num_bits >> 2))
		return 1;
	else
		return 0;
}

int				
sparse_bitset_size(sparse_bitset *sb)
{
	if(sb == NULL)
		return 0;

	return sb->sb_num_ranges;
}

char*		 
sparse_bitset_to_set(sparse_bitset *b)
{
	return NULL;
}

sparse_bitset*	
set_to_sparse_bitset(char *set)
{
	return NULL;
}

char*		 	
sparse_bitset_serialize(sparse_bitset *sb)
{
	int index;
	int sep = 0;
	varstr *	v;

	if (sb == NULL || sb->sb_num_ranges <= 0)
		return strdup("{}");

	v = varstr_fromstr("{");

	for(index = 0; index < sb->sb_num_ranges; index++)
	{
		if(sep == 0)
			sep = 1;
		else
			varstr_add(v, ',');

		if (sb->sb_ranges[index].sr_start != sb->sb_ranges[index].sr_end) {
			varstr_sprintf(v, "%x-%x", sb->sb_ranges[index].sr_start, sb->sb_ranges[index].sr_end);
		} else {
			varstr_sprintf(v, "%x", sb->sb_ranges[index].sr_start);
		}
	}

	varstr_add(v, '}');

	char* str = varstr_tostr(v);
	varstr_free(v);

	return str;
}

sparse_bitset*	
sparse_bitset_deserialize(int num_bits, char *set, int len)
{
	sparse_bitset* res = NULL;

	if(num_bits < 1 || set == NULL || set[0] != '{' || set[len-1] != '}' || len <= 2)
		return res;

	res = sparse_bitset_new(num_bits);

	int lower, upper;
	int start, end, index;
	start = 1;
	for(index = 1; index < len; index++)
	{
		if(set[index] == ',' || set[index] == '}')
		{
			end = index - 1;

			if(end >= start)
			{
				parse_range(&lower, &upper, set, start, end);
				if(lower < upper)
					sparse_bitset_set_range(res, lower, upper);
				else if (lower == upper)
					sparse_bitset_set(res, lower);
			}
			start = index + 1;
		}
	}

	return res;
}
