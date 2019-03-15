/*
 * Master header file for the AIF library.
 *
 * Copyright (c) 1996-2002 by Guardsoft Pty Ltd.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 */

#ifndef _AIF_H
#define _AIF_H

#ifdef __cplusplus
extern "C" {
#endif

//#include <gsl/gsl_histogram.h>

#ifdef WINNT
#define WINDOWSNT
#endif /* WINNT */

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__)
#else /* DEBUG */
#define DPRINT(...) /**/
#endif /*DEBUG */

/*
 * Include for MIN & MAX
 */
#if defined(sun)
#include <sys/sysmacros.h>
#elif defined(__sgi) || defined(__linux)
#include <sys/param.h>
#else
#ifndef MAX
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#endif /* MAX */

#ifndef MIN
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#endif /* MIN */
#endif

#ifndef AIFLONGEST
#ifdef CC_HAS_LONG_LONG
typedef long long int AIFLONGEST;
#else
typedef long int AIFLONGEST;
#endif /* CC_HAS_LONG_LONG */
#endif /* !LONGEST */

#ifdef HAVE_LONG_DOUBLE
typedef long double AIFDOUBLEST;
#else
typedef double AIFDOUBLEST;
#endif /* HAVE_LONG_DOUBLE */

#ifdef WIN32
#define R_OK				4
#define W_OK				2
#define X_OK				1
#define F_OK				0
#define CLOSE_SOCKET(s)	closesocket(s)
#define snprintf			_snprintf
#define alloca				_alloca
#define MAXHOSTNAMELEN	255
#define MAXPATHLEN		1024
#else /* WIN32 */
#define SOCKET				int
#define CLOSE_SOCKET(s)	(void)close(s)
#define INVALID_SOCKET	-1
#define SOCKET_ERROR		-1
#endif /* WIN32 */

/* AIF defined types */
#define AIF_INVALID				0
#define AIF_INTEGER				1
#define AIF_FLOATING				2
#define AIF_POINTER				3
#define AIF_ARRAY					4
#define AIF_STRUCT				5
#define AIF_UNION					6
#define AIF_FUNCTION				7
#define AIF_VOID					8
#define AIF_REGION				9
#define AIF_NAME					10
#define AIF_REFERENCE				11
#define AIF_STRING				12
#define AIF_CHARACTER				13
#define AIF_RANGE					14
#define AIF_ENUM					15
#define AIF_BOOLEAN				16
#define AIF_ADDRESS				17
#define AIF_GENERIC_POINTER		18
// guard extensions
#define AIF_HASHED_SIGNATURE		19
// fortran extensions
#define AIF_FTN_INTEGER	 		20
#define AIF_FTN_REAL				21
#define AIF_FTN_COMPLEX	 		22
#define AIF_FTN_LOGICAL	 		23
#define AIF_FTN_CHARACTER		24
#define AIF_FTN_ARRAY				25

/* Total AIF types */
#define NUM_AIF_TYPES				26

#define AIF_PTR_NIL				0
#define AIF_PTR_NORMAL			1
#define AIF_PTR_NAME				2
#define AIF_PTR_REFERENCE		3
#define AIF_PTR_INVALID			4

/*
 * Structure used to index into a 
 * muli-dimensional array.
 */
struct AIFIndex
{
	int		i_finished;
	int		i_rank;
	int		i_nel;
	int *	i_min;
	int *	i_max;
	int *	i_index;
	char *	i_btype;
	int		i_bsize;
};
typedef struct AIFIndex	AIFIndex;

/*
 * AIF Object
 */
struct AIF
{
	char *	a_fds;

	struct
	{
		unsigned	a_data_len;
		char *		a_data_val;
	} a_data;
};
typedef struct AIF AIF;

#define AIF_FORMAT(a)	(a)->a_fds
#define AIF_LEN(a)	(a)->a_data.a_data_len
#define AIF_DATA(a)	(a)->a_data.a_data_val

/*
 * Arithmetic/logical operations that can be 
 * performed on AIF objects.
 */
enum aifop
{
	AIFOP_ADD,
	AIFOP_SUB,
	AIFOP_MUL,
	AIFOP_DIV,
	AIFOP_REM,
	AIFOP_AND,
	AIFOP_OR,
	AIFOP_NEG,
	AIFOP_NOT,
	AIFOP_ABS
};
typedef enum aifop	aifop;

