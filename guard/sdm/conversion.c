/*********************************************************************************\
 * <conversion.c>
 *
 * © 2012-2013 Cray Inc.  All Rights Reserved.
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
 *********************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>

#include "bitset.h"
#include "varstr.h"
#include "compat.h"

int
emit_range(varstr *v, char sep, int lower, int upper, int hex)
{
	if (lower < 0 || upper < lower) {
		return 0;
	}

	if (sep) {
		varstr_add(v, sep);
	}

	if (lower != upper) {
		if(hex)
			varstr_sprintf(v, "%x-%x", lower, upper);
		else
			varstr_sprintf(v, "%d..%d", lower, upper);
	} else {
		if(hex)
			varstr_sprintf(v, "%x", lower);
		else
			varstr_sprintf(v, "%d", lower);
	}

	return 1;
}

/*convert a hex string to an integer*/
int
atoi_hex(char* str, int start, int end, int* result)
{
	int res = 0, index;
	if(str == NULL || start < 0 || end < start || start >= strlen(str))
		return -1; 

	for (index = start; index <= end && isxdigit(str[index]); index++) {
		res <<= 4;
		res += digittoint(str[index]);
	}

	*result = res;
	return 0;
}

void
parse_range(int* lower, int* upper, char* set, int start, int end)
{
	int index, res;

	if(set == NULL || start < 0 || end < start)
	{
		*lower = -1;
		*upper = -1;	
	}

	for (index = start; index <= end && (set[index] != '-' && set[index] != '.'); index++);
	res = atoi_hex(set, start, index - 1, lower);
	if(res < 0)
		*lower = -1;

	if(end > index && (set[index] == '-' || set[index] == '.'))
	{
		res = atoi_hex(set, index + 1, end, upper);
		if(res < 0)
			*upper = -1;
	}
	else
		*upper = *lower;
}
