/******************************************************************************\
 * proto_message.c - protocol message interface
 *
 * Copyright 2013-2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "bitset.h"
#include "defs.h"
#include "list.h"
#include "proto_message.h"
#include "useful.h"

// The following functions are used to pass protocol messages up to the dbgclnt
// layer where they will be turned into the proper event type.

proto_message *NewProtoMsg(bitset *group, char *msg) {
    proto_message *res = (proto_message *)Alloc(sizeof(proto_message));
    res->group = group;
    res->message = msg;

    return res;
}

void FreeProtoMsg(proto_message *msg) {
    if (msg == NULL) return;

    bitset_free(msg->group);
    Free(msg->message);
    Free(msg);
}

proto_msg_set *NewProtoMsgSet(void) {
    proto_msg_set *set = (proto_msg_set *)Alloc(sizeof(proto_msg_set));
    set->msg_list = NewList();

    return set;
}

void AddProtoMsgSet(proto_msg_set *set, proto_message *msg) {
    if (set == NULL || msg == NULL) return;

    AddToList(set->msg_list, msg);
}

void ResetProtoMsgSet(proto_msg_set *set) {
    if (set == NULL) return;

    SetList(set->msg_list);
}

proto_message *NextProtoMsgInSet(proto_msg_set *set) {
    if (set != NULL)
        return (proto_message *)GetListElement(set->msg_list);
    else
        return NULL;
}

proto_message *RemoveProtoMsgInSet(proto_msg_set *set) {
    if (set == NULL) return NULL;

    return (proto_message *)RemoveFirst(set->msg_list);
}

void FreeProtoMsgSet(proto_msg_set *set) {
    if (set == NULL) return;

    DestroyList(set->msg_list, (void (*)(void *))FreeProtoMsg);
    Free(set);
}
