/******************************************************************************\
 * Header file for libmi to aif conversion routines
 *
 * Copyright 2011-2013 Cray Inc.	All Rights Reserved.
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

#ifndef _DBGSRV_LIBMI2AIF_H
#define _DBGSRV_LIBMI2AIF_H

// requisite includes
#include "MI.h"
#include "MIOOBRecord.h"
#include "aif.h"
#include "dbgevent.h"

/********************************************************
 * TYPE CONVERSION
 ********************************************************/
#define T_UNKNOWN -1
/*
 * Simple types
 */
#define T_CHAR 0
#define T_UCHAR 1
#define T_SHORT 2
#define T_USHORT 3
#define T_INT 4
#define T_UINT 5
#define T_LONG 6
#define T_ULONG 7
#define T_LONGLONG 8
#define T_ULONGLONG 9
#define T_FLOAT 10
#define T_DOUBLE 11
#define T_LONGDOUBLE 12
#define T_STRING 13
#define T_BOOLEAN 14
/*
 * Complex (structure) types
 */
#define T_FUNCTION 15
#define T_UNION 16
#define T_ENUM 17
#define T_ARRAY 18
#define T_STRUCT 19
#define T_POINTER 20
#define T_CLASS 21
/*
 * Complex fortran types
 */
#define T_FTNARRAY 22
#define T_FTNDTYPE 23
/*
 * Fortran intrinsic type extensions (these are simple types)
 */
#define T_INTEGER_1 \
    24  // INTEGER*1 or BYTE				translation: char
#define T_INTEGER_2 \
    25  // INTEGER*2						translation:
        // short
#define T_INTEGER_4 \
    26  // INTEGER*4			 			translation: long or
        // int
#define T_INTEGER_8 \
    27  // INTEGER*8			 			translation: long
        // long
#define T_REAL_4 \
    28               // REAL*4							translation:
                     // float
#define T_REAL_8 29  // REAL*8 or DOUBLE PRECISION	translation: double
#define T_REAL_16 \
    30  // REAL*16							translation: long
        // double
#define T_COMPLEX_4 \
    31  // COMPLEX*8						translation: struct{float
        // realnum; float imagnum;}
#define T_COMPLEX_8 \
    32  // COMPLEX*16 or DOUBLE COMPLEX	translation: struct{double realnum;
        // double imagnum;}
#define T_COMPLEX_16 \
    33  // COMPLEX*32						translation: struct{long
        // double realnum; long double imagnum;}
#define T_LOGICAL_1 \
    34  // LOGICAL*1						translation:
        // char
#define T_LOGICAL_2 \
    35  // LOGICAL*2						translation:
        // short
#define T_LOGICAL_4 \
    36  // LOGICAL*4						translation: long or
        // int
#define T_LOGICAL_8 \
    37  // LOGICAL*8						translation: long
        // long
#define T_CHARACTER \
    38  // CHARACTER*1						translation:
        // char[k]
/*
 * Void types
 */
#define T_VOID 39
#define T_VOID_PTR 40

/*
        C++ Standard Containers
*/
#define T_STD_ARRAY 41
#define T_STD_VECTOR 42
#define T_STD_DEQUE 43
#define T_STD_FWD_LIST 44
#define T_STD_LIST 45
#define T_STD_STACK 46
#define T_STD_QUEUE 47
#define T_STD_PQUEUE 48
#define T_STD_SET 49
#define T_STD_MULTISET 50
#define T_STD_MAP 51
#define T_STD_MULTMAP 52
#define T_STD_UNORD_SET 53
#define T_STD_UNORD_MULTISET 54
#define T_STD_UNORD_MAP 55
#define T_STD_UNORD_MULTIMAP 56

/*
 * C String delimiter characters
 */
#define C_STRING_START '\"'
#define C_STRING_END '\"'

/*
 * Array value delimiter characters
 */
#define ARRAY_SEP ','
#define ARRAY_C_START '{'
#define ARRAY_C_END '}'
#define ARRAY_FTN_START '('
#define ARRAY_FTN_END ')'
/*
 * Array type delimiter characters
 */
#define ARRAY_TYPE_SEP ','
#define ARRAY_C_TYPE_START '['
#define ARRAY_C_TYPE_END ']'
#define ARRAY_FTN_TYPE_START '('
#define ARRAY_FTN_TYPE_END ')'
#define ARRAY_FTN_RANGE_SEP ':'

/*
 * Function type delimiter characters
 */
#define FUNCTION_START '('
#define FUNCTION_ARG_SEP ','
#define FUNCTION_END ')'

/*
 * Known type strings
 */
#define VOID_FUNCTION_STR "(*)"

/*
** Nvidia extensions
*/
#define CODE_S "@code"
#define CONSTANT_S "@constant"
#define GENERIC_S "@generic"
#define GLOBAL_S "@global"
#define PARAMETER_S "@parameter"
#define SHARED_S "@shared"
#define TEXTURE_S "@texture"
#define LOCAL_S "@local"
#define REGISTER_S "@register"

#define NV_CHECK_ADDR_IDENT(a, b) (strncmp(a, b, strlen(b)) == 0)

#define NV_CHECK_ADDR_IDENT_L(a, b) \
    ((strncmp(a, b, strlen(b)) == 0) && (strlen(a) == strlen(b)))

/*
 * Function prototypes
 */
dbgevent_t *GetAIFVar(MISession *, const char *, AIFStream **, char **);
dbgevent_t *GetAIFVarHash(MISession *, char *, AIF **, char *, int, int, int);

#endif /* _DBGSRV_LIBMI2AIF_H */
