/*********************************************************************************\
 * Queue routines
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




#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdlib.h>

#include	"defs.h"
#include	"queue.h"
#include	"alloc.h"

tkqueue_t *
NewQueue(int size)
{
	tkqueue_t *	q;

	q = (tkqueue_t *)Alloc(sizeof(tkqueue_t));

	q->q_nel = 0;
	q->q_head = (qel_t *)NULL;
	q->q_tail = (qel_t *)NULL;
	q->q_qlen = size;

	return q;
}

void
DestroyQueue(tkqueue_t *q)
{
	qel_t *	qen;
	qel_t *	qep;

	if ( q == (tkqueue_t *)NULL )
		return;

	if ( q->q_nel > 0 )
	{
		qep = q->q_head;

		while ( q->q_nel-- > 0 )
		{
			qen = qep->q_next;
			Free(qep);
			qep = qen;
		}
	}

	Free(q);
}

void *
RemoveFromQueue(tkqueue_t *q)
{
	void *	e;
	qel_t *	qe;

	if ( q->q_nel == 0 )
		return NULL;

	q->q_nel--;

	qe = q->q_head;

	q->q_head = qe->q_next;

	e = qe->q_el;

	Free(qe);

	return e;
}

void
AddToQueue(tkqueue_t *q, void *e)
{
	qel_t *	qe;

	if ( q->q_nel == q->q_qlen )
		return; /* queue full */

	qe = (qel_t *)Alloc(sizeof(qel_t));
	qe->q_el = e;
	qe->q_next = (qel_t *)NULL;

	if ( q->q_nel++ > 0 )
	{
		q->q_tail->q_next = qe;
		q->q_tail = qe;
	}
	else
		q->q_head = q->q_tail = qe;
}

int
EmptyQueue(tkqueue_t *q)
{
	return (int)(q->q_nel == 0);
}

int
FullQueue(tkqueue_t *q)
{
	return (int)(q->q_nel == q->q_qlen);
}

void *
HeadOfQueue(tkqueue_t *q)
{
	if ( q->q_nel == 0 )
		return (void *)NULL;

	return q->q_head->q_el;
}
