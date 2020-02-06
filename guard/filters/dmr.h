/*********************************************************************************\
 * <dmr.h>
 *
 * © 2012-2013 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#ifndef _DMR_H
#define _DMR_H

#include <iostream>
#include <vector>

#include "bitset.h"
#include "smart_bitset.h"

using namespace std;

namespace DMR /* DebuggingMessageReduction */
{
/*
A message consists of "content" and a bitset indicating the MPI ranks
*/
class Message {
   public:
    Message(char* mes, char* bt);
    ~Message();

    void SetReleaseMemory(bool release);
    char* GetContent();
    char* GetBitsetStr();
    void MergeBitset(char* btStr);
    int Compare(Message* pMessage);
    bool Equal(Message* pMessage);
    bool Less(Message* pMessage);
    bool LessEqual(Message* pMessage);

   private:
    bool m_release;
    char* m_content;
    char* m_btStr;
    smart_bitset* m_sb;
};

/*
A stream consists of a number of ordered messages.
*/
class MessageVector : public vector<Message*> {
   public:
    MessageVector(bool releaseMemory = true);
    ~MessageVector();

    void Initialization(char* messages[], int number);
    void Append(Message* pMessage);

    vector<Message*>::iterator Current();
    bool MoveNext();

   private:
    bool m_release;
    vector<Message*>::iterator m_iterator;
};

struct MessageComparison {
   public:
    static bool Less(MessageVector* left, MessageVector* right) {
        vector<Message*>::iterator leftIt = left->Current();
        vector<Message*>::iterator rightIt = right->Current();

        return (*leftIt)->Less(*rightIt);
    }

    static bool LessEqual(MessageVector* left, MessageVector* right) {
        vector<Message*>::iterator leftIt = left->Current();
        vector<Message*>::iterator rightIt = right->Current();

        return (*leftIt)->LessEqual(*rightIt);
    }
};

template <class T, class Compare>
class PriorityQueue {
   public:
    PriorityQueue();
    ~PriorityQueue();

    void Initialization(int capacity);
    void Clear();

    int Size();

    bool Insert(T* insert);

    void Adjust();
    T* Top();
    T* Pop();

   private:
    T** m_heap;

    int m_heapCapacity;
    int m_capacity;

    int m_size;

    void SiftUp();
    void SiftDown();
};

template <class T, class Compare>
PriorityQueue<T, Compare>::PriorityQueue() {
    m_capacity = 0;
    m_heapCapacity = 0;
    m_size = -1;

    m_heap = NULL;
}

template <class T, class Compare>
PriorityQueue<T, Compare>::~PriorityQueue() {
    if (m_heap != NULL) delete[] m_heap;

    m_capacity = 0;
    m_heapCapacity = 0;
    m_size = -1;
    m_heap = NULL;
}

template <class T, class Compare>
void PriorityQueue<T, Compare>::Initialization(int capacity) {
    if (capacity <= 0) return;

    m_capacity = capacity;
    m_heapCapacity = m_capacity + 1;

    m_heap = new T*[m_heapCapacity];

    m_size = 0;
}

template <class T, class Compare>
void PriorityQueue<T, Compare>::Clear() {
    if (m_heap == NULL) return;

    int index;
    for (index = 0; index <= m_size; index++) m_heap[index] = NULL;

    m_size = 0;
}

template <class T, class Compare>
int PriorityQueue<T, Compare>::Size() {
    return m_size;
}

template <class T, class Compare>
bool PriorityQueue<T, Compare>::Insert(T* insert) {
    if (m_heap != NULL && m_size < m_capacity) {
        ++m_size;

        m_heap[m_size] = insert;
        SiftUp();

        return true;
    }

    return false;
}

template <class T, class Compare>
void PriorityQueue<T, Compare>::Adjust() {
    SiftDown();
}

template <class T, class Compare>
T* PriorityQueue<T, Compare>::Top() {
    if (m_heap != NULL && m_size > 0) {
        return m_heap[1];
    }

    return NULL;
}

template <class T, class Compare>
T* PriorityQueue<T, Compare>::Pop() {
    if (m_heap == NULL) return NULL;

    T* pop = NULL;
    if (m_size > 0) {
        pop = m_heap[1];

        m_heap[1] = m_heap[m_size];
        --m_size;

        SiftDown();
    }

    return pop;
}

template <class T, class Compare>
void PriorityQueue<T, Compare>::SiftUp() {
    if (m_heap == NULL) return;

    int index = m_size;
    int parent = index / 2;

    T* sift = m_heap[index];
    while (parent != 0 && Compare::Less(sift, m_heap[parent])) {
        m_heap[index] = m_heap[parent];

        index = parent;
        parent = index / 2;
    }

    m_heap[index] = sift;
}

template <class T, class Compare>
void PriorityQueue<T, Compare>::SiftDown() {
    if (m_heap == NULL) return;

    int index = 1;
    int child = index * 2;
    T* down = m_heap[index];

    while (child <= m_size) {
        if (child + 1 <= m_size &&
            Compare::Less(m_heap[child + 1], m_heap[child]))
            child++;

        if (Compare::LessEqual(down, m_heap[child])) break;

        m_heap[index] = m_heap[child];

        index = child;
        child = 2 * index;
    }

    m_heap[index] = down;
}

}  // namespace DMR
#endif /*_DMR_H*/
