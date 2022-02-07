/*********************************************************************************\
 * List routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>

/*
** This library needs to be made thread safe since threads
** are using unsafe routines inside of it.
**
** My implementation uses simple mutex locks, further design
** could be added to use read/write locks depending on if
** performance is impacted. Note that heavy inter-thread
** push/pops to the list should use simple mutex and heavy
** reads should use read/write locks. There is additional
** overhead with read/write locks.
*/
#include <pthread.h>

#include "alloc.h"
#include "defs.h"
#include "list.h"

list_t *NewList(void) {
    list_t *l;

    l = (list_t *)Alloc(sizeof(list_t));

    // initialize the mutex lock
    pthread_mutexattr_init(&l->_list_lock_attr);
    pthread_mutex_init(&l->l_lock, &l->_list_lock_attr);

    l->l_head = (liste_t *)NULL;
    l->l_tail = &l->l_head;
    l->l_nel = 0;
    l->l_scan = NULL;

    return l;
}

/*
** Add new element to end of list.
*/
void AddToList(list_t *l, void *v) {
    liste_t *e;

    if (l == (list_t *)NULL) return;

    e = (liste_t *)Alloc(sizeof(liste_t));

    e->l_value = v;
    e->l_next = (liste_t *)NULL;

    // thread safe
    pthread_mutex_lock(&l->l_lock);

    *(l->l_tail) = e;

    l->l_tail = &e->l_next;
    l->l_nel++;

    pthread_mutex_unlock(&l->l_lock);
}

/*
** Add new element to beginning of list (stack emulation).
*/
void AddFirst(list_t *l, void *v) {
    liste_t *e;
    liste_t *ep;

    if (l == (list_t *)NULL) return;

    e = (liste_t *)Alloc(sizeof(liste_t));

    // thread safe
    pthread_mutex_lock(&l->l_lock);

    e->l_value = v;
    e->l_next = l->l_head;

    /*
    ** Adjust tail if necessary.
    */
    if ((ep = l->l_head) != (liste_t *)NULL && ep->l_next == (liste_t *)NULL)
        l->l_tail = &ep->l_next;

    l->l_head = e;
    l->l_nel++;

    // thread safe
    pthread_mutex_unlock(&l->l_lock);
}

void AppendList(list_t *dst, list_t *src) {
    void *e;

    SetList(src);

    while ((e = GetListElement(src)) != (void *)NULL) AddToList(dst, e);
}

void RemoveFromList(list_t *l, void *v) {
    liste_t *e;
    liste_t *ep;

    if (l == (list_t *)NULL || l->l_nel == 0) return;

    // thread safe
    pthread_mutex_lock(&l->l_lock);

    for (ep = e = l->l_head; e != (liste_t *)NULL;) {
        if (e->l_value != v) {
            if (ep != e) ep = ep->l_next;
            e = e->l_next;
            continue;
        }

        if (l->l_scan == e) l->l_scan = ep;

        if (ep == e) {
            l->l_head = e->l_next;

            if (e->l_next == (liste_t *)NULL) l->l_tail = &l->l_head;
        } else {
            ep->l_next = e->l_next;

            if (e->l_next == (liste_t *)NULL) l->l_tail = &ep->l_next;
        }
        break;
    }

    if (e != NULL) l->l_nel--;

    // thread safe
    pthread_mutex_unlock(&l->l_lock);

    if (e == NULL) return;

    Free(e);
}

/*
** Remove first element of list (stack emulation)
*/
void *RemoveFirst(list_t *l) {
    void *v;
    liste_t *e;

    if (l == (list_t *)NULL || l->l_nel == 0) return (void *)NULL;

    // thread safe
    pthread_mutex_lock(&l->l_lock);

    e = l->l_head;

    if (l->l_scan == e) l->l_scan = e->l_next;

    l->l_head = e->l_next;

    if (e->l_next == (liste_t *)NULL) l->l_tail = &l->l_head;

    v = e->l_value;

    l->l_nel--;

    // thread safe
    pthread_mutex_unlock(&l->l_lock);

    Free(e);

    return v;
}

void DestroyList(list_t *l, void (*destroy)(void *)) {
    liste_t *ep;
    liste_t *en;

    // Not thread-safe. Need to ensure all threads have joined
    // before destroying a list. This could be fixed with semaphores.

    if (l == (list_t *)NULL) return;

    ep = l->l_head;

    while (ep != (liste_t *)NULL) {
        en = ep->l_next;

        if (destroy != NULL) destroy(ep->l_value);

        Free(ep);

        ep = en;
    }

    // Destroy the mutex lock
    pthread_mutexattr_destroy(&l->_list_lock_attr);
    pthread_mutex_destroy(&l->l_lock);

    Free(l);
}

