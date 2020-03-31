#include "AsyncConnection.h"
#include "IStream.h"
#include "OStream.h"
#include "ByteStream.h"
#include "Error.h"

#include <thread>
#include <iostream>

AsyncConnection::AsyncConnection()
{
    auto byteStream = std::make_shared<ByteStream>();
    m_reader = std::make_shared<IStream>(byteStream);
    m_writer = std::make_shared<OStream>(byteStream);
}
