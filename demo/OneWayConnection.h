#pragma once

#include <memory>

class IStream; class OStream;

class OneWayConnection
{
public:
    OneWayConnection();
    static auto make() { return std::make_shared<OneWayConnection>(); }
    std::shared_ptr<IStream> getReader() { return m_reader; }
    std::shared_ptr<OStream> getWriter() { return m_writer; }

    OneWayConnection(const OneWayConnection&) = delete;
    OneWayConnection& operator=(const OneWayConnection&) = delete;

private:
    std::shared_ptr<IStream> m_reader;
    std::shared_ptr<OStream> m_writer;
};
