
#include "Compare.h"
#include "OpFactory.h"
#include "Value.h"
#include "OpBuilder.h"
#include "UnaryOp.h"
#include <iostream>

static OpFactory factory("compare", Compare::make);

Compare::Compare() : BinaryOp("compare") {}

std::shared_ptr<Op> Compare::make(OpBuilder& builder)
{
    return std::make_shared<Compare>();
}

void Compare::apply(const Value& a, const Value& b) 
{
    getOutput()->apply(a-b);
}

