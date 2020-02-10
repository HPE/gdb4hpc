#pragma once

#include <memory>

class ByteStream;

class Stream
{
public:
    enum Type { UNKNOWN, END, INT, DOUBLE };

    enum State { DATA_READY, END_OF_FILE, CANCEL };

    Stream(std::shared_ptr<ByteStream> byteStream);

    virtual ~Stream() = default;
    
protected:

    const int SENTINAL = 0x7bc0981;

    std::shared_ptr<ByteStream> m_byteStream;
};
