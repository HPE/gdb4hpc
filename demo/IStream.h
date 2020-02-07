#pragma once

#include "DataStream.h"

class ReadDataStream : public DataStream
{
public:
    ReadDataStream(std::shared_ptr<ByteStream> byteStream);

    double getDouble();
    int getInt();

    bool hasData() const; // temporary?

private:
    void readType();
    int getIntL();

    Type m_currentType;
    size_t m_remainingOfType;
};
