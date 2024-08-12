// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// pybe bootstrapping and initialization

#include <csignal>
#include <sys/socket.h>
#include <sys/un.h>

#include <unistd.h>

#include "pythoncapi_compat.h"

#include "pybe_stl.h"
#include "pybe_comms_pybe.h"

struct DbgsrvConnection {
    pybe::Channel channel;
};

DbgsrvConnection* g_connection = nullptr;

namespace pybe {

// TODO: name these functions better

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

bool get_backtrace(uint32_t seq, unsigned long py_thread_id) {
    log("get_backtrace\n");
    if (!g_connection) [[ unlikely ]] {
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

    for (auto frame = StrongRef(PyThreadState_GetFrame(tstate)); frame; frame = StrongRef(PyFrame_GetBack(frame.ptr))) {
        auto code = StrongRef(PyFrame_GetCode(frame.ptr));

        // these are string views because _AsUTF8 triggers the internal generation
        // of the PyObject's UTF8 representation, which caches and returns it.
        //
        // XXX: these member accesses are not ABI portable and need to be wrapped in a portable function.
        //      revisit when we do multiple builds for multiple python versions.
        auto funcname = StringView(PyUnicode_AsUTF8(code->co_name));
        auto filename = StringView(PyUnicode_AsUTF8(code->co_filename));
        auto linenum = PyFrame_GetLineNumber(frame.ptr);

        log("%s in %s:%d\n", funcname.data(), filename.data(), linenum);
        result.frames.emplace_back(
            Frame{
                .function_name = String(funcname.data(), funcname.size()),
                .file_name = String(filename.data(), filename.size()),
                .line_number = linenum
            }
        );
    }

    auto wire = result.serialize();
    log("serialized backtrace: \"%s\"\n", wire.c_str());
    if (g_connection->channel.send_with_timeout(wire.c_str(), wire.size(), seq) == -1) [[ unlikely ]] {
        log("failed to send backtrace result\n");
        return false;
    }

    return true;
}

bool eval_expr(uint32_t seq, unsigned long py_thread_id, const char* expr) {
    auto* tstate = get_threadstate(py_thread_id);
    if (tstate == nullptr) {
        log("couldn't find thread\n");
        return false;
    }

    auto frame = StrongRef(PyThreadState_GetFrame(tstate));
    if (!frame.ptr) {
        log("missing frame\n");
        return false;
    }

    auto globals = StrongRef(PyFrame_GetGlobals(frame.ptr));
    auto locals = StrongRef(PyFrame_GetLocals(frame.ptr));

    auto result = StrongRef(PyRun_String(expr, Py_eval_input, globals.ptr, locals.ptr));

    char buf[8192];
    auto* stream = ::fmemopen(buf, sizeof(buf), "w");

    PyObject_Print(result.ptr, stream, 0); // TODO error check
    ::fclose(stream); // adds the null terminator

    if (g_connection->channel.send_with_timeout(buf, strlen(buf), seq) == -1) [[ unlikely ]] {
        log("failed to send eval result\n");
        return false;
    }

    return true;
}

bool is_python_thread() {
    return PyGILState_GetThisThreadState() != nullptr;
}

unsigned long get_python_thread_id() {
    // check that this is safe with is_python_thread first!
    //
    // XXX: in python 3.6 and below, the thread id is signed, but in
    // python 3.7 and above it's unsigned. since we're just comparing bits we
    // always cast to unsigned
    return (unsigned long)PyGILState_GetThisThreadState()->thread_id;
}

// endpoint is a const char* instead of say, a string or a string_view, because
// it is easier to create a const char* on the gdb command line.
bool connect_to_dbgsrv(const char* endpoint) {
    auto maybe_client = pybe::Client::make(endpoint);
    if (!maybe_client) [[ unlikely ]] {
        // TODO better error
        return false;
    }
    auto& client = *maybe_client;

    auto maybe_channel = client.connect_with_timeout();
    if (!maybe_channel) {
        // TODO better error
        return false;
    }

    g_connection = new DbgsrvConnection{
        .channel = std::move(*maybe_channel),
    };

    return true;
}

void get_GIL_then_breakpoint() {
    auto gil = PyGILState_Ensure();
    ::raise(get_gil_signal_number());
    // gdb does its thing here...
    // ...
    // then gdb sends a continue
    PyGILState_Release(gil);
}

// argument and return value are unused, just needed to conform to pthread
void* wait_for_grab_request(void*) {
    bool keep_going_forever_until_gdb_says_so = true;
    while (keep_going_forever_until_gdb_says_so) {
        ::sleep(1);
    };
    return nullptr;
}

bool spawn_GIL_grabbing_thread() {
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
        // TODO error reporting mechanism
        return false;
    }
    auto& grabber = *maybe_grabber;

    if (grabber.detach()) {
        // TODO error reporting mechanism
        // ... or is this a fatal error? what are we supposed to do?
        return false;
    }

    return true;
}

void init_GIL() {
    Py_Initialize(); // no-op when called for a 2nd time, so safe to call without any check
#if PY_VERSION_HEX < 0x03070000
    // this is required in 3.6 and below. it is a no-op in 3.7 and above.
    // in 3.6, this MUST be called in python's main thread.
    PyEval_InitThreads();
#endif
}
} // namespace pybe
