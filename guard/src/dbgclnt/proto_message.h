/******************************************************************************\
 * proto_message.h - protocol message interface
 *
 * Copyright 2013,2020 Hewlett Packard Enterprise Development LP.
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
 ******************************************************************************/

#ifndef _PROTO_MESSAGE_H
#define _PROTO_MESSAGE_H

#include "bitset.h"
#include "list.h"

// Used to pass unprocessed message strings up to the dbgclnt layer.
struct proto_message {
    bitset *group;
    char *message;
};
typedef struct proto_message proto_message;

struct proto_msg_set {
    list_t *msg_list;  // list of proto_message types
};
typedef struct proto_msg_set proto_msg_set;

extern proto_message *NewProtoMsg(bitset *group, char *msg);
extern void FreeProtoMsg(proto_message *msg);
extern proto_msg_set *NewProtoMsgSet(void);
extern void AddProtoMsgSet(proto_msg_set *set, proto_message *msg);
extern void ResetProtoMsgSet(proto_msg_set *set);
extern proto_message *NextProtoMsgInSet(proto_msg_set *set);
extern proto_message *RemoveProtoMsgInSet(proto_msg_set *set);
extern void FreeProtoMsgSet(proto_msg_set *set);

#endif /* ! _PROTO_MESSAGE_H */
