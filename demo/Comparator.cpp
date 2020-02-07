#include "Comparator.h"
#include "ReadDataStream.h"
#include "WriteDataStream.h"
#include "Error.h"
#include <iostream>

Comparator::Comparator(std::shared_ptr<ReadDataStream> fromA, std::shared_ptr<WriteDataStream> toA,
                       std::shared_ptr<ReadDataStream> fromB, std::shared_ptr<WriteDataStream> toB)
    : m_fromA(fromA), m_toA(toA), m_fromB(fromB), m_toB(toB)
{
}

void Comparator::compare(const std::string& expression)
{
    // send out the clowns
    m_toA->putInt(12);
    m_toB->putInt(12);

    int typeA = m_fromA->getInt();
    if (typeA != DataStream::DOUBLE && typeA != DataStream::INT) {
        throw Error("unrecognized type index " + std::to_string(typeA));
    }

    int typeB = m_fromB->getInt();
    if (typeB != DataStream::DOUBLE && typeB != DataStream::INT) {
        throw Error("unrecognized type index " + std::to_string(typeB));
    }

    if (typeA != typeB) {
        throw Error("not handling mismatched types");
    }

    bool isDouble = (typeA == DataStream::DOUBLE);
    
    int numElemsA = m_fromA->getInt();
    int numElemsB = m_fromB->getInt();

    if (numElemsA != numElemsB) {
        throw Error("not handling mismatched array sizes");
    }

    if (isDouble) {
        for (int i=0; i<numElemsA; ++i) {
            double a = m_fromA->getDouble();
            double b = m_fromB->getDouble();
            std::cout << i << " values " << a << " and " << b << " are " << (a==b ? "" : "not ") << "equal\n";
        }
    } else {
        for (int i=0; i<numElemsA; ++i) {
            int a = m_fromA->getInt();
            int b = m_fromB->getInt();
            std::cout << i << " values " << a << " and " << b << " are " << (a==b ? "" : "not ") << "equal\n";
        }

    }
}

