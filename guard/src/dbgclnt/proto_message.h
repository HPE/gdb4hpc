/******************************************************************************\
 * proto_message.h - protocol message interface
 *
 * Copyright 2013,2020-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#pragma once

#include <vector>

#include <stdlib.h>

#include "bitset.h"

class ProtoMessage {
public:
    // takes ownership of group and message
    ProtoMessage(bitset* group, char* message)
        : m_group{group}, m_message{message}
    {};

    // move instead of copy
    ProtoMessage(const ProtoMessage &other) = delete;
    ProtoMessage& operator= (const ProtoMessage &other) = delete;

    ProtoMessage(ProtoMessage &&other)
        : m_group{other.m_group}, m_message{other.m_message}
    {
        other.m_group = nullptr;
        other.m_message = nullptr;
    };

    ProtoMessage& operator= (ProtoMessage &&other) {
        clear();

        m_group = other.m_group;
        m_message = other.m_message;

        other.m_group = nullptr;
        other.m_message = nullptr;

        return *this;
    }

    ~ProtoMessage() {
        clear();
    }
    
    const bitset* getBitset() const { return m_group; };
    const char* getMessage() const { return m_message; };
private:
    void clear() {
        bitset_free(m_group);
        free(m_message);
    }

    bitset* m_group;
    char* m_message;
};

typedef std::vector<ProtoMessage> ProtoMessageSet;
