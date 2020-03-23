#pragma once

#include "UnaryTerminator.h"
#include <vector>

class Value;

//! Operator to collect the stream input into a vector in memory.
/*! Store up to the first maxNumItems values from the input
  into memory.
*/
class Collect : public UnaryTerminator
{
public:
    Collect(size_t maxNumItems);
    void apply(const Value& val) override;

    const std::vector<Value>& getValues() const { return m_values; }

private:
    size_t m_maxNumItems;
    std::vector<Value> m_values;
};
