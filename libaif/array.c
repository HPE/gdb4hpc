/*
** AIF routines that operate specifically on arrays.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<memory.h>
#include	<string.h>

#include	"aif.h"
#include	"aiferr.h"
#include	"aifint.h"

int
AIFArrayRank(AIF *a)
{
	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	if ( AIFType(a) != AIF_ARRAY  && AIFType(a) != AIF_FTN_ARRAY )
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	return FDSArrayRank(AIF_FORMAT(a));
}

int
AIFArraySize(AIF *a)
{
	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

        switch ( AIFType(a) )
        {
        case AIF_ARRAY:
        case AIF_FTN_ARRAY:
                break;
        default:
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	return FDSArraySize(AIF_FORMAT(a));
}

/*
** Parse array type descriptor and extract the minimum and maximum
** array index values for each dimension. Also, if size is not NULL,
** will return the size of dimension.
*/
int
AIFArrayBounds(AIF *a, int rank, int **min, int **max, int **size)
{
	if ( a == (AIF *)NULL || min == (int **)NULL || max == (int **)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	if ( AIFType(a) != AIF_ARRAY )
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	FDSArrayBounds(AIF_FORMAT(a), rank, min, max, size);

	return 0;
}

/*
** Extract info about an array. Returns the index type of the
** first dimension (assumes all dimensions are the same type),
** the element type of the array, and the number of dimensions
** of the array.
*/
int
AIFArrayInfo(AIF *a, int *rank, char **type, int *idx)
{
	char *	itype;

	if ( a == (AIF *)NULL || rank == (int *)NULL || type == (char **)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	if ( AIFType(a) != AIF_ARRAY )
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	FDSArrayInfo(AIF_FORMAT(a), rank, (char **)type, &itype);

	if ( idx != (int *)NULL )
		*idx = FDSType(itype);

	return 0;
}

void
_build_array_slice(int cur_rank, AIFIndex *orig_ix, char **orig_data, int *slice_low, int *slice_hi, char **new_data)
{
	int i;
	int la_rank;
	int	skip_elms;
	int nel;
	
	if ( cur_rank == orig_ix->i_rank )
		return;
		
	// determine if there are any elements we need to skip over. This "looks ahead"
	// at all the ranks to figure out how many elements we need to skip to deal
	// with multi-dimensional arrays.
	// Start by checking if we need to skip over the low part of the data
	if (slice_low[cur_rank] > orig_ix->i_min[cur_rank])
	{
		skip_elms = slice_low[cur_rank] - orig_ix->i_min[cur_rank];
		for (la_rank=cur_rank+1; la_rank<orig_ix->i_rank; ++la_rank)
		{
			// we want to "skip over" the entire inner dimension
			skip_elms *= orig_ix->i_max[la_rank] - orig_ix->i_min[la_rank] + 1;
		}
		// now skip over the total number of elements times its base size
		*orig_data += skip_elms * orig_ix->i_bsize;
	}
		
	nel = slice_hi[cur_rank] - slice_low[cur_rank] + 1;
	
	// If we are the innermost rank, we can copy all the data at once.
	if ( cur_rank == orig_ix->i_rank - 1)
	{
		// first copy the data
		memcpy(*new_data, *orig_data, nel*orig_ix->i_bsize);
		// now advance the new_data pointer past the data we just copied
		*new_data += nel*orig_ix->i_bsize;
		// also advance the orig_data pointer past the data we just copied
		*orig_data += nel*orig_ix->i_bsize;
		
		// we still need to possibly skip over some of the high data
		goto skip_hi;
	}
	
	// If we are not the innermost rank, we need to recursively call this
	// function to deal with the possibility that inner dimensions may also be
	// slicing their data.
	for (i=0; i < nel; ++i)
	{
		_build_array_slice(cur_rank+1, orig_ix, orig_data, slice_low, slice_hi, new_data);
	}
	
skip_hi:
	// now we need to account for the possibility of skipping over the high part
	// of the data. This is to cope with the recursive nature of this function
	if (orig_ix->i_max[cur_rank] > slice_hi[cur_rank])
	{
		skip_elms = orig_ix->i_max[cur_rank] - slice_hi[cur_rank];
		for (la_rank=cur_rank+1; la_rank<orig_ix->i_rank; ++la_rank)
		{
			// Again, we want to "skip over" the entire inner dimension
			skip_elms *= orig_ix->i_max[la_rank] - orig_ix->i_min[la_rank] + 1;
		}
		// now skip over the total number of elements times its base size
		*orig_data += skip_elms * orig_ix->i_bsize;
	}
	// Done. new_data now contains the sliced array.
}

/*
** Evaluate array slice.
*/
AIF *
AIFArraySlice(AIF *a, int rank, int *mn, int *mx)
{
	int			i;
	// full slice bit, used to see if we are trying to create a slice of the
	// entire data. We don't want to do so as its expensive.
	int			full_slice = 1;
	char		fds_array_start_chr = 0;
	char		fds_array_end_chr = 0;
	// this is a temporary string when creating the new fds and its len
	char *		tmpbuf;
	int			tmpbuf_len = 128;
	// this is used as the remainder returned by snprintf
	int			write_len;
	// this is the new fds string and its len
	char *		fds = NULL;
	int			fds_len = 0;
	AIF *		ra;
	AIFIndex *	ix1;
	// the following is used as temporary pointers to the data for the 
	// recursive _build_array_slice routine.
	char *		old_data;
	char *		new_data;

	if ( a == (AIF *)NULL || mn == (int *)NULL || mx == (int *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return (AIF *)NULL;
	}

	if ( AIFType(a) == AIF_ARRAY )
	{
		ix1 = FDSArrayIndexInit(AIF_FORMAT(a));
		// this is a hack to deal with fortran/c arrays. When aif is fixed you
		// can safely remove the fds_array_start/end_chr reference and rely on
		// the normal FDS_ARRAY_START character
		fds_array_start_chr = FDS_ARRAY_START;
		fds_array_end_chr = FDS_ARRAY_END;
	} else if ( AIFType(a) == AIF_FTN_ARRAY )
	{
		ix1 = FDSFtnArrayIndexInit(AIF_FORMAT(a));
		fds_array_start_chr = FDS_FTN_ARRAY_START;
		fds_array_end_chr = FDS_FTN_ARRAY_END;
	} else
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return (AIF *)NULL;
	}

	if ( rank != ix1->i_rank )
	{
		SetAIFError(AIFERR_INDEX, NULL);
		AIFArrayIndexFree(ix1);
		return (AIF *)NULL;
	}

	/*
	** Create a tmpbuf with the default length.
	*/
	tmpbuf = (char *)_aif_alloc(tmpbuf_len);

	/*
	** Caluculate the number of dimensions of the (new) array.
	** This corresponds to the number of elements in each dimension.
	** If mn == mx then we want to reference a single element in that dimension,
	** the final slice will be lacking that dimension.
	*/
	for ( i = 0 ; i < rank ; ++i )
	{
		// first ensure that the mn and mx are in range.
		if
		(
			mn[i] < ix1->i_min[i] 
			||
			mn[i] > ix1->i_max[i]
			||
			mx[i] < ix1->i_min[i] 
			||
			mx[i] > ix1->i_max[i]
		)
		{
			SetAIFError(AIFERR_INDEX, NULL);
			AIFArrayIndexFree(ix1);
			if (fds != NULL)
				_aif_free(fds);
			_aif_free(tmpbuf);
			return (AIF *)NULL;
		} else if ( mn[i] == mx[i] )
		{
			// Here we reference a single element, not a range. Thus the new
			// rank will be missing this dimension.
			
			// we still need to check if the one dimensional object cooresponds
			// to a one dimensional array with a single element for the full_slice
			// logic
			if ( full_slice)
			{
				if ( !((mx[i] == ix1->i_max[i]) && (mn[i] == ix1->i_min[i])) )
					full_slice = 0;
			}
			
			continue;
		} else if ( full_slice )
		{
			// we only want to check this if full_slice is still true.
			if ( !((mx[i] == ix1->i_max[i]) && (mn[i] == ix1->i_min[i])) )
				full_slice = 0;
		}
		
		// write the new string
		write_len = snprintf(tmpbuf, tmpbuf_len, "%cr%d..%dis4%c", fds_array_start_chr, mn[i], mx[i], fds_array_end_chr);
		
		// Handle the case were the string we are trying to write is longer than
		// the tmpbuf.
		// This assumes glibc > 2.1
		while (write_len >= tmpbuf_len)
		{
			// resize tmpbuf
			tmpbuf_len = write_len + 1;
			tmpbuf = (char *)_aif_resize(tmpbuf, tmpbuf_len);
			// try to write the string again
			write_len = snprintf(tmpbuf, tmpbuf_len, "%cr%d..%dis4%c", fds_array_start_chr, mn[i], mx[i], fds_array_end_chr);
		}
		
		// resize the fds buf, don't forget the null term
		fds = (char *)_aif_resize(fds, fds_len+strlen(tmpbuf)+1);
		// write the tmpbuf into the fds string
		strcpy(fds+fds_len, tmpbuf);
		// update the fds_len
		fds_len += strlen(tmpbuf);
		// force the final null term
		fds[fds_len] = '\0';
	}
	
	// free the tmpbuf, we are done with it
	_aif_free(tmpbuf);
	
	// if full_slice is still set, just copy and return.
	if (full_slice)
	{
		ra = CopyAIF(a);
		AIFArrayIndexFree(ix1);
		_aif_free(fds);
		return ra;
	}
	
	// copy the base type to the fds string
	fds = (char *)_aif_resize(fds, fds_len+strlen(ix1->i_btype)+1);
	strcpy(fds+fds_len, ix1->i_btype);
	// update the fds_len
	fds_len += strlen(ix1->i_btype);
	// force the final null term
	fds[fds_len] = '\0';

	/*
	** Create a new structure to hold the sliced array.
	*/
	ra = MakeAIF(fds, NULL);

	/*
	** Now build the data recursively (unflattened).
	*/
	old_data = AIF_DATA(a);
	new_data = AIF_DATA(ra);
	
/*
	fprintf(stderr, "Total elements: %d\n", ix1->i_nel);
	fprintf(stderr, "Base size: %d\n", ix1->i_bsize);
	for (i=0; i<ix1->i_rank; ++i)
	{
		fprintf(stderr, "For rank %d max: %d min: %d\n", i, ix1->i_max[i], ix1->i_min[i]);
	}
*/
	_build_array_slice(0, ix1, &old_data, mn, mx, &new_data);

	AIFArrayIndexFree(ix1);

	return ra;
}

/*
 * The old AIFArrayPerm() relied on the assumption that all elements of the
 * AIF array have the same size (i.e. it wrote the data for the 
 * resultant array in a "random access" way).
 * To solve this problem, we create dynamic arrays to hold the data and the
 * length, after that we iterate the arrays to copy the data to the resultant
 * AIF array.
 */
AIF *
AIFArrayPerm(AIF *a, int *index)
{
	int			i;
	int			rl;
	char *		rf;
	AIF *		ra;
	AIFIndex *	ix;
	char **		data_array;
	int *		len_array;
	char *		d;
	char *		rd;
	int 		len, counter, limit;

	if ( a == (AIF *)NULL || index == (int *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return (AIF *)NULL;
	}

	if ( AIFType(a) != AIF_ARRAY && AIFType(a) != AIF_FTN_ARRAY )
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return (AIF *)NULL;
	}

	/*
	** Initialise the index arrays.
	*/
	if ( AIFType(a) == AIF_ARRAY )
		ix = FDSArrayIndexInit(AIF_FORMAT(a));
	else
		ix = FDSFtnArrayIndexInit(AIF_FORMAT(a));

	/*
	** Create a new structure to hold the permuted array.
	*/
	ra = NewAIF(strlen(AIF_FORMAT(a))+1, BUFSIZ);

	/*
	** Construct a new fds.
	*/
	memcpy(AIF_FORMAT(ra), AIF_FORMAT(a), strlen(AIF_FORMAT(a))+1);
	rf = AIF_FORMAT(ra);
	rl = strlen(rf) + 1;

	for ( i = 0 ; i < ix->i_rank ; i++ )
	{
		snprintf(rf, rl, "[r%d..%dis4]", ix->i_min[index[i]], ix->i_max[index[i]]);
		rf += strlen(rf);
		rl -= strlen(rf);
	}

	strcat(AIF_FORMAT(ra), ix->i_btype);

	len_array = _aif_alloc( ix->i_nel * sizeof(int) );
	data_array = _aif_alloc( ix->i_nel * sizeof(char *) );

	/*
	** Now build the data.
	*/
	for ( i = 0 ; i < ix->i_nel ; i++ )
	{
		len = 0; 

		limit = AIFIndexOffset(ix->i_rank, ix->i_index, ix->i_min, ix->i_max, NULL);

		for ( counter = 0; counter < limit; counter++ )
		{
			len += FDSTypeSize(ix->i_btype);
		}

		d = AIF_DATA(a) + len;
		len = FDSTypeSize(ix->i_btype);

		limit = AIFIndexOffset(ix->i_rank, ix->i_index, ix->i_min, ix->i_max, index);

		data_array[limit] = d;
		len_array[limit] = len;

		AIFArrayIndexInc(ix);
	}

	len = 0;

	/* Copy the data to the resultant AIF array */
	for ( i = 0 ; i < ix->i_nel ; i++ )
	{
		rd = AIF_DATA(ra) + len;

		memcpy(rd, data_array[i], len_array[i]);

		len += len_array[i];
	}

	AIFArrayIndexFree(ix);

	_aif_free(data_array);
	_aif_free(len_array);

	return ra;
}

char *
AIFArrayIndexType(AIF *a)
{
	char *result;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return NULL;
	}

	if ( AIFType(a) == AIF_ARRAY )
		result = FDSArrayIndexType(AIF_FORMAT(a));
	else if ( AIFType(a) == AIF_FTN_ARRAY )
		result = FDSFtnArrayIndexType(AIF_FORMAT(a));
	else
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return NULL;
	}
	
	return result;
}

