#include "WriteDataStream.h"
#include "ByteStream.h"
#include "Error.h"

WriteDataStream::WriteDataStream(std::shared_ptr<ByteStream> byteStream)
    : DataStream(byteStream)
{
}

void WriteDataStream::putDouble(double val)
{
    putDoubles(&val, 1);
}

void WriteDataStream::putDoubles(const double* val, size_t cnt)
{
    writeType(DOUBLE, cnt);
    m_byteStream->write(cnt*sizeof(double), reinterpret_cast<const char*>(val));
}

void WriteDataStream::putInt(int val)
{
    putInts(&val, 1);
}

void WriteDataStream::putInts(const int* val, size_t cnt) {
    writeType(INT, cnt);
    m_byteStream->write(cnt*sizeof(int), reinterpret_cast<const char*>(val));
}

void WriteDataStream::putIntL(int val)
{
    m_byteStream->write(sizeof(int), reinterpret_cast<const char*>(&val));
}

void WriteDataStream::writeType(Type type, size_t repeat)
{
    // write out a little sentinal for sanity checking
    putIntL(SENTINAL);
    putIntL(type);
    putIntL(int(repeat));  // not going to chuff around with putSize_t for our purposes
}

