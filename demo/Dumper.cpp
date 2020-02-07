#include "Dumper.h"
#include "WriteDataStream.h"
#include "ReadDataStream.h"
#include "Error.h"
#include <iostream>

Dumper::Dumper(std::shared_ptr<ReadDataStream> dataIn, std::shared_ptr<WriteDataStream> commandOut)
    : m_dataIn(dataIn), m_commandOut(commandOut)
{}

void Dumper::dumpVals()
{
    // it's just a dummy value thus far, but this will trigger the data output
    m_commandOut->putInt(12);
    
    int type = m_dataIn->getInt();
    if (type != DataStream::DOUBLE && type != DataStream::INT) {
        throw Error("unrecognized type index " + std::to_string(type));
    }

    bool isDouble = (type == DataStream::DOUBLE);
    int numElems = m_dataIn->getInt();

    if (isDouble) {
        for (int i=0; i<numElems; ++i) {
            std::cout << i << " " << m_dataIn->getDouble() << "\n";
        }
    } else {
        for (int i=0; i<numElems; ++i) {
            std::cout << i << " " << m_dataIn->getInt() << "\n";
        }
    }
}

