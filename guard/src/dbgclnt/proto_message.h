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
private:
    using unique_c_char_pointer = std::unique_ptr<char, decltype(&std::free)>;

public:
    // takes ownership of group and message.
    ProtoMessage(std::unique_ptr<Bitset> &&group, unique_c_char_pointer &&message)
        : m_group{std::move(group)}, m_message{std::move(message)}
    {};
    // helper for raw char pointers. takes ownership of group and message.
    ProtoMessage(std::unique_ptr<Bitset> &&group, char *&&message)
        : ProtoMessage(std::move(group), unique_c_char_pointer(message, std::free))
    {
        message = nullptr;
    };

    ProtoMessage(ProtoMessage &&other)
        : m_group{std::move(other.m_group)}, m_message{std::move(other.m_message)}
    {};

    ProtoMessage& operator= (ProtoMessage &&other) {
        m_group = std::move(other.m_group);
        m_message = std::move(other.m_message);

        return *this;
    }
    
    const Bitset* getBitset() const { return m_group.get(); };
    const char* getMessage() const { return m_message.get(); };

private:
    std::unique_ptr<Bitset> m_group;
    unique_c_char_pointer m_message;
};

typedef std::vector<ProtoMessage> ProtoMessageSet;
