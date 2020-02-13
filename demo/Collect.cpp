#include "Collect.h"
#include "IStream.h"
#include "Error.h"
#include "Value.h"
#include <iostream>

Collect::Collect(std::shared_ptr<IStream> in, size_t maxNumItems)
    : UnaryTerminator("collect", in), m_maxNumItems(maxNumItems)
{}

void Collect::apply()
{
    Value val;
    if (!getIn()->read(val)) {
        notifyDone();
        return;
    }
    
    if (!m_maxNumItems || m_values.size() < m_maxNumItems) {
        m_values.push_back(val);
    }
}
