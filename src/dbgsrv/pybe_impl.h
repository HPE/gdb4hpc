// Copyright 2024 Hewlett Packard Enterprise Development LP.

#pragma once

#include "pybe_comms_pybe.h"

#include <memory>

namespace pybe {

class PybeImpl {
    // collection of all the global state in pybe. functions are called
    // by functions in pybe_entry.cpp, which are ultimately initiated by gdb.
    //
    // see doc comments in the pybe::RPC class for what each of these functions do.
public:

    bool get_backtrace(uint32_t seq, unsigned long py_thread_id);
    bool get_frame(uint32_t seq, unsigned long py_thread_id);
    bool eval_expr(uint32_t seq, unsigned long py_thread_id, const char* expr);
    bool is_python_thread();
    unsigned long get_python_thread_id();
    bool connect_to_dbgsrv(const char* endpoint);
    PyGILState_STATE get_GIL_then_breakpoint();
    bool spawn_GIL_grabbing_thread();
    void init_GIL();
    bool init_trace(Py_tracefunc tracefunc);
    int trace_call(PyFrameObject& frame);
    int trace_return(PyFrameObject& frame, PyObject& return_value);
    int trace_line(PyFrameObject& frame);
    bool set_breakpoint(int feid, const char* filename, int line, const char* funcname);
    bool have_breakpoint();
    bool take_current_breakpoint(uint32_t seq);
    bool delete_breakpoint(int feid);
    bool install_step_trap(unsigned long py_thread_id, bool step_in);

private:
    void stop_and_notify_gdb();

    std::unique_ptr<pybe::Channel> m_connection = nullptr;

    // this could have been an associative structure like a (unordered) set,
    // but the most common operation is not looking up a breakpoint by id, but
    // searching through every breakpoint to see if it matches the current
    // line/func. and that happens as a callback on every single python line
    // that's interpreted! to keep that as fast as possible, we use a plain old
    // vector.
    Vector<Breakpoint> m_breakpoints;

    std::unique_ptr<Breakpoint> m_last_hit_breakpoint = nullptr;

    struct Step {
        Frame start;
        int depth_from_start; // positive is deeper
        bool step_into;
    };

    std::unique_ptr<Step> m_step_in_progress = nullptr;
};

} // namespace pybe