int
AIFArrayMinIndex(AIF *a, int n)
{
	int result;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	if ( AIFType(a) == AIF_ARRAY )
		result = FDSArrayMinIndex(AIF_FORMAT(a), n);
	else if ( AIFType(a) == AIF_FTN_ARRAY )
		result = FDSFtnArrayMinIndex(AIF_FORMAT(a), n);
	else
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	return result;
}

int
AIFArrayMaxIndex(AIF *a, int n)
{
	int result;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	if ( AIFType(a) == AIF_ARRAY )
		result = FDSArrayMaxIndex(AIF_FORMAT(a), n);
	else if ( AIFType(a) == AIF_FTN_ARRAY )
		result = FDSFtnArrayMaxIndex(AIF_FORMAT(a), n);
	else
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	return result;
}

/*
** The index array is used to compute the location of data in
** a multi-dimensional array, and basically corresponds to a loop
** counter in normal circumstances.
*/

#define PERMUTE(p, d) ((p) != NULL ? (p)[d] : d)

/*
** Initialise an index array to the minimum index value for each 
** dimension, given the number of dimensions (rank), and the minimum 
** and maximum values of each dimension (min, max). Returns the number 
** of elements in the array.
*/
AIFIndex *
AIFArrayIndexInit(AIF *a)
{
	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return (AIFIndex *)NULL;
	}
	
	switch ( AIFType(a) )
	{
	case AIF_ARRAY:
	        return FDSArrayIndexInit(AIF_FORMAT(a));
	        
	case AIF_FTN_ARRAY:
	        return FDSFtnArrayIndexInit(AIF_FORMAT(a));
	        
	default:
		break;
	}

        // error case
        SetAIFError(AIFERR_TYPE, NULL);
	return (AIFIndex *)NULL;
}

