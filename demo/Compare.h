
#pragma once
#include "BinaryOp.h"

class OpBuilder;

//! Create the difference of two streams
/*! For each pair of input values, create the output
  a-b.   This operator should probably be renamed diff.
*/
class Compare : public BinaryOp
{
public:
    Compare();
    static std::shared_ptr<Op> make(OpBuilder& builder);
    void apply(const Value& a, const Value& b) override;
};