enum aifcmpop
{
	AIFOP_EQ,
	AIFOP_NEQ,
	AIFOP_LEQ,
	AIFOP_GEQ,
	AIFOP_LT,
	AIFOP_GT
};
typedef enum aifcmpop	aifcmpop;

/*
 * AIF Object Access Specifiers
 */
enum aifaccess
{
	AIFACC_PRIVATE,
	AIFACC_PROTECTED,
	AIFACC_PUBLIC
};
typedef enum aifaccess	aifaccess;

/*
 * AIF error codes.
 */
enum aiferr
{
	AIFERR_NOERR,
	AIFERR_SIZE,
	AIFERR_CONV,
	AIFERR_TYPE,
	AIFERR_BADARG,
	AIFERR_NOTIMP,
	AIFERR_OPEN,
	AIFERR_READ,
	AIFERR_WRITE,
	AIFERR_SEEK,
	AIFERR_BADHDR,
	AIFERR_MODE,
	AIFERR_FDS,
	AIFERR_FIELD,
	AIFERR_ARITH,
	AIFERR_BASE,
	AIFERR_INDEX,
	AIFERR_STRING,
	AIFERR_GEN,
	AIFERR_CANTHAPPEN
};
typedef enum aiferr	aiferr;
extern aiferr		AIFErrno;

/*
 * AIF option codes.
 */
enum aifopt
{
	AIFOPT_CMP_DEPTH,
	AIFOPT_CMP_METHOD
};
typedef enum aifopt	aifopt;

#define AIF_CMP_BY_POSITION	0
#define AIF_CMP_BY_NAME		1

/*
 * Definitons for reading/writing AIF
 * data to a file.
 */

#define AIFMODE_READ		0x001
#define AIFMODE_CREATE	0x002
#define AIFMODE_APPEND	0x004

#include	<stdio.h>

struct AIFFILE
{
	FILE *		af_fp;
	int		af_mode;
	int		af_cnt;
};
typedef struct AIFFILE	AIFFILE;

#include	"fds.h"

#define AIF_ARRAY_TYPE(range, base)		TypeToFDS(AIF_ARRAY, (range), (base))
#define AIF_BOOLEAN_TYPE()				TypeToFDS(AIF_BOOLEAN)
#define AIF_CHARACTER_TYPE()				TypeToFDS(AIF_CHARACTER)
#define AIF_ENUM_TYPE(sign)				TypeToFDS(AIF_ENUM, (sign))
#define AIF_FLOATING_TYPE(len)			TypeToFDS(AIF_FLOATING, (len))
#define AIF_FUNCTION_TYPE(args_fmt, name_len, rtn_fmt, addr_fmt) \
			TypeToFDS(AIF_FUNCTION, (args_fmt), (name_len), (rtn_fmt), (addr_fmt))
#define AIF_INTEGER_TYPE(sign, len)		TypeToFDS(AIF_INTEGER, (sign), (len))
#define AIF_NAME_TYPE(name, base)		TypeToFDS(AIF_NAME, (name), (base))
#define AIF_POINTER_TYPE(addr, base)	TypeToFDS(AIF_POINTER, (addr), (base))
#define AIF_RANGE_TYPE(lo, hi, base)	TypeToFDS(AIF_RANGE, (lo), (hi), (base))
#define AIF_REFERENCE_TYPE(ref)			TypeToFDS(AIF_REFERENCE, (ref))
#define AIF_REGION_TYPE(name, base)		TypeToFDS(AIF_REGION, (name), (base))
#define AIF_STRING_TYPE(len)				TypeToFDS(AIF_STRING, (len))
#define AIF_STRUCT_TYPE()				TypeToFDS(AIF_STRUCT)
#define AIF_UNION_TYPE()					TypeToFDS(AIF_UNION)
#define AIF_VOID_TYPE(len)				TypeToFDS(AIF_VOID, (len))
#define AIF_ADDRESS_TYPE(len)			TypeToFDS(AIF_ADDRESS, (len))
#define AIF_GENERIC_POINTER_TYPE(len, addr_fmt) \
			TypeToFDS(AIF_GENERIC_POINTER, (len), (addr_fmt))
