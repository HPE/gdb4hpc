#pragma once

#include <deque>
#include <condition_variable>
#include <mutex>

class ByteStream
{
public:
    ByteStream();
    
    void read(size_t elems, char* val);
    void write(size_t elems, const char* val);

    bool hasData() const;
private:
    mutable std::mutex m_mutex;
    std::condition_variable m_ready;
    std::deque<char> m_data;
};
