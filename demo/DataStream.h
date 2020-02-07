#pragma once

#include <memory>

class ByteStream;

class DataStream
{
public:
    enum Type { UNKNOWN, INT, DOUBLE };

    DataStream(std::shared_ptr<ByteStream> byteStream);

    virtual ~DataStream() = default;
    
protected:

    const int SENTINAL = 0x7bc0981;

    std::shared_ptr<ByteStream> m_byteStream;
};