#define AIF_HASHED_TYPE(len)				TypeToFDS(AIF_HASHED_SIGNATURE, (len))
#define AIF_FTN_INTEGER_TYPE(len)		TypeToFDS(AIF_FTN_INTEGER, (len))
#define AIF_FTN_REAL_TYPE(len)			TypeToFDS(AIF_FTN_REAL, (len))
#define AIF_FTN_COMPLEX_TYPE(len)		TypeToFDS(AIF_FTN_COMPLEX, (len))
#define AIF_FTN_LOGICAL_TYPE(len)		TypeToFDS(AIF_FTN_LOGICAL, (len))
#define AIF_FTN_CHARACTER_TYPE(len)		TypeToFDS(AIF_FTN_CHARACTER, (len))
#define AIF_FTN_ARRAY_TYPE(range, base) \
			TypeToFDS(AIF_FTN_ARRAY, (range), (base))

/*
 * AIF routines.
 */
extern AIF *		AIFAdd(AIF *, AIF *);
extern int		AIFAddConstToEnum(AIF *, char *, AIF *);
extern int		AIFAddFieldToStruct(AIF *, char *, AIF *);
extern int		AIFAddFieldToUnion(AIF *, char *, char *);
extern void		AIFAddArrayElement(AIF *, int, AIF *);
extern void		AIFAddFtnArrayElement(AIF *, int, AIF *);
extern int		AIFAddressToStr(char **, int, AIF *);
extern AIF *		AIFAnd(AIF *, AIF *);
extern int		AIFArrayBounds(AIF *, int, int **, int **, int **);
extern AIF *		AIFArrayElement(AIF *, AIFIndex *);
extern int		AIFArrayElementToDouble(AIF *, AIFIndex *, double *);
extern int		AIFArrayElementToDoublest(AIF *, AIFIndex *, AIFDOUBLEST *);
extern int		AIFArrayElementToInt(AIF *, AIFIndex *, int *);
extern int		AIFArrayElementToLongest(AIF *, AIFIndex *, AIFLONGEST *);
extern void		AIFArrayIndexFree(AIFIndex *);
extern int		AIFArrayIndexInc(AIFIndex *);
extern AIFIndex	 *AIFArrayIndexInit(AIF *);
extern int		AIFArrayIndexInRange(int, int *, int *, int *);
extern int		AIFArrayIndexToStr(char **, AIFIndex *);
extern char *		AIFArrayIndexType(AIF *);
extern int		AIFArrayInfo(AIF *, int *, char **, int *);
extern int		AIFArrayMaxIndex(AIF *, int);
extern int		AIFArrayMinIndex(AIF *, int);
extern AIF *		AIFArrayPerm(AIF *, int *);
extern int		AIFArrayRank(AIF *);
extern AIF *		AIFArrayRef(AIF *, int, int *);
extern int		AIFArraySize(AIF *);
extern AIF *		AIFArraySlice(AIF *, int, int *, int *);
extern int		AIFArrayToStr(char **, int, AIF *);
extern int		AIFBaseType(AIF *);
extern int		AIFBoolToStr(char **, int, AIF *);
extern int		AIFCloseSet(AIFFILE *);
extern AIF *		AIFCoerce(AIF *, char *);
extern int		AIFCompare(AIF *a1, AIF *a2, int *res, AIF **d_res, AIF *lo, AIF *hi);
extern AIF *		AIFDiv(AIF *, AIF *);
extern int		AIFEnumToStr(char **, int, AIF *);
extern aiferr		AIFError(void);
extern char *		AIFErrorStr(void);
extern int		AIFFieldToDouble(AIF *, char *, double *);
extern int		AIFFieldToDoublest(AIF *, char *, AIFDOUBLEST *);
extern int		AIFFieldToInt(AIF *, char *, int *);
extern int		AIFFieldToLongest(AIF *, char *, AIFLONGEST *);
extern int		AIFFieldType(AIF *, char *);
extern int		AIFFloatToStr(char **, int, AIF *);
extern char *		FDSToStr(char *);
extern int		AIFFunctionToStr(char **, int, AIF *);
extern void		AIFFree(AIF *);
extern AIF *		AIFGetEnum(AIF *);
extern char *		AIFGetIdentifier(AIF *);
extern int		AIFGetOption(aifopt);
extern AIF *		AIFGetStruct(AIF *, char *);
extern AIF *		AIFGetUnion(AIF *, char *);
extern int		AIFIndexInRange(int, int *, int *, int *);
extern int		AIFIndexOffset(int, int *, int *, int *, int *);
extern int		AIFIntToStr(char **, int, AIF *);
extern int		AIFIsZero(AIF *, int *);
extern AIF *		AIFMul(AIF *, AIF *);
extern AIF *		AIFNeg(AIF *);
extern AIF *		AIFAbs(AIF *);
extern void		AIFNormalise(char *, int, char *, int);
extern AIF *		AIFNot(AIF *);
extern AIF *		AIFNull(AIF *);
extern int		AIFNumFields(AIF *);
extern AIFFILE *	AIFOpenSet(char *, int);
extern AIF *		AIFOr(AIF *, AIF *);
extern int		AIFPointerToStr(char **, int, AIF *);
extern int		AIFGenericPointerToStr(char **, int, AIF *);
extern int		AIFPrint(FILE *, int, AIF *);
extern int		AIFPrintArray(FILE *, int, AIF *);
extern int		AIFPrintBool(FILE *, int, AIF *);
extern int		AIFPrintChar(FILE *, int, AIF *);
extern int		AIFPrintEnum(FILE *, int, AIF *);
extern int		AIFPrintFloat(FILE *, int, AIF *);
extern int		AIFPrintFunction(FILE *, int, AIF *);
extern int		AIFPrintInt(FILE *, int, AIF *);
extern int		AIFPrintName(FILE *, int, AIF *);
extern int		AIFPrintPointer(FILE *, int, AIF *);
extern int		AIFPrintReference(FILE *, int, AIF *);
extern int		AIFPrintRegion(FILE *, int, AIF *);
extern int		AIFPrintString(FILE *, int, AIF *);
extern int		AIFPrintStruct(FILE *, int, AIF *);
extern int		AIFPrintUnion(FILE *, int, AIF *);
extern int		AIFPrintVoid(FILE *, int, AIF *);
extern int		AIFPrintArrayIndex(FILE *, AIFIndex *);
extern int		AIFReferenceToStr(char **, int, AIF *);
extern AIF *		AIFReadSet(AIFFILE *, char **);
extern AIF *		AIFRem(AIF *, AIF *);
extern int		AIFSetArrayData(AIF *, AIFIndex *, AIF *);
extern void		AIFSetData(AIF *, char *, int);
extern int		AIFSetEnum(AIF *, char *);
extern int		AIFSetIdentifier(AIF *, char *);
extern int		AIFSetOption(aifopt, int);
extern int		AIFSetStruct(AIF *, char *, AIF *);
extern int		AIFSetUnion(AIF *, char *, AIF *);
extern int		AIFStructToStr(char **, int, AIF *);
extern AIF *		AIFSub(AIF *, AIF *);
extern int		AIFToChar(AIF *, char *);
extern int		AIFToDouble(AIF *, double *);
extern int		AIFToDoublest(AIF *, AIFDOUBLEST *);
extern int		AIFToInt(AIF *, int *);
extern int		AIFToLongest(AIF *, AIFLONGEST *);
extern int		AIFToFloat(AIF *, float *);
extern int		AIFToStr(char **, int, AIF *);
extern int		AIFToStrPrecision(char **, int, AIF *, int);
extern int		AIFToVoid(AIF *, char *, int);
extern int		AIFType(AIF *);
extern int		AIFTypeCompare(AIF *, AIF *);
extern long		AIFTypeSize(AIF *);
extern int		AIFUnionToStr(char **, int, AIF *);
extern int		AIFVoidToStr(char **, int, AIF *);
extern int		AIFWriteSet(AIFFILE *, AIF *, char *);
extern AIF *		ArrayToAIF(int, int *, int *, char *, int, char *);
extern AIF *		FtnArrayToAIF(int, int *, int *, char *, int, char *);
extern AIF *		AsciiToAIF(char *, char *);
extern AIF * 		BoolToAIF(int);
extern AIF *		CharToAIF(char);
extern AIF *		CharacterToAIF(char);
extern AIF *		CopyAIF(AIF *);
extern AIF *		DoubleToAIF(double);
extern AIF *		EmptyArrayToAIF(int, int, AIF *);
extern AIF *		EmptyEnumToAIF(char *);
extern AIF *		EmptyStructToAIF(char *);
extern AIF *		EmptyUnionToAIF(char *);
extern AIF *		FloatToAIF(float);
extern AIF *		IntToAIF(int);
extern AIF *		LongToAIF(long);
#ifdef HAVE_LONG_DOUBLE
extern AIF *		LongDoubleToAIF(long double);
#endif /* HAVE_LONG_DOUBLE */
extern AIF *		FunctionToAIF(AIF *, char *, char *, int, char **);

