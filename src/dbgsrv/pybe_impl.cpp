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

void set_trace_all_threads(Py_tracefunc tracefunc) {
#if PY_VERSION_HEX >= 0x030c0000
    PyEval_SetTraceAllThreads(tracefunc, nullptr);
#else
    // PyEval_SetTraceAllThreads was added in 3.12. pythoncapi_compat doesn't help us with
    // this one, so we need to implement it ourselves.

    // set current thread
    PyEval_SetTrace(tracefunc, nullptr);

    // set other threads
    auto* intr = PyInterpreterState_Head();
    auto* thr = PyInterpreterState_ThreadHead(intr);

    while (thr) {
        // todo store old ones?
        // XXX: this is a pretty ugly hack, there is probably a better way...
        thr->c_tracefunc = tracefunc;
        thr->c_traceobj = nullptr;
#if PY_VERSION_HEX >= 0x030a0000 && PY_VERSION_HEX < 0x030c0000
        if (thr->cframe) {
            thr->cframe->use_tracing = 255;
        }
#endif
        thr = PyThreadState_Next(thr);
    }

    // note, from the python docs:
    //
    // > For more fine-grained usage, it’s possible to set a trace function by
    // > assigning frame.f_trace = tracefunc explicitly, rather than relying on it
    // > being set indirectly via the return value from an already installed trace
    // > function. This is also required for activating the trace function on the
    // > current frame, which settrace() doesn’t do. Note that in order for this
    // > to work, a global tracing function must have been installed with
    // > settrace() in order to enable the runtime tracing machinery, but it
    // > doesn’t need to be the same tracing function (e.g. it could be a low
    // > overhead tracing function that simply returns None to disable itself
    // > immediately on each frame).
    //
    // so should we set frame.f_trace here too?
#endif
}

} // anon namespace

namespace pybe {

Expected<StrongRef<PyFrameObject>, std::nullptr_t>
PybeImpl::get_current_frame_with_fallback(unsigned long py_thread_id) {
    auto* tstate = get_threadstate(py_thread_id);
    if (tstate == nullptr) [[ unlikely ]] {
        log("couldn't find thread\n");
        return nullptr;
    }
    if (m_level < 0) [[ unlikely ]] {
        log("level < 0 (%d)\n", m_level);
        return nullptr;
    }

    auto iter_frame = StrongRef<PyFrameObject>(nullptr);
    int iter_level = 0;
    for (
        iter_frame = StrongRef(PyThreadState_GetFrame(tstate));
        iter_frame && iter_level < m_level;
        iter_frame = StrongRef(PyFrame_GetBack(iter_frame.ptr)), iter_level++)
    {}

    if (!iter_frame) {
        if (iter_level == 0) {
            // no python frames were found. this is a correct result when this function
            // is called before python has started interpreting code or after it's done.
            // fall through.
        } else {
            // we iterated at least 1 frame but overshot the top of the stack. this
            // is a programmer error.
            log("error: failed to get frame at level %d\n", m_level);
            return nullptr;
        }
    }

    return iter_frame;
}

static Expected<Backtrace, std::nullptr_t> get_backtrace_data(unsigned long py_thread_id) {
    auto* tstate = get_threadstate(py_thread_id);
    if (tstate == nullptr) {
        log("couldn't find thread\n");
        return nullptr;
    }

    auto result = Backtrace{};

    int level = 0;
    for (
        auto frame = StrongRef(PyThreadState_GetFrame(tstate));
        frame;
        frame = StrongRef(PyFrame_GetBack(frame.ptr)), level++)
    {
        result.frames.emplace_back(to_frame(*frame, level));
    }

    return result;
}

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

    auto maybe_result = get_backtrace_data(py_thread_id);
    if (!maybe_result) {
        // error message already logged
        return false;
    }
    auto& result = *maybe_result;

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

