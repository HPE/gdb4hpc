#pragma once

#include <deque>
#include <condition_variable>
#include <mutex>

//! Low level primitive data buffer for simulating the 
//! MRNet data transport.
/*! Note that the type aware protocol information is handled
  at a higher level - this guy is just slinging bytes.
*/
class ByteStream
{
public:
    ByteStream();
    
    void read(size_t elems, char* val);
    void write(size_t elems, const char* val);
    void wait(size_t minElems) const;
private:
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_ready;
    std::deque<char> m_data;
};
