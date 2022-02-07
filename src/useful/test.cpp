/*********************************************************************************\
 * test.c
 *
 * Copyright 2010-2013, 2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#include "hash.h"
#define LINELEN 4096

void hstat(htable_t *);

int main() {
    char buf[LINELEN];
    char *key;
    unsigned int keyl;
    htable_t *t;
    hentry_t *a;
    hentry_t *b;
    hentry_t *c;

    t = HashCreate(2); /* create hash table */

    if (HashInsert(t, "zero", 0))
        printf("succeeded\n");
    else
        printf("failed\n");

    if (HashInsert(t, "one", 1))
        printf("succeeded\n");
    else
        printf("failed\n");

    printf("size=%d,count=%d\n", t->size, t->count);

    hstat(t);

    if (b = HashInsert(t, "two", 2))
        printf("succeeded\n");
    else
        printf("failed\n");

    if (c = HashInsert(t, "three", 3))
        printf("succeeded\n");
    else
        printf("failed\n");

    printf("size=%d,count=%d\n", t->size, t->count);

    hstat(t);

    if (HashInsert(t, "three", 4))
        printf("succeeded\n");
    else
        printf("failed\n");

    if (HashInsert(t, "five", 5))
        printf("succeeded\n");
    else
        printf("failed\n");

    printf("size=%d,count=%d\n", t->size, t->count);

    hstat(t);

    if (a = HashInsert(t, "six", 6))
        printf("succeeded\n");
    else
        printf("failed\n");

    if (HashInsert(t, "seven", 7))
        printf("succeeded\n");
    else
        printf("failed\n");

    printf("size=%d,count=%d\n", t->size, t->count);

    hstat(t);

    HashRemove(t, a);
    HashRemove(t, b);
    HashRemove(t, c);

    printf("size=%d,count=%d\n", t->size, t->count);

    hstat(t);
    exit(0);
}

void hstat(htable_t *t) {
    unsigned int i, j;
    double total = 0.0;
    hentry_t *h;
    hentry_t *walk, *walk2, *stat = (hentry_t *)0;

    /* in stat, keyl will store length of list, hval the number of buckets */
    for (i = 0; i < t->size; ++i) {
        for (h = t->table[i], j = 0; h; ++j, h = h->h_next)
            ;
        for (walk = stat; walk && (walk->h_klen != j); walk = walk->h_next)
            ;
        if (walk) {
            ++(walk->h_hval);
        } else {
            walk = (hentry_t *)Alloc(sizeof(hentry_t));
            walk->h_klen = j;
            walk->h_hval = 1;
            if (!stat || stat->h_klen > j) {
                walk->h_next = stat;
                stat = walk;
            } else {
                for (walk2 = stat; walk2->h_next && (walk2->h_next->h_klen < j);
                     walk2 = walk2->h_next)
                    ;
                walk->h_next = walk2->h_next;
                walk2->h_next = walk;
            }
        }
    }

    /* figure out average list length for existing elements */
    for (walk = stat; walk; walk = walk->h_next) {
        total +=
            (double)walk->h_hval * (double)walk->h_klen * (double)walk->h_klen;
    }
    if (t->count)
        total /= (double)t->count;
    else
        total = (double)0;

    /* print statistics */
    printf("\n");
    for (walk = stat; walk; walk = walk->h_next) {
        printf("%ld buckets have length %ld\n", walk->h_hval, walk->h_klen);
    }
    printf("\ntotal buckets: %ld  total items: %ld  existing: %g\n\n",
           ((unsigned int)1 << t->logsize), t->count, total);

    /* clean up */
    while (stat) {
        walk = stat->h_next;
        Free(stat);
        stat = walk;
    }
}
