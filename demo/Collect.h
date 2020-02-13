#pragma once

#include "UnaryTerminator.h"
#include <vector>

class Value;

class Collect : public UnaryTerminator
{
public:
    Collect(std::shared_ptr<IStream> in, size_t maxNumItems);
    void apply() override;

    const std::vector<Value>& getValues() const { return m_values; }

private:
    size_t m_maxNumItems;
    std::vector<Value> m_values;
};
