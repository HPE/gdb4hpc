#pragma once

#include <memory>

class IStream; class OStream;

//! Simulator for the MRNet stream.
/*! The AsyncConnection represents a data pipe connecting a write on 
  one end to a reader on another.   

  Any read operations will block until data is available.
*/
class AsyncConnection
{
public:
    AsyncConnection();
    static auto make() { return std::make_shared<AsyncConnection>(); }

    std::shared_ptr<IStream> getReader() { return m_reader; }
    std::shared_ptr<OStream> getWriter() { return m_writer; }

    AsyncConnection(const AsyncConnection&) = delete;
    AsyncConnection& operator=(const AsyncConnection&) = delete;

private:
    std::shared_ptr<IStream> m_reader;
    std::shared_ptr<OStream> m_writer;
};
