// Copyright 2024 Hewlett Packard Enterprise Development LP.

#include "pythoncapi_compat.h"
#include "onscopeexit.h"

#include "pybe_impl.h"

namespace {

PyThreadState* get_threadstate(unsigned long py_thread_id) {
    auto* intr = PyInterpreterState_Head();
    auto* result = PyInterpreterState_ThreadHead(intr);
    // XXX: in python 3.6 and below, the thread id is signed, but in
    // python 3.7 and above it's unsigned. since we're just comparing bits we
    // always case to unsigned
    while (result && (unsigned long)result->thread_id != py_thread_id) {
        result = PyThreadState_Next(result);
    }
    // XXX: add error handling
    return result;
}

// argument and return value are unused, just needed to conform to pthread
void* wait_for_grab_request(void*) {
    bool keep_going_forever_until_gdb_says_so = true;
    while (keep_going_forever_until_gdb_says_so) {
        ::sleep(1);
    };
    return nullptr;
}

} // anon namespace

namespace pybe {

bool PybeImpl::get_backtrace(uint32_t seq, unsigned long py_thread_id) {
    log("get_backtrace\n");
    if (!m_connection) [[ unlikely ]] {
        log("no connection to send result on\n");
        return false;
    }
    if (!PyGILState_Check()) [[ unlikely ]] {
        log("need gil\n");
        return false;
    }

    auto* tstate = get_threadstate(py_thread_id);
    if (tstate == nullptr) {
        log("couldn't find thread\n");
        return false;
    }

    auto result = Backtrace{};

    for (
        auto frame = StrongRef(PyThreadState_GetFrame(tstate));
        frame;
        frame = StrongRef(PyFrame_GetBack(frame.ptr)))
    {
        result.frames.emplace_back(to_frame(*frame));
    }

    auto wire = result.serialize();
    log("serialized backtrace: \"%s\"\n", wire.c_str());
    if (m_connection->send_with_timeout(wire.c_str(), wire.size(), seq) == -1) [[ unlikely ]] {
        log("failed to send backtrace result\n");
        return false;
    }

    return true;
}

bool PybeImpl::get_frame(uint32_t seq, unsigned long py_thread_id) {
    log("get_frame\n");
    if (!m_connection) [[ unlikely ]] {
        log("no connection to send result on\n");
        return false;
    }
    if (!PyGILState_Check()) [[ unlikely ]] {
        log("need gil\n");
        return false;
    }

    auto* tstate = get_threadstate(py_thread_id);
    if (tstate == nullptr) {
        log("couldn't find thread\n");
        return false;
    }

    auto pyframe = StrongRef(PyThreadState_GetFrame(tstate));
    if (!pyframe) {
        log("no frame?\n");
        return false;
    }

    auto result = to_frame(*pyframe);
    log("%s in %s:%d\n", result.function_name.c_str(), result.file_name.c_str(), result.line_number);

    auto wire = result.serialize();
    log("serialized frame: \"%s\"\n", wire.c_str());
    if (m_connection->send_with_timeout(wire.c_str(), wire.size(), seq) == -1) [[ unlikely ]] {
        log("failed to send backtrace result\n");
        return false;
    }

    return true;
}

bool PybeImpl::eval_expr(uint32_t seq, unsigned long py_thread_id, const char* expr) {
    auto* tstate = get_threadstate(py_thread_id);
    if (tstate == nullptr) {
        log("couldn't find thread\n");
        return false;
    }
    log("tstate: %p\n", tstate);

    auto frame = StrongRef(PyThreadState_GetFrame(tstate));
    if (!frame.ptr) {
        log("missing frame\n");
        return false;
    }
    log("frame: %p\n", frame.ptr);

    auto globals = StrongRef(PyFrame_GetGlobals(frame.ptr));
    log("globals: %p\n", globals.ptr);
    auto locals = StrongRef(PyFrame_GetLocals(frame.ptr));
    log("locals: %p\n", locals.ptr);

    auto result = StrongRef(PyRun_String(expr, Py_eval_input, globals.ptr, locals.ptr));

    auto [result_buf, size] = [&](){
        char* buf;
        size_t size;
        auto* stream = ::open_memstream(&buf, &size);
        if (PyObject_Print(result.ptr, stream, 0) == -1) {
            return std::pair{static_cast<char*>(nullptr), 0ul};
        }
        ::fclose(stream);
        return std::pair{buf, size};
    }();

    if (result_buf == nullptr) {
        log("PyObject_Print failed\n");
        return false;
    }

    auto result_buf_cleanup = OnScopeExit([&](){ ::free(result_buf); });

    if (m_connection->send_with_timeout(result_buf, size, seq) == -1) [[ unlikely ]] {
        log("failed to send eval result\n");
        return false;
    }

    return true;
}

bool PybeImpl::is_python_thread() {
    return PyGILState_GetThisThreadState() != nullptr;
}

unsigned long PybeImpl::get_python_thread_id() {
    // check that this is safe with is_python_thread first!
    //
    // XXX: in python 3.6 and below, the thread id is signed, but in
    // python 3.7 and above it's unsigned. since we're just comparing bits we
    // always cast to unsigned
    return (unsigned long)PyGILState_GetThisThreadState()->thread_id;
}

bool PybeImpl::connect_to_dbgsrv(const char* endpoint) {
    auto maybe_client = pybe::Client::make(endpoint);
    if (!maybe_client) [[ unlikely ]] {
        log("failed create client\n");
        return false;
    }
    auto& client = *maybe_client;

    auto maybe_channel = client.connect_with_timeout();
    if (!maybe_channel) {
        log("failed to connect to server\n");
        return false;
    }

    m_connection = std::make_unique<pybe::Channel>(std::move(*maybe_channel));

    return true;
}

PyGILState_STATE PybeImpl::get_GIL_then_breakpoint() {
    // be careful when changing this function, dbgsrv relies on certain
    // local variables, backtrace structures, function names (!) etc. in here

    // get the gil
    auto gil = PyGILState_Ensure();

    // volatile, otherwise the compiler would optimize this out and gdb couldn't set it
    volatile auto done = false;

    // tell gdb that we have it. gdb will set `done` to true and finish this
    // function when it's time to release.
    stop_and_notify_gdb();

    // trap ourselves in a loop. the fn call above will stop us, but if we are
    // in all-stop mode and gdb decides to run some functions on the main
    // thread, we could slip out of this function and release the lock!
    while (!done) ::usleep(1000); // 1 ms

    // we could release the gil now, but there are a few more calling functions
    // that gdb will finish. in all-stop mode, if we released the gil now,
    // python could steal the released gil while we were finishing those
    // functions and slip. so we pass the gil all the way up the call chain
    // and release at the last possible moment.
    return gil;
}

bool PybeImpl::spawn_GIL_grabbing_thread() {
    // block all signals except our designated GIL signal
    sigset_t all_signals_except_gil;
    sigfillset(&all_signals_except_gil);
    sigdelset(&all_signals_except_gil, get_gil_signal_number());

    sigset_t old_signals;
    pthread_sigmask(SIG_BLOCK, &all_signals_except_gil, &old_signals);

    auto maybe_grabber = pybe::Thread::make(&wait_for_grab_request, nullptr);

    // unblock so we don't interfere with the python interpreter
    pthread_sigmask(SIG_BLOCK, &old_signals, nullptr);

    if (!maybe_grabber) {
        log("failed to create GIL thread\n");
        return false;
    }
    auto& grabber = *maybe_grabber;

    if (grabber.detach()) {
        log("failed to detach GIL grabbing thread\n");
        return false;
    }

    return true;
}

void PybeImpl::init_GIL() {
    Py_Initialize(); // no-op when called for a 2nd time, so safe to call without any check
#if PY_VERSION_HEX < 0x03070000
    // this is required in 3.6 and below. it is a no-op in 3.7 and above.
    // in 3.6, this MUST be called in python's main thread.
    PyEval_InitThreads();
#endif
}

void PybeImpl::stop_and_notify_gdb() {
    m_step_in_progress.reset();
    ::raise(get_gil_signal_number());
}

int PybeImpl::trace_call(PyFrameObject& pyframe) {
    auto frame = to_frame(pyframe);
    // log("[CALL] %s in %s:%d\n", frame.funcname.c_str(), frame.filename.c_str(), frame.linenum);

    auto matching_breakpoint = [&]() -> std::unique_ptr<Breakpoint> {
        for (const auto& bp : m_breakpoints) {
            if (bp.location.function_name == frame.function_name) {
                return std::make_unique<Breakpoint>(bp);
            }
        }
        return nullptr;
    }();

    if (matching_breakpoint) {
        m_last_hit_breakpoint = std::move(matching_breakpoint);
        stop_and_notify_gdb();
    } else if (m_step_in_progress) {
        m_step_in_progress->depth_from_start++;
        if (m_step_in_progress->step_into) {
            stop_and_notify_gdb();
        }
    }

    return 0;
}

int PybeImpl::trace_return(PyFrameObject& /* frame */, PyObject& /* return_value */) {
    if (m_step_in_progress) {
        m_step_in_progress->depth_from_start--;
    }

    return 0;
}

int PybeImpl::trace_line(PyFrameObject& pyframe) {
    auto frame = to_frame(pyframe);
    // log("[LINE] %s in %s:%d\n", frame.funcname.c_str(), frame.filename.c_str(), frame.linenum);

    auto matching_breakpoint = [&]() -> std::unique_ptr<Breakpoint> {
        for (const auto& bp : m_breakpoints) {
            // the user might have set a breakpoint with only a line number and
            // no file name, so only check the file name if it's not empty.
            // if they did set a filename, assume that it was the basename.
            // python gives us the full path so we need to basename it.
            frame.file_name = ::basename(frame.file_name.c_str());
            if ((bp.location.file_name.empty() || bp.location.file_name == frame.file_name) &&
                 bp.location.line_number == frame.line_number)
            {
                return std::make_unique<Breakpoint>(bp);
            }
        }
        return nullptr;
    }();

    if (matching_breakpoint) {
        m_last_hit_breakpoint = std::move(matching_breakpoint);
        stop_and_notify_gdb();
    } else if (m_step_in_progress &&
              (m_step_in_progress->step_into || m_step_in_progress->depth_from_start <= 0) &&
               m_step_in_progress->start.line_number != frame.line_number)
    {
        stop_and_notify_gdb();
    }

    return 0;
}

bool PybeImpl::init_trace(Py_tracefunc tracefunc) {
    if (!PyGILState_Check()) [[ unlikely ]] {
        log("need gil\n");
        return false;
    }

    PyEval_SetTrace(tracefunc, nullptr);
    return true;
}

bool PybeImpl::set_breakpoint(int feid, const char* filename, int line, const char* funcname) {
    m_breakpoints.push_back({
        .feid = feid,
        .location = Frame {
            .function_name = funcname,
            .file_name = filename,
            .line_number = line,
        }
    });
    return true;
}

bool PybeImpl::have_breakpoint() {
    return m_last_hit_breakpoint != nullptr;
}

bool PybeImpl::take_current_breakpoint(uint32_t seq) {
    if (!m_connection) [[ unlikely ]] return false;
    if (!m_last_hit_breakpoint) [[ unlikely ]] return false;

    auto breakpoint = std::move(m_last_hit_breakpoint);

    auto wire = breakpoint->serialize();
    log("serialized breakpoint: \"%s\"\n", wire.c_str());
    if (m_connection->send_with_timeout(wire.c_str(), wire.size(), seq) == -1) [[ unlikely ]] {
        log("failed to send breakpoint result\n");
        return false;
    }

    return true;
}

bool PybeImpl::delete_breakpoint(int feid) {
    auto idx = [&](){
        auto i = m_breakpoints.begin();
        for (; i != m_breakpoints.end(); i++) {
            if (i->feid == feid) return i;
        }
        return i;
    }();

    if (idx == m_breakpoints.end()) return false;
    m_breakpoints.erase(idx);
    return true;
}

bool PybeImpl::install_step_trap(unsigned long py_thread_id, bool step_in) {
    auto* tstate = get_threadstate(py_thread_id);
    if (!tstate) return false;

    auto frame = StrongRef(PyThreadState_GetFrame(tstate));
    if (!frame.ptr) return false;

    m_step_in_progress = std::make_unique<Step>(Step{
        .start = to_frame(*frame),
        .depth_from_start = 0,
        .step_into = step_in,
    });

    return true;
}

} // namespace pybe
