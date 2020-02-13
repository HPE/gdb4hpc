
#pragma once
#include "BinaryOp.h"

class IStream; class OStream; class OpBuilder;

class Compare : public BinaryOp
{
public:
    Compare(std::shared_ptr<IStream> a, std::shared_ptr<IStream> b, std::shared_ptr<OStream> diffs);

    static std::shared_ptr<Op> make(OpBuilder& builder);

    void apply() override;

private:
    bool m_eof[2];
    
};

