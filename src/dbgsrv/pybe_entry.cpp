// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// ... TODO ...

#include <csignal>
#include <sys/socket.h>
#include <sys/un.h>

#include "pythoncapi_compat.h"

#include "pybe_stl.h"
#include "pybe_comms.h"

struct DbgsrvConnection {
    int fd = -1;
};

auto g_connection = DbgsrvConnection{};

namespace pybe {

// TODO: name these functions better

bool get_backtrace() {
    if (!PyGILState_Check()) {
        log("need gil\n");
        return false;
    }

    auto* intr = PyInterpreterState_Head();
    auto* tstate = PyInterpreterState_ThreadHead(intr);

    // find real thread state
    // XXX: in my current test program, the thread returned above is not the
    //      correct thread - it's the thread that pybe spawned. so we need to
    //      find the current *python* thread. make a more robust way to find that.
    tstate = PyThreadState_Next(tstate);

    // TODO: assert(tstate != nullptr);

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

        // log("%s in %s:%d\n", funcname.data(), filename.data(), linenum);
        result.frames.emplace_back(
            Frame{
                .function_name = String(funcname.data(), funcname.size()),
                .file_name = String(filename.data(), filename.size()),
                .line_number = linenum
            }
        );
    }

    auto wire = result.serialize();
    // TODO every socket call needs protection from indefinite blocking, signal interrupts, etc.
    if (::send(g_connection.fd, wire.c_str(), wire.size(), 0) < 1) {
        return false;
    }

    return true;
}

// endpoint is a const char* instead of say, a string or a string_view, because
// it is easier to create a const char* on the gdb command line.
bool connect_to_dbgsrv(const char* endpoint) {
    g_connection.fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (g_connection.fd == -1) {
        log("failed to create socket: %s\n", strerror(errno));
        return false;
    }

    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    ::memset(addr.sun_path, 0, sizeof(addr.sun_path));
    // TODO safer copy
    ::memcpy(addr.sun_path, endpoint, ::strlen(endpoint));

    if (addr.sun_path[0] == '@') {
        // setting the first byte to zero indicates that the path is
        // in an abstract namespace - it doesn't depend on the filesystem
        // and won't be present there. it still has to be OS-unique, we
        // assume dbgsrv did that work an gave us a unique path already.
        addr.sun_path[0] = '\0';
    }

    // TODO every socket call needs protection from indefinite blocking, signal interrupts, etc.
    if (::connect(g_connection.fd, (sockaddr*)&addr, sizeof(addr))) {
        log("failed to connect socket: %s\n", strerror(errno));
        return false;
    }

    return true;
}

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
    auto maybe_grabber = pybe::Thread::make(&get_GIL_then_breakpoint, nullptr);
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

    // TODO restore signals

    return true;
}

} // namespace pybe
