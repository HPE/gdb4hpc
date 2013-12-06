/*
** Original copyright by Bob Jenkins, December 1996.
**
** By Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  You may use this
** code any way you wish, private, educational, or commercial.
**
*/

#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "alloc.h"
#include "hash.h"

/*
** mix 3 32-bit values reversibly.
** For every delta with one or two bit set, and the deltas of all three
**   high bits or all three low bits, whether the original value of a,b,c
**   is almost all zero or is uniformly distributed,
** If mix() is run forward or backward, at least 32 bits in a,b,c
**  have at least 1/4 probability of changing.
** If mix() is run forward, every bit of c will change between 1/3 and
**  2/3 of the time.  (Well, 22/100 and 78/100 for some 2-bit deltas.)
** mix() was built out of 36 single-cycle latency instructions in a 
**  structure that could supported 2x parallelism, like so:
**      a -= b; 
**      a -= c; x = (c>>13);
**      b -= c; a ^= x;
**      b -= a; x = (a<<8);
**      c -= a; b ^= x;
**      c -= b; x = (b>>13);
**      ...
**  Unfortunately, superscalar Pentiums and Sparcs can't take advantage 
**  of that parallelism.  They've also turned some of those single-cycle
**  latency instructions into multi-cycle latency instructions.  Still,
**  this is the fastest good hash I could find.  There were about 2^^68
**  to choose from.  I only looked at a billion or so.
*/
#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

static void _hgrow(htable_t *);

htable_t *
HashCreate(int logsize)
{
	unsigned int	i;
	htable_t *	htab;

	htab = (htable_t *)Alloc(sizeof(htable_t));

	/*
	** Maximum size is 2^31
	*/
	if ( logsize > BITSPERBYTE * sizeof(unsigned int) - 1 )
		logsize = BITSPERBYTE * sizeof(unsigned int) - 1;

	htab->logsize = logsize;
	htab->size = (unsigned int)(1 << logsize);
	htab->mask = htab->size - 1;
	htab->count = 0;

	/* allocate memory and zero out */
	htab->table = (hentry_t **)Alloc(htab->size * sizeof(hentry_t *));

	for ( i = 0 ; i < htab->size ; i++ )
		htab->table[i] = (hentry_t *)NULL;

	/* everything went alright */
	return htab;
}

void
HashDestroy(htable_t *htab,  void (*destroy)(char *, void *))
{
	int		i;
	hentry_t *	h;
	hentry_t *	hn;

	/* Test for correct arguments.  */
	if ( htab == (htable_t *)NULL )
		return;

	for ( i = 0 ; i < (int)htab->size ; i++ )
	{
		for ( h = htab->table[i] ; h != (hentry_t *)NULL ; )
		{
			if ( destroy != (void (*)(char *, void *))NULL )
				destroy(h->h_key, h->h_data);
			
			Free(h->h_key);

			hn = h->h_next;
			Free(h);
			h = hn;
		}
	}

	if (htab->table != (hentry_t **)NULL)
		Free(htab->table);

	Free(htab);
}

/*
** findentry(tab, key, hv) -- hash a variable-length key into a 32-bit value
**
**  key: the key (the unaligned variable-length array of bytes)
**  hv : can be any 4-byte value
**
** Returns a 32-bit value.  Every bit of the key affects every bit of
** the return value.  Every 1-bit and 2-bit delta achieves avalanche.
** About 6len+35 instructions.
**
** The best hash table sizes are powers of 2.  There is no need to do
** mod a prime (mod is sooo slow!).  If you need less than 32 bits,
** use a bitmask.  For example, if you need only 10 bits, do
**   h = (h & hashmask(10));
** In which case, the hash table should have hashsize(10) elements.
** 
** If you are hashing n strings (ub1 **)k, do it like this:
**   for (i=0, h=0; i<n; ++i) h = findentry( k[i], h);
** 
** By Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  You may use this
** code any way you wish, private, educational, or commercial.
** 
** See http://burtleburtle.net/bob/hash/evahash.html
** Use for hash table lookup, or anything where one collision in 2^32 is
** acceptable.  Do NOT use for cryptographic purposes.
*/

unsigned int
findentry(htable_t *htab, char *key, int hv)
{
	unsigned int	a;
	unsigned int	b;
	unsigned int	c;
	unsigned int	l;
	unsigned int	len;

	if ( key == NULL )
		return 0;

	/* Set up the internal state */
	len = strlen(key);
	a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
	c = hv;           /* the previous hash value */

	/*---------------------------------------- handle most of the key */
	for ( l = len ; l >= 12 ; l -= 12 )
	{
		a += (key[0] +((unsigned int)key[1]<<8) +((unsigned int)key[2]<<16) +((unsigned int)key[3]<<24));
		b += (key[4] +((unsigned int)key[5]<<8) +((unsigned int)key[6]<<16) +((unsigned int)key[7]<<24));
		c += (key[8] +((unsigned int)key[9]<<8) +((unsigned int)key[10]<<16)+((unsigned int)key[11]<<24));

		mix(a,b,c);

		key += 12;
	}

	/*------------------------------------- handle the last 11 bytes */
	c += len;

	switch ( l )              /* all the case statements fall through */
	{
	case 11: c += ((unsigned int)key[10]<<24);
	case 10: c += ((unsigned int)key[9]<<16);
	case 9 : c += ((unsigned int)key[8]<<8);
	/* the first byte of c is reserved for the length */
	case 8 : b += ((unsigned int)key[7]<<24);
	case 7 : b += ((unsigned int)key[6]<<16);
	case 6 : b += ((unsigned int)key[5]<<8);
	case 5 : b += key[4];
	case 4 : a += ((unsigned int)key[3]<<24);
	case 3 : a += ((unsigned int)key[2]<<16);
	case 2 : a += ((unsigned int)key[1]<<8);
	case 1 : a += key[0];
	/* case 0: nothing left to add */
	}

	mix(a,b,c);

	return c;
}

