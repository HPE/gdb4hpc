// Copyright (c) 2024 Hewlett Packard Enterprise Development LP.

#include <algorithm>
#include <dlfcn.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "unittest_macros.h"

#include "dbgevent.h"
#include "dbgsrv_defs.h"

#include "dbgsrv_pybe.h"

namespace {

bool python_version_is_supported(const char* version) {
    return std::ranges::any_of((const char* [])SUPPORTED_PYTHON_VERSIONS, [&](const char* v) {
        return ::strcmp(version, v) == 0;
    });
}

} // anon namespace

Pybe::Pybe(MISession& mi)
    : m_rpc{mi}
{
    const auto current_thread = m_rpc.current_thread_get_id();
    m_cached_py_minor_version = m_rpc.get_python_minor_version(current_thread);

    const auto python_version_str = std::string{"3."} + std::to_string(m_cached_py_minor_version);

    if (!python_version_is_supported(python_version_str.c_str())) {
        throw NewErrorEvent(DBGERR_DEBUGGER,
            "gdb4hpc does not support python %s.\n"
            "Supported versions: " SUPPORTED_PYTHON_VERSIONS_STR "\n"
            "Disabling python debugging for this rank. Re-enable with `set python on`.",
            python_version_str.c_str());
    }

    m_rpc.inject_pybe_so(m_cached_py_minor_version, current_thread);
    m_gil_thread_id = m_rpc.spawn_pybe_gil_thread(current_thread);

    // note that even after creating the gil thread, we use the current thread
    // to do the dirty work here because in all-stop mode using the gil thread
    // would cause the main thread to slip. the launch process has been very
    // careful to set up the thread so that calling init_GIL below is safe.
    // slipping would destroy that and that would be very sad.

    m_rpc.thread_name(m_gil_thread_id, "gdb4hpc python");
    m_rpc.thread_interrupt(m_gil_thread_id);
    m_rpc.open_pybe_channel(current_thread);

    // we need to do some extra work for python 3.6
    if (m_cached_py_minor_version <= 6) {
        // this must be called on the main python thread.
        m_rpc.thread_interrupt(current_thread);
        m_rpc.init_GIL(current_thread);
    }
}

int Pybe::get_python_minor_version() {
    return m_cached_py_minor_version;
}

Pybe::GILHolder Pybe::coerce_gil() {
    return GILHolder(m_rpc, m_gil_thread_id);
}

void Pybe::trivialize_gil() {
    // get the gil and then immediately drop it
    [[ maybe_unused ]] auto gil = coerce_gil();
}

Pybe::GILHolder::GILHolder(pybe::RPC& rpc, int gil_thread_id)
    : m_parent_rpc{rpc}
    , m_gil_thread_id{gil_thread_id}
{
    auto old_thread_id = m_parent_rpc.current_thread_get_id();
    m_py_thread_id = m_parent_rpc.thread_get_python_id(old_thread_id);

    auto maybe_gil_thread = m_parent_rpc.any_thread_has_GIL();
    // FIXME: this should check if gil thread was running. right now
    // any_thread_has_GIL simply
    //        stops all threads and leaves them stopped. eventually the function
    //        will be side-effect free and it will be our job to be smart about
    //        side effects.
    //
    // TODO redesign this so we can start our gil thread running first to
    // minimize slip in the main thread. we can't right now because
    // thread_take_GIL runs until the GIL is obtained (or failure).
    // thread_take_GIL could (should?) be split into two functions to allow for:
    // 1. start GIL grabbing thread
    // 2. start GIL holding thread
    // 3. exchange GIL, stop stuff, cleanup stuff

    // XXX: don't look at a global (non_stop_flag)
    // if we are in all-stop mode, the thread_take_GIL call will start the holding thread implicitly
    if (maybe_gil_thread && non_stop_flag) m_parent_rpc.thread_continue(*maybe_gil_thread);
    m_parent_rpc.thread_take_GIL(m_gil_thread_id);
    if (maybe_gil_thread && non_stop_flag) m_parent_rpc.thread_interrupt(*maybe_gil_thread);
}

Pybe::GILHolder::~GILHolder() {
    // reset state to what it was before GILHolder.
    // assume we are in the sigtrap func called from wait loop
    try {
        // FIXME: be smarter, verify we are where we think we are first
        m_parent_rpc.thread_finish_with_timeout(m_gil_thread_id); // finish std::raise
        m_parent_rpc.thread_finish_with_timeout(m_gil_thread_id); // finish get_GIL
    } catch (const std::exception& e) {
        DVPRINT("In ~GILHolder, caught exception! %s\n", e.what());
    } catch (...) {
        DVPRINT("In ~GILHolder, caught unknown exception!\n");
    }
}

pybe::Backtrace Pybe::GILHolder::get_backtrace() {
    return m_parent_rpc.get_python_backtrace(m_gil_thread_id, m_py_thread_id);
}

std::string Pybe::GILHolder::eval_py_expression(std::string_view expr) {
    return m_parent_rpc.eval_py_expression(m_gil_thread_id, m_py_thread_id, expr);
}

TEST_CASE("supported python versions are indeed supported") {
    for (const char* version : (const char* [])SUPPORTED_PYTHON_VERSIONS) {
        CHECK(python_version_is_supported(version));
    }

    CHECK(!python_version_is_supported("beta prerelease six"));
}
