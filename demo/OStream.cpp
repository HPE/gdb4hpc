#include "OStream.h"
#include "ByteStream.h"
#include "Error.h"
#include "Value.h"
#include <iostream>

OStream::OStream(std::shared_ptr<ByteStream> byteStream)
    : Stream(byteStream)
{
}

void OStream::putDouble(double val)
{
    putDoubles(&val, 1);
}

void OStream::putDoubles(const double* val, size_t cnt)
{
    writeType(DOUBLE, cnt);
    m_byteStream->write(cnt*sizeof(double), reinterpret_cast<const char*>(val));
}

void OStream::putInt(int val)
{
    putInts(&val, 1);
}

void OStream::putValue(const Value& val) {
    int iv; double dv;
    if (val.to(iv))
        putInt(iv);
    else if (val.to(dv))
        putDouble(dv);
    else
        throw Error("unrecognized type");

}

void OStream::putInts(const int* val, size_t cnt) {
    writeType(INT, cnt);
    m_byteStream->write(cnt*sizeof(int), reinterpret_cast<const char*>(val));
}

void OStream::eof() {
    writeType(END,0);
}

void OStream::cancel() {
    std::cerr << "OStream::cancel is not implemented\n";
}

void OStream::putIntL(int val)
{
    m_byteStream->write(sizeof(int), reinterpret_cast<const char*>(&val));
}

void OStream::writeType(Type type, size_t repeat)
{
    // write out a little sentinal for sanity checking
    putIntL(SENTINAL);
    putIntL(type);
    putIntL(int(repeat));  // not going to chuff around with putSize_t for our purposes
}

