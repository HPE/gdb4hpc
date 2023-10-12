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

// header only
#include "proto_message.h"

std::unique_ptr<Bitset> ProtoMessageSet::fullBitset() const {
    std::unique_ptr<Bitset> result = nullptr;
    for (const auto& message : *this) {
        if (!result) {
            result = message.getBitset()->clone();
        } else {
            *result |= *message.getBitset();
        }
    }
    return result;
}
