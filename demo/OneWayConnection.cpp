#include "OneWayConnection.h"
#include "ReadDataStream.h"
#include "WriteDataStream.h"
#include "ByteStream.h"

OneWayConnection::OneWayConnection()
{
    auto byteStream = std::make_shared<ByteStream>();
    m_reader = std::make_shared<ReadDataStream>(byteStream);
    m_writer = std::make_shared<WriteDataStream>(byteStream);
}