void SetList(list_t *l) {
    if (l == (list_t *)NULL) return;

    // thread safe
    pthread_mutex_lock(&l->l_lock);

    l->l_scan = l->l_head;

    // thread safe
    pthread_mutex_unlock(&l->l_lock);
}

void *GetListElement(list_t *l) {
    liste_t *le;

    if (l == (list_t *)NULL) return (void *)NULL;

    // thread safe
    pthread_mutex_lock(&l->l_lock);

    if (l->l_scan == (liste_t *)NULL) {
        pthread_mutex_unlock(&l->l_lock);
        return (void *)NULL;
    }

    le = l->l_scan;

    l->l_scan = le->l_next;

    // thread safe
    pthread_mutex_unlock(&l->l_lock);

    return le->l_value;
}

void *GetListNumElement(list_t *l, int num) {
    if (l == (list_t *)NULL) return (void *)NULL;

    // thread safe
    pthread_mutex_lock(&l->l_lock);

    liste_t *le = l->l_head;

    for (int i = 0; le && i < num; ++i) le = le->l_next;

    void *v = (le ? le->l_value : NULL);

    // thread safe
    pthread_mutex_unlock(&l->l_lock);

    return v;
}

int EmptyList(list_t *l) { return (int)(l == (list_t *)NULL || l->l_nel == 0); }

int InList(list_t *l, void *v) {
    liste_t *e;

    if (l == (list_t *)NULL) return 0;

    // thread safe
    pthread_mutex_lock(&l->l_lock);

    for (e = l->l_head; e != (liste_t *)NULL; e = e->l_next) {
        if (e->l_value == v) {
            // thread safe
            pthread_mutex_unlock(&l->l_lock);
            return 1;
        }
    }

    // thread safe
    pthread_mutex_unlock(&l->l_lock);
    return 0;
}

int SizeOfList(list_t *l) {
    if (l == (list_t *)NULL) return 0;

    return l->l_nel;
}

void SwapWithNextNeighbor(list_t *l, void *v) {
    liste_t *e1;
    liste_t *e2;
    liste_t *e3;
    liste_t *e4;

    if (l == (list_t *)NULL ? 1 : (l->l_nel < 2)) return;

    // thread safe
    pthread_mutex_lock(&l->l_lock);

    for (e1 = e2 = l->l_head; e2 != (liste_t *)NULL;) {
        if (e2->l_value != v) {
            // only want to advance e1 after the first time through
            if (e2 != e1) e1 = e1->l_next;
            e2 = e2->l_next;
            continue;
        }

        if (e1 == e2) {
            // This means the element to swap is the head of the list
            // e1 will be the new head
            e1 = e1->l_next;
            // e3 is what the new head used to point to
            e3 = e1->l_next;

            // change the current list head to point at the new head
            l->l_head = e1;

            // if the current list scan is pointing at the old head, update it
            // to point at the new head
            if (l->l_scan == e2) {
                l->l_scan = e1;
                // do the same if it was pointing at the new head
            } else if (l->l_scan == e1) {
                l->l_scan = e2;
            }

            // change the new head to point at the old head
            e1->l_next = e2;
            // change the old head to point at what the new head used to point
            // at
            e2->l_next = e3;

            // if e3 is null, then update the tail to point at e2's next
            if (e3 == (liste_t *)NULL) l->l_tail = &e2->l_next;

            break;
        } else {
            // e3 is now going to be e2
            e3 = e2;
            // point e2 at the old e3
            e2 = e2->l_next;

            // ensure the old e3 is not null, if so we can't swap it
            if (e2 == (liste_t *)NULL) break;

            // point e4 at what the old e3 was pointing at
            e4 = e2->l_next;

            // point e1 at the new e2
            e1->l_next = e2;
            // point e2 at the new e3
            e2->l_next = e3;
            // point the new e3 at what the old e3 was pointing at
            e3->l_next = e4;

            // if the current list scan is pointing at the old e2, update it to
            // point at the new e2
            if (l->l_scan == e3) {
                l->l_scan = e2;
                // do the same if it was pointing at the old e3
            } else if (l->l_scan == e2) {
                l->l_scan = e3;
            }

            // if e4 is null, then update the tail to point at e3's next
            if (e4 == (liste_t *)NULL) l->l_tail = &e3->l_next;
        }
        break;
    }

    // thread safe
    pthread_mutex_unlock(&l->l_lock);

    return;
}
