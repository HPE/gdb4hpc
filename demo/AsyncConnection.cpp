#include "AsyncConnection.h"
#include "IStream.h"
#include "OStream.h"
#include "ByteStream.h"
#include "Error.h"

#include <thread>

AsyncConnection::AsyncConnection()
{
    auto byteStream = std::make_shared<ByteStream>();
    m_reader = std::make_shared<IStream>(byteStream);
    m_writer = std::make_shared<OStream>(byteStream);

    m_reader->connectToHead(*m_writer);
}

void AsyncConnection::start()
{
    std::shared_ptr<IStream> reader = m_reader;

    /*
    if (!reader->getNotifier()) {
        throw Error("reader has to have a notifier");
    }
    */

    std::thread t([reader]() {
            while (true) {
                reader->waitAndNotify();
            }
        });
    
    t.detach();
}
