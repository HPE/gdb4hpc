#pragma once

#include <memory>

class ByteStream;

//! Base class for the type data streams.  
class Stream
{
public:
    enum Type { UNKNOWN, END, INT, DOUBLE };

    Stream(std::shared_ptr<ByteStream> byteStream);

    virtual ~Stream() = default;
    
protected:

    const int SENTINAL = 0x7bc0981;

    std::shared_ptr<ByteStream> m_byteStream;
};
