// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// ... TODO ...

#include "pythoncapi_compat.h"
#include <csignal>

#include "pybe_stl.h"

namespace pybe {

// TODO: name these functions better

void* get_GIL_then_breakpoint(void*) {
    auto gil = PyGILState_Ensure();
    ::raise(SIGTRAP);
    // gdb does its thing here...
    // ...
    // then gdb sends a continue
    PyGILState_Release(gil);
    return nullptr;
}

bool spawn_GIL_grabbing_thread() {
    // TODO block all signals except SIGTRAP?
    auto m_grabber = pybe::Thread::make(&get_GIL_then_breakpoint, nullptr);
    if (!m_grabber) {
        // TODO error reporting mechanism
        return false;
    }

    auto& grabber = *m_grabber;
    if (grabber.detach()) {
        // TODO error reporting mechanism
        // ... or is this a fatal error? what are we supposed to do?
        return false;
    }

    // TODO restore signals

    return true;
}

} // namespace pybe
