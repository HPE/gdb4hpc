/*
** Routines for comparing AIF objects.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<string.h>
#include	<math.h>

#include	"aif.h"
#include	"aiferr.h"
#include	"aifint.h"

int
AIFIsZero(AIF *a, int *val)
{
	char *	f;
	char *	d;

	if ( a == (AIF *)NULL || val == (int *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	f = AIF_FORMAT(a);
	d = AIF_DATA(a);

	return _aif_is_zero(&f, &d, AIF_LEN(a), val);
}

int
_aif_is_zero(char **fds, char **data, int len, int *val)
{
	int		i;
	int		nv;
	int		bytes;
	int		reg;
	int		success;
	char		c;
	char *		target;
	AIFIndex *	ix;
	char *		fmt;
	AIFDOUBLEST	v;

	if ( **fds == FDS_NAME )
	{
		*fds = _fds_skipto(*fds, "/");
		(*fds)++;
	}

	switch ( FDSType(*fds) )
	{
	case AIF_UNION:
		bytes = FDSTypeSize(*fds);
		*val = 1; /* zero unless shown otherwise */

		(*fds)++; /* past open brace */

		_fds_skipid(fds);

		while ( **fds != FDS_UNION_END )
		{
			*fds = strchr(*fds, FDS_UNION_FIELD_NAME_END) + 1; /* to start of field */

			if ( (i = FDSTypeSize(*fds)) < 0 )
				return -1;
			fmt = _aif_alloc( sizeof(char) * i );
			target = fmt;
			memcpy(target, *data, i);

			if ( _aif_is_zero(fds, &target, len, val) < 0 )
			{
				_aif_free(fmt);
				return -1;
			}

			if ( *val == 0 )
				return 0; /* zero res means no need to look further */
			if ( **fds == FDS_UNION_FIELD_SEP )
				(*fds)++;
		}

		*fds = strchr(*fds, FDS_STRUCT_END) + 1; /*past close brace*/
		*data += bytes;

		return 0;

	case AIF_ENUM:
		*fds = _fds_base_type(*fds);
		bytes = _fds_count_bytes(fds);
		*val = _aif_int_is_zero(*data, bytes);
		(*data) += bytes;

		return 0;

	case AIF_POINTER:
		reg = (**data < (char) AIF_PTR_REFERENCE); /* data not a reference */

		(*fds)++; /* past ^ */

		target = _find_target(data, 0);

		if ( target == 0 )
		{
			/*
			** null pointer.  We call it equivalent to 0
			*/
			*val = 1;
			_fds_advance(fds); /* skip over the target fds */
			return 0; /* but we fail to find it. */
		}

		success = _aif_is_zero(fds, &target, len, val);

		if ( reg )
			*data = target; /* otherwise _find_target advanced for us */
		return success;

	case AIF_CHARACTER:
		bytes = _fds_count_bytes(fds);

		if ( _aif_to_char(*data, &c) < 0 )
			return -1;

		*val = (c == '\0');
		(*data) += bytes;

		return 0;

	case AIF_INTEGER:
	case AIF_HASHED_SIGNATURE: /* Minh add here 22/04/2009 */
	case AIF_BOOLEAN: /* if it is FALSE, val = 1, if it is TRUE, val = 0 */
		bytes = _fds_count_bytes(fds);
		*val = _aif_int_is_zero(*data, bytes);
		(*data) += bytes;

		return 0;

	case AIF_FLOATING:
		bytes = _fds_count_bytes(fds);

		if ( _aif_to_doublest(*data, bytes, &v) < 0 )
			return -1;

		*val = (v == (AIFDOUBLEST)0.0L);
		(*data) += bytes;

		return 0;

	case AIF_ARRAY:
		ix = FDSArrayIndexInit(*fds);

		_fds_advance(fds); /* skip over the entire array fds */

		for ( i = 0 ; i < ix->i_nel ; i++ )
		{
			fmt = ix->i_btype;

			if ( _aif_is_zero(&fmt, data, ix->i_bsize, &nv) < 0 )
				return -1;

			if ( nv == 0 ) /* no need to look further */
			{
				*val = 0;
				return 0;
			}
		}

		AIFArrayIndexFree(ix);

		*val = 1;

		return 0;

	/*
	** only uses the public section in AIF_STRUCT 
	*/
	case AIF_STRUCT:
		*val = 1; /* zero unless shown otherwise */

		(*fds)++; /* past open brace */
		_fds_skipid(fds);

		while ( **fds != FDS_STRUCT_ACCESS_SEP )
		{
			*fds = strchr(*fds, FDS_STRUCT_FIELD_NAME_END) + 1; /* to start of field */

			if ( _aif_is_zero(fds, data, len, val) < 0 )
				return -1;

			if ( *val == 0 )
				return 0; /* zero res means no need to look further */
			if ( **fds == FDS_STRUCT_FIELD_SEP )
				(*fds)++;
		}

		*fds = strchr(*fds, FDS_STRUCT_END) + 1; /*past close brace*/

		return 0;

	case AIF_STRING:
		bytes = _fds_count_bytes(fds);
		(*data) += bytes;
		*val = (bytes == 0);
		return 0;

	default:
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}
}

int
_aif_int_is_zero(char *data, int len)
{
	int	i;

	for ( i = 0 ; i < len ; i++ )
		if ( (int)*data++ != 0 )
			return 0;

	return 1;
}

/*
** Compare types of two AIF values. Return true if they are equivalent,
** false if not.
*/
int
AIFTypeCompare(AIF *a1, AIF *a2)
{
	return FDSTypeCompare(AIF_FORMAT(a1), AIF_FORMAT(a2));
}