/*
** Check that the index value of each index in rankp is within the range
** specified by min and max. Returns 1 if they are, otherwise 0.
**
** Note: rankp, index, min and max are guaranteed to have at least
** rank elements.
*/
int
AIFIndexInRange(int rank, int *index, int *min, int *max)
{
	int	d;

	for ( d = rank - 1 ; d >= 0 ; d-- )
	{
		if ( min[d] < 0 )
			continue;

		if ( index[d] < min[d] || index[d] > max[d] )
			return 0;
	}

	return 1;
}

/*
** Step through indexes in the order specified by the perm array.
** Indexes cycle from min to max. Returns 1 on carry out.
*/
int
AIFArrayIndexInc(AIFIndex *ix)
{
	int	d;

	for ( d = ix->i_rank - 1 ; d >= 0 ; d-- )
	{
		if ( ++(ix->i_index[d]) > ix->i_max[d] )
		{
			ix->i_index[d] = ix->i_min[d];
			continue;
		}

		return 0;
	}

	ix->i_finished = 1;

	return 1;
}

void
AIFArrayIndexFree(AIFIndex *ix)
{
	_aif_free(ix->i_btype);
	_aif_free(ix->i_index);
	_aif_free(ix->i_min);
	_aif_free(ix->i_max);
	_aif_free(ix);
}

