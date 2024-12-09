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
    bool move_frame_level(unsigned long py_thread_id, int count, pybe::FrameDirection dir);
    bool eval_expr(uint32_t seq, unsigned long py_thread_id, const char* expr);
    bool is_python_thread();
    unsigned long get_python_thread_id();
    bool connect_to_dbgsrv(const char* endpoint);
    PyGILState_STATE get_GIL_then_breakpoint();
    bool spawn_GIL_grabbing_thread();
    void init_GIL();
    bool init_trace(Py_tracefunc tracefunc, PyCFunction wrapped_tracefunc);
    int trace_call(PyFrameObject& frame);
    int trace_return(PyFrameObject& frame, PyObject& return_value);
    int trace_line(PyFrameObject& frame);
    bool set_breakpoint(uint32_t seq, unsigned long ptid, int feid, const char* filename, int line, const char* funcname);
    bool have_breakpoint();
    bool take_current_breakpoint(uint32_t seq);
    bool delete_breakpoint(int feid);
    bool install_step_trap(unsigned long py_thread_id, bool step_in);
    bool install_finish_trap(unsigned long py_thread_id);

private:
    void stop_and_notify_gdb();

    // find the python backtrace associated with `py_thread_id` and find the
    // frame at level m_level. the deepest frame is level 0.
    //
    // it is easy for a user to set m_level to an invalid value, e.g.
    // 1. > up 10
    // 2. > thread 2 (thread 2 only has 5 frames in its stack)
    //
    // if the current m_level is found to be invalid when compared to the stack
    // associated with `py_thread_id`, this function will set (fall back)
    // m_level to 0 and return the deepest frame.
    //
    // also note that a StrongRef that holds a null ptr is a valid return value.
    // this most commonly happens when the user tries to get frame/backtrace
    // data when the python interpreter is stopped at the entry breakpoint. the
    // interpreter hasn't started interpreting yet, so there are indeed no
    // frames available.
    //
    // on success, returns a StrongRef containing the found frame (or nullptr).
    // on failure, returns a raw nullptr.
    Expected<StrongRef<PyFrameObject>, std::nullptr_t>
    get_current_frame_with_fallback(unsigned long py_thread_id);

    std::unique_ptr<pybe::Channel> m_connection = nullptr;

    // this could have been an associative structure like a (unordered) set,
    // but the most common operation is not looking up a breakpoint by id, but
    // searching through every breakpoint to see if it matches the current
    // line/func. and that happens as a callback on every single python line
    // that's interpreted! to keep that as fast as possible, we use a plain old
    // vector.
    Vector<Breakpoint> m_breakpoints;

    std::unique_ptr<Breakpoint> m_last_hit_breakpoint = nullptr;

    struct Move {
        Frame start;
        int current_depth; // relative to `start`, positive is deeper

        enum class Type {
            Step,
            Next,
            Finish,
        };

        Type type;
    };

    std::unique_ptr<Move> m_move_in_progress = nullptr;

    // current stack level being inspected. changed via move_frame_level
    int m_level = 0;
};

} // namespace pybe
