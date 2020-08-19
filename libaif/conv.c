/*
 * Routines for converting between native data format and
 * AIF or for constructing AIF objects.
 *
 * Copyright (c) 1996-2002 by Guardsoft Pty Ltd.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 */

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<assert.h>
#include	<ctype.h>
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<stdarg.h>

#include	"aif.h"
#include	"aiferr.h"
#include	"aifint.h"

#if defined(WINDOWSNT) || defined(WIN32)
#define BITSPERBYTE	8
#elif defined(__APPLE__)
#include	<sys/types.h>
#define BITSPERBYTE	NBBY
#else /* WINDOWSNT || WIN32 */
#include	<values.h>
#endif /* WINDOWSNT || WIN32 */

union ieee
{
	char		c;
	int		i;
	AIFLONGEST	li;
	float		f;
	double		d;
	AIFDOUBLEST	dd;
	/*
	** long enough for any conceivable precision
	*/
	char		bytes[256/BITSPERBYTE];
};      

/*
 * For _aif_*_to_str() functions
 */
static char *_str_buf = NULL;
static char *_str_pos = NULL;
static int _str_buf_len = 0;

/*
 * Convert base type to standard byte ordering.
 */
void
_aif_normalise(char *dst, int dstlen, char *src, int srclen)
{
#ifndef WORDS_BIGENDIAN
	int	i;
#endif
	int	len;
	int	pad = 0;

	ResetAIFError();

	if ( srclen < dstlen )
	{
		len = srclen;
		pad = dstlen - srclen;
	} 
	else if ( srclen >= dstlen )
		len = dstlen;

#ifdef WORDS_BIGENDIAN
	memcpy(dst, src+srclen-len, len);

	if ( pad > 0 )
		memset(dst+len, '\0', pad);
#else /* WORDS_BIGENDIAN */
	for ( i = len - 1 ; i >= 0 ; i-- )
		*dst++ = src[i];

	if ( pad > 0 )
		memset(dst, '\0', pad);
#endif /* WORDS_BIGENDIAN */
}

/*
 * Normalise a value to the standard format
 */
void
AIFNormalise(char *dst, int dstlen, char *src, int srclen)
{
	_aif_normalise(dst, dstlen, src, srclen);
}

/*
 * Convert a pointer value to AIF data format. Result is rd.
 */
