/*
** Queue definitions
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _USEFUL_QUEUE_H
#define _USEFUL_QUEUE_H

struct qel_t
{
	void *		q_el;
	struct qel_t *	q_next;
};
typedef struct qel_t	qel_t;

struct tkqueue_t
{
	int	q_nel;	/* number of elements in queue */
	int	q_qlen;	/* maximum length of queue */
	qel_t *	q_head;
	qel_t *	q_tail;
};
typedef struct tkqueue_t	tkqueue_t;

tkqueue_t *	NewQueue(int);
void		DestroyQueue(tkqueue_t *);
void *		RemoveFromQueue(tkqueue_t *);
void *		HeadOfQueue(tkqueue_t *);
void		AddToQueue(tkqueue_t *, void *);
int		EmptyQueue(tkqueue_t *);
int		FullQueue(tkqueue_t *);

#endif /* !_USEFUL_QUEUE_H */
