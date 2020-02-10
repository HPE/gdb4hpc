/*********************************************************************************\
 * <smart_bitset.h>
 *
 * © 2012-2013,2020 Cray Inc.
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

#ifndef _SMART_BITSET_H_
#define _SMART_BITSET_H_

#include "bitset.h"
#include "sparse_bitset.h"

/*
Smart bitset could be a dense bitset or a sparse bitset.
It converts a "bitset" between the two formats based on the number of bits.
*/

struct smart_bitset {
    bitset *sb_dense;         /*a dense bitset */
    sparse_bitset *sb_sparse; /*a spare bitset*/
};
typedef struct smart_bitset smart_bitset;

#ifdef __cplusplus
extern "C" {
#endif

smart_bitset *smart_bitset_new(
    int num); /* create a new smart bitset to contain 'num' bits */
void smart_bitset_free(smart_bitset *b);  /* dispose of a smart bitset */
void smart_bitset_clear(smart_bitset *b); /* set all bits to 0 */

void smart_bitset_copy(smart_bitset *sb1, smart_bitset *sb2);
smart_bitset *smart_bitset_dup(smart_bitset *sb);

int smart_bitset_is_sparse(
    smart_bitset *b); /* return if it is a sparse bitset */
int smart_bitset_is_dense(smart_bitset *b); /* return if it is a dense bitset */

void smart_bitset_set(smart_bitset *b, int n); /* set bit 'n' to 1 */

smart_bitset *smart_bitset_or(smart_bitset *b1,
                              smart_bitset *b2); /* compute b3 = b1 | b2 */
void smart_bitset_oreq(smart_bitset *b1,
                       smart_bitset *b2); /* compute b1 |= b2 */

int smart_bitset_convert(
    smart_bitset *b); /* it converts a sparse bitset to a dense bitset */

char *smart_bitset_serialize(
    smart_bitset *b); /* convert b to set notation of hex */
smart_bitset *smart_bitset_deserialize(
    char *set); /* convert a set notation of hex to a smart bitset*/

#ifdef __cplusplus
}
#endif

#endif /*_SMART_BITSET_H_*/
