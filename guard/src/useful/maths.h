/*********************************************************************************\
 * maths.h
 *
 * Copyright 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _USEFUL_MATHS_H
#define _USEFUL_MATHS_H

#ifdef __cplusplus
extern "C" {
#endif

int min(int, int);
int max(int, int);
int lcm(int, int);
int gcd(int, int);
int lcf(int, int, int);
long int product(int*, int);
int quickSort(int*, int);

#ifdef __cplusplus
}
#endif

#endif /* _USEFUL_MATHS_H */
