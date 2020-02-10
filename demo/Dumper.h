#pragma once

#include <memory>

class IStream; class OStream;

class Dumper
{
public:
    Dumper(std::shared_ptr<IStream> dataIn, std::shared_ptr<OStream> commandOut);

    void dumpVals();
private:
    std::shared_ptr<IStream> m_dataIn;
    std::shared_ptr<OStream> m_commandOut;
};

