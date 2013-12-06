/*
** Utility routines
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdlib.h>

void *
_aif_alloc(int size)
{
	void *	m = malloc(size);

	if ( m == (void *)NULL )
		perror("malloc error");

	return m;
}

void *
_aif_resize(void *m, int s)
{
	if ( m == (void *)NULL )
		return _aif_alloc(s);

	m = realloc(m, s);

	if ( m == (void *)NULL )
		perror("realloc error");

	return m;
}

void
_aif_free(void *m)
{
	if ( m == (void *)NULL )
	{
		fprintf(stderr, "Warning: freeing NULL pointer.\n");
		return;
	}

	(void)free(m);
}
