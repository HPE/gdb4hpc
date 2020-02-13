#include "Range.h"
#include "IStream.h"
#include "OStream.h"
#include "Value.h"
#include "OpFactory.h"
#include "OpBuilder.h"

static OpFactory factory("range", Range::make);

Range::Range(std::shared_ptr<IStream> in, std::shared_ptr<OStream> out)
    : UnaryOp("range", in, out)
{}

std::shared_ptr<Op> Range::make(OpBuilder& builder)
{
    if (builder.getNumIStreams() != 1) {
        std::cerr << "$range needs an input\n";
        return nullptr;
    }

    return std::make_shared<Range>(builder.getNthIStream(0), builder.makeOStream());
}

void Range::apply()
{
    Value val;
    if (getIn()->read(val)) {
        if (!m_min.isDefined() || val < m_min)
            m_min = val;
        if (!m_max.isDefined() || val > m_max)
            m_max = val;
    } else {
        getOut()->putValue(m_min);
        getOut()->putValue(m_max);
        getOut()->eof();
    }
}

