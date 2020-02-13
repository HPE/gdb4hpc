#pragma once

#include "UnaryTerminator.h"
class OpBuilder;

class More : public UnaryTerminator
{
public:
    More(std::shared_ptr<IStream> in, size_t numItems);

    static std::shared_ptr<Op> make(OpBuilder& builder);

    virtual void apply();
private:
    size_t m_maxItems;
    size_t m_numPrinted;
    bool m_quit;
};

