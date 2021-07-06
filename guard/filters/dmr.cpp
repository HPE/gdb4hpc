/*********************************************************************************\
 * <dmr.cpp>
 *
 * Copyright 2012-2013,2020 Hewlett Packard Enterprise Development LP.
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

#include "dmr.h"

#include <stdlib.h>
#include <string.h>

#include <vector>

#include "sparse_bitset.h"

using namespace std;

namespace DMR /* DebuggingMessageReduction */
{

Message::Message(char* mes, char* bt)
    : m_release(true), m_content(mes), m_btStr(bt), m_sb() {}

Message::~Message() {
    if (m_content != NULL && m_release) {
        free(m_content);
    }
    m_content = NULL;
}

void Message::SetReleaseMemory(bool release) { m_release = release; }

char* Message::GetContent() { return m_content; }

Bitset::Serialized& Message::GetBitsetStr() {
    if (m_sb != nullptr) m_btStr = m_sb->serialize();

    return m_btStr;
}

/*
less (<0); equal (0); more (>0)
*/
int Message::Compare(Message* pMessage) {
    if (pMessage == NULL && m_content == NULL)
        return 0;
    else if (m_content == NULL)
        return -1;
    else if (pMessage == NULL)
        return 1;

    char* leftM = m_content;
    char* rightM = pMessage->GetContent();

    return strcmp(leftM, rightM);
}

bool Message::Less(Message* pMessage) {
    int comp = Compare(pMessage);
    return comp < 0;
}

bool Message::Equal(Message* pMessage) {
    int comp = Compare(pMessage);
    return comp == 0;
}

bool Message::LessEqual(Message* pMessage) {
    int comp = Compare(pMessage);
    return comp <= 0;
}

void Message::MergeBitset(const Bitset::Serialized &bitsetStr) {
    if (bitsetStr.get() == nullptr) return;

    auto sb = SparseBitset(bitsetStr);

    if (m_sb == nullptr) {
        m_sb = std::make_unique<SparseBitset>(m_btStr);
    }

    *m_sb |= sb;
}

MessageVector::MessageVector(bool releaseMemory) { m_release = releaseMemory; }

MessageVector::~MessageVector() {
    if (!m_release) return;

    Message* pMessage;
    MessageVector::iterator it;
    for (it = vector<Message*>::begin(); it < vector<Message*>::end(); ++it) {
        pMessage = *it;
        delete pMessage;
    }
}

void MessageVector::Initialization(char* messages[], int number) {
    int index;

    if (messages == NULL || number <= 0 || number % 2 != 0) return;

    for (index = 0; index < number; index += 2) {
        Message* pMessage = new Message(messages[index], messages[index + 1]);
        vector<Message*>::push_back(pMessage);
    }

    m_iterator = vector<Message*>::begin();
}

void MessageVector::Append(Message* pMessage) {
    Message* lastMessage;
    if (vector<Message*>::size() == 0)
        vector<Message*>::push_back(pMessage);
    else {
        lastMessage = vector<Message*>::back();
        if (lastMessage->Equal(pMessage)) {
            lastMessage->MergeBitset(pMessage->GetBitsetStr());
        } else {
            vector<Message*>::push_back(pMessage);
        }
    }
}

vector<Message*>::iterator MessageVector::Current() { return m_iterator; }

bool MessageVector::MoveNext() {
    m_iterator++;
    return (m_iterator < vector<Message*>::end());
}

}  // namespace DMR
