#include "ReadDataStream.h"
#include "ByteStream.h"
#include "Error.h"

ReadDataStream::ReadDataStream(std::shared_ptr<ByteStream> byteStream)
    : DataStream(byteStream), m_currentType(UNKNOWN), m_remainingOfType(0)
{}

double ReadDataStream::getDouble() 
{
    if (!m_remainingOfType)
        readType();

    if (m_currentType != DOUBLE) {
        throw Error("expected a double");
    }

    double val;
    m_byteStream->read(sizeof(double), reinterpret_cast<char*>(&val));
    --m_remainingOfType;

    return val;
}

int ReadDataStream::getInt()
{
    if (!m_remainingOfType)
        readType();

    if (m_currentType != INT) {
        throw Error("expected an int");
    }

    int val = getIntL();
    --m_remainingOfType;

    return val;
}

bool ReadDataStream::hasData() const
{
    return m_byteStream->hasData();
}

void ReadDataStream::readType()
{
    int sentinal = getIntL();
    if (sentinal != SENTINAL) {
        throw Error("record mis-match on read");
    }
    int ttype = getIntL();
    if (ttype < 0 || ttype > DOUBLE) {
        throw Error("expected a type index, got " + std::to_string(ttype));
    }

    m_currentType = Type(ttype);
    m_remainingOfType = getIntL();
}

int ReadDataStream::getIntL()
{
    int val;
    m_byteStream->read(sizeof(int), reinterpret_cast<char*>(&val));    
    return val;
}

