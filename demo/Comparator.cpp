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

    more.wait();
}

