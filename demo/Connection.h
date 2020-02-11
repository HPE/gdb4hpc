#pragma once

#include <memory>

class IStream; class OStream;

class Connection
{
public:
    Connection();
    static auto make() { return std::make_shared<Connection>(); }
    std::shared_ptr<IStream> getReader() { return m_reader; }
    std::shared_ptr<OStream> getWriter() { return m_writer; }

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

private:
    std::shared_ptr<IStream> m_reader;
    std::shared_ptr<OStream> m_writer;
};
