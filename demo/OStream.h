#pragma once

#include "Stream.h"
#include <memory>

class Notifier;

class OStream : public Stream
{
public:
    OStream(std::shared_ptr<ByteStream> byteStream);

    void putDouble(double val);
    void putDoubles(const double* vals, size_t cnt);

    void putInt(int val);
    void putInts(const int* vals, size_t cnt);

    void eof();
    void eor();
    void cancel();

    void setNotifier(std::shared_ptr<Notifier> notifier);

private:
    void writeType(Type type, size_t repeat);
    void putIntL(int val);

    std::shared_ptr<Notifier> m_notifier;
};



