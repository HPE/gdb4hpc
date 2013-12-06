/*
** Hash definition.
**
** Copyright (c) 1996-2001 by GuardSoft Pty. Ltd.
**
*/

#ifndef _USEFUL_HASH_H
#define _USEFUL_HASH_H

struct hentry
{
	unsigned int		h_hval;	/* actual hash value */
	unsigned int		h_klen;	/* length of key */
	char *				h_key;	/* key */
	void *				h_data;	/* entry data */
	struct hentry *		h_next;	/* next in chain */
};
typedef struct hentry	hentry_t;

/* Data type for reentrant functions.  */
struct htable
{
	hentry_t **		table;		/* hash chain entries */
	unsigned int	logsize;	/* log 2 number of chains in table */
	unsigned int	size;		/* number of chains in table */
	unsigned int	count;		/* number of entries in table */
	unsigned int	mask;		/* mask used to compute chain */
};
typedef struct htable	htable_t;

extern htable_t *	HashCreate(int);
extern void			HashDestroy(htable_t *, void (*)(char *,void *));
extern hentry_t *	HashSearch(htable_t *, char *);
extern hentry_t *	HashInsert(htable_t *, char *, void *);
extern void			HashRemove(htable_t *, hentry_t *);

#endif /* !_USEFUL_HASH_H */
