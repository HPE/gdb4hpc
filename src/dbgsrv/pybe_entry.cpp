// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// pybe API boundary with dbgsrv

#include "pythoncapi_compat.h"

#include "pybe_impl.h"

namespace pybe {

static PybeImpl& get_pybe() {
    static PybeImpl impl;
    return impl;
}

// note that dbgsrv is sensitive to all of these function names

bool get_backtrace(uint32_t seq, unsigned long py_thread_id) {
    return get_pybe().get_backtrace(seq, py_thread_id);
}

bool get_frame(uint32_t seq, unsigned long py_thread_id) {
    return get_pybe().get_frame(seq, py_thread_id);
}

bool eval_expr(uint32_t seq, unsigned long py_thread_id, const char* expr) {
    return get_pybe().eval_expr(seq, py_thread_id, expr);
}

bool is_python_thread() {
    return get_pybe().is_python_thread();
}

unsigned long get_python_thread_id() {
    return get_pybe().get_python_thread_id();
}

bool connect_to_dbgsrv(const char* endpoint) {
    return get_pybe().connect_to_dbgsrv(endpoint);
}

void get_GIL_then_breakpoint() {
    // be careful when changing this function, dbgsrv relies on certain
    // local variables, backtrace structures, function names (!) etc. in here

    auto gil = get_pybe().get_GIL_then_breakpoint();
    // see implementation of get_GIL_then_breakpoint for why we release the GIL
    // here instead of within the fn
    PyGILState_Release(gil);
}

bool spawn_GIL_grabbing_thread() {
    return get_pybe().spawn_GIL_grabbing_thread();
}

void init_GIL() {
    return get_pybe().init_GIL();
}

// compilers love to tailcall optimize the `return get_pybe().trace_*` lines in
// this function - i.e. turning the calls into straight up jumps. this causes
// pybe::trace to not show up in gdb backtraces (because the jump completely
// replaces the current stack) and can break the dbgsrv logic that detects when
// we are in the tracing function.
[[gnu::optimize("no-optimize-sibling-calls")]]
int trace(PyObject* /* ctx */, PyFrameObject* frame, int what, PyObject* arg) {
    // note that when python calls this function it holds the GIL

    switch (what) {
        case PyTrace_CALL: return get_pybe().trace_call(*frame);
        case PyTrace_RETURN: return get_pybe().trace_return(*frame, *arg);
        case PyTrace_LINE: return get_pybe().trace_line(*frame);
        default: break;
    }

    return 0;
}

bool init_trace() {
    return get_pybe().init_trace(trace);
}

bool set_breakpoint(int feid, const char* filename, int line, const char* funcname) {
    return get_pybe().set_breakpoint(feid, filename, line, funcname);
}

bool have_breakpoint() {
    return get_pybe().have_breakpoint();
}

bool take_current_breakpoint(uint32_t seq) {
    return get_pybe().take_current_breakpoint(seq);
}

bool delete_breakpoint(int feid) {
    return get_pybe().delete_breakpoint(feid);
}

bool install_step_trap(unsigned long py_thread_id, bool step_in) {
    return get_pybe().install_step_trap(py_thread_id, step_in);
}

} // namespace pybe