extern AIF *		AIFSum(AIF *);			// Minh changes here
extern AIF *		AIFSumArray(AIF **, int);	// Minh changes here
extern AIF *		AIFProduct(AIF *);		// Minh changes here
extern AIF *		AIFProductArray(AIF **, int);	// Minh changes here
extern AIF *		AIFAverage(AIF *);		// Minh changes here
extern AIF *		AIFAverageArray(AIF **, int);	// Minh changes here
extern AIF *		AIFMax(AIF *);			// Minh changes here
extern AIF *		AIFMaxArray(AIF **, int);	// Minh changes here
extern AIF *		AIFMin(AIF *);			// Minh changes here
extern AIF *		AIFMinArray(AIF **, int);	// Minh changes here
extern AIF *		AIFStdev(AIF *);		// Minh changes here
extern AIF *		AIFStdevArray(AIF **, int);	// Minh changes here
extern AIF *		AIFCountElem(AIF *, double, double);	// Minh changes here 
extern AIF *		AIFCountElemArray(AIF **, int);	// Minh changes here 
/*
extern AIF *		AIFHistogram(AIF *, double, double, double);	// Minh changes here
extern AIF *		HistogramToAIF(gsl_histogram *);	// Minh changes here
extern AIF *		MergeAIFHistograms(AIF *, AIF *);	// Minh changes here
extern AIF *		MergeAIFHistogramArray(AIF **, int);	// Minh changes here
extern int		PrintAIFHistogram(FILE *, int, AIF *);	// Minh changes here
extern int		DisplayAIFHistogram(FILE *, AIF *);	// Minh changes here
extern int		CompareAIFHistograms(AIF *, AIF *, double *, double *);	// Minh changes here
*/