    auto maybe_py_frame = get_current_frame_with_fallback(py_thread_id);
    if (!maybe_py_frame) {
        log("get_frame_at_level_with_fallback(%lu) failed\n", py_thread_id);
        return false;
    }
    auto& py_frame = *maybe_py_frame;

    // convert to pybe::Frame
    auto result = [&]() {
        if (py_frame) {
            return to_frame(*py_frame, m_level);
        } else {
            // if the user requests a frame at the entry breakpoint, python is still initializing
            // and hasn't started interpreting code yet and there is legitimately no python frame
            // available.
            return pybe::Frame{
                .level = 0,
                .location = {
                    .function_name = "No python frame",
                    .file_name = "",
                    .line_number = 0,
                }
            };
        }
    }();

    // send the result
    log("%s in %s:%d\n",
        result.location.function_name.c_str(),
        result.location.file_name.c_str(),
        result.location.line_number);

    auto wire = result.serialize();
    log("serialized frame: \"%s\"\n", wire.c_str());
    if (m_connection->send_with_timeout(wire.c_str(), wire.size(), seq) == -1) [[ unlikely ]] {
        log("failed to send backtrace result\n");
        return false;
    }

    return true;

}

bool PybeImpl::move_frame_level(unsigned long py_thread_id, int count, pybe::FrameDirection dir) {
    log("move_frame_level\n");
    if (!PyGILState_Check()) [[ unlikely ]] {
        log("need gil\n");
        return false;
    }
    if (count < 0) [[ unlikely ]] {
        log("negative count (%d)\n", count);
        return false;
    }

    auto maybe_backtrace = get_backtrace_data(py_thread_id);
    if (!maybe_backtrace) {
        // error message already logged
        return false;
    }
    auto& backtrace = *maybe_backtrace;

    if (backtrace.frames.empty()) {
        // this is not necessarily an error - if the backtrace is fetched right
        // at python startup, there will legitimately not be any python backtrace.
        // just set the level to 0 and move on
        log("warning: move_frame_level: no python backtrace, setting level to 0\n");
        m_level = 0;
        return true;
    }

    m_level = [&](){
        using enum FrameDirection;
        switch(dir) {
            case Up: {
                return std::min(backtrace.frames.back().level, m_level + count);
            }
            case Down: {
                return std::max(0, m_level - count);
            }
            case Absolute: {
                return count;
            }
        }
        // unreachable
        log("warning: corrupted direction input\n");
        return count;
    }();
    log("new level: %d\n", m_level);

    return true;
}

bool PybeImpl::eval_expr(uint32_t seq, unsigned long py_thread_id, const char* expr) {
    auto maybe_frame = get_current_frame_with_fallback(py_thread_id);
    // this check is a little confusing. first we check if get_frame had an
    // error (the optional it returned was empty), then we check if we are in an
    // edge case where get_frame correctly returns and optional containing a
    // null ptr because there is no python frame yet (e.g. we're at the entry
    // breakpoint and python hasn't started running yet)
    if (!maybe_frame || !(*maybe_frame)) {
        log("missing frame\n");
        return false;
    }
    auto& frame = *maybe_frame;
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
#if PY_VERSION_HEX < 0x03090000
    // this is required in 3.6 and below. it is a no-op in 3.7 and above.
    // despite being a no-op in 3.7 and above, we still wrap it in a version check
    // because the function is deprecated in 3.9 and will emit a compiler warning.
    // in 3.6, this MUST be called in python's main thread.
    PyEval_InitThreads();

    // XXX: I think importing threading in init_trace will implicitly call PyEval_InitThreads
    // for us so we don't have to do this?
#endif
}

void PybeImpl::stop_and_notify_gdb() {
    m_level = 0;
    m_move_in_progress.reset();
    ::raise(get_gil_signal_number());
}

