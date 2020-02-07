#pragma once

#include "DataStream.h"

class WriteDataStream : public DataStream
{
public:
    WriteDataStream(std::shared_ptr<ByteStream> byteStream);

    void putDouble(double val);
    void putDoubles(const double* vals, size_t cnt);

    void putInt(int val);
    void putInts(const int* vals, size_t cnt);

private:
    void writeType(Type type, size_t repeat);
    void putIntL(int val);
};



