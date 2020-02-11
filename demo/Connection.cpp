#include "Connection.h"
#include "IStream.h"
#include "OStream.h"
#include "ByteStream.h"

Connection::Connection()
{
    auto byteStream = std::make_shared<ByteStream>();
    m_reader = std::make_shared<IStream>(byteStream);
    m_writer = std::make_shared<OStream>(byteStream);

    m_reader->connectToHead(*m_writer);
}
