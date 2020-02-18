#include "Gt.h"
#include "IStream.h"
#include "OStream.h"
#include "Value.h"
#include "OpFactory.h"
#include "OpBuilder.h"

static OpFactory factory("gt", Gt::make);

Gt::Gt(const Value& limit)
    : UnaryOp("gt"), m_limit(limit)
{}

std::shared_ptr<Op> Gt::make(OpBuilder& builder)
{
    Value limit;
    std::string args = builder.getOpArgs();
    if (!args.empty()) {
        try {
            limit = std::stod(args);
        } catch (std::exception) {
            std::cerr << "expecting an numeric argument for $gt\n";
        }
    }

    return std::make_shared<Gt>(limit);
}

void Gt::apply(const Value& val)
{
    if (val > m_limit)
        getOutput()->apply(val);
}

