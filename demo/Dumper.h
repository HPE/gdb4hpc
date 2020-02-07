#pragma once

#include <memory>

class ReadDataStream; class WriteDataStream;

class Dumper
{
public:
    Dumper(std::shared_ptr<ReadDataStream> dataIn, std::shared_ptr<WriteDataStream> commandOut);

    void dumpVals();
private:
    std::shared_ptr<ReadDataStream> m_dataIn;
    std::shared_ptr<WriteDataStream> m_commandOut;
};