#ifdef CC_HAS_LONG_LONG
extern AIF *		LongLongToAIF(long long);
#endif
extern AIF *		LongestToAIF(AIFLONGEST);
extern AIF *		MakeAIF(char *, char *);
extern AIF *		NameAIF(AIF *, int);
extern AIF *		NewAIF(int, int);
extern AIF *		PointerNameToAIF(AIF *);
extern AIF *		PointerReferenceToAIF(AIF *);
extern AIF *		PointerToAIF(AIF *, AIF *);
extern AIF *		ReferenceAIF(int); 
extern AIF *		ShortToAIF(short);
extern AIF *		StringToAIF(char *);
extern AIF *		UnsignedCharToAIF(unsigned char);
extern AIF *		UnsignedShortToAIF(unsigned short);
extern AIF *		UnsignedIntToAIF(unsigned int);
extern AIF *		UnsignedLongToAIF(unsigned long);
#ifdef CC_HAS_LONG_LONG
extern AIF *		UnsignedLongLongToAIF(unsigned long long);
#endif
extern AIF *		VoidToAIF(char *, int);
extern AIF *		AddressToAIF(void *, int);
extern AIF *		GenericPointerToAIF(AIF *, char *);
extern AIF *		EmptyComplexArrayToAIF(int, int, AIF *);
extern void		AIFAddComplexArrayElement(AIF *, AIF *);
extern AIF *		HashToAIF(int);

/* Fortran extensions */
extern AIF *		INTEGER1ToAIF(char);
extern AIF *		INTEGER2ToAIF(short);
extern AIF *		INTEGER4ToAIF(int);
#ifdef CC_HAS_LONG_LONG
extern AIF *		INTEGER8ToAIF(long long);
#endif
extern AIF *		REAL4ToAIF(float);
extern AIF *		REAL8ToAIF(double);
#ifdef HAVE_LONG_DOUBLE
extern AIF *		REAL16ToAIF(long double);
#endif
extern AIF *		LOGICAL1ToAIF(char);
extern AIF *		LOGICAL2ToAIF(short);
extern AIF *		LOGICAL4ToAIF(int);
#ifdef CC_HAS_LONG_LONG
extern AIF *		LOGICAL8ToAIF(long long);
#endif
extern AIF *		COMPLEX4ToAIF(float, float);
extern AIF *		COMPLEX8ToAIF(double, double);
#ifdef HAVE_LONG_DOUBLE
extern AIF *		COMPLEX16ToAIF(long double, long double);
#endif
extern AIF *		CHARACTERToAIF(char *);
extern AIF *		EmptyFtnArrayToAIF(int, int, AIF *);
extern int		AIFFtnIntegerToStr(char **, int, AIF *);
extern int		AIFFtnRealToStr(char **, int, AIF *);
extern int		AIFFtnComplexToStr(char **, int, AIF *);
extern int		AIFFtnLogicalToStr(char **, int, AIF *);

#ifdef __cplusplus
}
#endif

#endif /* !_AIF_H */