int PybeImpl::trace_call(PyFrameObject& pyframe) {
    auto frame = to_frame(pyframe, 0);
    // log("[CALL] %s in %s:%d\n", frame.location.function_name.c_str(), frame.location.file_name.c_str(), frame.location.line_number);

    auto matching_breakpoint = [&]() -> std::unique_ptr<Breakpoint> {
        for (const auto& bp : m_breakpoints) {
            if (bp.location.function_name == frame.location.function_name) {
                return std::make_unique<Breakpoint>(bp);
            }
            // if a filename was set, assume that it was the basename.
            // python gives us the full path so we need to basename it.
            const auto basenm = String{::basename(frame.location.file_name.c_str())};
            if (bp.location.file_name == basenm && bp.location.line_number == frame.location.line_number) {
                return std::make_unique<Breakpoint>(bp);
            }
        }
        return nullptr;
    }();

    if (matching_breakpoint) {
        m_last_hit_breakpoint = std::move(matching_breakpoint);
        stop_and_notify_gdb();
    } else if (m_move_in_progress) {
        m_move_in_progress->current_depth++;
        using enum PybeImpl::Move::Type;
        if (m_move_in_progress->type == Step) {
            stop_and_notify_gdb();
        }
    }

    return 0;
}

int PybeImpl::trace_return(PyFrameObject& frame, PyObject& /* return_value */) {
    auto next_frame = to_frame(frame, 0);
    // log("[RETURN] %s in %s:%d\n", next_frame.location.function_name.c_str(), next_frame.location.file_name.c_str(), next_frame.location.line_number);

    using enum PybeImpl::Move::Type;
    if (m_move_in_progress) {
        m_move_in_progress->current_depth--;
    }

    return 0;
}

int PybeImpl::trace_line(PyFrameObject& pyframe) {
    auto next_frame = to_frame(pyframe, 0);
    // log("[LINE] %s in %s:%d\n", next_frame.location.function_name.c_str(), next_frame.location.file_name.c_str(), next_frame.location.line_number);

    auto matching_breakpoint = [&]() -> std::unique_ptr<Breakpoint> {
        for (const auto& bp : m_breakpoints) {
            // if a filename was set, assume that it was the basename.
            // python gives us the full path so we need to basename it.
            const auto basenm = String{::basename(next_frame.location.file_name.c_str())};
            if (bp.location.file_name == basenm && bp.location.line_number == next_frame.location.line_number) {
                return std::make_unique<Breakpoint>(bp);
            }
        }
        return nullptr;
    }();

    using enum PybeImpl::Move::Type;

    if (matching_breakpoint) {
        m_last_hit_breakpoint = std::move(matching_breakpoint);
        stop_and_notify_gdb();
    } else if (m_move_in_progress){
        const auto& move = *m_move_in_progress;
        switch (move.type) {
            case Step: {
                if (next_frame != move.start) stop_and_notify_gdb();
                break;
            }
            case Next: {
                if (next_frame != move.start && move.current_depth <= 0) stop_and_notify_gdb();
                break;
            }
            case Finish: {
                if (next_frame != move.start && move.current_depth < 0) stop_and_notify_gdb();
                break;
            }
        }
    }

    return 0;
}