hentry_t *
HashSearch(htable_t *htab, char *key)
{
	unsigned int	idx;
	hentry_t *		h;

	if ( htab == (htable_t *)NULL )
		return (hentry_t *)NULL;

	idx = findentry(htab, key, 0);

	for ( h = htab->table[idx & htab->mask] ; h != (hentry_t *)NULL ; h = h->h_next)
	{
		if
		(
			idx == h->h_hval
			&&
			strlen(key) == h->h_klen
			&&
			strcmp(key, h->h_key) == 0
		)
			return h;
	}

	return (hentry_t *)NULL;
}

hentry_t *
HashInsert(htable_t *htab, char *key, void *data)
{
	unsigned int	idx;
	unsigned int	len = strlen(key);
	hentry_t *		h;
	hentry_t **		hp;

	idx = findentry(htab, key, 0);

	/*
	** Check if entry already exists
	*/
	for ( h = htab->table[idx & htab->mask] ; h != (hentry_t *)NULL ; h = h->h_next)
	{
		if
		(
			idx == h->h_hval
			&&
			len == h->h_klen
			&&
			strcmp(key, h->h_key) == 0
		)
		      return (hentry_t *)NULL;
	}

	/*
	** Grow hash table if it's getting full.
	*/

	if ( ++htab->count > htab->size )
	{
		_hgrow(htab);
	}

	h = (hentry_t *)Alloc(sizeof(hentry_t));
	h->h_hval = idx;	
	h->h_klen = len;	
	h->h_key = strdup(key);
	h->h_data = data;

	hp = &htab->table[idx & htab->mask];
	h->h_next = *hp;
	*hp = h;

	return h;
}

void
HashRemove(htable_t *htab, hentry_t *item)
{
	unsigned int	idx;
	hentry_t **		hp;
	
	idx = findentry(htab, item->h_key, 0);

	/*
	** Find item.
	*/
	for ( hp = &htab->table[idx & htab->mask] ; *hp != item ; )
		hp = &(*hp)->h_next;

	/*
	** Remove item.
	*/
	*hp = (*hp)->h_next;
	
	Free(item->h_key);
	--(htab->count);
}

void
HashStat(htable_t *t)
{
	unsigned int	i;
	unsigned int	j;
	double		total = 0.0;
	hentry_t  *	h;
	hentry_t  *	walk;
	hentry_t  *	walk2;
	hentry_t  *	stat = (hentry_t *)0;

	/* 
	** in stat, h_klen will store length of list, 
	** h_hval the number of buckets 
	*/

	for ( i = 0 ; i < t->size ; ++i )
	{
		for ( h = t->table[i], j=0 ; h != NULL ; ++j, h=h->h_next ) ;

		for ( walk = stat ; walk && (walk->h_klen != j) ; walk = walk->h_next ) ;
		if (walk)
		{
			++(walk->h_hval);
		}
		else
		{
			walk = (hentry_t *)Alloc(sizeof(hentry_t));
			walk->h_klen = j;
			walk->h_hval = 1;

			if ( !stat || stat->h_klen > j )
			{
				walk->h_next = stat; 
				stat = walk;
			}
			else
			{
				for (walk2 = stat;
				     walk2->h_next && (walk2->h_next->h_klen<j);
				     walk2 = walk2->h_next) ;

				walk->h_next = walk2->h_next;
				walk2->h_next = walk;
			}
			}
	}

	/* figure out average list length for existing elements */
	for ( walk = stat ; walk != NULL ; walk = walk->h_next )
		total += (double)walk->h_hval*(double)walk->h_klen*(double)walk->h_klen;

	if ( t->count )
		total /= (double)t->count;
	else 
		total = (double)0;

	/* print statistics */

	printf("\n");

	for ( walk = stat ; walk != NULL ; walk = walk->h_next )
		printf("%d buckets have length %d\n", walk->h_hval, walk->h_klen);

	printf("\ntotal buckets: %d  total items: %d  existing: %g\n\n", ((unsigned int)1<<t->logsize), t->count, total);

	/* clean up */
	while (stat)
	{
		walk = stat->h_next;
		Free(stat);
		stat = walk;
	}
}

static void
_hgrow(htable_t *htab)
{
	unsigned int	i;
	unsigned int	nsize;
	unsigned int	nmask;
	hentry_t **	ntab;

	nsize = (unsigned int)(1 << ++htab->logsize);
	nmask = nsize - 1;
	ntab = (hentry_t **)Alloc(htab->size * sizeof(hentry_t *));

	for ( i = 0 ; i < nsize ; i++ )
		ntab[i] = (hentry_t *)NULL;

	/*
	** Move old entries to new table.
	*/
	for ( i = 0 ; i < htab->size ; i++ )
	{
		hentry_t *	h;
		hentry_t *	h2;
		hentry_t **	hp;

		for ( h = htab->table[i] ; h != (hentry_t *)NULL ; )
		{
			h2 = h;
			h = h->h_next;

			hp = &ntab[h2->h_hval & nmask];
			h2->h_next = *hp;
			*hp = h2;
		}
	}

	Free(htab->table);

	htab->table = ntab;
	htab->size = nsize;
	htab->mask = nmask;
}
