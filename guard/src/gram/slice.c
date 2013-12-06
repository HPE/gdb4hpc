/*
** Slice support.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<string.h>

#include	"defs.h"
#include	"useful.h"
#include	"gram.h"

#define INIT_SLICE	4

slice_t *
NewSlice(void)
{
	slice_t *	s;

	s = (slice_t *)Alloc(sizeof(slice_t));

	s->s_size = INIT_SLICE;
	s->s_rank = 0;

	s->s_low = (int *)Alloc(sizeof(int) * INIT_SLICE);
	s->s_high = (int *)Alloc(sizeof(int) * INIT_SLICE);
	s->s_type = (slice_type *)Alloc(sizeof(slice_type) * INIT_SLICE);

	return s;
}

void
ResizeSlice(slice_t *s)
{
	s->s_size *= 2;

	s->s_low = (int *)Resize((void *)s->s_low, sizeof(int) * s->s_size);
	s->s_high = (int *)Resize((void *)s->s_high, sizeof(int) * s->s_size);
	s->s_type = (slice_type *)Resize((void *)s->s_type, sizeof(slice_type) * s->s_size);
}

void
AddSlice(slice_t *s, int f, int t, slice_type st)
{
	int	rank = s->s_rank;

	if ( rank >= s->s_size )
		ResizeSlice(s);

	s->s_type[rank] = st;
	s->s_low[rank] = f;
	s->s_high[rank]= t;

	s->s_rank++;
}

slice_t *
CopySlice(slice_t *s)
{
	slice_t *	ns;
	
	if (s == NULL)
		return NULL;

	ns = (slice_t *)Alloc(sizeof(slice_t));

	ns->s_size = s->s_size;
	ns->s_rank = s->s_rank;

	ns->s_low = (int *)Alloc(sizeof(int) * s->s_size);
	memcpy(ns->s_low, s->s_low, sizeof(int) * s->s_size);

	ns->s_high = (int *)Alloc(sizeof(int) * s->s_size);
	memcpy(ns->s_high, s->s_high, sizeof(int) * s->s_size);

	ns->s_type = (slice_type *)Alloc(sizeof(slice_type) * s->s_size);
	memcpy(ns->s_type, s->s_type, sizeof(slice_type) * s->s_size);

	return ns;
}

void
_reverse_slice(int rank, int *ix)
{
	int	i;
	int	tmp;

	for ( i = 0 ; i < rank/2 ; i++ )
	{
		tmp = ix[rank - i - 1];
		ix[rank - i - 1] = ix[i];
		ix[i] = tmp;
	}
}

/*
** Reverse order of slices.
*/
void
ReverseSlice(slice_t *s)
{
	_reverse_slice(s->s_rank, (int *)s->s_type);
	_reverse_slice(s->s_rank, s->s_low);
	_reverse_slice(s->s_rank, s->s_high);
}

void
DestroySlice(slice_t *s)
{
	if(s == NULL)
		return;

	Free(s->s_low);
	Free(s->s_high);
	Free(s->s_type);
	Free(s);
}
