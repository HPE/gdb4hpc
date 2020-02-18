#pragma once

#include "UnaryTerminator.h"
class OpBuilder;

class More : public UnaryTerminator
{
public:
    More(size_t numItems);

    static std::shared_ptr<Op> make(OpBuilder& builder);

    void apply(const Value& val) override;
private:
    size_t m_maxItems;
    size_t m_numPrinted;
    bool m_quit;
};