int
AIFCompare(AIF *a1, AIF *a2, int *res, AIF **d_res, AIF *lo, AIF *hi)
{
	char *	f1;
	char *	f2;
	char *	d1;
	char *	d2;
	int		ctl_d = 0;
	char *	df = NULL;
	char *	dd = NULL;
	int		ctl_e = 0;
	char *	lo_f = NULL;
	char *	lo_d = NULL;
	int		lo_bytes = 0;
	char *	hi_f = NULL;
	char *	hi_d = NULL;
	int		hi_bytes = 0;
	AIF *	diff = NULL;
	int		rtn;

	if ( a1 == (AIF *)NULL || a2 == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	f1 = AIF_FORMAT(a1);
	f2 = AIF_FORMAT(a2);
	d1 = AIF_DATA(a1);
	d2 = AIF_DATA(a2);
	
	// Set the diff control variable
	if (d_res != NULL)
	{
		++ctl_d;
	}
	
	// Set the error tolerance control variable
	if (lo != NULL && hi != NULL)
	{
		++ctl_e;
		lo_f = AIF_FORMAT(lo);
		hi_f = AIF_FORMAT(hi);
		lo_d = AIF_DATA(lo);
		hi_d = AIF_DATA(hi);
		
		// Resolve any references in the type
		if (FDSType(lo_f) == AIF_REFERENCE)
		{
			lo_f = _fds_lookup(&lo_f);
		}
		if (FDSType(hi_f) == AIF_REFERENCE)
		{
			hi_f = _fds_lookup(&hi_f);
		}
	
		// Ensure that the lo and hi are floating point values, otherwise 
		// return with error
		switch (FDSType(lo_f))
		{
			case AIF_FLOATING:
			case AIF_FTN_REAL:
				lo_bytes = _fds_count_bytes_na(&lo_f);
				break;
			default:
				SetAIFError(AIFERR_BADARG, NULL);
	 			return -1;
		}
		switch (FDSType(hi_f))
		{
			case AIF_FLOATING:
			case AIF_FTN_REAL:
				hi_bytes = _fds_count_bytes_na(&hi_f);
				break;
			default:
				SetAIFError(AIFERR_BADARG, NULL);
	 			return -1;
		}
	}

	if ((rtn = _aif_compare(res, &f1, &d1, &f2, &d2, ctl_d, &df, &dd, ctl_e, lo_d, lo_bytes, hi_d, hi_bytes)) != 0)
	{
		if (df != NULL)
			_aif_free(df);
		if (dd != NULL)
			_aif_free(dd);
		*d_res = NULL;
		
		return rtn;
	}

	// If the res is not 0 , then we want to actually create the diff
	// We do not create a diff for warning on pointer compare.
	if (*res != 0 && *res != 3)
	{
		if ((diff = MakeAIF(df, dd)) == NULL)
		{
			_aif_free(df);
			_aif_free(dd);
		} else
		{
			// memory leak - we don't free df since it gets used later, but we want
			// to free rd since its memcpy'ed from. This needs to be rethought IMO
			_aif_free(dd);
		}
	} else
	{
		// free the format and data since it is not used
		_aif_free(df);
		_aif_free(dd);
	}
	
	// Set the diff AIF, this is NULL when a diff wasn't calculated
	if (ctl_d)
	{
		*d_res = diff;
	}
	
	return rtn;
}

int
_aif_cmp_int(	int *res, char *d1, int l1, char *d2, int l2, 
				int diff, char **diff_d )
{
	AIFLONGEST	v1;
	AIFLONGEST	v2;
	AIFLONGEST	v_sub;
	
	if (	
			_aif_to_longest(d1, l1, &v1) < 0 ||
			_aif_to_longest(d2, l2, &v2) < 0
		)
			return -1;
	
	v_sub = v1 - v2;
	
	// compare the sub value
	if (v_sub < 0)
	{
		// negative result => a < b
		*res = -1;
	} else if (v_sub == 0)
	{
		// zero result => a == b
		*res = 0;
	} else
	{
		// positive result => a > b
		*res = 1;
	}
	
	// Handle the case were we are to create the diff value
	if (diff)
	{
		// convert v_sub to aif data
		_longest_to_aif(diff_d, MAX(l1, l2), v_sub);
	}
	
	return 0;
}

int
_aif_cmp_float(	int *res, char *d1, int l1, char *d2, int l2,
				int diff, char **diff_d,
				int err, char *lo_d, int lo_l, char *hi_d, int hi_l )
{
	AIFDOUBLEST	v1;
	AIFDOUBLEST	v2;
	AIFDOUBLEST lo_v;
	AIFDOUBLEST	hi_v;
	AIFDOUBLEST	v_sub = 0.0;
	AIFDOUBLEST v_abs;

	if
	(
		_aif_to_doublest(d1, l1, &v1) < 0
		||
		_aif_to_doublest(d2, l2, &v2) < 0
	)
		return -1;
		
	// If this fails to convert, fail
	if (err)
	{
		if (
				_aif_to_doublest(lo_d, lo_l, &lo_v) < 0 ||
				_aif_to_doublest(hi_d, hi_l, &hi_v) < 0
			)
				return -1;
	}

	// first set the res ignoring the err setting, this gets used later possibly
	if ( v1 < v2 )
	{
		*res = -1;
	} else if ( v1 > v2 )
	{
		*res = 1;
	} else
	{
		*res = 0;
	}
		
	// Now handle the case where error tolerance is being used
	if (err)
	{
		// first calculate the difference of the two values
		v_sub = v1 - v2;
		
		// Now calculate the abs of the difference
#ifdef HAVE_LONG_DOUBLE
		v_abs = fabs(v_sub);
#else
		v_abs = fabsl(v_sub);
#endif

		// check it against the ranges
		if (v_abs < lo_v)
		{
			// no difference
			*res = 0;
		} else if (v_abs <= hi_v)
		{
			// tolerable difference, but warn
			if (*res == -1)
			{
				*res = -2;
			} else
			{
				*res = 2;
			}
		}
		// for differences above hi_v, we don't make any changes to the res value
		// and instead leave it as is
	}
	
	if (diff)
	{
		// We should ensure that v_sub is 0.0 if res is 0
		if (*res == 0)
		{
			v_sub = 0.0;
		} else
		{
			// We only need to compute v_sub if err is false
			if (!err)
			{
				v_sub = v1 - v2;
			}
		}
		
		// convert v_sub to aif data - if this fails return error
		if (_doublest_to_aif(diff_d, MAX(l1, l2), v_sub) < 0)
		{
			return -1;
		}
	}

	return 0;
}

int
_aif_cmp_complex(	int *res, char *d1, int l1, char *d2, int l2,
					int diff, char **diff_d,
					int err, char *lo_d, int lo_l, char *hi_d, int hi_l )
{
	AIFDOUBLEST	r1;
	AIFDOUBLEST	i1;
	AIFDOUBLEST	r2;
	AIFDOUBLEST	i2;
	AIFDOUBLEST lo_v;
	AIFDOUBLEST	hi_v;
	AIFDOUBLEST	r_sub = 0.0;
	AIFDOUBLEST i_sub = 0.0;
	AIFDOUBLEST r_abs = 0.0;
	AIFDOUBLEST i_abs;
	
	if
	(
		_aif_to_doublest(d1, l1, &r1) < 0
		||
		_aif_to_doublest(d1+l1, l1, &i1) < 0
		||
		_aif_to_doublest(d2, l2, &r2) < 0
		||
		_aif_to_doublest(d2+l2, l2, &i2) < 0
	)
		return -1;
		
	// If this fails to convert, fail
	if (err)
	{
		if (
				_aif_to_doublest(lo_d, lo_l, &lo_v) < 0 ||
				_aif_to_doublest(hi_d, hi_l, &hi_v) < 0
			)
				return -1;
	}
	
	// first set the res ignoring the err setting, this gets used later possibly
	if (r1 < r2 || r1 > r2)
	{
		*res = 1;
	} else
	{
		*res = 0;
	}
	
	// Note here that we do not need to reset the final 0 entry. This is
	// to prevent a accidental pass case by overwriting a mismatch between
	// the real components. We only want to check for the case where the img
	// part is different.
	if (i1 < i2 || i1 > i2)
	{
		*res = -1;
	}
		
	// Now handle the case where error tolerance is being used
	if (err)
	{
		// first calculate the difference of the two values
		r_sub = r1 - r2;
		i_sub = i1 - i2;
		
		// Now calculate the abs of the difference
#ifdef HAVE_LONG_DOUBLE
		r_abs = fabs(r_sub);
		i_abs = fabs(i_sub);
#else
		r_abs = fabsl(r_sub);
		i_abs = fabsl(i_sub);
#endif

		// check it against the ranges
		if (r_abs < lo_v && i_abs < lo_v)
		{
			// no difference
			*res = 0;
		} else if (r_abs <= hi_v && i_abs <= hi_v)
		{
			// tolerable difference, but warn
			*res = 2;
		}
		// for differences above hi_v, we don't make any changes to the res value
		// and instead leave it as is
	}
		
	if (diff)
	{
		// We should ensure that v_sub is 0.0 if res is 0
		if (*res == 0)
		{
			r_sub = 0.0;
			i_sub = 0.0;
		} else
		{
			// We only need to compute v_sub if err is false
			if (!err)
			{
				r_sub = r1 - r2;
				i_sub = i1 - i2;
			}
		}
		
		// convert r_sub/i_sub to aif data - if this fails return error
		if (_complex_to_aif(diff_d, MAX(l1, l2), r_sub, i_sub) < 0)
		{
			return -1;
		}
	}
	
	return 0;
}

int
_aif_cmp_char(	int *res, char *d1, char *d2,
				int diff, char **diff_d )
{
	char	v1;
	char	v2;
	char	v_sub;

	if
	(
		_aif_to_char(d1, &v1) < 0 ||
		_aif_to_char(d2, &v2) < 0
	)
		return -1;

	v_sub = v1 - v2;

	// compare the sub value
	if (v_sub < 0)
	{
		// negative result => a < b
		*res = -1;
	} else if (v_sub == 0)
	{
		// zero result => a == b
		*res = 0;
	} else
	{
		// positive result => a > b
		*res = 1;
	}
	
	// Handle the case were we are to create the diff value
	if (diff)
	{
		// convert v_sub to aif data
		_char_to_aif(diff_d, v_sub);
	}

	return 0;
}

int
_aif_cmp_hashed(int *res, char *d1, int l1, char *d2, int l2)
{
	int	i;
	
	/*
	** Check for any different
	*/
	for ( i = 0 ; i < MAX(l1, l2) ; i++ ) 
	{
		if ( d1[i] != d2[i] ) 
		{
			*res = -1;
			return 0;
		}
	}
	*res = 0;
	return 0;
}

/*
** Compare two AIF values. Calculate optional diff of entire datatype.
** Use optional epsilon for floating types.
**
** TODO: Extend this to support the aifcmpop operators. This should be able to
**       handle other operators than simple equality for all of the complex types.
**
** TODO: How to generate diff in light of the above todo?
**
** For integer values, sets res to:
**
**      -1 if d1 < d2
**       0 if d1 == d2
**       1 if d1 > d2
**
** For float values, warn and err epsilon can be provided and sets res to:
**
**      -2 if d1 < d2 and lo_d <= abs(d1-d2) <= hi_d
**      -1 if d1 < d2 and hi_d < abs(d1-d2)
**       0 if d1 == d2 or abs(d1-d2) < lo_d
**       1 if d1 > d2 and hi_d < abs(d1-d2)
**       2 if d1 > d2 and lo_d <= abs(d1-d2) <= hi_d
**
** For complex values, warn and err epsilon can be provided and sets res to:
**
**       0 if real(d1) == real(d2) && img(d1) == img(d2)
**       1 if real(d1) != real(d2) || img(d1) != img(d2) AND
**            hi_d < abs(real(d1)-real(d2)) || hi_d < abs(img(d1)-img(d2))
**       2 if real(d1) != real(d2) || img(d1) != img(d2) AND
**            lo_d <= abs(real(d1)-real(d2)) <= hi_d && 
**            lo_d <= abs(img(d1)-img(d2)) <= hi_d
**
** For structured types, sets res to:
**
**       0 if all fields of d1 and d2 are equal or below the warn epsilon
**       1 if one or more of the fields were different or above the error 
**         epsilon
**       2 if one or more of the fields were above the warning epsilon but below
**         the error epsilon. All other fields matched.
**       3 if any of the fields has an indeterminate comparison result and there
**         are no fields that differ or have diffs above or equal to lo_d.
**         
**       warn and err epsilon can be provided, are only taken into consideration
**       for floating point values.
**
** For array values, sets res to:
**
**       0 if all elements of d1 and d2 are equal
**       1 if any element is different or the diff of any element is above hi_d
**       2 if the diff of any element is between lo_d and hi_d and there are no
**         elements with diffs above hi_d.
**       3 if any element has an indeterminate comparison result and there are
**         no elements that differ or have diffs above or equal to lo_d.
**
**       warn and err epsilon can be provided, are only taken into consideration
**       for floating point values.
**
** For other values, sets res to:
**
**       0 if d1 == d2
**       1 if d1 != d2
**       3 if there was an indeterminate comparison result. As an example, this 
**         is returned when two generic pointers are compared.
**
** On error, sets res to 1 and the return value is negative.
**
** The parameters f1, d1, f2, d2 are advanced through their structures; on
** return, they are in the position following the relevant parts.
**
**
** This function can take warning and error epsilon tolerance into account for
** floating point values. It is turned on by setting ctl_e to true and providing
** lo_d, lo_bytes, hi_d, and hi_bytes arguments which is the aif data and length
** of the aif data of the warn and error tolerance levels respectively. 
**
** It is the callers responsibility to check that these arguments are valid 
** floating point AIF types that can be passed directly to the 
** _aif_to_doublest() function.
**
** For all types other than floating point values, the ctl_e value is ignored.
**
**
** This function can calculate the diff value if directed to do so. It is turned
** on by setting ctl_d to true and providing diff_f and diff_d pointers which
** will contain the format and data of the diff result on return.
**
** If epsilon tolerance is provided, diff values will be set to 0 for
** differences below lo_d.
**
** The diff is calculated in the following way.
**
** For scalar values:
**
**       The difference is val1 - val2.
**
** For booleans, enums, and hashed signatures:
**
**       0 if the values are the same
**       1 if the values are different
**
** For strings and fortran characters:
**
**      -1 if val1 < val2
**       0 if val1 == val2
**       1 if val1 > val2
**
**       Note that Fortran characters are always ' ' extended to be the same 
**       length according to the Fortran standard on character compare.
**
** For indeterminate comparison results:
**
**       The difference is an AIF void type.
**
** For arrays/structured types:
**
**       The above rules are applied to each field or element in the object.
**
**
** TODO: This no longer supports AIF_POINTER or AIF_UNION. In gdb4hpc, these types
**       are never used. If they are, fix this function!!!
**
*/
int
_aif_compare(	int *res, char **f1, char **d1, char **f2, char **d2, 
				int ctl_d, char **diff_f, char **diff_d,
				int ctl_e, char *lo_d, int lo_bytes, char *hi_d, int hi_bytes
			)
{
	int			i, j;
	int			l3;
	int			success; /* value to return indicating type match */
	int			bytes1;
	int			bytes2; /* number of bytes in arithmetic data */
	char *		fmt;
	char *		d3 = NULL;
	AIFLONGEST	val1;
	AIFLONGEST	val2;
	int			isFtn = 0; // used for creating format strings
	// Used in array/struct compare
	AIFIndex *	ix1;
	AIFIndex *	ix2;
	char *		fmt1;
	char *		fmt2;
	int			res2=0;
	char *		df;
	char *		dd;
	int			dlen;
	int			curlen=0;
	char *		fdf = NULL;
	// Used in struct compare
	int			nsf1;
	int			nsf2;
	char *		name1 = NULL;
	char *		name2 = NULL;
	char *		type1 = NULL;
	char *		type2 = NULL;
	char *		t1;
	char *		t2;
	// used by ftn character
	char *	td;
	// Used when comparing diff in place
	AIFLONGEST v_diff=0;

	ResetAIFError();

	/*
	** resolve names and references first
	*/
	_fds_resolve(f1);
	_fds_resolve(f2);

	if ( FDSType(*f1) == AIF_REFERENCE )
	{
		fmt = _fds_lookup(f1);
		return _aif_compare(	res, &fmt, d1, f2, d2, 
								ctl_d, diff_f, diff_d, ctl_e, lo_d, lo_bytes, 
								hi_d, hi_bytes );
	}

	if ( FDSType(*f2) == AIF_REFERENCE )
	{
		fmt = _fds_lookup(f2);
		return _aif_compare(	res, f1, d1, &fmt, d2,
								ctl_d, diff_f, diff_d, ctl_e, lo_d, lo_bytes, 
								hi_d, hi_bytes );
	}

	switch ( FDSType(*f1) )
	{
	case AIF_FUNCTION:
	
		switch ( FDSType(*f2) )
		{
		case AIF_FUNCTION:
			// advance the fds/data pointers
			_fds_skip_data(f1, d1);
			_fds_skip_data(f2, d2);
		
			// right now we do not actually implement comparison of functions.
			// warn the caller of this
			*res = 3;
			
			// Handle the diff calculation, just return a void type since we
			// didn't actually calculate anything.
			if (ctl_d)
			{
				*diff_f = strdup(AIF_VOID_TYPE(0));
				*diff_d = NULL;
			}
			
			return 0;
			
		default:
			// incompatible types
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;
	
	case AIF_GENERIC_POINTER:
	
		switch ( FDSType(*f2) )
		{
		case AIF_GENERIC_POINTER:
			// advance the fds/data pointers
			_fds_skip_data(f1, d1);
			_fds_skip_data(f2, d2);
		
			// We do not want to actually compare a generic pointer since
			// it simply points to a memory address. 
			// warn the caller of this
			*res = 3;
			
			// Handle the diff calculation, just return a void type since we
			// didn't actually calculate anything.
			if (ctl_d)
			{
				*diff_f = strdup(AIF_VOID_TYPE(0));
				*diff_d = NULL;
			}
			
			return 0;
			
		default:
			// incompatible types
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;

	case AIF_ENUM:
	
		switch ( FDSType(*f2) )
		{
		case AIF_ENUM:
			*f1 = _fds_base_type(*f1);
			*f2 = _fds_base_type(*f2);

			bytes1 = _fds_count_bytes(f1);
			bytes2 = _fds_count_bytes(f2);

			// reuse the integer compare routine, but we hardcode the diff 
			// to 0 since we want to do that part ourselves here.
			success = _aif_cmp_int(res, *d1, bytes1, *d2, bytes2, 0, NULL);
			
			// Do special things when calculating the diff
			if (ctl_d)
			{
				// If we created the diff, then we need to create the format 
				// string of the result and set the data depending on the value
				// of res
				if (success == 0)
				{
					*diff_f = strdup(AIF_INTEGER_TYPE(1, MAX(bytes1, bytes2)));
					if (*res == 0)
					{
						v_diff = 0;
					} else
					{
						*res = 1;
						v_diff = 1;
					}
					if (_longest_to_aif(diff_d, MAX(bytes1, bytes2), v_diff) < 0)
					{
						return -1;
					}
				} else
				{
					// Ensure everything is nulled
					*diff_f = NULL;
					*diff_d = NULL;
				}
			}

			*d1 += bytes1;
			*d2 += bytes2;

			return success;

		default:
			// incompatible types
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;

	case AIF_FTN_INTEGER:
		// TODO: What do we do for ftn versus c comparison? I am just
		// looking at the first type to determine the result type.
		isFtn = 1;
	case AIF_INTEGER:
	
		switch ( FDSType(*f2) )
		{
		case AIF_FTN_INTEGER:
		case AIF_INTEGER:
			bytes1 = _fds_count_bytes(f1);
			bytes2 = _fds_count_bytes(f2);

			success = _aif_cmp_int(res, *d1, bytes1, *d2, bytes2, ctl_d, diff_d);
			
			// Do special things when calculating the diff
			if (ctl_d)
			{
				// If we created the diff, then we need to create the format 
				// string of the result
				if (success == 0)
				{
					if (isFtn)
					{
						*diff_f = strdup(AIF_FTN_INTEGER_TYPE(MAX(bytes1, bytes2)));
					} else
					{
						*diff_f = strdup(AIF_INTEGER_TYPE(1, MAX(bytes1, bytes2)));
					}
				} else
				{
					// Ensure everything is nulled
					*diff_f = NULL;
					*diff_d = NULL;
				}
			}

			// increment the data pointers
			*d1 += bytes1;
			*d2 += bytes2;

			return success;

		case AIF_FTN_REAL:
			// We need to return a floating type if the first one was int and the
			// second one was float. This determines if we need ftn float or not.
			isFtn = 1;
			goto int_to_float_cmp;
			
		case AIF_FLOATING:
			isFtn = 0;
			
int_to_float_cmp:

			bytes1 = _fds_count_bytes(f1);
			l3 = _aif_int_to_aif_float(&d3, *d1, bytes1);
			*d1 += bytes1;

			if ( l3 < 0 )
			{
				SetAIFError(AIFERR_CONV, NULL);
				*res = 1;
				if (ctl_d)
				{
					// Ensure everything is nulled
					*diff_f = NULL;
					*diff_d = NULL;
				}
				return -1;
			}

			bytes2 = _fds_count_bytes(f2);
			
			// Use the float compare routine
			success = _aif_cmp_float(	res, d3, l3, *d2, bytes2, ctl_d, diff_d, 
										ctl_e, lo_d, lo_bytes, hi_d, hi_bytes );
			
			// Do special things when calculating the diff
			if (ctl_d)
			{
				// If we created the diff, then we need to create the format 
				// string of the result
				if (success == 0)
				{
					if (isFtn)
					{
						*diff_f = strdup(AIF_FTN_REAL_TYPE(MAX(l3, bytes2)));
					} else
					{
						*diff_f = strdup(AIF_FLOATING_TYPE(MAX(l3, bytes2)));
					}
				} else
				{
					// Ensure everything is nulled
					*diff_f = NULL;
					*diff_d = NULL;
				}
			}
			
			*d2 += bytes2;

			return success;

		default:
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;

	case AIF_HASHED_SIGNATURE:
	
		switch ( FDSType(*f2) )
		{
		case AIF_HASHED_SIGNATURE:
			bytes1 = _fds_count_bytes(f1);
			bytes2 = _fds_count_bytes(f2);

			// call the hash compare function
			success = _aif_cmp_hashed(res, *d1, bytes1, *d2, bytes2);
			
			// Do special things when calculating the diff
			if (ctl_d)
			{
				if (success == 0)
				{
					*diff_f = strdup(AIF_INTEGER_TYPE(1, sizeof(*res)));
					v_diff = *res;
					if (_longest_to_aif(diff_d, sizeof(*res), v_diff) < 0)
					{
						return -1;
					}
				} else
				{
					// Ensure everything is nulled
					*diff_f = NULL;
					*diff_d = NULL;
				}
			}

			*d1 += bytes1;
			*d2 += bytes2;

			return success;
		
		default:
			SetAIFError(AIFERR_CONV, NULL);
			*res = -1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;

	case AIF_FTN_REAL:
		// TODO: What do we do for ftn versus c comparison? I am just
		// looking at the first type to determine the result type.
		isFtn = 1;
	case AIF_FLOATING:
	
		switch ( FDSType(*f2) )
		{
		case AIF_FTN_INTEGER:
		case AIF_INTEGER:
			// We always return a floating type, so we always use the initial
			// value of isFtn here.
			bytes2 = _fds_count_bytes(f2);
			l3 = _aif_int_to_aif_float(&d3, *d2, bytes2);
			*d2 += bytes2;

			if ( l3 < 0 )
			{
				SetAIFError(AIFERR_CONV, NULL);
				*res = 1;
				if (ctl_d)
				{
					// Ensure everything is nulled
					*diff_f = NULL;
					*diff_d = NULL;
				}
				return -1;
			}

			bytes1 = _fds_count_bytes(f1);
			
			// Use the float compare routine
			success = _aif_cmp_float(	res, *d1, bytes1, d3, l3, ctl_d, diff_d, 
										ctl_e, lo_d, lo_bytes, hi_d, hi_bytes );
			
			// Do special things when calculating the diff
			if (ctl_d)
			{
				// If we created the diff, then we need to create the format 
				// string of the result
				if (success == 0)
				{
					if (isFtn)
					{
						*diff_f = strdup(AIF_FTN_REAL_TYPE(MAX(bytes1, l3)));
					} else
					{
						*diff_f = strdup(AIF_FLOATING_TYPE(MAX(bytes1, l3)));
					}
				} else
				{
					// Ensure everything is nulled
					*diff_f = NULL;
					*diff_d = NULL;
				}
			}
			
			*d1 += bytes1;
			
			return success;
			
		case AIF_FLOATING:
		case AIF_FTN_REAL:
			bytes1 = _fds_count_bytes(f1);
			bytes2 = _fds_count_bytes(f2);
			
			success = _aif_cmp_float(	res, *d1, bytes1, *d2, bytes2, ctl_d, diff_d,
										ctl_e, lo_d, lo_bytes, hi_d, hi_bytes );
			
			// Do special things when calculating the diff
			if (ctl_d)
			{
				// If we created the diff, then we need to create the format 
				// string of the result
				if (success == 0)
				{
					if (isFtn)
					{
						*diff_f = strdup(AIF_FTN_REAL_TYPE(MAX(bytes1, bytes2)));
					} else
					{
						*diff_f = strdup(AIF_FLOATING_TYPE(MAX(bytes1, bytes2)));
					}
				} else
				{
					// Ensure everything is nulled
					*diff_f = NULL;
					*diff_d = NULL;
				}
			}

			*d1 += bytes1;
			*d2 += bytes2;

			return success;

		default:
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;
		
	case AIF_FTN_COMPLEX:
	
		switch ( FDSType(*f2) )
		{
		case AIF_FTN_COMPLEX:
			// note that _fds_count_bytes returns enough room for both the real and img portions
			bytes1 = _fds_count_bytes(f1);
			bytes2 = _fds_count_bytes(f2);
			
			success = _aif_cmp_complex(	res, *d1, bytes1/2, *d2, bytes2/2,
										ctl_d, diff_d, ctl_e, lo_d, lo_bytes,
										hi_d, hi_bytes );
			
			// Do special things when calculating the diff
			if (ctl_d)
			{
				// If we created the diff, then we need to create the format 
				// string of the result
				if (success == 0)
				{
					*diff_f = strdup(AIF_FTN_COMPLEX_TYPE(MAX(bytes1/2, bytes2/2)));
				} else
				{
					// Ensure everything is nulled
					*diff_f = NULL;
					*diff_d = NULL;
				}
			}
			
			*d1 += bytes1;
			*d2 += bytes2;
			
			return success;
			
		default:
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;

	case AIF_CHARACTER:
	
		switch ( FDSType(*f2) )
		{
		case AIF_CHARACTER:
			bytes1 = _fds_count_bytes(f1);
			bytes2 = _fds_count_bytes(f2);

			success = _aif_cmp_char(res, *d1, *d2, ctl_d, diff_d);
			
			// Do special things when calculating the diff
			if (ctl_d)
			{
				// If we created the diff, then we need to create the format 
				// string of the result
				if (success == 0)
				{
					*diff_f = strdup(AIF_CHARACTER_TYPE());
				} else
				{
					// Ensure everything is nulled
					*diff_f = NULL;
					*diff_d = NULL;
				}
			}

			*d1 += bytes1;
			*d2 += bytes2;

			return success;

		default:
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;

	case AIF_ARRAY:
		// init the first ix for C array
		ix1 = FDSArrayIndexInit(*f1);
		goto cmp_array_ix2;
		
	case AIF_FTN_ARRAY:
		// init the first ix for Ftn array
		ix1 = FDSFtnArrayIndexInit(*f1);
		isFtn = 1;

cmp_array_ix2:
		switch ( FDSType(*f2) )
		{
		case AIF_ARRAY:
			ix2 = FDSArrayIndexInit(*f2);
			goto cmp_array;
			
		case AIF_FTN_ARRAY:
			ix2 = FDSFtnArrayIndexInit(*f2);

cmp_array:
			// Ensure that the number of elements between the two match
			if ( ix1->i_nel != ix2->i_nel )
			{
				SetAIFError(AIFERR_SIZE, NULL);
				if (ctl_d)
				{
					// Ensure everything is nulled
					*diff_f = NULL;
					*diff_d = NULL;
				}
				*res = 1;
				AIFArrayIndexFree(ix1);
				AIFArrayIndexFree(ix2);
				return -1;
			}

			/* 
			** skip over the entire array fds for f1 and f2
			*/
			_fds_advance(f1);
			_fds_advance(f2);

			*res = 0; /* first assumption, they are equal */
			
			for ( i = 0 ; i < ix1->i_nel ; i++ )
			{
				res2 = 0;
				fmt1 = ix1->i_btype;
				fmt2 = ix2->i_btype;
				df = NULL;
				dd = NULL;
				
				success = _aif_compare(	&res2, &fmt1, d1, &fmt2, d2,
										ctl_d, &df, &dd, ctl_e, lo_d, lo_bytes,
										hi_d, hi_bytes );
				if ( success < 0 )
				{
					// clean things up
					AIFArrayIndexFree(ix1);
					AIFArrayIndexFree(ix2);
					if (df != NULL)
						_aif_free(df);
					if (dd != NULL)
						_aif_free(dd);
					if (ctl_d)
					{
						_aif_free(*diff_f);
						_aif_free(*diff_d);
					}
					*diff_f = NULL;
					*diff_d = NULL;
					*res = res2;
					return -1;
				}
				
				/*
				** If res2 is 0, do not modify res. Our initial assumption was
				** to set res to 0. We only modify res if we find a "worst"
				** outcome. If we find an error and we are not creating the diff
				** we are finished and can return the final result. 
				*/
				switch (res2)
				{
					case -1:		
					case 1:
						// This has the highest priority and always overwrites res
						*res = 1;
						// If we are not creating the diff, we want to return at
						// this point since we have found an error and there is no
						// way we can return anything else.
						if (!ctl_d)
						{
							AIFArrayIndexFree(ix1);
							AIFArrayIndexFree(ix2);
							return 0;
						}
						break;
					
					case -2:
					case 2:
						// Only overwrite if res is not already 1
						if (*res != 1)
						{
							*res = 2;
						}
						break;
						
					case 3:
						// Only overwrite if res is not 1 or 2
						if (*res != 1 && *res != 2)
						{
							*res = 3;
						}
						break;
						
					default:
						// Do nothing
						break;
				}
				
				// Now create the diff for this data if directed to do so
				if (ctl_d)
				{
					// Ensure df is not null
					if (df == NULL)
					{
						// Error occured
						SetAIFError(AIFERR_GEN, NULL);
						// Cleanup
						_aif_free(*diff_f);
						_aif_free(*diff_d);
						*diff_f = NULL;
						*diff_d = NULL;
						*res = 1;
						AIFArrayIndexFree(ix1);
						AIFArrayIndexFree(ix2);
						return -1;
					}
				
					// Create the format/data for this entire rank if we haven't
					// already done so. This assumes homogenous array elements.
					if (fdf == NULL)
					{
						char *ftmp;
						char *rtype;
						char *itype = strdup(AIF_INTEGER_TYPE(0, sizeof(int)));
						
						// XXX: This is using the range information of ix1, I think
						//       this is okay because we ensure the number of elements
						//       match between the two arrays, but the diff result
						//       could be wrong if the ranks don't match...
						//
						// create the innermost rank string first
						rtype = strdup(AIF_RANGE_TYPE(ix1->i_min[ix1->i_rank-1], ix1->i_max[ix1->i_rank-1], itype));
						
						// Note that this is the only time we use df. We must use df
						// because it could be different than the base type we passed
						// into the compare function.
						if (isFtn)
						{
							fdf = strdup(AIF_FTN_ARRAY_TYPE(rtype, df));
						} else
						{
							fdf = strdup(AIF_ARRAY_TYPE(rtype, df));
						}
						
						_aif_free(rtype);
						
						// now loop over each of the other ranks inner to outer
						for (j=ix1->i_rank-2; j >= 0; --j)
						{
							// create the range for this rank - reuse itype this is always the same
							rtype = strdup(AIF_RANGE_TYPE(ix1->i_min[j], ix1->i_max[j], itype));
							// Now create the array type based on the previous fdf. Save this to
							// ftmp so that we can free the existing fdf
							if (isFtn)
							{
								ftmp = strdup(AIF_FTN_ARRAY_TYPE(rtype, fdf));
							} else
							{
								ftmp = strdup(AIF_ARRAY_TYPE(rtype, fdf));
							}
							_aif_free(rtype);
							_aif_free(fdf);
							fdf = ftmp;
						}
						
						// free the itype, we are done with it
						_aif_free(itype);
						
						// Set the final diff_f, this gets freed by the caller
						*diff_f = fdf;
						
						// Now allocate space for the data section
						int type_len = FDSTypeSize(fdf);
						if (type_len != 0)
						{
							// FIXME: This is using the number of elements from ix1, probably
							//        not a valid thing to do...
							*diff_d = (char *)_aif_alloc(type_len);
						} else
						{
							*diff_d = NULL;
						}
						
						// reset curlen to 0
						curlen = 0;
					}
					
					// calculate the size of the created dd from df
					dlen = FDSTypeSize(df);
					
					// Ensure that there is data for this type, otherwise continue
					if (dlen == 0)
					{
						_aif_free(df);
						continue;
					}
					
					// now copy the data to diff_d
					memcpy(*diff_d + curlen, dd, dlen);
					// add dlen to curlen
					curlen += dlen;
					// cleanup df and dd
					_aif_free(df);
					_aif_free(dd);
				}
			}

			AIFArrayIndexFree(ix1);
			AIFArrayIndexFree(ix2);
			
			return 0;
			
		default:
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;

	/*
	** only uses the public section in AIF_STRUCT 
	** structure equivalence: we ignore field names
	*/
	case AIF_STRUCT:
	
		switch ( FDSType(*f2) )
		{
		case AIF_STRUCT:
			// We start out by assuming they are equal
			*res = 0;
			
			// Get the number of fields in each struct
			nsf1 = FDSNumFields(*f1);
			nsf2 = FDSNumFields(*f2);
			
			// Ensure the number of fields are the same for both structs, we let
			// the recursive calls to _aif_compare work out if the types match.
			if (nsf1 != nsf2)
			{
				// num fields do not match
				SetAIFError(AIFERR_CONV, NULL);
				if (ctl_d)
				{
					*diff_f = NULL;
					*diff_d = NULL;
				}
				*res = 1;
				return -1;
			}
			
			// Do special things if we are calculating the diff
			if (ctl_d)
			{
				// Create an empty struct format string based on f1.
				// TODO: What if f1 and f2 differ??? The struct names (i.e. id)
				//       and field names can be different, but as long as the
				//       types line up we will compare it. This could be confusing.
				
				// get the id of f1
				fdf = FDSGetIdentifier(*f1);
				// now init the struct fds
				*diff_f = FDSStructInit(fdf);
				_aif_free(fdf);
				
				// DO NOT ALLOCATE THE DATA SECTION YET!!!
				// We do not expect the size of the data to be exactly the same
				// since the diff on each field could return a result different
				// than the original. This is because it doesn't make sense to
				// return the same type for certain diffs (like bool results).
				*diff_d = NULL;
				
				// reset curlen to 0
				curlen = 0;
			}
			
			for (i=0; i < nsf1; i++)
			{
				// Get the name and type of this field
				if (	FDSStructFieldByNumber(*f1, i, &name1, &type1) < 0 ||
						FDSStructFieldByNumber(*f2, i, &name2, &type2) < 0 )
				{
					// could not extract field information
					SetAIFError(AIFERR_CONV, NULL);
					if (ctl_d)
					{
						_aif_free(*diff_f);
						if (*diff_d != NULL)
						{
							_aif_free(*diff_d);
						}
						*diff_f = NULL;
						*diff_d = NULL;
					}
					*res = 1;
					return -1;
				}
				
				// free name2, we don't care about it
				_aif_free(name2);

				// init stuff for the recursive call
				res2 = 0;
				df = NULL;
				dd = NULL;
				t1 = type1;
				t2 = type2;
				
				// Recursively call _aif_compare using this fields type
				// information. This will advance the data pointer for us.
				success = _aif_compare(	&res2, &t1, d1, &t2, d2,
										ctl_d, &df, &dd, ctl_e, lo_d, lo_bytes,
										hi_d, hi_bytes );
				if ( success < 0 )
				{
					// clean things up
					_aif_free(type1);
					_aif_free(type2);
					_aif_free(name1);
					if (df != NULL)
						_aif_free(df);
					if (dd != NULL)
						_aif_free(dd);
					if (ctl_d)
					{
						_aif_free(*diff_f);
						if (*diff_d != NULL)
						{
							_aif_free(*diff_d);
						}
						*diff_f = NULL;
						*diff_d = NULL;
					}
					*res = res2;
					return -1;
				}
				
				// Done with the field types
				_aif_free(type1);
				_aif_free(type2);
				
				/*
				** If res2 is 0, do not modify res. Our initial assumption was
				** to set res to 0. We only modify res if we find a "worst"
				** outcome. If we find an error and we are not creating the diff
				** we are finished and can return the final result. 
				*/
				switch (res2)
				{
					case -1:		
					case 1:
						// This has the highest priority and always overwrites res
						*res = 1;
						// If we are not creating the diff, we want to return at
						// this point since we have found an error and there is no
						// way we can return anything else.
						if (!ctl_d)
						{
							// cleanup
							_aif_free(name1);
							
							return 0;
						}
						break;
					
					case -2:
					case 2:
						// Only overwrite if res is not already 1
						if (*res != 1)
						{
							*res = 2;
						}
						break;
						
					case 3:
						// Only overwrite if res is not 1 or 2
						if (*res != 1 && *res != 2)
						{
							*res = 3;
						}
						break;
						
					default:
						// Do nothing
						break;
				}
				
				// Now create the diff for this data if directed to do so
				if (ctl_d)
				{
					// Ensure df is not null
					if (df == NULL)
					{
						// Error occured
						SetAIFError(AIFERR_GEN, NULL);
						// Cleanup
						_aif_free(name1);
						_aif_free(*diff_f);
						if (*diff_d != NULL)
						{
							_aif_free(*diff_d);
						}
						*diff_f = NULL;
						*diff_d = NULL;
						*res = 1;
						return -1;
					}
					
					// add this field to the fds for the struct based on f1
					// TODO: Is it okay to always use the first struct as a basis?
					fdf = FDSAddFieldToStruct(*diff_f, name1, df);
					// Free old diff_f and set it to the new one
					_aif_free(*diff_f);
					*diff_f = fdf;
					
					// resize diff_d to hold enough data for this new field if
					// the new field has any data.
					dlen = FDSTypeSize(df);
					if (dlen)
					{
						// need to resize diff_d
						*diff_d = (char *)_aif_resize(*diff_d, curlen + dlen);
						// write the data
						memcpy(*diff_d + curlen, dd, dlen);
						// add dlen to curlen
						curlen += dlen;
						// cleanup dd
						_aif_free(dd);
					}
					
					// cleanup
					_aif_free(df);
				}
				
				// cleanup
				_aif_free(name1);
				name1 = NULL;
				name2 = NULL;
				type1 = NULL;
				type2 = NULL;
			}
			
			// advance the fds 
			_fds_advance(f1);
			_fds_advance(f2);
			
			return 0;
			
		default:
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;

	case AIF_FTN_CHARACTER:
		
		switch ( FDSType(*f2) )
		{
		case AIF_FTN_CHARACTER:
		
			bytes1 = _fds_count_bytes(f1);
			bytes2 = _fds_count_bytes(f2);

			/* 
			 * if the length does not match, we need to pad with spaces
			 *
			 * Character operands are compared one character at a time, in order,
			 * starting with the first character of each operand. If the two character
			 * operands are not the same length, the shorter one is padded on the right
			 * with blanks until the lengths are equal; for example:
			 * 
			 * 'ABC' .EQ. 'ABC '
			 * 'AB' .LT. 'C'
			 *
			 * The first relational expression has the value .TRUE. even though the
			 * lengths of the expressions are not equal, and the second has the value
			 * .TRUE. even though 'AB' is longer than 'C'.
			 */
			if (bytes1 != bytes2)
			{
				if (bytes1 < bytes2)
				{
					td = (char *)_aif_alloc(bytes2);
					// init to ' '
					memset(td, ' ', bytes2);
					// copy data
					memcpy(td, *d1, bytes1);
					
					*res = strncmp(td, *d2, bytes2);
					
					_aif_free(td);
				} else
				{
					td = (char *)_aif_alloc(bytes1);
					// init to ' '
					memset(td, ' ', bytes1);
					// copy data
					memcpy(td, *d2, bytes2);
					
					*res = strncmp(*d1, td, bytes1);
					
					_aif_free(td);
				}
			} else
			{
				*res = strncmp(*d1, *d2, MIN(bytes1, bytes2));
			}

			*d1 += bytes1;
			*d2 += bytes2;
			
			// Create the diff for this data if directed to do so
			if (ctl_d)
			{
				*diff_f = strdup(AIF_INTEGER_TYPE(1, sizeof(*res)));
				v_diff = *res;
				if (_longest_to_aif(diff_d, sizeof(*res), v_diff) < 0)
				{
					return -1;
				}
			} else
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}

			return 0;
			
		default:
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;
	
	case AIF_STRING:
	
		switch ( FDSType(*f2) )
		{
		case AIF_STRING:
			bytes1 = _fds_count_bytes(f1);
			bytes2 = _fds_count_bytes(f2);

			*res = strncmp(*d1, *d2, MIN(bytes1, bytes2));

			*d1 += bytes1;
			*d2 += bytes2;
			
			// Create the diff for this data if directed to do so
			if (ctl_d)
			{
				*diff_f = strdup(AIF_INTEGER_TYPE(1, sizeof(*res)));
				v_diff = *res;
				if (_longest_to_aif(diff_d, sizeof(*res), v_diff) < 0)
				{
					return -1;
				}
			} else
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}

			if ( *res )
				return 0;

			if ( bytes1 == bytes2 )
			{
				/* 
				** they are the same in length and contents
				*/
				return 0;
			} else
			{
				/*
				** they start the same, but one is longer
				*/
				*res = (bytes1 < bytes2) ? -1 : 1;
				
				return 0;
			}
			
		default:
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;

	case AIF_BOOLEAN:
	case AIF_FTN_LOGICAL:
	
		switch ( FDSType(*f2) )
		{
		case AIF_BOOLEAN:
		case AIF_FTN_LOGICAL:
			bytes1 = _fds_count_bytes(f1);
			bytes2 = _fds_count_bytes(f2);
			
			_aif_to_longest(*d1, bytes1, &val1);
			_aif_to_longest(*d2, bytes2, &val2);

			if( val1 == val2)
				*res = 0;
			else
				*res = 1;
			
			*d1 += bytes1;
			*d2 += bytes2;
			
			// Create the diff for this data if directed to do so
			if (ctl_d)
			{
				*diff_f = strdup(AIF_INTEGER_TYPE(1, sizeof(*res)));
				v_diff = *res;
				if (_longest_to_aif(diff_d, sizeof(*res), v_diff) < 0)
				{
					return -1;
				}
			} else
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}

			return 0;

		default:
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;

	case AIF_VOID:
	
		switch ( FDSType(*f2) )
		{
		case AIF_VOID:
			// advance the fds/data pointers
			_fds_skip_data(f1, d1);
			_fds_skip_data(f2, d2);
			
			// not actually comparing things here, so give the warning
			*res = 3;
			
			// Handle the diff calculation, just return a void type since we
			// didn't actually calculate anything.
			if (ctl_d)
			{
				*diff_f = strdup(AIF_VOID_TYPE(0));
				*diff_d = NULL;
			}
			
			return 0;
		
		default:
			SetAIFError(AIFERR_CONV, NULL);
			*res = 1;
			if (ctl_d)
			{
				// Ensure everything is nulled
				*diff_f = NULL;
				*diff_d = NULL;
			}
			return -1;
		}
		break;

	default:
		SetAIFError(AIFERR_TYPE, NULL);
		*res = 1;
		if (ctl_d)
		{
			// Ensure everything is nulled
			*diff_f = NULL;
			*diff_d = NULL;
		}
		return -1;
	}
}

/*
** data points to the data part of an AIF, starting at the format byte.
**  0 means null
**  1 means ordinary pointer
**  2 means ordinary pointer, named
**  3 means refer to existing name
**
** We save any names we see.  We offset the index for saving/retrieving 
** values by "offset" in order to simultaneously maintain targets for multiple 
** AIFs.  We return a pointer to the data region where the value actually 
** starts, or 0 if null.  We also advance the data pointer past the initial 
** code (and name if present).
*/

char *
_find_target(char **data, int offset)
{
	int	name;
	char	code = **data;

	(*data)++; /* past the code */

	switch ( code )
	{
	case AIF_PTR_NIL:
		return (char) 0;

	case AIF_PTR_NORMAL: /* unadorned value */
		return *data;

	case AIF_PTR_NAME: /* named value */
		_ptrname_to_int(*data, &name);
		(*data) += 4; /* past the name */
		_aif_values_seen[name+offset] = *data;

		return *data;

	case AIF_PTR_REFERENCE: /* reference to named value */
		_ptrname_to_int(*data, &name);
		(*data) += 4; /* past the name */

		return _aif_values_seen[name+offset];
	}

	return 0; /* if we fall through, no target to be found */
}

