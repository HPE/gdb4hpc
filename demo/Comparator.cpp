#include "Comparator.h"
#include "IStream.h"
#include "OStream.h"
#include "Error.h"
#include "Compare.h"
#include "Connection.h"
#include "More.h"

#include <iostream>

Comparator::Comparator(std::shared_ptr<IStream> fromA, std::shared_ptr<OStream> toA,
                       std::shared_ptr<IStream> fromB, std::shared_ptr<OStream> toB)
    : m_fromA(fromA), m_toA(toA), m_fromB(fromB), m_toB(toB)
{
    
}

void Comparator::compare(const std::string& expression)
{
    Connection output;
    Compare compare(m_fromA, m_fromB, output.getWriter());

    More more(output.getReader(), 5);

    // send out the clowns
    m_toA->putInt(12);
    m_toB->putInt(12);

    while (true) {}

    /*
    std::any val;
    while (output.getReader()->read(val)) {}
    */

    /*
    int typeA = m_fromA->getInt();
    if (typeA != Stream::DOUBLE && typeA != Stream::INT) {
        throw Error("unrecognized type index " + std::to_string(typeA));
    }

    int typeB = m_fromB->getInt();
    if (typeB != Stream::DOUBLE && typeB != Stream::INT) {
        throw Error("unrecognized type index " + std::to_string(typeB));
    }

    if (typeA != typeB) {
        throw Error("not handling mismatched types");
    }

    bool isDouble = (typeA == Stream::DOUBLE);
    
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
    */
}