/*
** Index array element and return as doublest
** XXX: TOFIX
*/
int
AIFArrayElementToDoublest(AIF *a, AIFIndex *ix, AIFDOUBLEST *val)
{
	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	if ( AIFType(a) != AIF_ARRAY && AIFType(a) != AIF_FTN_ARRAY )
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	if ( AIFBaseType(a) != AIF_FLOATING && AIFType(a) != AIF_FTN_REAL )
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	return _aif_to_doublest(AIF_DATA(a) + AIFIndexOffset(ix->i_rank, ix->i_index, ix->i_min, ix->i_max, NULL) * ix->i_bsize, ix->i_bsize, val);
}

int
AIFArrayElementToDouble(AIF *a, AIFIndex *ix, double *val)
{
	int		res;
	AIFDOUBLEST	dd;

	res = AIFArrayElementToDoublest(a, ix, &dd);
	
	if ( res >= 0 )
		*val = (double)dd; /* maybe lose some precision */

	return res;
}

/*
** Index array element and return as longest
** XXX: TOFIX
*/
int
AIFArrayElementToLongest(AIF *a, AIFIndex *ix, AIFLONGEST *val)
{
	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	if ( AIFType(a) != AIF_ARRAY  && AIFType(a) != AIF_FTN_ARRAY )
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	if ( AIFBaseType(a) != AIF_INTEGER && AIFBaseType(a) != AIF_FTN_INTEGER )
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	return _aif_to_longest(AIF_DATA(a) + AIFIndexOffset(ix->i_rank, ix->i_index, ix->i_min, ix->i_max, NULL) * ix->i_bsize, ix->i_bsize, val);
}

