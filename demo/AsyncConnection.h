#pragma once

#include <memory>

class IStream; class OStream;

class AsyncConnection
{
public:
    AsyncConnection();
    static auto make() { return std::make_shared<AsyncConnection>(); }
    std::shared_ptr<IStream> getReader() { return m_reader; }
    std::shared_ptr<OStream> getWriter() { return m_writer; }

    void start();

    AsyncConnection(const AsyncConnection&) = delete;
    AsyncConnection& operator=(const AsyncConnection&) = delete;

private:
    std::shared_ptr<IStream> m_reader;
    std::shared_ptr<OStream> m_writer;
};
