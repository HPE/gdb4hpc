#pragma once

#include "Stream.h"
#include <memory>

class Value;

//! Output stream to a typed data pipe.
/*! The protocol is trivial - just doubles and ints */
class OStream : public Stream
{
public:
    OStream(std::shared_ptr<ByteStream> byteStream);

    void putDouble(double val);
    void putDoubles(const double* vals, size_t cnt);

    void putInt(int val);
    void putInts(const int* vals, size_t cnt);

    void putValue(const Value& val);

    void eof();
    void cancel();

private:
    void writeType(Type type, size_t repeat);
    void putIntL(int val);
};



