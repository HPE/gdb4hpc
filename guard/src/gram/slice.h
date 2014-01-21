/*********************************************************************************\
 * Slice defines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
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

enum slice_type
{
	SLICE_RANGE,
	SLICE_POS,
	SLICE_ALL
};
typedef enum slice_type	slice_type;

struct slice
{
	int				s_rank;
	int				s_size;
	int *			s_index;
	int *			s_low;
	int *			s_high;
	slice_type *	s_type;
};
typedef struct slice	slice_t;

slice_t *	NewSlice(void);
void		AddSlice(slice_t *, int, int, slice_type);
slice_t *	CopySlice(slice_t *);
void		ReverseSlice(slice_t *);
void		DestroySlice(slice_t *);

#endif /* !_SLICE_H */

