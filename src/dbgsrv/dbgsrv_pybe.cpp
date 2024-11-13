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

int Pybe::get_python_minor_version(MISession& sess) {
    auto rpc = pybe::RPC{sess};
    auto tid = rpc.current_thread_get_id();
    return rpc.get_python_minor_version(tid);
}

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

    m_rpc.thread_set_name(m_gil_thread_id, "gdb4hpc python");
    m_rpc.thread_interrupt(m_gil_thread_id);
    m_rpc.open_pybe_channel(current_thread);

    // initialize GIL
    // this must be called on the main python thread.
    // (technically this is only needed in python <= 3.6 and is a no-op otherwise)
    m_rpc.thread_interrupt(current_thread);
    m_rpc.init_GIL(current_thread);

    {
        // set up breakpoints. needs to be done on the main python thread and the
        // current thread needs to hold the GIL. after a launch, the main thread should
        // already have the GIL, but after an attach, there might be some slippage in getting it
        auto py_thread_id = m_rpc.thread_get_python_id(current_thread);
        auto gil = GILHolder(m_rpc, current_thread, py_thread_id);
        m_rpc.install_breakpoint_trace(current_thread);
    }

    // XXX: this constructor might be called twice for a single inferior:
    // 1. attach, init
    // 2. detach
    // 3. attach again, init again
    // this is a low priority but still valid usecase - tracked in PE-55412
}

Pybe::GILHolder Pybe::run_until_gil() {
    // throws if not a python thread
    auto main_thread_id = m_rpc.current_thread_get_id();
    auto py_thread_id = m_rpc.thread_get_python_id(main_thread_id);

    // there are three scenarios for getting the GIL:
    // 1. we have just stopped after the entry breakpoint. the main thread
    //    already holds the GIL. no slippage is allowed here because python
    //    is still initializing and slipping might put us in a segfaulty place.
    // 2. python is stopped in our tracing function after a breakpoint.
    //    the tracing function is part of the main python thread and already
    //    holds the GIL. no slippage is allowed here - we can't have the frame
    //    change between hitting a breakpoint and printing a backtrace/variable.
    // 3. any other arbitrary scenario. we use our dedicated GIL thread to
    //    compete for the GIL. this might cause a little slippage. we hide this
    //    slippage from the user at a higher level than this function by
    //    only competing for the GIL when the user doesn't know the "correct"
    //    location. (see python.md)
    //
    // GILHolder is smart enough to not slip when the thread it uses already
    // holds the GIL, but we need to figure out which scenario we're in and give
    // it the right thread to use.

    auto chosen_thread = [&](){
        auto bt = m_rpc.get_elf_backtrace(main_thread_id);

        // check for scenario 1
        constexpr auto py_main_fns = std::array{
            // note that these are different from the python entry point fns.
            // the entry point fns will be hit on launch, a finish will be
            // executed, and we will end up in one of these.
            "Py_Main",
            "pymain_init",
        };
        // note that we are in scenario 1 only if the Py_Main function is the
        // bottom (deepest) of the stack. Py_Main will always be *somewhere* in
        // the stack at any point in the python interpreter. a pybe backtrace
        // lists the deepest frame first.
        if (std::ranges::any_of(py_main_fns, [&](const auto& fn){
            return fn == bt.frames.front().function_name;
        })) {
            DVPRINT("gil scenario 1 (at main)\n");
            return main_thread_id;
        }

        // check for scenario 2
        if (std::ranges::any_of(bt.frames, [&](const auto& fr){
            return fr.function_name == m_rpc.get_tracing_function_name();
        })) {
            DVPRINT("gil scenario 2 (at trace)\n");
            return main_thread_id;
        }

        // must be scneario 3
        DVPRINT("gil scenario 3 (arbitrary)\n");
        return m_gil_thread_id;
    }();

    DVPRINT("getting gil on %d\n", chosen_thread);

    return GILHolder(m_rpc, chosen_thread, py_thread_id);
}

void Pybe::trivialize_gil() {
    // get the gil and then immediately drop it
    [[ maybe_unused ]] auto gil = run_until_gil();
}

namespace {

class EscapeFromGILHoldingFns : public Pybe::GILHolder::Escape {
    // escape from the call tree that starts with pybe::get_GIL_then_breakpoint in pybe_entry.cpp.
    //
    // this is very sensitive to *everything* (function names, local variables, function call structure)
    // under pybe::get_GIL_then_breakpoint in pybe_entry.cpp, so be careful changing those.
public:
    EscapeFromGILHoldingFns(pybe::RPC& rpc, pybe::RPC::c_thread_id_t gil_holding_thread)
        : m_rpc{rpc}
        , m_gil_holding_thread{gil_holding_thread}
    {}

