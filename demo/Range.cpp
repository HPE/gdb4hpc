#include "Range.h"
#include "Value.h"
#include "OpFactory.h"
#include "OpBuilder.h"

static OpFactory factory("range", Range::make);

Range::Range() : UnaryOp("range") {}

std::shared_ptr<Op> Range::make(OpBuilder& builder)
{
    return std::make_shared<Range>();
}

void Range::apply(const Value& val)
{
    if (!m_min.isDefined() || val < m_min)
        m_min = val;
    if (!m_max.isDefined() || val > m_max)
        m_max = val;
}

void Range::done() {
    getOutput()->apply(m_min);
    getOutput()->apply(m_max);
    UnaryOp::done();
}

