/*********************************************************************************\
 * Queue definitions
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

#ifndef _USEFUL_QUEUE_H
#define _USEFUL_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

struct qel_t {
    void *q_el;
    struct qel_t *q_next;
};
typedef struct qel_t qel_t;

struct tkqueue_t {
    int q_nel;  /* number of elements in queue */
    int q_qlen; /* maximum length of queue */
    qel_t *q_head;
    qel_t *q_tail;
};
typedef struct tkqueue_t tkqueue_t;

tkqueue_t *NewQueue(int);
void DestroyQueue(tkqueue_t *);
void *RemoveFromQueue(tkqueue_t *);
void *HeadOfQueue(tkqueue_t *);
void AddToQueue(tkqueue_t *, void *);
int EmptyQueue(tkqueue_t *);
int FullQueue(tkqueue_t *);

#ifdef __cplusplus
}
#endif

#endif /* !_USEFUL_QUEUE_H */
