/*********************************************************************************\
 * List definition
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _USEFUL_LIST_H
#define _USEFUL_LIST_H

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

struct list_t {
    int l_nel;
    struct liste_t *l_head;
    struct liste_t *l_scan;
    struct liste_t **l_tail;
    pthread_mutex_t l_lock;
    pthread_mutexattr_t _list_lock_attr;
};
typedef struct list_t list_t;

struct liste_t {
    void *l_value;
    struct liste_t *l_next;
};
typedef struct liste_t liste_t;

extern list_t *NewList(void);
extern void AddToList(list_t *, void *);
extern void AddFirst(list_t *, void *);
extern void AppendList(list_t *, list_t *);
extern void RemoveFromList(list_t *, void *);
extern void *RemoveFirst(list_t *);
extern void DestroyList(list_t *, void (*)(void *));
extern void SetList(list_t *);
extern void *GetListElement(list_t *);
extern void *GetListNumElement(list_t *, int);
extern int EmptyList(list_t *);
extern int InList(list_t *, void *);
extern int SizeOfList(list_t *);
extern void SwapWithNextNeighbor(list_t *, void *);

#ifdef __cplusplus
}
#endif

#endif /* !_USEFUL_LIST_H */
