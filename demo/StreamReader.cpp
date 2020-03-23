#include "StreamReader.h"
#include "Error.h"
#include "IStream.h"
#include "UnaryOp.h"
#include "Value.h"
#include <thread>

StreamReader::StreamReader(std::shared_ptr<IStream> in) 
    : m_in(in)
{
}

void StreamReader::pipeTo(std::shared_ptr<UnaryOp> to)
{
    m_to = to;
}

void StreamReader::start() const
{
    if (!m_to)
        throw Error("no output set for stream");

    auto in = m_in;
    auto out = m_to;

    std::thread readThread([in,out]() {
            Value val;
            while (in->read(val))
                out->apply(val);

            out->done();
        });

    readThread.detach();
}
