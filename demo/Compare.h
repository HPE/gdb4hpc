
#pragma once
#include "BinaryOp.h"

class IStream; class OStream; class OpBuilder;

class Compare : public BinaryOp
{
public:
    Compare();
    static std::shared_ptr<Op> make(OpBuilder& builder);
    void apply(const Value& a, const Value& b) override;
};

