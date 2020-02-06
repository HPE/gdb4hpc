/*********************************************************************************\
 * <conversion.h>
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

#ifndef _BITSET_CONVERSION_H_
#define _BITSET_CONVERSION_H_

#include "varstr.h"

#ifdef __cplusplus
extern "C" {
#endif

int emit_range(varstr* v, char sep, int lower, int upper, int hex);

int atoi_hex(char* str, int start, int end, int* result);
void parse_range(int* lower, int* upper, char* set, int start, int end);

#ifdef __cplusplus
}
#endif

#endif /*_BITSET_CONVERSION_H_*/
