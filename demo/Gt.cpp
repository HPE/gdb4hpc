#include "Gt.h"
#include "IStream.h"
#include "OStream.h"
#include "Value.h"
#include "OpFactory.h"
#include "OpBuilder.h"

static OpFactory factory("gt", Gt::make);

Gt::Gt(std::shared_ptr<IStream> in, std::shared_ptr<OStream> out, Value limit)
    : UnaryOp("gt", in, out), m_limit(limit)
{}

std::shared_ptr<Op> Gt::make(OpBuilder& builder)
{
    if (builder.getNumIStreams() != 1) {
        std::cerr << "$gt needs an input\n";
        return nullptr;
    }

    Value limit;
    std::string args = builder.getOpArgs();
    if (!args.empty()) {
        try {
            limit = std::stod(args);
        } catch (std::exception) {
            std::cerr << "expecting an numeric argument for $gt\n";
        }
    }

    return std::make_shared<Gt>(builder.getNthIStream(0), builder.makeOStream(), limit);
}

void Gt::apply()
{
    Value val;
    if (getIn()->read(val)) {
        if (val > m_limit)
            getOut()->putValue(val);
    } else {
        getOut()->eof();
    }
}

