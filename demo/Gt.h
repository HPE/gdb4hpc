#pragma once

#include "UnaryOp.h"
#include "Value.h"

class OpBuilder;

//! Greater than filter.
/*! Output only the input values that are greater than \a limit.
 */
class Gt : public UnaryOp
{
public:
    Gt(const Value& limit);
    static std::shared_ptr<Op> make(OpBuilder& builder);
    void apply(const Value& val);
private:
    Value m_limit;
};
