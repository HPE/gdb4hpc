#pragma once

#include "Stream.h"
#include <mutex>

class Value;

//! Stream for reading typed data.
/*! The protocol is trivial, so it only supports
  ints and doubles. */
class IStream : public Stream
{
public:
    IStream(std::shared_ptr<ByteStream> byteStream);

    // lookat: keep these or not?
    double getDouble();
    int getInt();

    // Block until data is ready.  Return false if eof.
    bool read(Value& val);

private:
    void readType();
    int getIntL();

    Type m_currentType;
    size_t m_remainingOfType;
    bool m_headIsClosed;
};
