#pragma once

#include "UnaryTerminator.h"

class More : public UnaryTerminator
{
public:
    More(std::shared_ptr<IStream> in, size_t numItems);
    virtual void apply();
private:
    size_t m_maxItems;
    size_t m_numPrinted;
    bool m_quit;
};

