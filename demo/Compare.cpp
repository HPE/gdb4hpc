
#include "Compare.h"
#include "IStream.h"
#include "OStream.h"
#include "Error.h"
#include "Value.h"
#include <iostream>

Compare::Compare(std::shared_ptr<IStream> a, std::shared_ptr<IStream> b, std::shared_ptr<OStream> diffs)
    : BinaryOp(a, b, diffs)
{
    m_eof[0] = m_eof[1] = false;
}

void Compare::apply() 
{
    Value v1, v2;

    // lookat: IStream is just looking at ByteStream->hasData which isn't smart enough to 
    // know the next thing is EOF.   I.e. we've read the last available data and the *next*
    // read will fail.   So for now, handle the read failure here.
    bool in1 = !m_eof[0] && getIn1()->read(v1);
    bool in2 = !m_eof[1] && getIn2()->read(v2);

    m_eof[0] = !in1;
    m_eof[1] = !in2;

    if (in1 && in2) {
        getOut()->putValue(v1-v2);
        return;
    } else if (m_eof[0] && m_eof[1]) {
        getOut()->eof();
    }
}
