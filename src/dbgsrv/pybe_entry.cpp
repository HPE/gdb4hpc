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

bool have_uncaught_exception() {
    return get_pybe().have_uncaught_exception();
}

bool take_current_uncaught_exception(uint32_t seq) {
    return get_pybe().take_current_uncaught_exception(seq);
}

bool enable_exception_catchpoint(int feid) {
    return get_pybe().enable_exception_catchpoint(feid);
}

bool get_frame(uint32_t seq, unsigned long py_thread_id) {
    return get_pybe().get_frame(seq, py_thread_id);
}

bool move_frame_level(unsigned long py_thread_id, int count, int dir) {
    return get_pybe().move_frame_level(py_thread_id, count, static_cast<FrameDirection>(dir));
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
        case PyTrace_EXCEPTION: {
            // note that unlike `pybe_excepthook`, where it's possible that we will receive
            // Py_None as the backtrace, here we always get a backtrace. we always get one
            // since in order to get to the point where python is calling `trace`, the
            // interpreter is running code and a backtrace exists.
            //
            // if that assumption happens to be wrong, the worst that will happen is
            // `PyArg_ParseTuple` will detect it and return an error.
            PyTypeObject* exception_type = nullptr;
            PyObject* exception_instance = nullptr;
            PyTracebackObject* backtrace = nullptr;
            if (!PyArg_ParseTuple(arg, "O!OO!", &PyType_Type, &exception_type, &exception_instance, &PyTraceBack_Type, &backtrace)) {
                log("pybe::trace: failed to unpack tuple\n");
                return 0;
            }
            if (!PyExceptionInstance_Check(exception_instance)) {
                log("pybe::trace: exception instance is wrong type\n");
                return 0;
            }
            if (!exception_type || !exception_instance || !backtrace) {
                log("pybe::trace: missing data: %p %p %p\n", exception_type, exception_instance, backtrace);
                return 0;
            }

            return get_pybe().trace_exception(*frame, *exception_type, *exception_instance, *backtrace);
        }
        default: break;
    }

    return 0;
}

// the python C api does not give us access to threading.settrace which gives python a
// tracing function to install on every new thread. we need to call threading.settrace
// it from python (done elsewhere). python_wrapped_trace is the function we pass
// when setting the tracing function this way.
//
// a trace function installed via python has more overhead than one installed via the C
// API, so this function will swap itself out for the C API tracing function when it is
// called for the first time.
PyObject* python_wrapped_trace(PyObject* /*self*/, PyObject* args) {
    log("pybe: wrapped trace\n");

    PyFrameObject* frame = nullptr;
    const char* event = nullptr;
    PyObject* trace_arg = nullptr;

    if (!PyArg_ParseTuple(args, "O!sO", &PyFrame_Type, &frame, &event, &trace_arg)) {
        log("python_wrapped_trace: failed to unpack tuple\n");
        // TODO not sure how to handle this failure correctly
    }

    // swap this thread's trace function from the wrapped python function to
    // the native C function for speed in the future
    PyEval_SetTrace(trace, nullptr);

    // now manually call the trace function so we don't miss any line

    int what = 0;

    if (strcmp(event, "call") == 0) {
      what = PyTrace_CALL;
    } else if (strcmp(event, "line") == 0) {
      what = PyTrace_LINE;
    } else if (strcmp(event, "return") == 0) {
      what = PyTrace_RETURN;
    } else if (strcmp(event, "exception") == 0) {
      what = PyTrace_EXCEPTION;
    } else if (strcmp(event, "c_call") == 0) {
      what = PyTrace_C_CALL;
    } else if (strcmp(event, "c_return") == 0) {
      what = PyTrace_C_RETURN;
    } else if (strcmp(event, "c_exception") == 0) {
      what = PyTrace_C_EXCEPTION;
#if PY_VERSION_HEX >= 0x03070000
    // PyTrace_OPCODE introduced in python 3.7
    } else if (strcmp(event, "opcode") == 0) {
      what = PyTrace_OPCODE;
#endif
    } else {
        Py_RETURN_NONE;
    }

    trace(nullptr, frame, what, trace_arg);

    // todo convert the integer that trace returns to a python object?
    Py_RETURN_NONE;
}

// when pybe sets up tracing, this function is set to sys.excepthook. sys.excepthook
// is called when an uncaught exception bubbles all the way up to the top of the program.
// the program is in an unrecoverable state at that point, so we can't save it, but
// we can grab the exception information before python dies and give it to the gdb4hpc user.
PyObject* pybe_excepthook(PyObject* /*self*/, PyObject* args) {
    log("pybe_excepthook\n");
    PyTypeObject* exception_type = nullptr;
    PyObject* exception_instance = nullptr;
    PyTracebackObject* backtrace = nullptr;

    if (!PyArg_ParseTuple(args, "O!OO", &PyType_Type, &exception_type, &exception_instance, &backtrace)) {
        log("pybe_excepthook: failed to unpack tuple\n");
        // TODO not sure how to handle this failure correctly
        Py_RETURN_NONE;
    }
    if (!PyExceptionInstance_Check(exception_instance)) {
        log("pybe_excepthook: exception instance is wrong type\n");
        // TODO not sure how to handle this failure correctly
        Py_RETURN_NONE;
    }
    // some exceptions might not have a backtrace, e.g. syntax errors that are emitted
    // before any python code is interpreted. in that case `backtrace` will be set to
    // Py_None. Py_None is a singleton and the correct way to check for it is to simply
    // compare memory addresses of two objects.
    if (static_cast<void*>(backtrace) == static_cast<void*>(Py_None)) {
        log("backtrace is Py_None, setting to nullptr\n");
        backtrace = nullptr;
    } else if (!PyTraceBack_Check(backtrace)) {
        // if not Py_None, it is required to be a traceback object for the below fn calls
        log("backtrace pt not a backtrace object\n");
        Py_RETURN_NONE;
    }

    if (!exception_type || !exception_instance) {
        log("pybe_excepthook: missing data: %p %p %p\n", exception_type, exception_instance, backtrace);
        Py_RETURN_NONE;
    }

    get_pybe().notify_uncaught_exception(*exception_type, *exception_instance, backtrace);

    log("done handling exception\n");

    Py_RETURN_NONE;
}

bool init_trace() {
    return get_pybe().init_trace(trace, python_wrapped_trace, pybe_excepthook);
}

bool set_breakpoint(uint32_t seq, unsigned long ptid, int feid, const char* filename, int line, const char* funcname) {
    return get_pybe().set_breakpoint(seq, ptid, feid, filename, line, funcname);
}

bool verify_breakpoint(unsigned long ptid, const char* filename, int line, const char* funcname) {
    return get_pybe().verify_breakpoint(ptid, filename, line, funcname);
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

bool install_finish_trap(unsigned long py_thread_id) {
    return get_pybe().install_finish_trap(py_thread_id);
}

} // namespace pybe