int
AIFArrayElementToInt(AIF *a, AIFIndex *ix, int *val)
{
	int		res;
	AIFLONGEST	l;

	res = AIFArrayElementToLongest(a, ix, &l);

	if ( res >= 0 )
		*val = (int)l;
	
	return res;
}

AIF *
_aif_array_ref(AIF *a, int rank, int *index, int *min, int *max, char *btype)
{
	AIF *	ae;
	int	counter;
	int	offset;
	char *	theFormat;
	char *	dataStart = NULL;
	char *	dataEnd;

	ae = MakeAIF(strdup(btype), NULL);

	offset = AIFIndexOffset(rank, index, min, max, (int *)NULL);
	dataEnd = AIF_DATA(a);

	for ( counter = 0 ; counter <= offset ; counter++ )
	{
		theFormat = btype;
		dataStart = dataEnd;
		_fds_skip_data(&theFormat, &dataEnd);
	}

	AIF_DATA(ae) = _aif_alloc(dataEnd - dataStart + 1);
	memcpy(AIF_DATA(ae), dataStart, dataEnd - dataStart);

	return ae;
}

/*
** Compute size of array.
*/
int
_aif_array_size(char *fds, char *data)
{
	int		i;
	int		n = 0;
	char *		dstart;
	char *		fmt;
	char *		btype;
	AIFIndex *	ix;

	ix = FDSArrayIndexInit(fds);

	btype = FDSBaseType(fds);

	for ( i = 0 ; i <= ix->i_nel ; i++ )
	{
		fmt = btype;
		dstart = data;
		_fds_skip_data(&fmt, &data);
		n += data - dstart;
	}

	AIFArrayIndexFree(ix);

	return n;
}