int
_pointer_to_aif(char **rd, const AIF *addr, const AIF *val)
{
	if ( rd == NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	ResetAIFError();

	if( *rd == NULL ) {
		*rd = (char *)_aif_alloc(AIF_LEN(addr) + AIF_LEN(val) + 1);
	}

	**rd = (char) AIF_PTR_NORMAL; /* indicates normal pointer value */
	
	memcpy((*rd)+1, AIF_DATA(addr), AIF_LEN(addr));
	memcpy((*rd)+1+AIF_LEN(addr), AIF_DATA(val), AIF_LEN(val));

	return 0;
}

/*
 * Convert an integer value of any length to AIF data format.
 * Result is rd.
 */
int
_longest_to_aif(char **rd, int len, AIFLONGEST val)
{
	int		srclen;
	union ieee	f;

	if ( rd == NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	ResetAIFError();

	if( *rd == NULL )
		*rd = (char *)_aif_alloc(len);

	if ( len <= (int)sizeof(int) )
	{
		f.i = (int)val;
		srclen = sizeof(int);
	}
	else
	{
		f.li = val;
		srclen = sizeof(AIFLONGEST);
	}

	_aif_normalise(*rd, len, f.bytes, srclen);

	return 0;
}

/*
 * Convert a character value to AIF data format. Result is rd.
 */
int
_char_to_aif(char **rd, char val)
{
	union ieee	f;

	if ( rd == NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	ResetAIFError();

	if( *rd == NULL )
		*rd = (char *)_aif_alloc(sizeof(char));

	f.c = val;

	AIFNormalise(*rd, sizeof(char), f.bytes, sizeof(char));

	return 0;
}

/*
 * Create an empty named AIF object.
 */
AIF *
NameAIF(AIF *oldAIF, int name)
{
	AIF *	a;

	ResetAIFError();

	a = NewAIF(0, 0);

	AIF_FORMAT(a) = strdup(AIF_NAME_TYPE(name, AIF_FORMAT(oldAIF)));
	AIF_DATA(a) = _aif_alloc(AIF_LEN(oldAIF));
	memcpy(AIF_DATA(a), AIF_DATA(oldAIF), AIF_LEN(oldAIF));
	AIF_LEN(a) = AIF_LEN(oldAIF);

	return a;
}

int
AIFSetIdentifier(AIF *a, char *id)
{
	return FDSSetIdentifier(&(AIF_FORMAT(a)), id);
}

char *
AIFGetIdentifier(AIF *a)
{
	return FDSGetIdentifier(AIF_FORMAT(a));
}

/*
 * Create an empty reference to an AIF object
 */
AIF *
ReferenceAIF(int name)
{
	AIF *	a;

	ResetAIFError();

	a = NewAIF(0, 0);

	AIF_FORMAT(a) = strdup(AIF_REFERENCE_TYPE(name));
	AIF_LEN(a) = 0;
	
	return a;
}

/*
 * toWhat represents the sort of thing to which a null pointer needs to be
 * created.  If it is named, that name is used in the resulting AIF; otherwise,
 * the format of toWhat is used to generate the format of the new AIF.
 */
AIF *
AIFNull(AIF *toWhat)
{
	int	name;
	AIF *	a;
	char *	fmt;
	char *	oldFormat = AIF_FORMAT(toWhat);

	ResetAIFError();

	a = NewAIF(0, 1);

	if ( *oldFormat == '%' )
	{
		/*
		** a reference
		*/
		sscanf(oldFormat, "%%%d/", &name);
		fmt = strdup(AIF_REFERENCE_TYPE(name));
		AIF_FORMAT(a) = strdup(AIF_POINTER_TYPE(NULL, fmt));
		_aif_free(fmt);
	}
	else
	{
		/* 
		** repeat the format
		*/
		AIF_FORMAT(a) = strdup(AIF_POINTER_TYPE(NULL, oldFormat));
	}

	AIF_DATA(a)[0] = AIF_PTR_NIL;
	AIF_LEN(a) = 1;
	
	return a;
}

AIF *
PointerNameToAIF(AIF *i)
{
	AIF *	a;
	int	name;
	char *	fmt = AIF_FORMAT(i);

	a = NewAIF(0, AIF_LEN(i)+2);

	sscanf(fmt, "%%%d/", &name);

	AIF_FORMAT(a) = strdup(AIF_POINTER_TYPE(NULL, AIF_FORMAT(i)));

	*(AIF_DATA(a)) = (char) AIF_PTR_NAME;
	_int_to_ptrname( (int) name, AIF_DATA(a)+1);
	
	memcpy(AIF_DATA(a)+5, AIF_DATA(i), AIF_LEN(i));

	ResetAIFError();

	return a;
}

AIF *
PointerReferenceToAIF(AIF *toWhat)
{
	int	name;
	AIF *	a;
	char *	fmt;
	char *	oldFormat = AIF_FORMAT(toWhat);

	ResetAIFError();

	a = NewAIF(0, 2);

	if ( *oldFormat == '%' )
	{
		/*
		** a reference
		*/
		sscanf(oldFormat, "%%%d/", &name);
		fmt = strdup(AIF_REFERENCE_TYPE(name));
		AIF_FORMAT(a) = strdup(AIF_POINTER_TYPE(NULL, fmt));
		_aif_free(fmt);
	}

	AIF_DATA(a)[0] = AIF_PTR_REFERENCE;
	_int_to_ptrname( (int) name, AIF_DATA(a)+1);
	AIF_LEN(a) = 5;
	
	return a;
}

/*
 * Create a pointer to an AIF object.
 */
AIF *
PointerToAIF(AIF *addr, AIF *i)
{
	AIF *	a;

	a = NewAIF(0, AIF_LEN(addr)+AIF_LEN(i)+1);
	AIF_FORMAT(a) = strdup(AIF_POINTER_TYPE(AIF_FORMAT(addr), AIF_FORMAT(i)));

	if ( _pointer_to_aif(&AIF_DATA(a), addr, i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	ResetAIFError();

	return a;
}

/*
** Convert a generic pointer represented by the string ptr_type to AIF
*/
AIF *
GenericPointerToAIF(AIF *addr, char *ptr_type)
{
	AIF *	a;
	int	ptr_length = strlen(ptr_type)+1;

	ResetAIFError();

	a = NewAIF(0, ptr_length+AIF_LEN(addr));
	
	AIF_FORMAT(a) = strdup(AIF_GENERIC_POINTER_TYPE(ptr_length, AIF_FORMAT(addr)));

	strncpy(AIF_DATA(a), ptr_type, ptr_length);
	memcpy(AIF_DATA(a)+ptr_length, AIF_DATA(addr), AIF_LEN(addr));

	return a;
}

/*
 * Convert a generic function (defined as a named prototype with a variable 
 * number of arguments, and having an address value) to AIF
 *
 * TODO: This function takes in fds format strings for both the return type
 *	and the arguments. This is not intuitive for the end user! There
 *	should be a special fds type created to distinguish between a true
 *	fds and a random char pointer. 
 */
AIF *
FunctionToAIF(AIF *func_addr, char *func_name, char *func_rtn_fmt, int argc, char *argv[])
{
	AIF *	a;
	int		i;
	int		data_len = 0;
	int		args_fmt_len = 0;
	int		cpy_len;
	char *	args_fmt_string = NULL;
	int		func_name_len;
	
	ResetAIFError();
	
	// We start by creating the argument format string
	if (argc > 0)
	{
		// calculate the length of the args_fmt_string
		for (i=0; i<argc; ++i)
		{
			// add in the length of the arg format string plus the
			// length of the FDS_FUNCTION_ARG_SEP to the 
			//args_fmt_len
			if (argv[i] == NULL)
			{
				SetAIFError(AIFERR_BADARG, "Null argv entry detected in valid argv index.");
				return (AIF *)NULL;
			}
			args_fmt_len += strlen(argv[i]) + 1;
		}
		
		// create the args_fmt_string
		// NOTE: since we added in the FDS_FUNCTION_ARG_SEP for each of
		//       the arg_fmt entries, we already are including the NULL
		//       terminator for the string.
		if ((args_fmt_string = malloc(args_fmt_len)) == (void *)0)
		{
			SetAIFError(AIFERR_STRING, "Malloc failed");
			return (AIF *)NULL;
		}
		// copy the first argument to the format string
		snprintf(args_fmt_string, args_fmt_len, "%s", argv[0]);
		cpy_len = strlen(argv[0]);
		// copy the rest of the arguments to the format string
		for (i=1; i<argc; ++i)
		{
			snprintf(args_fmt_string+cpy_len, args_fmt_len, ",%s", argv[i]);
			cpy_len += strlen(argv[i])+1;
		}
	} else
	{
		args_fmt_string = strdup("");
	}
	
	// The data for the AIF_FUNCTION type consists of the functions name string
	// followed by the functions address data
	
	// get the length of the addr data
	data_len = AIF_LEN(func_addr);
	
	// get the length of the func_name string and add that in to data_len, don't forget the null terminator.
	func_name_len = strlen(func_name) + 1;
	data_len += func_name_len;
	
	// create the aif object
	a = NewAIF(0, data_len);
	
	// set the aif format string
	AIF_FORMAT(a) = strdup(AIF_FUNCTION_TYPE(args_fmt_string, func_name_len, func_rtn_fmt, AIF_FORMAT(func_addr)));
	
	// free the args_fmt_string
	_aif_free(args_fmt_string);
	
	// set the aif data
	strncpy(AIF_DATA(a), func_name, func_name_len);
	memcpy(AIF_DATA(a)+func_name_len, AIF_DATA(func_addr), AIF_LEN(func_addr));
	
	return a;
}

/*
 * Convert a string (defined as a pointer to a null terminated array
 * of characters) to AIF format.
 */
AIF *
StringToAIF(char *i)
{
	AIF *	a;
	int	length = sizeof(char)*strlen(i);

	ResetAIFError();

	a = NewAIF(0, length);

	AIF_FORMAT(a) = strdup(AIF_STRING_TYPE(length));

	strncpy(AIF_DATA(a), i, length);

	return a;
}

/*
 * Convert an address of a pointer to AIF format.
 */
AIF *
AddressToAIF(void *addr, int addr_len)
{
	AIF * a;

	a = NewAIF(0, addr_len);
	AIF_FORMAT(a) = strdup(AIF_ADDRESS_TYPE(addr_len));

	if ( _longest_to_aif(&AIF_DATA(a), addr_len, (AIFLONGEST)addr) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}
	
	return a;
}

/*
 * Convert a boolean value to AIF.
 */
AIF *
BoolToAIF(int b)
{
	AIF  * a;
	int    size;

	size = sizeof(int);

	a = NewAIF(0, size);

	AIF_FORMAT(a) = strdup(AIF_BOOLEAN_TYPE());

	if ( b )
		memset(AIF_DATA(a), 0xff, size);
	else
		memset(AIF_DATA(a), 0, size);

	return a;
}

/*
 * Convert a char to AIF.
 */
AIF *
CharToAIF(char i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(char));

	AIF_FORMAT(a) = strdup(AIF_INTEGER_TYPE(1, sizeof(char)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(char), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert a unsigned char to AIF.
 */
AIF *
UnsignedCharToAIF(unsigned char i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(unsigned char));

	AIF_FORMAT(a) = strdup(AIF_INTEGER_TYPE(0, sizeof(unsigned char)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(unsigned char), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert a short integer to AIF.
 */
AIF *
ShortToAIF(short i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(short));

	AIF_FORMAT(a) = strdup(AIF_INTEGER_TYPE(1, sizeof(short)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(short), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert an unsigned short to AIF.
 */
AIF *
UnsignedShortToAIF(unsigned short i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(unsigned short));

	AIF_FORMAT(a) = strdup(AIF_INTEGER_TYPE(0, sizeof(unsigned short)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(unsigned short), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert an integer to AIF.
 */
AIF *
IntToAIF(int i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(int));

	AIF_FORMAT(a) = strdup(AIF_INTEGER_TYPE(1, sizeof(int)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(int), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert a hashed signature to AIF.
 */
AIF *
HashToAIF(int i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(int));

	AIF_FORMAT(a) = strdup(AIF_HASHED_TYPE(sizeof(int)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(int), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert an unsigned integer to AIF.
 */
AIF *
UnsignedIntToAIF(unsigned int i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(unsigned int));

	AIF_FORMAT(a) = strdup(AIF_INTEGER_TYPE(0, sizeof(unsigned int)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(unsigned int), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert a long integer to AIF.
 */
AIF *
LongToAIF(long i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(long));

	AIF_FORMAT(a) = strdup(AIF_INTEGER_TYPE(1, sizeof(long)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(long), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert an unsigned long to AIF.
 */
AIF *
UnsignedLongToAIF(unsigned long i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(unsigned long));

	AIF_FORMAT(a) = strdup(AIF_INTEGER_TYPE(0, sizeof(unsigned long)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(unsigned long), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

#ifdef CC_HAS_LONG_LONG
/*
 * Convert a long long integer to AIF.
 */
AIF *
LongLongToAIF(long long i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(long long));

	AIF_FORMAT(a) = strdup(AIF_INTEGER_TYPE(1, sizeof(long long)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(long long), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert an unsigned long long to AIF.
 */
AIF *
UnsignedLongLongToAIF(unsigned long long i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(unsigned long long));

	AIF_FORMAT(a) = strdup(AIF_INTEGER_TYPE(0, sizeof(unsigned long long)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(unsigned long long), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}
#endif /* CC_HAS_LONG_LONG */

/*
 * Convert an integer of any length to AIF.
 */
AIF *
LongestToAIF(AIFLONGEST i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(AIFLONGEST));

	AIF_FORMAT(a) = strdup(AIF_INTEGER_TYPE(1, sizeof(AIFLONGEST)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(AIFLONGEST), i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert a floating point value of any length to AIF data format.
 * Result in rd.
 */
int
_doublest_to_aif(char **rd, int len, AIFDOUBLEST val)
{
	union ieee	v;

	if ( rd == NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	ResetAIFError();

	if( *rd == NULL )
		*rd = (char *)_aif_alloc(len);

	if ( len <= (int)sizeof(float) )
		v.f = (float)val;
	else if ( len == sizeof(double) )
		v.d = (double)val;
	else
		v.dd = val;

	AIFNormalise(*rd, len, v.bytes, len);

	return 0;
}

/*
 * Convert a single precision floating point value to AIF.
 */
AIF *
FloatToAIF(float f)
{
	AIF *	a;

	a = NewAIF(0, sizeof(float));

	AIF_FORMAT(a) = strdup(AIF_FLOATING_TYPE(sizeof(float)));

	if ( _doublest_to_aif(&AIF_DATA(a), sizeof(float), (AIFDOUBLEST)f) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert a double precision floating point value to AIF.
 */
AIF *
DoubleToAIF(double f)
{
	AIF *	a;

	a = NewAIF(0, sizeof(double));

	AIF_FORMAT(a) = strdup(AIF_FLOATING_TYPE(sizeof(double)));

	if ( _doublest_to_aif(&AIF_DATA(a), sizeof(double), (AIFDOUBLEST)f) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

#ifdef HAVE_LONG_DOUBLE
/*
 * Convert a long double precision floating point value to AIF.
 */
AIF *
LongDoubleToAIF(long double f)
{
	AIF *	a;

	a = NewAIF(0, sizeof(long double));

	AIF_FORMAT(a) = strdup(AIF_FLOATING_TYPE(sizeof(long double)));

	if ( _doublest_to_aif(&AIF_DATA(a), sizeof(long double), (AIFDOUBLEST)f) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}
#endif /* HAVE_LONG_DOUBLE */

/*******************************
 * Fortran Type Extensions
 *******************************/
 
/*
 * Convert a INTEGER*1 value to AIF
 * INTEGER*1 translates to a char in c
 */
AIF *
INTEGER1ToAIF(char c)
{
	AIF *   a;
	
	a = NewAIF(0, sizeof(char));
	
	AIF_FORMAT(a) = strdup(AIF_FTN_INTEGER_TYPE(sizeof(char)));
	
	if ( _char_to_aif(&AIF_DATA(a), c) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}
	
	return a;
}

/*
 * Convert a INTEGER*2 value to AIF
 * INTEGER*2 translates to a short in c
 */
AIF *
INTEGER2ToAIF(short i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(short));

	AIF_FORMAT(a) = strdup(AIF_FTN_INTEGER_TYPE(sizeof(short)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(short), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert a INTEGER*4 value to AIF
 * INTEGER*4 translates to a int in c
 */
AIF *
INTEGER4ToAIF(int i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(int));

	AIF_FORMAT(a) = strdup(AIF_FTN_INTEGER_TYPE(sizeof(int)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(int), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

#ifdef CC_HAS_LONG_LONG
/*
 * Convert a INTEGER*8 value to AIF
 * INTEGER*8 translates to a long long in c
 */
AIF *
INTEGER8ToAIF(long long i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(long long));

	AIF_FORMAT(a) = strdup(AIF_FTN_INTEGER_TYPE(sizeof(long long)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(long long), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}
#endif /* CC_HAS_LONG_LONG */

/*
 * Convert a REAL*4 value to AIF
 * REAL*4 translates to a float in c
 */
AIF *
REAL4ToAIF(float f)
{
	AIF *	a;

	a = NewAIF(0, sizeof(float));

	AIF_FORMAT(a) = strdup(AIF_FTN_REAL_TYPE(sizeof(float)));

	if ( _doublest_to_aif(&AIF_DATA(a), sizeof(float), (AIFDOUBLEST)f) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert a REAL*8 value to AIF
 * REAL*8 translates to a double in c
 */
AIF *
REAL8ToAIF(double f)
{
	AIF *	a;

	a = NewAIF(0, sizeof(double));

	AIF_FORMAT(a) = strdup(AIF_FTN_REAL_TYPE(sizeof(double)));

	if ( _doublest_to_aif(&AIF_DATA(a), sizeof(double), (AIFDOUBLEST)f) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

#ifdef HAVE_LONG_DOUBLE
/*
 * Convert a REAL*16 value to AIF
 * REAL*16 translates to a long double in c
 */
AIF *
REAL16ToAIF(long double f)
{
	AIF *	a;

	a = NewAIF(0, sizeof(long double));

	AIF_FORMAT(a) = strdup(AIF_FTN_REAL_TYPE(sizeof(long double)));

	if ( _doublest_to_aif(&AIF_DATA(a), sizeof(long double), (AIFDOUBLEST)f) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}
#endif /* HAVE_LONG_DOUBLE */

/*
 * Convert a LOGICAL*1 value to AIF
 * LOGICAL*1 translates to a char in c
 */
AIF *
LOGICAL1ToAIF(char c)
{
	AIF *   a;
	
	a = NewAIF(0, sizeof(char));
	
	AIF_FORMAT(a) = strdup(AIF_FTN_LOGICAL_TYPE(sizeof(char)));
	
	if ( _char_to_aif(&AIF_DATA(a), c) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}
	
	return a;
}

/*
 * Convert a LOGICAL*2 value to AIF
 * LOGICAL*2 translates to a short in c
 */
AIF *
LOGICAL2ToAIF(short i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(short));

	AIF_FORMAT(a) = strdup(AIF_FTN_LOGICAL_TYPE(sizeof(short)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(short), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

/*
 * Convert a LOGICAL*4 value to AIF
 * LOGICAL*4 translates to a int in c
 */
AIF *
LOGICAL4ToAIF(int i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(int));

	AIF_FORMAT(a) = strdup(AIF_FTN_LOGICAL_TYPE(sizeof(int)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(int), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

#ifdef CC_HAS_LONG_LONG
/*
 * Convert a LOGICAL*8 value to AIF
 * LOGICAL*8 translates to a long long in c
 */
AIF *
LOGICAL8ToAIF(long long i)
{
	AIF *	a;

	a = NewAIF(0, sizeof(long long));

	AIF_FORMAT(a) = strdup(AIF_FTN_LOGICAL_TYPE(sizeof(long long)));

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(long long), (AIFLONGEST)i) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}
#endif /* CC_HAS_LONG_LONG */

/*
 * Convert a COMPLEX value of any length to AIF data format.
 * Result in rd.
 */
int
_complex_to_aif(char **rd, int len, AIFDOUBLEST real, AIFDOUBLEST img)
{
	int	     srclen = len;
	union ieee      v;
	union ieee      i;

	if ( rd == NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	ResetAIFError();

	if( *rd == NULL )
		*rd = (char *)_aif_alloc(len*2);

	if ( len <= (int)sizeof(float) )
	{
		v.f = (float)real;
		i.f = (float)img;
		srclen = sizeof(float);
	} else if ( len == sizeof(double) )
	{
		v.d = (double)real;
		i.d = (double)img;
		srclen = sizeof(double);
	} else
	{
		v.dd = real;
		i.dd = img;
		srclen = sizeof(AIFDOUBLEST);
	}

	// First normalise the real part
	AIFNormalise(*rd, len, v.bytes, srclen);
	// Next normalise the imaginary part
	AIFNormalise(*rd+len, len, i.bytes, srclen);

	return 0;
}

AIF *
COMPLEX4ToAIF(float real, float j)
{
	AIF *   a;
	
	a = NewAIF(0, sizeof(float)*2);
	
	AIF_FORMAT(a) = strdup(AIF_FTN_COMPLEX_TYPE(sizeof(float)));
	
	if ( _complex_to_aif(&AIF_DATA(a), sizeof(float), (AIFDOUBLEST)real, (AIFDOUBLEST)j) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

AIF *
COMPLEX8ToAIF(double real, double j)
{
	AIF *   a;
	
	a = NewAIF(0, sizeof(double)*2);
	
	AIF_FORMAT(a) = strdup(AIF_FTN_COMPLEX_TYPE(sizeof(double)));
	
	if ( _complex_to_aif(&AIF_DATA(a), sizeof(double), (AIFDOUBLEST)real, (AIFDOUBLEST)j) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}

#ifdef HAVE_LONG_DOUBLE
AIF *
COMPLEX16ToAIF(long double real, long double j)
{
	AIF *   a;
	
	a = NewAIF(0, sizeof(long double)*2);
	
	AIF_FORMAT(a) = strdup(AIF_FTN_COMPLEX_TYPE(sizeof(long double)));
	
	if ( _complex_to_aif(&AIF_DATA(a), sizeof(long double), (AIFDOUBLEST)real, (AIFDOUBLEST)j) < 0 )
	{
		AIFFree(a);
		return (AIF *)NULL;
	}

	return a;
}
#endif /* HAVE_LONG_DOUBLE */

/*
 * Convert a CHARACTER to AIF format.
 */
AIF *
CHARACTERToAIF(char *i)
{
	AIF *	a;
	int	length = sizeof(char)*strlen(i);

	ResetAIFError();

	a = NewAIF(0, length);

	AIF_FORMAT(a) = strdup(AIF_FTN_CHARACTER_TYPE(length));

	strncpy(AIF_DATA(a), i, length);
	
	return a;
}

/*
 * Convert an array to AIF.
 *
 * AIF *ArrayToAIF(int rank, int *min, int *max, char *data, int len, char *btype)
 *
 * - rank is the rank of the array
 * - min is the index lower bound, or an array of lower bounds if rank > 1
 * - max is the index upper bound, or an array of upper bounds if rank > 1
 * - data is a pointer to the array data, or NULL, and must be normalised
 * - len is length in bytes of the entire array
 * - btype is the type of the array elements
 */
AIF *
ArrayToAIF(int rank, int *min, int *max, char *data, int len, char *btype)
{
	int	i;
	AIF *	a;
	char *	itype; /* index type */
	char *	rtype; /* range type */
	char *	fds=NULL;

	a = NewAIF(0, len);

	btype = strdup(btype);
	itype = strdup(AIF_INTEGER_TYPE(0, sizeof(int)));

	for ( i = rank - 1  ; i >= 0 ; i-- )
	{
		rtype = strdup(AIF_RANGE_TYPE(min[i], max[i], itype));
		fds = strdup(AIF_ARRAY_TYPE(rtype, btype));

		_aif_free(rtype);
		_aif_free(btype);

		btype = fds;
	}

	assert(fds!=NULL);
	AIF_FORMAT(a) = fds;

	if ( len > 0 && data != NULL )
		memcpy(AIF_DATA(a), data, len);

	_aif_free(itype);

	ResetAIFError();

	return a;
}

/*
 * Create an empty array containing elemenst min..max and
 * with a base type given by btype
 */
AIF *
EmptyArrayToAIF(int min, int max, AIF *btype)
{
	AIF *	a;
	int len = 0;

	if (AIF_FORMAT(btype)[0] != FDS_GENERIC_POINTER) {
		len = (max - min + 1) * AIFTypeSize(btype);
	}

	a = NewAIF(0, len);
	
	AIF_FORMAT(a) = FDSArrayInit(min, max, AIF_FORMAT(btype));
	
	ResetAIFError();
	
	return a;
}
/*
 * Add the element el to the array at the index given by idx.
 * This is used for array construction.
 */
void
AIFAddArrayElement(AIF *a, int idx, AIF *el)
{
	int	min;
	int	max;
	int len = AIFTypeSize(el);

	//if (len == 0 || AIFTypeSize(a) == 0)
		//return;

	if (AIF_FORMAT(el)[0] == FDS_GENERIC_POINTER) {
		AIFAddComplexArrayElement(a, el);
	}
	else {
		min = FDSArrayMinIndex(AIF_FORMAT(a), 0);
		max = FDSArrayMaxIndex(AIF_FORMAT(a), 0);
		
		if (idx < min || idx > max)
			return;
					
		memcpy(AIF_DATA(a) + len * (idx - min), AIF_DATA(el), len);
	}
}

AIF *
EmptyComplexArrayToAIF(int min, int max, AIF *btype)
{
	AIF *	a;
	
	a = NewAIF(0, 0);
	
	AIF_FORMAT(a) = FDSArrayInit(min, max, AIF_FORMAT(btype));
	
	ResetAIFError();
	
	return a;
}

void
AIFAddComplexArrayElement(AIF *a, AIF *el)
{
	char *newData;
	int len = AIFTypeSize(el);
	int cap = AIFTypeSize(a);

	AIF_LEN(a) += len;
	newData = _aif_alloc(AIF_LEN(a));

	memcpy(newData, AIF_DATA(a), cap);
	memcpy(newData+cap, AIF_DATA(el), len);

	if (AIF_DATA(a))
		_aif_free(AIF_DATA(a));

	AIF_DATA(a) = _aif_alloc(AIF_LEN(a));

	memcpy(AIF_DATA(a), newData, AIF_LEN(a));

	_aif_free(newData);
}

/***************************
 * Fortran Array support
 */

/*
 * Create an empty fortran array containing elemenst min..max and
 * with a base type given by btype
 * Fortran arrays index from 1..N
 */
AIF *
EmptyFtnArrayToAIF(int min, int max, AIF *btype)
{
	AIF *	a;
	int len = 0;

	len = (max - min + 1) * AIFTypeSize(btype);

	a = NewAIF(0, len);
	
	AIF_FORMAT(a) = FDSFtnArrayInit(min, max, AIF_FORMAT(btype));
	
	ResetAIFError();
	
	return a;
}

AIF *
FtnArrayToAIF(int rank, int *min, int *max, char *data, int len, char *btype)
{
	int	i;
	AIF *	a;
	char *	itype; /* index type */
	char *	rtype; /* range type */
	char *	fds=NULL;

	a = NewAIF(0, len);

	btype = strdup(btype);
	itype = strdup(AIF_INTEGER_TYPE(0, sizeof(int)));

	for ( i = rank - 1  ; i >= 0 ; i-- )
	{
		rtype = strdup(AIF_RANGE_TYPE(min[i], max[i], itype));
		fds = strdup(AIF_FTN_ARRAY_TYPE(rtype, btype));

		_aif_free(rtype);
		_aif_free(btype);

		btype = fds;
	}

	assert(fds!=NULL);
	AIF_FORMAT(a) = fds;

	if ( len > 0 && data != NULL )
		memcpy(AIF_DATA(a), data, len);

	_aif_free(itype);

	ResetAIFError();

	return a;
}

/*
 * Add the element el to the fortran array at the index given by idx.
 * This is used for fortran array construction.
 */
void
AIFAddFtnArrayElement(AIF *a, int idx, AIF *el)
{
	int	min;
	int	max;
	int len = AIFTypeSize(el);

	//if (len == 0 || AIFTypeSize(a) == 0)
		//return;

	min = FDSFtnArrayMinIndex(AIF_FORMAT(a), 0);
	max = FDSFtnArrayMaxIndex(AIF_FORMAT(a), 0);
		
	if (idx < min || idx > max)
		return;
					
	memcpy(AIF_DATA(a) + len * (idx - min), AIF_DATA(el), len);
}

/*
 * Create an empty AIF enumerated type.
 */
AIF *
EmptyEnumToAIF(char *id) 
{
	AIF *   a;

	a = NewAIF(0, sizeof(int));

	AIF_FORMAT(a) = FDSEnumInit(id);

	ResetAIFError();

	return(a);
}

/*
 * Add a constant to enumerated type.
 */
int
AIFAddConstToEnum(AIF *a, char *id, AIF *content)
{
	char *nfmt;
	int val;

	if ( AIFToInt(content, &val) < 0 )
		return -1;

	nfmt = FDSAddConstToEnum(AIF_FORMAT(a), id, val);

	if ( nfmt == NULL )
	{
		return -1;
	}
	else
	{
		_aif_free(AIF_FORMAT(a));
		AIF_FORMAT(a) = nfmt;
		return 0;
	}
}

/*
 * Set AIF enum type to a particular value.
 */
int
AIFSetEnum(AIF *a, char *id)
{
	int v;

	if ( FDSEnumConstByName(AIF_FORMAT(a), id, &v) < 0 )
	{
		SetAIFError(AIFERR_FIELD, NULL);
		return -1;
	}

	if ( _longest_to_aif(&AIF_DATA(a), sizeof(int), (AIFLONGEST)v) < 0 )
	{
		AIFFree(a);
		return -1;
	}

	return 0;
}

/*
 * Get current value of enum.
 */
AIF *
AIFGetEnum(AIF *a)
{
	AIF  * new;
	char * btype;
	int    len;


	if ( (btype = FDSBaseType(AIF_FORMAT(a))) == NULL )
		return NULL;

	if ( (len = FDSTypeSize(AIF_FORMAT(a))) < 0 )
		return NULL;

	new = NewAIF(0, len);

	AIF_FORMAT(new) = strdup(btype);
	AIF_LEN(new) = len;
	memcpy(AIF_DATA(new), AIF_DATA(a), len);

	return new;

}

/*
 * Create an empty AIF union type.
 */
AIF *
EmptyUnionToAIF(char *id) 
{
	AIF *   a;

	a = NewAIF(0, 0);

	AIF_FORMAT(a) = FDSUnionInit(id);

	ResetAIFError();

	return(a);
}

/*
 * Add a filed to an AIF union.
 */
int
AIFAddFieldToUnion(AIF *a, char *field, char *fmt)
{
	char *nfmt;

	nfmt = FDSAddFieldToUnion(AIF_FORMAT(a), field, fmt);

	if ( nfmt == NULL )
	{
		return -1;
	}
	else
	{
		_aif_free(AIF_FORMAT(a));
		AIF_FORMAT(a) = nfmt;
		return 0;
	}
}

/*
 * Set the value of a union.
 */
int
AIFSetUnion(AIF *a, char *field, AIF *data)
{
	char *type;
	int   len;

	if ( FDSUnionFieldByName(AIF_FORMAT(a), field, &type) < 0 )
	{
		SetAIFError(AIFERR_FIELD, NULL);
		return -1;
	}

	if ( strcmp(type, AIF_FORMAT(data)) != 0 )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	len = FDSTypeSize(AIF_FORMAT(a));
	AIF_LEN(a) = len;

	if ( AIF_DATA(a) != NULL )
		_aif_free(AIF_DATA(a));

	AIF_DATA(a) = _aif_alloc(len);
	
	// if len is greater than the length of data, we need to memset to null first
	// otherwise we will be left with a bunch of junk in the final type.
	if (len > AIF_LEN(data))
	{
		memset(AIF_DATA(a), 0, len);
	}
	
	// We can only memcpy up to the length of the input data. Otherwise we are
	// reading off the end of the buffer!
	memcpy(AIF_DATA(a), AIF_DATA(data), AIF_LEN(data));

	return 0;

}

/*
 * Get the value of a union.
 */
AIF *
AIFGetUnion(AIF *a, char *field)
{
	AIF  * new;
	char * type;
	int    len;

	if ( FDSUnionFieldByName(AIF_FORMAT(a), field, &type) < 0 )
	{
		SetAIFError(AIFERR_FIELD, NULL);
		return NULL;
	}

	if ( (len = FDSTypeSize(type)) < 0 )
		return NULL;

	new = NewAIF(0, len);

	AIF_FORMAT(new) = type;
	AIF_LEN(new) = len;
	memcpy(AIF_DATA(new), AIF_DATA(a), len);

	return new;

}

/*
 * Create an empty AIF structure.
 */
AIF *
EmptyStructToAIF(char *id)
{
	AIF *	a;

	a = NewAIF(0, 0);

	AIF_FORMAT(a) = FDSStructInit(id);

	ResetAIFError();

	return a;
}

/* 
 * Add a field to the public section of a structure
 */
int
AIFAddFieldToStruct(AIF *a, char* field, AIF *content)
{
	int	pubLength;
	int	oldLength;
	int	newLength;
	char *	newFormat;
	char *	newData;
	char *	dummyString;

	if ( !FDSStructFieldByName(AIF_FORMAT(a), field, &dummyString) )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		_aif_free(dummyString);
		return -1;
	}

	pubLength = _data_len_public(AIF_FORMAT(a));
	oldLength = AIF_LEN(a);
	newLength = oldLength + AIF_LEN(content);

	newData = _aif_alloc(newLength);

	memcpy(newData, AIF_DATA(a), pubLength);
	memcpy(newData+pubLength, AIF_DATA(content), AIF_LEN(content));
	memcpy(newData+pubLength+AIF_LEN(content), AIF_DATA(a)+pubLength, oldLength-pubLength);

	if ( AIF_DATA(a) )
		_aif_free(AIF_DATA(a));

	AIF_DATA(a) = _aif_alloc(newLength);
	AIF_LEN(a) = newLength;

	memcpy(AIF_DATA(a), newData, newLength);

	newFormat = FDSAddFieldToStruct(AIF_FORMAT(a), field, AIF_FORMAT(content));

	_aif_free(AIF_FORMAT(a));
	AIF_FORMAT(a) = newFormat;

	_aif_free(newData);

	ResetAIFError();

	return 0;
}

/* 
 * Set the value of a field in a structure
 */
int
AIFSetStruct(AIF *a, char* field, AIF *content)
{
	int	preLength;
	char *	dummyString;
	int	index;

	if ( FDSStructFieldByName(AIF_FORMAT(a), field, &dummyString) )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	index = FDSStructFieldIndex(AIF_FORMAT(a), field);
	preLength = _data_len_index(AIF_FORMAT(a), index);
	preLength -= FDSTypeSize(dummyString);
	_aif_free(dummyString);

	memcpy(AIF_DATA(a)+preLength, AIF_DATA(content), AIF_LEN(content));

	ResetAIFError();

	return(0);
}

/*
 * Get the value of a field in a structure
 */
AIF *
AIFGetStruct(AIF *a, char *field)
{
	AIF  * new;
	char * type;
	int    len;
	int    pre;
	int    index;

	if ( FDSStructFieldByName(AIF_FORMAT(a), field, &type) < 0 )
	{
		SetAIFError(AIFERR_FIELD, NULL);
		return NULL;
	}

	if ( (len = FDSTypeSize(type)) < 0 )
	{
		_aif_free(type);
		return NULL;
	}

	new = NewAIF(0, len);

	index = FDSStructFieldIndex(AIF_FORMAT(a), field);
	pre = _data_len_index(AIF_FORMAT(a), index);
	pre -= len;

	AIF_FORMAT(new) = type;
	AIF_LEN(new) = len;
	memcpy(AIF_DATA(new), AIF_DATA(a)+pre, len);

	return new;
}

/*
 * Convert an AIF value to a native character.
 */
int
_aif_to_char(char *data, char *val)
{
	int		s;

	if ( data == NULL || val == (char *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	ResetAIFError();

	for ( s = 0 ; s < (int)sizeof(char) ; s++ )
		val[s] =  data[s];

	return 0;
}

/*
 * Convert an AIF integer to the longest integer available.
 */
int
_aif_to_longest(char *data, int len, AIFLONGEST *val)
{
	int			s;
	AIFLONGEST		i = 0;

	if ( data == NULL || val == (AIFLONGEST *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	if ( len > (int)sizeof(AIFLONGEST) )
	{
		SetAIFError(AIFERR_CONV, NULL);
		return -1;
	}

	ResetAIFError();

	for ( s = 0 ; s < len ; s++ )
		i = i * 0x100 + (data[s] & 0xff);

	*val = i;

	return 0;
}

/*
 * Convert AIF integer to native integer. If native integer
 * size is too small, return low order bytes.
 */
int
AIFToInt(AIF *a, int *val)
{
	AIFLONGEST	l;

	if ( AIFToLongest(a, &l) < 0 )
		return -1;

	*val = l;

	return 0;
}

/*
 * Convert AIF value to longest integer available. Implicitly converts
 * an floating value to an integer.
 */
int
AIFToLongest(AIF *a, AIFLONGEST *val)
{
	int		res = 0;
	AIFDOUBLEST	d;

	if ( a == (AIF *)NULL || val == (AIFLONGEST *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	switch ( FDSType(AIF_FORMAT(a)) )
	{
	case AIF_INTEGER:
	case AIF_FTN_INTEGER:
		res = _aif_to_longest(AIF_DATA(a), AIF_LEN(a), val);
		break;

	case AIF_FLOATING:
	case AIF_FTN_REAL:
		if ( _aif_to_doublest(AIF_DATA(a), AIF_LEN(a), &d) < 0 )
			return -1;

		*val = (AIFLONGEST)d;
		res = 0;
		break;

	default:
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	return res;
}

/*
 * Convert an AIF floating data type to the largest native
 * floating poing representation available.
 */
int
_aif_to_doublest(char *data, int len, AIFDOUBLEST *val)
{
	union ieee	f;

	if ( data == NULL || val == (AIFDOUBLEST *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	ResetAIFError();

	AIFNormalise(f.bytes, len, data, len);

	if ( len == sizeof(float) )
		*val = (AIFDOUBLEST)f.f;
	else if ( len == sizeof(double) )
		*val = (AIFDOUBLEST)f.d;
	else if ( len == sizeof(AIFDOUBLEST) )
		*val = f.dd;
	else {
		fprintf(stderr, "conversion to float: %d does not match any native precision\n", len);

		*val = 0;
		SetAIFError(AIFERR_CONV, NULL);
		return -1;
	}

	return 0;
}

/*
 * Convert an AIF float to a native single precision floating point value.
 */
int
AIFToFloat(AIF *a, float *val)
{
	int		res;
	AIFDOUBLEST	dd;

	if ( a == (AIF *)NULL || val == (float *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	switch ( FDSType(AIF_FORMAT(a)) )
	{
	case AIF_INTEGER:
	case AIF_FTN_INTEGER:
		res = _aif_int_to_doublest(AIF_DATA(a), AIF_LEN(a), &dd);
		break;

	case AIF_FLOATING:
	case AIF_FTN_REAL:
		res = _aif_to_doublest(AIF_DATA(a), AIF_LEN(a), &dd);
		break;

	default:
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	if ( res < 0 )
		return -1;

	*val = (float)dd; /* possible loss of precision */
	return 0;
}

/*
 * Convert an AIF float to a native double precision floating point value.
 */
int
AIFToDouble(AIF *a, double *val)
{
	AIFDOUBLEST	d;

	if ( a == (AIF *)NULL || val == (double *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	if  ( AIFToDoublest(a, &d) < 0 )
		return -1;

	*val = (double)d; /* possible loss of precision */

	return 0;
}

/*
 * Convert an AIF value to a native floating point value using the
 * largest precision available. Implicitly converts an integer to
 * a float.
 */
int
AIFToDoublest(AIF *a, AIFDOUBLEST *val)
{
	int	res;

	if ( a == (AIF *)NULL || val == (AIFDOUBLEST *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	switch ( FDSType(AIF_FORMAT(a)) )
	{
	case AIF_INTEGER:
	case AIF_FTN_INTEGER:
		res = _aif_int_to_doublest(AIF_DATA(a), AIF_LEN(a), val);
		break;

	case AIF_FLOATING:
	case AIF_FTN_REAL:
		res = _aif_to_doublest(AIF_DATA(a), AIF_LEN(a), val);
		break;

	default:
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	return res;
}

/*
 * Convert an AIF integer to and AIF float using largest precision
 * available.
 */
int
_aif_int_to_doublest(char *data, int len, AIFDOUBLEST *val)
{
	AIFLONGEST	l;

	if ( _aif_to_longest(data, len, &l) < 0 )
		return -1;

	*val = (AIFDOUBLEST)l;

	return 0;
}

/*
 * Convert AIF integer to AIF float. Result is always f8.
 */
int
_aif_int_to_aif_float(char **data, char *d, int l)
{
	AIFDOUBLEST	vd = 0;

	_aif_int_to_doublest(d, l, &vd);

	if ( _doublest_to_aif(data, sizeof(double), vd) < 0 )
		return -1;

	return sizeof(double);
}

/*
 * Convert AIF float to AIF integer. Result is always is4 (for convenience).
 */
int
_aif_float_to_aif_int(char **f, char *d, int l)
{
	int		vi;
	AIFDOUBLEST	vd;

	_aif_to_doublest(d, l, &vd);

	vi = (int)vd;

	if ( _longest_to_aif(f, sizeof(int), vi) < 0 )
		return -1;

	return sizeof(int);
}

/*
 * Convert number to ascii.
 */
char
ToAscii(int n, int base)
{
	if ( base < 10 || n < 10 )
		return (n & 0xf) + '0';

	return (n & 0xf) - 10 + 'a';
}

/*
 * Create a void AIF object.
 */
AIF *
VoidToAIF(char *data, int len)
{
	AIF *	a;

	a = NewAIF(0, len);

	AIF_FORMAT(a) = strdup(AIF_VOID_TYPE(len));

	if ( data != NULL && len > 0 )
		memcpy(AIF_DATA(a), data, len);

	return a;
}

/*
 * Convert an AIF void object to its native representation.
 */
int
AIFToVoid(AIF *a, char *data, int len)
{
	int	size;

	if ( data == NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	ResetAIFError();

	if ( len > 0 )
	{
		size = FDSTypeSize(AIF_FORMAT(a));
		memcpy(data, AIF_DATA(a), size > len ? size : len);
	}

	return 0;
}

/*
 * Coerce an AIF object into type t if possible.
 */
AIF *
AIFCoerce(AIF *a, char *t)
{
	int		tlen;
	int		alen;
	int		atype;
	AIFDOUBLEST	dval;
	char *		rd = NULL;
	AIF *		na;

	ResetAIFError();

	tlen = FDSTypeSize(t);
	alen = AIF_LEN(a);

	switch ( FDSType(t) )
	{
	case AIF_INTEGER:
		switch ( FDSType(AIF_FORMAT(a)) )
		{
		case AIF_INTEGER:
			if ( tlen > alen )
			{
				rd = (char *)_aif_alloc(tlen);
				memcpy(rd, AIF_DATA(a), tlen);
				return MakeAIF(strdup(t), rd);
			}
			else if ( tlen < alen )
			{
				SetAIFError(AIFERR_TYPE, NULL);
				return (AIF *)NULL;
			}

			return CopyAIF(a);

		case AIF_FLOATING:
			if ( _aif_float_to_aif_int(&rd, AIF_DATA(a), alen) < 0 )
				return (AIF *)NULL;

			return MakeAIF(strdup("is4"), rd);

		default:
			SetAIFError(AIFERR_TYPE, NULL);
			return (AIF *)NULL;
		}
		break;

	case AIF_FLOATING:
		switch ( atype = AIFType(a) )
		{
		case AIF_INTEGER:
		case AIF_FLOATING:
			if ( atype == AIF_INTEGER )
			{
				/*
				** Convert integer to double.
				*/
				if ( _aif_int_to_doublest(AIF_DATA(a), AIF_LEN(a), &dval) < 0 )
					return (AIF *)NULL;

				alen = sizeof(AIFDOUBLEST);
			}
			else
			{
				/*
				** Convert AIF float to double
				*/
				if ( _aif_to_doublest(AIF_DATA(a), AIF_LEN(a), &dval) < 0 )
					return (AIF *)NULL;
			}

			na = NewAIF(0, tlen);

			AIF_FORMAT(na) = strdup(AIF_FLOATING_TYPE(tlen));

			/*
			** Now take care of floating precision
			*/

			if ( _doublest_to_aif(&AIF_DATA(na), tlen, dval) < 0 )
				return (AIF *)NULL;

			return na;

		default:
			SetAIFError(AIFERR_TYPE, NULL);
			return (AIF *)NULL;
		}
		break;

	default:
		SetAIFError(AIFERR_TYPE, NULL);
		return (AIF *)NULL;
	}

	return na;
}

/* 
 * build an AIF from two ascii strings. 
 */
AIF *
AsciiToAIF(char *format, char *data)
{
	int	flen = strlen(format);
	int	dlen = strlen(data);
	long	acc;
	AIF *	a;
	char *	bdata;
	char *	bdp;
	char *	dp;
	char	buf[2];

	ResetAIFError();

	if ( dlen % 2 != 0 )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return (AIF *)NULL;
	}

	bdata = (char *) _aif_alloc(dlen/2);
	bdp = bdata;

	buf[1] = 0;

	dp = data;

	/*
	** convert two chars of data to one byte of bdata
	*/
	while ( *dp )
	{
		buf[0] = *dp++;
		acc = strtol(buf, 0, 16) << 4;
		buf[0] = *dp++;
		acc += strtol(buf, 0, 16);
		*bdp++ = (char) (acc & 0xff);
	}

	a = NewAIF(flen + 1, dlen/2);

	memcpy(AIF_DATA(a), bdata, AIF_LEN(a));
	memcpy(AIF_FORMAT(a), format, flen);

	_aif_free(bdata);

	return a;
}

/************************************
 *				  *
 *   AIFToStr()-related functions   *
 *				  *
 ************************************/

void
_str_init()
{
	if ( _str_buf_len == 0 )
	{
		_str_buf = _aif_alloc(BUFSIZ);
		_str_buf_len = BUFSIZ;
	}

	_str_pos = _str_buf;
}

void
_str_cat(char *str)
{
	int	len = strlen(str);

	while ( _str_pos + len + 1 > _str_buf + _str_buf_len )
	{
		int offset = _str_pos - _str_buf;
		_str_buf_len += BUFSIZ;
		_str_buf = _aif_resize(_str_buf, _str_buf_len);
		_str_pos = _str_buf + offset;
	}

	memcpy(_str_pos, str, len);
	_str_pos += len;
	*_str_pos = '\0';
}

void
_str_add(char ch)
{
	while ( _str_pos + 2 > _str_buf + _str_buf_len )
	{
		int offset = _str_pos - _str_buf;
		_str_buf_len += BUFSIZ;
		_str_buf = _aif_resize(_str_buf, _str_buf_len);
		_str_pos = _str_buf + offset;
	}

	*_str_pos++ = ch;
	*_str_pos = '\0';
}

char *
_str_get(void)
{
	return strdup(_str_buf);
}

int
_aif_bool_to_str(int depth, char **fds, char **data)
{
	int		bytes = sizeof(int);
	int		i;
	char		buf[BUFSIZ];
	AIFLONGEST	lli;

	(*fds)++;

	if ( _aif_to_longest(*data, bytes, &lli) < 0 )
		return -1;

	i = (int)lli;

	// it makes more sense to have 0 as false and otherwise as true
	// Minh changes here
	if ( i == -1 )
		snprintf(buf, BUFSIZ-1, "true");
	else if ( i == 0 )
		snprintf(buf, BUFSIZ-1, "false");
	else
		snprintf(buf, BUFSIZ-1, "%d", i);

	_str_cat(buf);
	*data += bytes;

	return 0;
}

int
AIFBoolToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_bool_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
_aif_char_to_str(int depth, char **fds, char **data)
{
	char	c;
	char	buf[BUFSIZ];

	(*fds)++;

	if ( _aif_to_char(*data, &c) < 0 )
		return -1;

	if ( c >= ' ' && c <= '~' )
		snprintf(buf, BUFSIZ-1, "%d '%c'", c, c);
	else
		snprintf(buf, BUFSIZ-1, "%d '\\%03o'", c & 0xff, c & 0xff);

	_str_cat(buf);
	*data += sizeof(char);

	return 0;
}

int
_aif_int_to_str(int depth, char **fds, char **data)
{
	int		bytes = FDSTypeSize(*fds);
	int		isSigned = FDSIsSigned(*fds);
	AIFLONGEST	lli;
	char	buf[BUFSIZ];

	*fds += 2;
	*fds = _fds_skipnum(*fds);

	if ( _aif_to_longest(*data, bytes, &lli) < 0 )
		return -1;

	if ( bytes <= (int)sizeof(char))
	{
		snprintf(buf, BUFSIZ-1, isSigned ? "%hhd" : "%hhu", isSigned ? (char)lli : (unsigned char)lli);
	} else if (bytes == (int)sizeof(short))
	{
		snprintf(buf, BUFSIZ-1, isSigned ? "%hd" : "%hu", isSigned ? (short)lli : (unsigned short)lli);
	} else if (bytes == (int)sizeof(int))
	{
		snprintf(buf, BUFSIZ-1, isSigned ? "%d" : "%u", isSigned ? (int)lli : (unsigned int)lli);
	}
	else if ( bytes == sizeof(long) )
	{
		snprintf(buf, BUFSIZ-1, isSigned ? "%ld" : "%lu" , isSigned ? (long)lli : (unsigned long)lli);
	}
	else
	{
#ifdef CC_HAS_LONG_LONG
	#ifdef WIN32
		snprintf(buf, BUFSIZ-1, isSigned ? "%I64d" : "%I64u" , lli);
	#else /* WIN32 */
		snprintf(buf, BUFSIZ-1, isSigned ? "%lld" : "%llu" , isSigned? (long long)lli : (unsigned long long)lli);
	#endif /* WIN32 */
#else /* CC_HAS_LONG_LONG */
		snprintf(buf, BUFSIZ-1, isSigned ? "%ld" : "%lu" , isSigned ? (long)lli : (unsigned long)lli);
#endif /* CC_HAS_LONG_LONG */
	}

	_str_cat(buf);

	if ( bytes > (int)sizeof(AIFLONGEST) )
	{
		snprintf(buf, BUFSIZ-1, "<truncated from %d bytes>", bytes);
		_str_cat(buf);
	}

	*data += bytes;

	return 0;
}

int
AIFIntToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_int_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
_aif_float_to_str(int depth, char **fds, char **data)
{
	AIFDOUBLEST	d;
	int		bytes = FDSTypeSize(*fds);
	char		buf[BUFSIZ];

	*fds += 1;
	*fds = _fds_skipnum(*fds);

	if ( _aif_to_doublest(*data, bytes, &d) < 0 )
		return -1;

	if ( bytes <= (int)sizeof(float) )
#ifdef HAVE_LONG_DOUBLE
		snprintf(buf, BUFSIZ-1, "%.8Lg", d);
#else /* HAVE_LONG_DOUBLE */
		snprintf(buf, BUFSIZ-1, "%.8g", (float)d);
#endif /* HAVE_LONG_DOUBLE */
	else if ( bytes == (int)sizeof(double) )
#ifdef HAVE_LONG_DOUBLE
		snprintf(buf, BUFSIZ-1, "%.16Lg", d);
	else
		snprintf(buf, BUFSIZ-1, "%.34Lg", d);
#else /* HAVE_LONG_DOUBLE */
		snprintf(buf, BUFSIZ-1, "%.16lg", (double)d);
#endif /* HAVE_LONG_DOUBLE */
	_str_cat(buf);
	*data += bytes;

	return 0;
}

/* 
 * print floats with given precision
 */
int
_aif_float_to_str_precise(int depth, char **fds, char **data, int precision)
{
	AIFDOUBLEST	d;
	int			bytes = FDSTypeSize(*fds);
	char		buf[BUFSIZ];
	char		format[BUFSIZ];

	strcpy(format, "\%.*");

	*fds += 1;
	*fds = _fds_skipnum(*fds);

	if ( _aif_to_doublest(*data, bytes, &d) < 0 )
		return -1;

	if ( bytes <= (int)sizeof(float) ) {
#ifdef HAVE_LONG_DOUBLE
		strcat(format, "Lf");
		snprintf(buf, BUFSIZ-1, format, precision*(-1), d);
#else /* HAVE_LONG_DOUBLE */
		strcat(format, "f");
		snprintf(buf, BUFSIZ-1, format, precision*(-1), (float)d);
#endif /* HAVE_LONG_DOUBLE */
	}
	else if ( bytes == (int)sizeof(double) ) {
#ifdef HAVE_LONG_DOUBLE
		strcat(format, "Lf");
		snprintf(buf, BUFSIZ-1, format, precision*(-1), d);
	}
	else {
		strcat(format, "Lf");
		snprintf(buf, BUFSIZ-1, format, precision*(-1), d);
#else /* HAVE_LONG_DOUBLE */
		strcat(format, "lf");
		snprintf(buf, BUFSIZ-1, format, precision*(-1), (double)d);
#endif /* HAVE_LONG_DOUBLE */
	}

	_str_cat(buf);
	*data += bytes;

	return 0;
}

int
AIFFloatToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_float_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

/*****
 * Base fortran types
 */
 
int
_aif_ftn_integer_to_str(int depth, char **fds, char **data)
{
	int		bytes = FDSTypeSize(*fds);
	AIFLONGEST	lli;
	char	buf[BUFSIZ];

	*fds += 1;
	*fds = _fds_skipnum(*fds);

	if ( _aif_to_longest(*data, bytes, &lli) < 0 )
		return -1;
		
	if ( bytes <= (int)sizeof(char))
	{
		snprintf(buf, BUFSIZ-1, "%hhd", (char)lli);
	} else if (bytes == (int)sizeof(short))
	{
		snprintf(buf, BUFSIZ-1, "%hd", (short)lli);
	} else if (bytes == (int)sizeof(int))
	{
		snprintf(buf, BUFSIZ-1, "%d", (int)lli);
	}
#ifdef CC_HAS_LONG_LONG
	else if (bytes == (int)sizeof(long long))
	{
		snprintf(buf, BUFSIZ-1, "%lld", (long long)lli);
	}
#else
	else
	{
		snprintf(buf, BUFSIZ-1, "%ld", (long)lli);
	}
#endif
	_str_cat(buf);

	if ( bytes > (int)sizeof(AIFLONGEST) )
	{
		snprintf(buf, BUFSIZ-1, "<truncated from %d bytes>", bytes);
		_str_cat(buf);
	}

	*data += bytes;

	return 0;
}

int
AIFFtnIntegerToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_ftn_integer_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
_aif_ftn_real_to_str(int depth, char **fds, char **data)
{
	AIFDOUBLEST	d;
	int		bytes = FDSTypeSize(*fds);
	char		buf[BUFSIZ];

	*fds += 1;
	*fds = _fds_skipnum(*fds);

	if ( _aif_to_doublest(*data, bytes, &d) < 0 )
		return -1;

	if (bytes <= (int)sizeof(float))
	{
		snprintf(buf, BUFSIZ-1, "%.8g", (float)d);
	} else if (bytes == sizeof(double))
	{
		snprintf(buf, BUFSIZ-1, "%.16g", (double)d);
	} else
	{
#ifdef HAVE_LONG_DOUBLE
		snprintf(buf, BUFSIZ-1, "%.32Lg", (long double)d);
#else
		snprintf(buf, BUFSIZ-1, "%.16g", (double)d);
#endif
	}
	
	_str_cat(buf);
	*data += bytes;
	
	if ( bytes > (int)sizeof(AIFDOUBLEST) )
	{
		snprintf(buf, BUFSIZ-1, "<truncated from %d bytes>", bytes);
		_str_cat(buf);
	}
	
	return 0;
}

int
AIFFtnRealToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_ftn_real_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
_aif_ftn_complex_to_str(int depth, char **fds, char **data)
{
	AIFDOUBLEST	real;
	AIFDOUBLEST     img;
	int		total_bytes = FDSTypeSize(*fds);
	int	     bytes = total_bytes / 2;
	char		buf[BUFSIZ];

	*fds += 1;
	*fds = _fds_skipnum(*fds);

	if ( _aif_to_doublest(*data, bytes, &real) < 0 )
		return -1;
		
	if ( _aif_to_doublest(*(data)+bytes, bytes, &img) < 0)
		return -1;

	if (bytes <= (int)sizeof(float))
	{
		snprintf(buf, BUFSIZ-1, "(%.8g, %.8g)", (float)real, (float)img);
	} else if (bytes == (int)sizeof(double))
	{
		snprintf(buf, BUFSIZ-1, "(%.16g,%.16g)", (double)real, (double)img);
	} else
	{
#ifdef HAVE_LONG_DOUBLE
		snprintf(buf, BUFSIZ-1, "(%.32Lg,%.32Lg)", (long double)real, (long double)img);
#else
		snprintf(buf, BUFSIZ-1, "(%.16g,%.16g)", (double)real, (double)img);
#endif
	}
	
	_str_cat(buf);
	// need to increment data past the real and imaginary part
	*data += total_bytes;
	
	if ( bytes > (int)sizeof(AIFDOUBLEST) )
	{
		snprintf(buf, BUFSIZ-1, "<truncated from %d bytes>", bytes);
		_str_cat(buf);
	}
	
	return 0;
}

int
AIFFtnComplexToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_ftn_complex_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
_aif_ftn_logical_to_str(int depth, char **fds, char **data)
{
	int		bytes = FDSTypeSize(*fds);
	char		buf[BUFSIZ];
	AIFLONGEST	lli;

	*fds += 1;
	*fds = _fds_skipnum(*fds);

	if ( _aif_to_longest(*data, bytes, &lli) < 0 )
		return -1;

	if ( lli == 0)
		snprintf(buf, BUFSIZ-1, ".FALSE.");
	else
		snprintf(buf, BUFSIZ-1, ".TRUE.");

	_str_cat(buf);
	*data += bytes;

	return 0;
}

int
AIFFtnLogicalToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_ftn_logical_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
_aif_ftn_character_to_str(int depth, char **fds, char **data)
{
	int	bytes = FDSTypeSize(*fds);
	char *	buf;

	(*fds)++; /* past "C" */
	*fds = _fds_skipnum(*fds); /* past length */

	buf = _aif_alloc(bytes+3);

	*buf = '\'';
	strncpy(&buf[1], *data, bytes);
	*(buf + bytes + 1) = '\'';
	*(buf + bytes + 2) = '\0'; /* terminator */

	_str_cat(buf);

	*data += bytes;

	_aif_free(buf);

	return 0;
}

int
AIFFtnCharacterToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_ftn_character_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

/*
 * A recursive utility function to convert AIF fortran array to string.
 * If it's multi-dimensional, the resultant string is unflattened.
 * This function is called by _aif_array_to_str().
 * XXX: Note that column major versus row major doesn't matter here.
 * Since aif created the flattened array in row major order the user
 * will never actually see the flattened array.
 */
void
_aif_ftn_array_to_str_unflattened(int rank, AIFIndex *ix, int depth, char **data)
{
	int i;
	int nel;
	char *fmt;

	if ( rank == ix->i_rank )
		return;

	nel = ix->i_max[rank] - ix->i_min[rank] + 1;

	_str_cat("(");

	for ( i = 0 ; i < nel ; i++ )
	{
		if ( rank == ix->i_rank - 1 )
		{
			fmt = ix->i_btype;
			_aif_to_str(depth, &fmt, data);
		}
		else
		{
			_aif_ftn_array_to_str_unflattened(rank+1, ix, depth, data);
		}

		if ( i < nel - 1 )
			_str_cat(", ");
	}

	_str_cat(")");
}

int
_aif_ftn_array_to_str(int depth, char **fds, char **data)
{
	AIFIndex *	ix;

	ix = FDSFtnArrayIndexInit(*fds);

	_aif_ftn_array_to_str_unflattened(0, ix, depth, data);

	AIFArrayIndexFree(ix);

	_fds_advance(fds); /* skip over entire array descriptor */

	return 0;
}

/*******
* Base pointer/ref types
*/

int
_aif_address_to_str(int depth, char **fds, char **data)
{
	int		addr_len = FDSTypeSize(*fds);
	int		addr_loc_i;
	AIFLONGEST	addr_loc_lli;
	char		buf[BUFSIZ];

	*fds += 1; /* skip past 'a' */
	*fds = _fds_skipnum(*fds);

	if ( _aif_to_longest(*data, addr_len, &addr_loc_lli) < 0 )
		return -1;

	if ( addr_loc_lli == 0 )
	{
		snprintf(buf, BUFSIZ-1, "0x0");
	}
	else if ( addr_len <= (int)sizeof(int) )
	{
		addr_loc_i = (int)addr_loc_lli;
		snprintf(buf, BUFSIZ-1, "%0#*x", addr_len, addr_loc_i);
	}
	else if ( addr_len == (int)sizeof(long) )
	{
#ifdef CC_HAS_LONG_LONG
		long    addr_loc_li;

		addr_loc_li = (long)addr_loc_lli;
		snprintf(buf, BUFSIZ-1, "%0#*lx", addr_len, addr_loc_li);
#else /* CC_HAS_LONG_LONG */
		snprintf(buf, BUFSIZ-1, "%0#*lx", addr_len, addr_loc_lli);
#endif /* CC_HAS_LONG_LONG */
	}
	else
	{
#ifdef CC_HAS_LONG_LONG
		snprintf(buf, BUFSIZ-1, "%0#*llx", addr_len, addr_loc_lli);
#else /* CC_HAS_LONG_LONG */
		snprintf(buf, BUFSIZ-1, "%0#*lx", sizeof(addr_loc_lli), addr_loc_lli);
#endif /* CC_HAS_LONG_LONG */
	}

	_str_cat(buf);

	if ( addr_len > (int)sizeof(AIFLONGEST) )
	{
		snprintf(buf, BUFSIZ-1, "<address truncated from %d bytes>", addr_len);
		_str_cat(buf);
	}

	*data += addr_len;

	return 0;
}

int
AIFAddressToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_address_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
_aif_pointer_to_str(int depth, char **fds, char **data)
{
	int 	index;
	char 	buf[BUFSIZ];

	(*fds)++; /* past "^" */
	_str_cat("^");

	switch ( (int)**data )
	{
	case AIF_PTR_NIL: /* nil value */
		if ( depth == 1 )
			depth = -1;
		else if ( depth > 1 )
			depth--;

		(*data)++;
		_str_cat("null");
		_fds_advance(fds);
		break;

	case AIF_PTR_NORMAL: /* normal value */
		(*data)++;

		if ( depth == -1 )
		{
			_str_cat("<normal>");
			_fds_skip_data(fds, data);
			break;
		}
		else if ( depth == 1 )
			depth = -1;
		else if ( depth > 1 )
			depth--;
		
		_aif_to_str(depth, fds, data);
		break;

	case AIF_PTR_NAME: /* normal value, but remember it */
		(*data)++;
		_ptrname_to_int(*data, &index);
		*data += 4;
		_aif_values_seen[index] = *data;
		snprintf(buf, BUFSIZ-1, "<%d:>", index);
		_str_cat(buf);

		if ( depth == -1 )
		{
			_fds_skip_data(fds, data);
			break;
		}
		else if ( depth == 1 )
			depth = -1;
		else if ( depth > 1 )
			depth--;

		_aif_to_str(depth, fds, data);
		break;

	case AIF_PTR_REFERENCE: /* reference to remembered value */
		if ( depth == 1 )
		{
			char * target;

			depth = -1;
			target = _find_target(data, 0);
			_aif_to_str(depth, fds, &target);
		}
		else if ( depth > 1 )
		{
			char * target;

			depth--;
			target = _find_target(data, 0);
			_aif_to_str(depth, fds, &target);
		}
		else
		{
			(*data)++;
			_ptrname_to_int(*data, &index);
			*data += 4;
			snprintf(buf, BUFSIZ-1, "<=%d>", index);
			_str_cat(buf);
			_fds_advance(fds);
		}

		break;

	case AIF_PTR_INVALID: /* pointer is invalid */
		(*data)++;
		_str_cat("<invalid>");
		_fds_advance(fds);
		break;
	}

	return 0;
}

int
AIFPointerToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_pointer_to_str(depth, &fmt, &data) )
		return -1;
	
	*str = _str_get();

	return 0;
}

int
_aif_generic_pointer_to_str(int depth, char **fds, char **data)
{
	char *	type_str;
	int     type_str_len;

	(*fds)++; /* past "p" */
	_str_cat("(");
	
	// get the length of the type string
	type_str_len = _fds_getnum(*fds);
	*fds = _fds_skipnum(*fds); /* past length */
	
	// get the type string
	type_str = _aif_alloc(type_str_len);
	strncpy(type_str, *data, type_str_len);
	
	// cat the type string to the global str buffer
	_str_cat(type_str);
	
	_str_cat(") ");
	
	// free the type string
	_aif_free(type_str);
	
	// point the data past the type string
	*data += type_str_len;
	
	// print the address string
	_aif_to_str(depth, fds, data);

	return 0;
}

int
AIFGenericPointerToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_generic_pointer_to_str(depth, &fmt, &data) )
		return -1;
	
	*str = _str_get();

	return 0;
}

int
_aif_region_to_str(int depth, char **fds, char **data)
{
	int		i;
	int		rank;
	int		size;
	AIFLONGEST	min;
	AIFLONGEST	max;
	char		buf[BUFSIZ];

	rank = _fds_getnum(*fds + 1);
	size = FDSTypeSize(FDSBaseType(*fds));

	_str_cat("[");

	for ( i = 0 ; i < rank ; i++ )
	{
		if ( i != 0 && rank > 1 )
			_str_cat(", ");

		if
		(
			_aif_to_longest(*data + 2 * i * size, size, &min) < 0
			||
			_aif_to_longest(*data + (2 * i + 1) * size, size, &max) < 0
		)
			return -1;

#ifdef CC_HAS_LONG_LONG
		snprintf(buf, BUFSIZ-1, "%lld..%lld", min, max);
#else /* CC_HAS_LONG_LONG */
		snprintf(buf, BUFSIZ-1, "%ld..%ld", min, max);
#endif /* CC_HAS_LONG_LONG */

		_str_cat(buf);
	}

	_str_cat("]");

	_fds_advance(fds);

	return 0;
}

/*
 * A recursive utility function to convert AIF array to string.
 * If it's multi-dimensional, the resultant string is unflattened.
 * This function is called by _aif_array_to_str().
 */
void
_aif_array_to_str_unflattened(int rank, AIFIndex *ix, int depth, char **data)
{
	int i;
	int nel;
	char *fmt;

	if ( rank == ix->i_rank )
		return;

	nel = ix->i_max[rank] - ix->i_min[rank] + 1;

	_str_cat("[");

	for ( i = 0 ; i < nel ; i++ )
	{
		if ( rank == ix->i_rank - 1 )
		{
			fmt = ix->i_btype;
			_aif_to_str(depth, &fmt, data);
		}
		else
		{
			_aif_array_to_str_unflattened(rank+1, ix, depth, data);
		}

		if ( i < nel - 1 )
			_str_cat(",");
	}

	_str_cat("]");
}

/* 
 * Print array of floats with a given precision
 * only used for float
 */
void
_aif_array_to_str_unflattened_precise(int rank, AIFIndex *ix, int depth, char **data, int precision)
{
	int i;
	int nel;
	char *fmt;

	if ( rank == ix->i_rank )
		return;

	nel = ix->i_max[rank] - ix->i_min[rank] + 1;

	_str_cat("[");

	for ( i = 0 ; i < nel ; i++ )
	{
		if ( rank == ix->i_rank - 1 )
		{
			fmt = ix->i_btype;
			_aif_float_to_str_precise(depth, &fmt, data, precision);
		}
		else
		{
			_aif_array_to_str_unflattened_precise(rank+1, ix, depth, data, precision);
		}

		if ( i < nel - 1 )
			_str_cat(",");
	}

	_str_cat("]");
}

int
_aif_array_to_str_precise(int depth, char **fds, char **data, int precision)
{
	AIFIndex *	ix;

	ix = FDSArrayIndexInit(*fds);

	_aif_array_to_str_unflattened_precise(0, ix, depth, data, precision);

	AIFArrayIndexFree(ix);

	_fds_advance(fds); /* skip over entire array descriptor */

	return 0;
}

int
_aif_array_to_str(int depth, char **fds, char **data)
{
	AIFIndex *	ix;

	ix = FDSArrayIndexInit(*fds);

/*	Uncomment this to convert the array flattened:

	_str_cat("[");

	for ( i = 0 ; i < ix->i_nel ; i++ )
	{
		fmt = ix->i_btype;

		_aif_to_str(depth, &fmt, data);

		if ( i < ix->i_nel - 1 )
			_str_cat(", ");

	}

	_str_cat("]");
*/

	_aif_array_to_str_unflattened(0, ix, depth, data);

	AIFArrayIndexFree(ix);

	_fds_advance(fds); /* skip over entire array descriptor */

	return 0;
}

int
AIFArrayToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	if ( AIFType(a) != AIF_ARRAY )
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_array_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
AIFArrayIndexToStr(char **str, AIFIndex *ix)
{
	int	i;
	char	buf[BUFSIZ];

	_str_init();

	for ( i = 0 ; i < ix->i_rank ; i++ )
	{
		snprintf(buf, BUFSIZ-1, "[%d]", ix->i_index[i]);
		_str_cat(buf);
	}

	*str = _str_get();

	return 0;
}

int
_aif_struct_to_str(int depth, char **fds, char **data)
{
	char *	inFds;	
	char	buf[BUFSIZ];
	int	isStruct = 1;	/* assume it is a struct */
				/* to avoid printing the semicolons if
				   it is a struct */

	_str_cat("{");

	(*fds)++; /* past open brace */

	/*
	** past nnn=
	*/

	_fds_skipid(fds);

	while ( **fds != FDS_STRUCT_END )
	{
		if ( isStruct 	&& (*fds)[0] == FDS_STRUCT_ACCESS_SEP
				&& (*fds)[1] == FDS_STRUCT_ACCESS_SEP
				&& (*fds)[2] == FDS_STRUCT_ACCESS_SEP )
		{
			(*fds) += 3;
			break;
		}
		else if ( **fds == FDS_STRUCT_ACCESS_SEP )
		{
			isStruct = 0;	/* it is a class */
			
			(*fds)++;
			_str_cat("; ");
			continue;
		}

		if ( (inFds = strchr(*fds, FDS_STRUCT_FIELD_NAME_END)) == NULL )
			break;

		*inFds = 0; /* temporarily */

		snprintf(buf, BUFSIZ-1, "%s = ", *fds);
		_str_cat(buf);

		*inFds++ = FDS_STRUCT_FIELD_NAME_END;

		*fds = inFds; /* to start of field */

		_aif_to_str(depth, fds, data);

		if ( **fds == FDS_STRUCT_FIELD_SEP )
		{
			(*fds)++;
			_str_cat(", ");
		}
	}

	_str_cat("}");

	(*fds)++; /* past close brace */

	return 0;
}

int
AIFStructToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_struct_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
_aif_reference_to_str(int depth, char **fds, char **data)
{
	int	index;
	char *	effectiveFds;

	(*fds)++; /* past ">" */
	index = (int)strtol(*fds, NULL, 10);
	*fds = _fds_skipnum(*fds); /* past name */
	(*fds)++; /* past "/" */
	effectiveFds = _aif_types_seen[index];

	_aif_to_str(depth, &effectiveFds, data);

	return 0;
}

int
AIFReferenceToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_reference_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
_aif_string_to_str(int depth, char **fds, char **data)
{
	int	bytes = FDSTypeSize(*fds);
	char *	buf;

	(*fds)++; /* past "s" */
	*fds = _fds_skipnum(*fds); /* past length */

	buf = _aif_alloc(bytes+3);

	*buf = '"';
	strncpy(&buf[1], *data, bytes);
	*(buf + bytes + 1) = '"';
	*(buf + bytes + 2) = '\0'; /* terminator */

	_str_cat(buf);

	*data += bytes;

	_aif_free(buf);

	return 0;
}

int
_aif_void_to_str(int depth, char **fds, char **data)
{
	int bytes = FDSTypeSize(*fds);

	*fds += 1; /* past "v" */
	*fds = _fds_skipnum(*fds);

	*data += bytes;

	_str_cat("void");
	return 0;
}

int
AIFVoidToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_void_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
_aif_union_to_str(int depth, char **fds, char **data)
{	
	char *	inFds;	
	char *	temp;
	char *	dt;
	int 	size;
	int	union_size;
	char	buf[BUFSIZ];

	_str_cat("{");

	union_size = FDSTypeSize(*fds);

	(*fds)++; /* past open brace */

	/*
	** past nnn=
	*/

	_fds_skipid(fds);

	while ( **fds != FDS_UNION_END )
	{
		if ( (inFds = strchr(*fds, FDS_UNION_FIELD_NAME_END)) == NULL )
			break;

		*inFds = 0; /* temporarily */

		snprintf(buf, BUFSIZ-1, "%s = ", *fds);
		_str_cat(buf);

		*inFds++ = FDS_UNION_FIELD_NAME_END;

		*fds = inFds; /* to start of field */

		size = FDSTypeSize(*fds);
		temp = _aif_alloc( sizeof(char) * size );
		dt = temp;
		memcpy(dt, *data, size);

		_aif_to_str(depth, fds, &dt);
		_aif_free(temp);

		if ( **fds == FDS_UNION_FIELD_SEP )
		{
			(*fds)++;
			_str_cat(", ");
		}
	}

	_str_cat("}");
	*data += union_size;

	(*fds)++; /* past close brace */

	return 0;
}

int
AIFUnionToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_union_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

char *
FDSToStr(char *fds)
{
	char *	  fds_ptr = fds;
	char *	  type;
	char *	  sign;
	char *	  base_type;
	int	     type_len;
	int	     index_len;
	char		buf[BUFSIZ];

	if
	(
		fds == NULL 
		|| 
		*fds == '\0'
	)
		return strdup("<unknown>");

	switch (*fds_ptr++)
	{
	case FDS_BOOLEAN: /* boolean */
		type = strdup("bool");
		break;

	case FDS_CHARACTER: /* character */
		type = strdup("char");
		break;

	case FDS_INTEGER: /* integer */
		// save the sign value
		sign = fds_ptr++;
		// get the length
		type_len = _fds_getnum(fds_ptr);
		// work out the type
		if ((int)sizeof(short) >= type_len)
		{
			snprintf(buf, BUFSIZ, "%sshort int", (*sign == FDS_INTEGER_SIGNED) ? "signed " : "");
			type = strdup(buf);
		} else if ((int)sizeof(int) >= type_len)
		{
			snprintf(buf, BUFSIZ, "%sint", (*sign == FDS_INTEGER_SIGNED) ? "signed " : "");
			type = strdup(buf);
		} else if ((int)sizeof(long) >= type_len)
		{
			snprintf(buf, BUFSIZ, "%slong int", (*sign == FDS_INTEGER_SIGNED) ? "signed " : "");
			type = strdup(buf);
#ifdef CC_HAS_LONG_LONG
		} else if ((int)sizeof(long long) >= type_len)
		{
			snprintf(buf, BUFSIZ, "%slong long int", (*sign == FDS_INTEGER_SIGNED) ? "signed " : "");
			type = strdup(buf);
#endif
		} else
		{
			snprintf(buf, BUFSIZ, "%sint <%d bytes>", (*sign == FDS_INTEGER_SIGNED) ? "signed " : "", type_len);
			type = strdup(buf);
		}
		break;

	case FDS_FLOATING: /* floating */
		// get the length
		type_len = _fds_getnum(fds_ptr);
		// work out the type
		if ((int)sizeof(float) >= type_len)
		{
			type = strdup("float");
		} else if ((int)sizeof(double) >= type_len)
		{
			type = strdup("double");
#ifdef HAVE_LONG_DOUBLE
		} else if ((int)sizeof(long double) >= type_len)
		{
			type = strdup("long double");
#endif
		} else
		{
			snprintf(buf, BUFSIZ, "double <%d bytes>", type_len);
			type = strdup(buf);
		}
		break;

	case FDS_POINTER: /* pointer */
		base_type = FDSToStr(fds_ptr);
		snprintf(buf, BUFSIZ, "%s *", base_type);
		type = strdup(buf);
		_aif_free(base_type);
		break;

	case FDS_ARRAY_START: /* array */
		index_len = FDSArraySize(--fds_ptr);
		base_type = FDSToStr(fds_ptr);
		snprintf(buf, BUFSIZ, "%s [%d]", base_type, index_len);
		type = strdup(buf);
		_aif_free(base_type);
		break;

	case FDS_STRUCT_START:  /* struct */
		base_type = FDSGetIdentifier(--fds_ptr);
		if (base_type == NULL)
		{
			type = strdup("struct <unknown>");
			break;
		}
		snprintf(buf, BUFSIZ, "struct %s", base_type);
		type = strdup(buf);
		_aif_free(base_type);
		break;

	case FDS_UNION_START:  /* union */
		base_type = FDSGetIdentifier(--fds_ptr);
		if (base_type == NULL)
		{
			type = strdup("union <unknown>");
			break;
		}
		snprintf(buf, BUFSIZ, "union %s", base_type);
		type = strdup(buf);
		_aif_free(base_type);
		break;

	case FDS_ENUM_START: /* enumeration */
		base_type = FDSGetIdentifier(--fds_ptr);
		if (base_type == NULL)
		{
			type = strdup("enum <unknown>");
			break;
		}
		snprintf(buf, BUFSIZ, "enum %s", base_type);
		type = strdup(buf);
		_aif_free(base_type);
		break;

	case FDS_VOID: /* void */
		type = strdup("void");
		break;
		
	case FDS_FTN_ARRAY_START: /* fortran array */
		index_len = FDSArraySize(--fds_ptr);
		base_type = FDSToStr(fds_ptr);
		snprintf(buf, BUFSIZ, "%s(%d)", base_type, index_len);
		type = strdup(buf);
		_aif_free(base_type);
		break;
		
	case FDS_FTN_INTEGER: /* fortran INTEGER */
		// get the length
		type_len = _fds_getnum(fds_ptr);
		// with fortran, we simply have the base type followed by length
		snprintf(buf, BUFSIZ, "INTEGER*%d", type_len);
		type = strdup(buf);
		break;
	
	case FDS_FTN_REAL: /* fortran REAL */
		// get the length
		type_len = _fds_getnum(fds_ptr);
		// with fortran, we simply have the base type followed by length
		snprintf(buf, BUFSIZ, "REAL*%d", type_len);
		type = strdup(buf);
		break;
	
	case FDS_FTN_COMPLEX: /* fortran COMPLEX */
		// get the length
		type_len = _fds_getnum(fds_ptr);
		// with fortran, we simply have the base type followed by length
		snprintf(buf, BUFSIZ, "COMPLEX*%d", type_len);
		type = strdup(buf);
		break;
	
	case FDS_FTN_LOGICAL: /* fortran LOGICAL */
		// get the length
		type_len = _fds_getnum(fds_ptr);
		// with fortran, we simply have the base type followed by length
		snprintf(buf, BUFSIZ, "LOGICAL*%d", type_len);
		type = strdup(buf);
		break;
	
	case FDS_FTN_CHARACTER: /* fortran CHARACTER */
		// get the length
		type_len = _fds_getnum(fds_ptr);
		// with fortran, we simply have the base type followed by length
		snprintf(buf, BUFSIZ, "CHARACTER*%d", type_len);
		type = strdup(buf);
		break;

	default:
		type = strdup("<unknown>");
	}

	return type;
}

int
_aif_function_to_str(int depth, char **fds, char **data)
{	
	char *	func_name;
	int     func_name_len;
	char *  addr_fds;
	int     addr_len;
	char *  addr_data;
	char *  args_start;
	char *  arg;
	char *  rtn_type;
	char	end[2] = {FDS_FUNCTION_ARG_END, 0};

	(*fds)++; /* past '&' */
	
	// first item in the fds string is the start of the args
	args_start = *fds;
	// advance to '/'
	*fds = _fds_skipto(*fds, end);
	(*fds)++; /* past '/' */
	
	// we start by advancing the data past the stuff we expect to be defined
	// by the functions base type (i.e. address and name string)
	
	// grab a copy of the function name in the data
	func_name = strdup(*data);
	// get the stored length of the name string
	func_name_len = _fds_getnum(*fds);
	// advance data by the length recorded in the fds
	*data += func_name_len;
	// skip past the string length number
	*fds = _fds_skipnum(*fds);
	// save the return type string
	rtn_type = FDSToStr(*fds);
	// skip past the return type
	_fds_advance(fds);
	// save the addr_fds for later
	addr_fds = *fds;
	// get the size of the address
	addr_len = _fds_count_bytes(fds);
	// point addr_data at the current address of data
	addr_data = *data;
	// advance data by the size of the address
	*data += addr_len;
	
	// we start by printing out the function prototype
	_str_cat("{");
	// print out the return type first
	_str_cat(rtn_type);
	_aif_free(rtn_type);
	// then start of the args
	_str_cat(" (*)(");
	// iterate over the args
	while (*args_start != FDS_FUNCTION_ARG_END)
	{
		// get the string representation of the FDS type
		arg = FDSToStr(args_start);
		// put it in the buffer and free it
		_str_cat(arg);
		_aif_free(arg);
		// advance the fds
		_fds_advance(&args_start);
		// check if we have found a FDS_FUNCTION_ARG_SEP char
		if (*args_start == FDS_FUNCTION_ARG_SEP)
		{
			++args_start;
			_str_cat(",");
		}
	}
	// print out the end of the args
	_str_cat(")");
	// followed by end of prototype
	_str_cat("} ");
	
	// print out the address
	_aif_to_str(depth, &addr_fds, &addr_data);
	
	// finally print out the function name
	_str_cat(" <");
	_str_cat(func_name);
	_str_cat(">");
	
	if (func_name != NULL)
		free(func_name);

	return 0;
}

int
AIFFunctionToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_function_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
_aif_enum_to_str(int depth, char **fds, char **data)
{	
	int		bytes = FDSTypeSize(*fds);
	AIFLONGEST	lli;
	int		i;
	char *		string = NULL;

	/* check whether it is an empty enum or not */
	char * 		f = *fds;

	while ( *f != FDS_ID )
		f++;

	f++;
	if ( *f == FDS_ENUM_END )
	{	/* it is an empty enum */
		_str_cat("0");	
		*data += bytes;
		_fds_advance(fds);
		return 0;
	}

	if ( _aif_to_longest(*data, bytes, &lli) < 0 )
		return -1;

	i = (int) lli;

	if ( FDSEnumConstByValue(*fds, &string, i) < 0)
		return -1;
	
	_str_cat(string);
	_aif_free(string);

	*data += bytes;
	_fds_advance(fds);

	return 0;
}

int
AIFEnumToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_enum_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

int
_aif_name_to_str(int depth, char **fds, char **data)
{	
	int	index;

	(*fds)++; /* past "%" */
	index = (int)strtol(*fds, NULL, 10);
	*fds = _fds_skipnum(*fds); /* past name */
	(*fds)++; /* past "/" */
	_aif_types_seen[index] = *fds;

	return _aif_to_str(depth, fds, data);
}

/* 
 * Prints the object in human-readable form to fp, advances fds and data to
 * the first location after the part printed.  Arithmetic values are given at
 * least len bytes of significance, more if the fds indicates.
 */
int
_aif_to_str(int depth, char **fds, char **data)
{
	char * tmp;

	_fds_resolve(fds);

	switch ( FDSType(*fds) )
	{
	case AIF_BOOLEAN:
		return _aif_bool_to_str(depth, fds, data); 

	case AIF_CHARACTER:
		return _aif_char_to_str(depth, fds, data);

	case AIF_INTEGER:
		return _aif_int_to_str(depth, fds, data);

	case AIF_FLOATING:
		return _aif_float_to_str(depth, fds, data);

	case AIF_ARRAY:
		return _aif_array_to_str(depth, fds, data);

	case AIF_POINTER:
		return _aif_pointer_to_str(depth, fds, data);
		
	case AIF_GENERIC_POINTER:
		return _aif_generic_pointer_to_str(depth, fds, data);
		
	case AIF_ADDRESS:
		return _aif_address_to_str(depth, fds, data);

	case AIF_REGION:
		return _aif_region_to_str(depth, fds, data);

	case AIF_STRUCT:
		return _aif_struct_to_str(depth, fds, data);

	case AIF_NAME:
		return _aif_name_to_str(depth, fds, data);

	case AIF_REFERENCE:
		tmp = _fds_lookup(fds);
		return _aif_to_str(depth, &tmp, data);

	case AIF_STRING:
		return _aif_string_to_str(depth, fds, data);

	case AIF_ENUM:
		return _aif_enum_to_str(depth, fds, data);

	case AIF_UNION:
		return _aif_union_to_str(depth, fds, data);

	case AIF_VOID:
		return _aif_void_to_str(depth, fds, data);

	case AIF_FUNCTION:
		return _aif_function_to_str(depth, fds, data);

	case AIF_HASHED_SIGNATURE:
		return _aif_int_to_str(depth, fds, data);
		
	case AIF_FTN_INTEGER:
		return _aif_ftn_integer_to_str(depth, fds, data);
	
	case AIF_FTN_REAL:
		return _aif_ftn_real_to_str(depth, fds, data);
	
	case AIF_FTN_COMPLEX:
		return _aif_ftn_complex_to_str(depth, fds, data);
	
	case AIF_FTN_LOGICAL:
		return _aif_ftn_logical_to_str(depth, fds, data);
	
	case AIF_FTN_CHARACTER:
		return _aif_ftn_character_to_str(depth, fds, data);
	
	case AIF_FTN_ARRAY:
		return _aif_ftn_array_to_str(depth, fds, data);
	
	default:
		SetAIFError(AIFERR_TYPE, NULL);
		break;
	}

	return -1;
}

int
AIFToStr(char **str, int depth, AIF *a)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	if ( _aif_to_str(depth, &fmt, &data) < 0 )
		return -1;

	*str = _str_get();

	return 0;
}

// convert AIF with float using precision
int
AIFToStrPrecision(char **str, int depth, AIF *a, int precision)
{
	char *	fmt;
	char *	data;

	if ( a == (AIF *)NULL )
	{
		SetAIFError(AIFERR_BADARG, NULL);
		return -1;
	}

	fmt = AIF_FORMAT(a);
	data = AIF_DATA(a);

	_str_init();

	switch ( FDSType(fmt) )
	{
	case AIF_FLOATING:
		if ( _aif_float_to_str_precise(depth, &fmt, &data, precision) < 0 )
			return -1;
		break;
	case AIF_ARRAY:
		if ( _aif_array_to_str_precise(depth, &fmt, &data, precision) < 0 )
			return -1;
		break;
	default:
		if ( _aif_to_str(depth, &fmt, &data) < 0 )
			return -1;
	}
	*str = _str_get();

	return 0;
}

/* Convert a pointer name into an int (4 bytes) */
void
_ptrname_to_int(char *name, int *number)
{
	int	s;
	int 	i = 0;

	for ( s = 0 ; s < 4 ; s++ )
		i = i * 0x100 + (name[s] & 0xff);

	*number = i;
}

/* Convert an int (4 bytes) into a string for the pointer name */
void
_int_to_ptrname(int number, char *name)
{
	union ieee	f;
	f.i = number;

	_aif_normalise(name, 4, f.bytes, 4);
}

