#include "IStream.h"
#include "OStream.h"
#include "ByteStream.h"
#include "Error.h"
#include "Value.h"
#include <iostream>

IStream::IStream(std::shared_ptr<ByteStream> byteStream)
    : Stream(byteStream), m_currentType(UNKNOWN), m_remainingOfType(0), 
      m_headIsClosed(false)
{}

double IStream::getDouble() 
{
    if (!m_remainingOfType) {
        readType();
    }

    if (m_currentType == END) {
        throw Error("unexpected EOF");
    }

    if (m_currentType != DOUBLE) {
        throw Error("expected a double");
    }

    double val;
    m_byteStream->read(sizeof(double), reinterpret_cast<char*>(&val));
    --m_remainingOfType;

    return val;
}

int IStream::getInt()
{
    if (!m_remainingOfType)
        readType();

    if (m_currentType == END) {
        throw Error("unexpected EOF");
    }

    if (m_currentType != INT) {
        throw Error("expected an int");
    }

    int val = getIntL();
    --m_remainingOfType;

    return val;
}

bool IStream::read(Value& val)
{
    if (!m_remainingOfType)
        readType();
    
    if (m_currentType == INT) 
        val = getInt();
    else if (m_currentType == DOUBLE)
        val = getDouble();
    else if (m_currentType == END)
        return false;
    else
        throw Error("unknown type");

    return true;
}

bool IStream::hasData() const
{
    return m_byteStream->hasData();
}

void IStream::readType()
{
    int sentinal = getIntL();
    if (sentinal != SENTINAL) {
        throw Error("record mis-match on read");
    }

    int ttype = getIntL();
    if (ttype <= UNKNOWN || ttype > DOUBLE) {
        throw Error("expected a type index, got " + std::to_string(ttype));
    }
    m_currentType = Type(ttype);

    m_remainingOfType = getIntL();
}

int IStream::getIntL()
{
    int val;
    m_byteStream->read(sizeof(int), reinterpret_cast<char*>(&val));    
    return val;
}

void IStream::close()
{
    std::cerr << "IStream::close is unimplemented\n";
}

void IStream::wait()
{
    m_byteStream->wait(1);
}

