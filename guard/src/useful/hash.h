/*********************************************************************************\
 * Hash definition.
 *
 * Copyright (c) 1996-2001 by GuardSoft Pty. Ltd.
 *
 * Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _USEFUL_HASH_H
#define _USEFUL_HASH_H

#ifdef __cplusplus
extern "C" {
#endif

struct hentry {
    unsigned int h_hval;   /* actual hash value */
    unsigned int h_klen;   /* length of key */
    char *h_key;           /* key */
    void *h_data;          /* entry data */
    struct hentry *h_next; /* next in chain */
};
typedef struct hentry hentry_t;

/* Data type for reentrant functions.  */
struct htable {
    hentry_t **table;     /* hash chain entries */
    unsigned int logsize; /* log 2 number of chains in table */
    unsigned int size;    /* number of chains in table */
    unsigned int count;   /* number of entries in table */
    unsigned int mask;    /* mask used to compute chain */
};
typedef struct htable htable_t;

extern htable_t *HashCreate(int);
extern void HashDestroy(htable_t *, void (*)(char *, void *));
extern hentry_t *HashSearch(htable_t *, const char *);
extern hentry_t *HashInsert(htable_t *, const char *, void *);
extern void HashRemove(htable_t *, hentry_t *);

#ifdef __cplusplus
}
#endif

#endif /* !_USEFUL_HASH_H */
