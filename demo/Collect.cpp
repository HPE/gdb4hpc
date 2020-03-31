#include "Collect.h"
#include "Value.h"
#include <iostream>

Collect::Collect(size_t maxNumItems)
    : UnaryTerminator("collect"), m_maxNumItems(maxNumItems)
{}

void Collect::apply(const Value& val)
{
    if (!m_maxNumItems || m_values.size() < m_maxNumItems) {
        m_values.push_back(val);
    }
}
