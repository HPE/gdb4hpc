#include "ByteStream.h"
#include "Error.h"

ByteStream::ByteStream() {}
    
void ByteStream::read(size_t elems, char* val)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_data.size() < elems)
        m_ready.wait(lock);

    auto de = m_data.begin()+elems;
    copy(m_data.begin(), de, val);
    m_data.erase(m_data.begin(), de);
}

void ByteStream::write(size_t elems, const char* val)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_data.insert(m_data.end(), val, val+elems);
    m_ready.notify_all();
}

bool ByteStream::hasData() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return !m_data.empty();
}

