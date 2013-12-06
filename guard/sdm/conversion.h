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
