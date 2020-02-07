#pragma once

#include <memory>

class ReadDataStream; class WriteDataStream;

class OneWayConnection
{
public:
    OneWayConnection();
    static auto make() { return std::make_shared<OneWayConnection>(); }
    std::shared_ptr<ReadDataStream> getReader() { return m_reader; }
    std::shared_ptr<WriteDataStream> getWriter() { return m_writer; }

    OneWayConnection(const OneWayConnection&) = delete;
    OneWayConnection& operator=(const OneWayConnection&) = delete;

private:
    std::shared_ptr<ReadDataStream> m_reader;
    std::shared_ptr<WriteDataStream> m_writer;
};
