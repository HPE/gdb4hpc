#pragma once

#include "Stream.h"
#include <mutex>

class OStream; class Value;

class IStream : public Stream
{
public:
    IStream(std::shared_ptr<ByteStream> byteStream);

    double getDouble();
    int getInt();

    bool read(Value& val);

    bool hasData() const; // temporary?

    void close();

    void wait();

private:
    void readType();
    int getIntL();

    Type m_currentType;
    size_t m_remainingOfType;
    bool m_headIsClosed;
};