    void escape() final override {
        // figure out which frame has the GIL and which frame will release is
        auto bt = m_rpc.get_elf_backtrace(m_gil_holding_thread);

        const auto gil_hold_frame = [&](){
            // pybe backtraces have the deepest frame at index 0, which works
            // in our favor here since that's also how gdb numbers frames
            // (deepest is frame 0, its caller is frame 1, etc).
            for (int i = 0; i < static_cast<int>(bt.frames.size()); i++) {
                if (bt.frames[i].function_name == m_rpc.get_GIL_holding_function_name()) {
                    return i;
                }
            }
            return -1;
        }();

        const auto gil_release_frame = [&](){
            for (int i = 0; i < static_cast<int>(bt.frames.size()); i++) {
                // I can't think of a scenario where we would find the releasing frame
                // under (called by) the holding frame, but it can't hurt to check
                if (bt.frames[i].function_name == m_rpc.get_GIL_releasing_function_name() && i >= gil_hold_frame) {
                    return i;
                }
            }
            return -1;
        }();

        DVPRINT("gil frames: %d, %d\n", gil_hold_frame, gil_release_frame);
        if (gil_hold_frame == -1 || gil_release_frame == -1 || gil_release_frame < gil_hold_frame) {
            DVPRINT("couldn't find GIL frames in ~GILHolder\n");
            return;
        }

        // unlock the GIL holding function
        m_rpc.eval_elf_expression(m_gil_holding_thread, gil_hold_frame, "done = true");

        // release the GIL
        //
        // XXX: releasing the GIL is the last statement in the following fns,
        // but i think there is still technically a race in all-stop mode
        // between exectuing the last statement and returning from the function
        // where the python main thread can swoop in and steal the GIL and slip
        // a little bit. i have never seen it actually happen in testing though.
        m_rpc.thread_finish_with_timeout(m_gil_holding_thread, gil_release_frame);
    };

private:
    pybe::RPC& m_rpc;
    pybe::RPC::c_thread_id_t m_gil_holding_thread;
};

} // anon namespace

Pybe::GILHolder::GILHolder(pybe::RPC& rpc, int gil_thread_id, long py_thread_id)
    : m_parent_rpc{rpc}
    , m_gil_holding_thread_id{gil_thread_id}
    , m_py_thread_id{py_thread_id}
{
    // our thread might already have it. in that case,
    // assume that we are in Py_Main or our tracing
    // function and we don't need to escape when we're done
    if (m_parent_rpc.thread_has_GIL(gil_thread_id)) {
        return;
    }

    // FIXME: this should check if gil thread was running. right now
    // any_thread_has_GIL simply stops all threads and leaves them stopped.
    // eventually the function will be side-effect free and it will be our job
    // to be smart about side effects.
    //
    // TODO redesign this so we can start our gil thread running first to
    // minimize slip in the main thread. we can't right now because
    // thread_take_GIL runs until the GIL is obtained (or failure).
    // thread_take_GIL could (should?) be split into two functions to allow for:
    // 1. start GIL grabbing thread
    // 2. start GIL holding thread
    // 3. exchange GIL, stop stuff, cleanup stuff

    // note that this will slip in all-stop mode, but that's okay because we've
    // already checked if we can get the gil without slipping above.
    auto maybe_gil_thread = m_parent_rpc.get_GIL_holding_thread();

    // XXX: don't look at a global (non_stop_flag)
    // if we are in all-stop mode, the thread_take_GIL call will start the holding thread implicitly
    if (maybe_gil_thread && non_stop_flag) m_parent_rpc.thread_continue(*maybe_gil_thread);
    m_parent_rpc.thread_take_GIL(m_gil_holding_thread_id);
    if (maybe_gil_thread && non_stop_flag) m_parent_rpc.thread_interrupt(*maybe_gil_thread);

    // under the assumption that thread_take_GIL put us in some frames that need escaping later
    m_escape = std::make_unique<EscapeFromGILHoldingFns>(m_parent_rpc, m_gil_holding_thread_id);
}

Pybe::GILHolder::~GILHolder() {
    try {
        if (m_escape) m_escape->escape();
    } catch (const std::exception& e) {
        DVPRINT("In ~GILHolder, caught exception! %s\n", e.what());
    } catch (...) {
        DVPRINT("In ~GILHolder, caught unknown exception!\n");
    }
}

pybe::Frame Pybe::GILHolder::get_frame() {
    return m_parent_rpc.get_python_frame(m_gil_holding_thread_id, m_py_thread_id);
}

pybe::Backtrace Pybe::GILHolder::get_backtrace() {
    return m_parent_rpc.get_python_backtrace(m_gil_holding_thread_id, m_py_thread_id);
}

std::string Pybe::GILHolder::eval_py_expression(std::string_view expr) {
    return m_parent_rpc.eval_py_expression(m_gil_holding_thread_id, m_py_thread_id, expr);
}

pybe::Breakpoint Pybe::GILHolder::set_breakpoint(int feid, std::string_view filename, int line, std::string_view funcname) {
    return m_parent_rpc.set_breakpoint(m_gil_holding_thread_id, m_py_thread_id, feid, filename, line, funcname);
}

void Pybe::GILHolder::delete_breakpoint(int feid) {
    m_parent_rpc.delete_breakpoint(m_gil_holding_thread_id, feid);
}

std::optional<pybe::Breakpoint> Pybe::GILHolder::find_current_breakpoint() {
    const auto current_thread = m_parent_rpc.current_thread_get_id();
    if (!m_parent_rpc.at_breakpoint(current_thread)) return {};
    return m_parent_rpc.take_current_breakpoint(current_thread);
}

void Pybe::GILHolder::insert_step_trap(bool step_in) {
    m_parent_rpc.insert_step_trap(m_gil_holding_thread_id, m_py_thread_id, step_in);
}

void Pybe::GILHolder::insert_finish_trap() {
    m_parent_rpc.insert_finish_trap(m_gil_holding_thread_id, m_py_thread_id);
}

TEST_CASE("supported python versions are indeed supported") {
    for (const char* version : (const char* [])SUPPORTED_PYTHON_VERSIONS) {
        CHECK(python_version_is_supported(version));
    }

    CHECK(!python_version_is_supported("beta prerelease six"));
}
