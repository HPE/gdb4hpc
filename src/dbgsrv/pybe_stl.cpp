// Copyright 2024 Hewlett Packard Enterprise Development LP.

#include "pybe_stl.h"

#include <cstddef>

namespace pybe {

Expected<Thread, std::nullptr_t> Thread::make
    (void* (*start_fn)(void *), void* start_arg)
{
    auto attr = ::pthread_attr_t{};
    if (::pthread_attr_init(&attr)) {
        // TODO return errno or strerror
        return nullptr;
    }

    auto pthread = ::pthread_t{};
    if (::pthread_create(&pthread, &attr, start_fn, start_arg)) {
        // TODO return errno or strerror
        return nullptr;
    }

    return Thread(pthread, attr);
}

Thread::Thread(::pthread_t id, ::pthread_attr_t attrs)
    : m_id{id}
    , m_attrs{attrs}
{}

Thread::Thread(Thread&& other)
    : m_id{other.m_id}
    , m_attrs{other.m_attrs}
{
    other.m_id = 0;
    // TODO: confirm that it's ok to leave other.m_attrs untouched
}

Thread::~Thread() {
    // our ID might be 0 after a std::move
    if (m_id != 0) {
        if (!m_detached) {
            // mimicking std::thread.
            // TODO support return values
            ::pthread_join(m_id, nullptr);
        }
    }
}

[[ nodiscard ]]
int Thread::detach() {
    int rc = ::pthread_detach(m_id);
    if (rc == 0) m_detached = true;
    return rc;
}

} // namespace pybe