bool PybeImpl::init_trace(Py_tracefunc tracefunc, PyCFunction wrapped_tracefunc) {
    log("pybe: init_trace\n");

    if (!PyGILState_Check()) [[ unlikely ]] {
        log("need gil\n");
        return false;
    }

    // install trace on currently running threads
    set_trace_all_threads(tracefunc);

    // the python C API doesn't give us access to threading.settrace directly. threading.settrace
    // tells the python interpreter to attach a tracing function to every new thread.
    //
    // so we wrap a C function as a python callable object and call threading.settrace from
    // python. the wrapped function will install the faster C API tracing method the first time
    // it's called.

    {
        // tell python to install it on any threads spawned in the future

        // FIXME: this import might fail. technically in python <= 3.6, the threading module
        // is optional and doesn't always come with python
        auto module_threading = StrongRef<PyObject>{nullptr};
        if (module_threading = PyImport_ImportModule("threading"); !module_threading) {
            log("failed to import threading\n");
            return false;
        }

        static auto gdb4hpc_trace_method = PyMethodDef{
            .ml_name = "gdb4hpc_trace",
            .ml_meth = wrapped_tracefunc,
            .ml_flags = METH_VARARGS,
            .ml_doc = "gdb4hpc python trace function",
        };

        auto wrapped_tracefunc = StrongRef<PyObject>(PyCFunction_New(&gdb4hpc_trace_method, nullptr));

        // note that the "O" fmt string tells python to increment the refcount
        // of wrapped_tracefunc so it's okay to let our handle to it drop when
        // this scope ends
        if (PyObject_CallMethod(module_threading.ptr, "settrace", "O", wrapped_tracefunc.ptr) == nullptr) {
            log("failed to threading.settrace\n");
            // TODO PyObject_CallMethod also sets an exception on failure, handle it (CPE-5688)
            return false;
        }
    }

    return true;
}

Expected<Breakpoint, std::nullptr_t> make_breakpoint(unsigned long ptid, int feid, const char* filename, int line, const char* funcname) {
    if (!filename || !funcname) [[ unlikely ]] {
        log("null parameters\n");
        return nullptr;
    }

    if (funcname[0] != '\0' || (filename[0] != '\0' && line != 0)) {
        // enough info to be inserted as-is

        return Breakpoint{
            .feid = feid,
            .location = {
                .function_name = funcname,
                .file_name = ::basename(filename),
                .line_number = line,
            }
        };
    } else if (line && filename[0] == '\0') {
        // get filename from current frame

        auto* tstate = get_threadstate(ptid);
        if (tstate == nullptr) {
            log("couldn't find thread\n");
            return nullptr;
        }

        auto py_frame = StrongRef(PyThreadState_GetFrame(tstate));
        if (!py_frame.ptr) [[ unlikely ]] {
            log("couldn't get frame from thread\n");
            return nullptr;
        }
        auto frame = to_frame(*py_frame.ptr, 0);

        return Breakpoint{
            .feid = feid,
            .location = {
                .function_name = "",
                .file_name = ::basename(frame.location.file_name.c_str()),
                .line_number = line,
            }
        };
    }

    log("invalid breakpoint parameter combo: %s in %s:%d\n",
        funcname, filename, line);
    return nullptr;
}

bool PybeImpl::set_breakpoint(uint32_t seq, unsigned long ptid, int feid, const char* filename, int line, const char* funcname) {
    if (!PyGILState_Check()) [[ unlikely ]] {
        log("need gil\n");
        return false;
    }

    if (const auto maybe_breakpoint = make_breakpoint(ptid, feid, filename, line, funcname)) {
        const auto& breakpoint = *maybe_breakpoint;
        m_breakpoints.push_back(breakpoint);

        auto wire = breakpoint.serialize();
        log("serialized breakpoint: \"%s\"\n", wire.c_str());
        if (m_connection->send_with_timeout(wire.c_str(), wire.size(), seq) == -1) [[ unlikely ]] {
            log("failed to send breakpoint result\n");
            return false;
        }

        return true;
    } else {
        return false;
    }
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

    using enum PybeImpl::Move::Type;
    m_move_in_progress = std::make_unique<Move>(Move{
        .start = to_frame(*frame, 0),
        .current_depth = 0,
        .type = step_in ? Step : Next,
    });

    return true;
}

bool PybeImpl::install_finish_trap(unsigned long py_thread_id) {
    auto* tstate = get_threadstate(py_thread_id);
    if (!tstate) return false;

    auto frame = StrongRef(PyThreadState_GetFrame(tstate));
    if (!frame.ptr) return false;

    using enum PybeImpl::Move::Type;
    m_move_in_progress = std::make_unique<Move>(Move{
        .start = to_frame(*frame, 0),
        .current_depth = 0,
        .type = Finish,
    });

    return true;
}

} // namespace pybe
