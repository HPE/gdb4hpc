/*********************************************************************************\
 * <conversion.h>
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


#ifndef _BITSET_CONVERSION_H_
#define _BITSET_CONVERSION_H_

#include "varstr.h"

#ifdef __cplusplus
extern "C" {
#endif

int				emit_range(varstr *v, char sep, int lower, int upper, int hex);

int				atoi_hex(char* str, int start, int end, int* result);
void			parse_range(int* lower, int* upper, char* set, int start, int end);

#ifdef __cplusplus
}
#endif

#endif /*_BITSET_CONVERSION_H_*/