/*
** Return array element at index.
*/
AIF *
AIFArrayElement(AIF *a, AIFIndex *ix)
{
	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return (AIF *)NULL;
	}

	if ( AIFType(a) != AIF_ARRAY && AIFType(a) != AIF_FTN_ARRAY )
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return (AIF *)NULL;
	}

	if ( ix->i_finished )
		return (AIF *)NULL;

	return _aif_array_ref(a, ix->i_rank, ix->i_index, ix->i_min, ix->i_max, ix->i_btype);
}

/*
** Return array element at location.
*/
AIF *
AIFArrayRef(AIF *a, int rank, int *loc)
{
	int		i;
	AIF *		ae;
	AIFIndex *	ix;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return (AIF *)NULL;
	}

	if ( AIFType(a) != AIF_ARRAY && AIFType(a) != AIF_FTN_ARRAY )
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return (AIF *)NULL;
	}

	ix = AIFArrayIndexInit(a);

	if ( rank != ix->i_rank )
	{
		AIFArrayIndexFree(ix);
		SetAIFError(AIFERR_INDEX, NULL);
		return (AIF *)NULL;
	}

	for ( i = 0 ; i < rank ; i++ )
	{
		if ( loc[i] < ix->i_min[i] || loc[i] > ix->i_max[i] )
		{
			AIFArrayIndexFree(ix);
			SetAIFError(AIFERR_INDEX, NULL);
			return (AIF *)NULL;
		}
	}

	ae = _aif_array_ref(a, ix->i_rank, loc, ix->i_min, ix->i_max, ix->i_btype);

	AIFArrayIndexFree(ix);

	return ae;
}

/*
** Copy data into an array.
** XXX: TOFIX  - DEAL WITH FORTRAN ARRAY TOO
*/
int
AIFSetArrayData(AIF *dst, AIFIndex *ix, AIF *src)
{
	char *	dd;

	if
	(
		dst == (AIF *)NULL
		||
		src == (AIF *)NULL
		||
		ix == (AIFIndex *)NULL
	)
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}               

	if
	( 
		AIFType(dst) != AIF_ARRAY
		||
		AIFBaseType(dst) != AIFType(src)
	)
	{       
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	dd = AIF_DATA(dst) + AIFIndexOffset(ix->i_rank, ix->i_index, ix->i_min, ix->i_max, NULL) * ix->i_bsize;

	memcpy(dd, AIF_DATA(src), ix->i_bsize);

	return 0;
}

/*
** Calculate the data offset in the array given the current value
** of the indexes, and the minimum and maximum indices of each dimension.
*/
int
AIFIndexOffset(int rank, int *index, int *min, int *max, int *perm)
{
	int	d;
	int	p;
	int	off = 0;
	int	sz = 1;

	for ( d = rank - 1 ; d >= 0 ; d-- )
	{
		p = PERMUTE(perm, d);
		off += (index[p] - min[p]) * sz;
		sz *= max[p] - min[p] + 1;
	}

	return off;
}
