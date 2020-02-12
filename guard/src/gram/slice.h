/*********************************************************************************\
 * Slice defines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2014,2020 Cray Inc.
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

#ifndef _SLICE_H
#define _SLICE_H

struct slice {
    int s_rank;
    int* s_low;
    int* s_high;
    int _s_size;  // Internal, do not use.
};
typedef struct slice slice_t;

#endif /* !_SLICE_H */
