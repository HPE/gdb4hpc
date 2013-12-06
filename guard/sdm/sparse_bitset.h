#ifndef _SPARSE_BITSET_H_
#define _SPARSE_BITSET_H_

/*
Sparse bitset consists of a number of ranges. Each ranges is denoted by using its start and end.
*/

struct sparse_range {
	unsigned int	sr_start;
	unsigned int	sr_end;
};
typedef struct sparse_range sparse_range;

struct sparse_bitset {
	unsigned int	sb_num_bits;	/* total number of bits that are supposed to have in a sparse set */
	unsigned int	sb_num_ranges; /* total number of ranges in a spare bitset*/
	sparse_range*	sb_ranges;  /* an array of ranges in an asceinding order*/
	unsigned int	sb_size; /* number of malloced space for ranges */
}; 
typedef struct sparse_bitset sparse_bitset;

#define SB_INITIAL_NUM_RANGES 4

#ifdef __cplusplus
extern "C" {
#endif

sparse_bitset *	sparse_bitset_new(int num);		/* create a new sparse bitset to contain 'num' bits */
void			sparse_bitset_free(sparse_bitset *b);	/* dispose of a sparse bitset */
void			sparse_bitset_clear(sparse_bitset *b);			/* set all bits to 0 */
void			sparse_bitset_copy(sparse_bitset *sb1, sparse_bitset *sb2);
sparse_bitset *	sparse_bitset_dup(sparse_bitset *b);			/* duplicate b */

void			sparse_bitset_set(sparse_bitset *b, int n);		/* set bit 'n' to 1 */

sparse_bitset *	sparse_bitset_or(sparse_bitset *b1, sparse_bitset *b2);		/* compute b3 = b1 | b2 */
void			sparse_bitset_oreq(sparse_bitset *b1, sparse_bitset *b2);	/* compute b1 |= b2 */

int				sparse_bitset_convert(sparse_bitset *b);	/* return if it needs to convert to a bitset */
int				sparse_bitset_size(sparse_bitset *b);	/* number of ranges this sparse bitset can represent */

char *		 	sparse_bitset_to_set(sparse_bitset *b);		/* convert b to set notation */
sparse_bitset *	set_to_sparse_bitset(char *set);		/* convert a set notation to a sparse bitset*/

char *		 	sparse_bitset_serialize(sparse_bitset *b);		/* convert b to set notation of hex */
sparse_bitset *	sparse_bitset_deserialize(int num_bits, char *set, int len);		/* convert a set notation of hex to a sparse bitset*/

#ifdef __cplusplus
}
#endif

#endif /*_SPARSE_BITSET_H_*/
