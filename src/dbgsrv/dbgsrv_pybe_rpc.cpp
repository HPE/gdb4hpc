// Copyright (c) 2024 Hewlett Packard Enterprise Development LP.

#include <dlfcn.h>
#include <future>
#include <regex>
#include <filesystem>

#include "config.h"
#include "unittest_macros.h"

#include "dbgevent.h"
#include "dbgsrv_defs.h"
#include "mi_utils.h"
#include "onscopeexit.h"
#include "str.h"

#include "dbgsrv_pybe_rpc.h"

namespace {

template <typename FN>
void try_send_command_impl(MISession& sess, MICommand& cmd, FN fail_callback, const char* file, int line) {
    DVPRINT("pybe mi command from %s:%d\n", file, line);
    SendMICommand(&sess, &cmd);
    if (!MICommandResultOK(&cmd)) {
        DVPRINT("pybe mi command failed: %s:%d\n", file, line);
        fail_callback();
    }
}

// using MISession& `sess`, run the mi command `cmd` and check
// MICommandResultOK. if it's not ok, call `fail_callback`.
//
// this is a macro so __file__ and __line__ reflect the callsite
#define try_send_command(sess, cmd, fail_callback) try_send_command_impl(sess, cmd, fail_callback, __FILE__, __LINE__);

template <typename FN>
void try_pybe_fn_check_true_impl(MISession& sess, const char* expr, pybe::RPC::c_thread_id_t tid, FN fail_callback, const char* file, int line) {
    auto cmd = make_MICmd(MIDataEvaluateExpressionThread(expr, tid));
    try_send_command_impl(sess, *cmd, fail_callback, file, line);

    const auto result = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
    if (result.get_view() != "true") {
        DVPRINT("pybe mi command ran successfully, but did not return true: %s:%d\n", file, line);
        fail_callback();
    }
}

// using MISession& `sess`, evaluate the expression const char* `pybe_fn` on
// c_thread_id_t `tid`. assume that the expression is calling a function that
// will return the value true on success, and check that it does.
//
// if the expression fails to evaluate (e.g. syntax error) or does not
// evaluate to true (e.g. pybe internal failure), call `fail_calllback`.
//
// this is a macro so __file__ and __line__ reflect the callsite
#define try_pybe_fn_check_true(sess, pybe_fn, tid, fail_callback) try_pybe_fn_check_true_impl(sess, pybe_fn, tid, fail_callback, __FILE__, __LINE__);

// this is a macro so __file__ and __line__ reflect the callsite
#define todo_write_better_error_message() (NewErrorEvent(DBGERR_DEBUGGER, "unknown python error: %s:%d", __FILE__, __LINE__))

} // anon namespace

namespace pybe {

RPC::RPC(MISession& parent_sess)
    : m_parent_session{parent_sess}
{}

void RPC::init_GIL(c_thread_id_t tid) {
    const auto fn = "pybe::init_GIL()";
    auto cmd = make_MICmd(MIDataEvaluateExpressionThread(fn, tid));
    try_send_command(m_parent_session, *cmd, [](){
        throw NewErrorEvent(DBGERR_DEBUGGER,
            "Failed to set up python debugging.\n"
            "This is a bug. Please report it and include the following info:\n"
            "failure: failed to initialze python");
        // TODO include error text from gdb
    });
}

int RPC::get_python_minor_version(c_thread_id_t tid) {
    const auto python_version_str = [&](){
        auto cmd = make_MICmd(MIDataEvaluateExpressionThread("(char*)Py_GetVersion()", tid));

        try_send_command(m_parent_session, *cmd, [](){
            throw NewErrorEvent(DBGERR_DEBUGGER,
                "Failed to detect python version. Disabling python debugging "
                "for this rank. Re-enable with `set python on`.\n");
        });

        // XXX: wasteful get_string seems to be required by std::regex api for some reason
        auto full_version_buf = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get())).get_string();
        // examples of what we're expecting:
        // 0x7fffed2cd760 "3.6.8 (default, Jan  5 2024, 08:58:17) \n[GCC 8.5.0 20210514 (Red Hat 8.5.0-20)]"
        // 0x7fffed2cd760 "3.0a5+ (py3k:63103M, May 12 2008, 00:53:55) \n[GCC 4.2.3]"
        static std::regex py_version_re{R"(\b3\.(\d+)[^\d])"};
        auto match = std::smatch{};
        if (std::regex_search(full_version_buf, match, py_version_re)) {
            return std::string{match[1].first, match[1].second};
        } else {
            DVPRINT("python inject: failed to parse python version.\n");
            // expecting caller to disable python
            throw NewErrorEvent(DBGERR_DEBUGGER,
                "Failed to detect python version. Disabling python debugging "
                "for this rank. Re-enable with `set python on`.");
        }
    }();

    const auto version = ::strtol(python_version_str.c_str(), nullptr, 10);
    if (version == 0) {
        throw todo_write_better_error_message();
    }
    return version;
}

void RPC::inject_pybe_so(int minor_version, c_thread_id_t tid) {
    // NOTE: not bothering to check that the inferior has loaded libdl because this
    //       is only used on python inferiors and it always has libdl.
    //
    // TODO: make sure this doesn't explode when it's run on a non-cpython binary
    //       (notably on a binary without libdl)

    const auto lib_path = [&]() {
        // in the attach case the user's app won't have gdb4hpc/lib in LD_LIBRARY_PATH
        // and the dlopen expression below will fail - so we need to construct an absolute path
        const auto dbgsrv = std::filesystem::read_symlink(std::string{"/proc/"} + std::to_string(::getpid()) + "/exe");
        const auto so_name = std::string{PYBE_SO_UNVERSIONED_BASENAME} + "3." + std::to_string(minor_version) + ".so";
        return dbgsrv.parent_path().parent_path() / "lib" / so_name;
    }();

    // hopefully the RTLD_NOW constant is the same on the build machine and the target machine...
    auto open_exp = std::string{"\"(void *)dlopen(\\\""} + lib_path.string() + "\\\", " + std::to_string(RTLD_NOW) + ")\"";
    auto cmd = make_MICmd(MIDataEvaluateExpressionThread(open_exp.c_str(), tid));

    try_send_command(m_parent_session, *cmd, [](){
        throw NewErrorEvent(DBGERR_DEBUGGER,
            "Failed to set up python debugging (loading gdb4hpc python library failed)\n"
            "Disabling python debugging for this rank. Re-enable with `set python on`.");
        }
    );

    auto lib_handle_addr = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
    if (lib_handle_addr.get_view() == "0x0") {
        DVPRINT("pythonBEInject: dlopen failed\n");

        auto dlerror_result = std::string{"unspecified"};
        auto dlerror_cmd = make_MICmd(MIDataEvaluateExpressionThread("(char*)dlerror()", tid));
        SendMICommand(&m_parent_session, dlerror_cmd.get());

        if (!MICommandResultOK(dlerror_cmd.get())) {
            DVPRINT("(%s) couldn't run dlerror to get failure reason\n", open_exp.c_str());
        } else {
            auto dlerror_str = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(dlerror_cmd.get()));
            dlerror_result = dlerror_str.get_string();
            DVPRINT("(%s) dlerror: %s\n", open_exp.c_str(), dlerror_result.c_str());
        }

        throw NewErrorEvent(DBGERR_DEBUGGER,
            "Failed to set up python debugging.\n"
            "This is a bug. Please report it and include the following info:\n"
            "failure: dlopen call\n"
            "error: %s\n",
            dlerror_result.c_str());
    }
}

void RPC::open_pybe_channel(c_thread_id_t tid) {
    const auto socket_path = []() {
        // this needs to start with @ - the communication implementation will replace it
        // with '\0' which will result in an anonymous namespace unix domain socket.
        // the anonymous namespace does not interact with the disk so we don't have to worry
        // about building legit/unused/permissioned paths, but the namespace is shared by
        // the whole os so this still needs to be unique to each dbgsrv.
        return std::string{"@gdb4_pybe_"} + std::to_string(::getpid());
    }();

    if (auto maybe_pybe_server = pybe::Server::make(socket_path)) {
        m_pybe_server = std::make_unique<pybe::Server>(std::move(*maybe_pybe_server));
    } else {
        // XXX: calling strerror is probably wrong here at this point
        DVPRINT("failed to create server socket: %s\n", strerror(errno));
        // TODO better error message
        throw NewErrorEvent(DBGERR_INTERNAL, "Connection to python failed.");
    }

    // SendMICommand will block until the command completes and the
    // connection function on the other end will block until it makes a
    // connection to us or times out (see pybe::connect_to_dbgsrv).
    // This is problematic because we also need to accept the connection on
    // our end. Since both ends necessarily block (or fail), we can't
    // complete the connection operation in a single thread so we put one
    // half of the operation in a future.
    //
    // We intentionally put the accept_with_timeout call in the future
    // in favor of the SendMICommand call because we know that
    // accept_with_timeout will never block indefinitely and cause cleaning
    // up the future to be problematic. SendMICommand isn't supposed to
    // block indefinitely either (the function on the other end has a
    // timeout of its own), but accept_with_timeout has a much smaller
    // surface area than the *entire MI subsystem* and it is easy to verify
    // that it will never block forever whereas that is not easy to verify
    // for a SendMICommand call.

    auto channel_f = std::async(std::launch::async, [](pybe::Server& server){
        if (auto maybe_channel = server.accept_with_timeout()) {
            return std::make_unique<pybe::Channel>(std::move(*maybe_channel));
        } else {
            // XXX: calling strerror is probably wrong here at this point
            DVPRINT("failed to create server socket: %s\n", strerror(errno));
            // TODO better error message
            throw NewErrorEvent(DBGERR_INTERNAL, "Connection to python failed.");
        }
    }, std::ref(*m_pybe_server));

    {   // call connection in pybe
        const auto expr = "(bool)pybe::connect_to_dbgsrv(\"" + socket_path + "\")";
        try_pybe_fn_check_true(m_parent_session, expr.c_str(), tid, [](){
            throw NewErrorEvent(DBGERR_INTERNAL, "Connection to python failed.");
        });
    }

    // letting any thrown exceptions propogate out here
    m_pybe_channel = channel_f.get();
}

RPC::c_thread_id_t RPC::spawn_pybe_gil_thread(c_thread_id_t tid) {
    // we are about to spawn a thread in the user's app, and we need the new
    // thread id to be able to switch to it and run pybe functions. to get the
    // id, we catch the `thread-created` notification that gdbmi will emit when we
    // call the function to start the thread.
    //
    // we need to do the parsing of the thread id directly in the callback. i.e. we
    // can't just save the MIList* and then get back to it. this is because the MIList*
    // only lives as long as the callback and is immediately destroyed once the callback returns.

    int pybe_thread_id = -1;

    {
        auto old_callback = m_parent_session.notify_callback;
        auto restore_callback_on_exit = OnScopeExit([&](){
            MISessionRegisterNotifyCallback(&m_parent_session, old_callback, nullptr);
        });

        MISessionRegisterNotifyCallback(&m_parent_session, [](void* tid, char *class_name, MIList *notif) {
            // assuming that there will only be one thread-created variable over
            // the course of this
            if (!class_name || std::string_view{class_name} != "thread-created") {
                // this isn't the notification with the thread id
                return;
            }

            MIListSet(notif);
            for (auto* keyval = (MIResult*)MIListGet(notif); keyval; keyval = (MIResult*)MIListGet(notif)) {
                if (::strcmp(keyval->variable, "id") == 0 && keyval->value && keyval->value->cstring) {
                    *static_cast<int*>(tid) = ::atoi(keyval->value->cstring);
                    return;
                }
            }
        }, &pybe_thread_id);

        const auto fn = "pybe::spawn_GIL_grabbing_thread()";
        auto cmd = make_MICmd(MIDataEvaluateExpressionThread(fn, tid));

        SendMICommand(&m_parent_session, cmd.get());
        if (!MICommandResultOK(cmd.get())) {
            // XXX: can't really check for error here because gdb reports the SIGTRAP that
            // the new thread receives as a debugger error despite well defined behavior.
            // but maybe we could do some parsing of the output text.

            // DVPRINT("PythonBEGetGIL: error starting GIL thread\n");
            // return NewErrorEvent(DBGERR_DEBUGGER,
            //     "Failed to set up python debugging.\n"
            //     "This is a bug. Please report it and include the following info:\n"
            //     "failure: failed to start GIL thread (%s)\n",
            //     fn);
            // // TODO include error text from gdb
        }

        // this should have been set in the callback we set up
        if (pybe_thread_id == -1) {
            DVPRINT("PythonBEGetGIL: error getting thread id\n");
            throw NewErrorEvent(DBGERR_DEBUGGER,
                "Failed to set up python debugging.\n"
                "This is a bug. Please report it and include the following info:\n"
                "failure: failed to get thread ID");
        }
    }

    return pybe_thread_id;
}

RPC::c_thread_id_t RPC::current_thread_get_id() {
    auto cmd = make_MICmd(CLIInfoThreads());

    try_send_command(m_parent_session, *cmd, [](){
        throw NewErrorEvent(DBGERR_DEBUGGER,
            "Failed to set up python debugging.\n"
            "This is a bug. Please report it and include the following info:\n"
            "failure: failed to get current thread");
        // TODO include error text from gdb
    });

    auto thread_info = std::unique_ptr<CLIInfoThreadsInfo, decltype(&CLIInfoThreadsInfoFree)>
        (CLIGetInfoThreadsInfo(cmd.get()), CLIInfoThreadsInfoFree);

    return thread_info->current_thread_id;
}

RPC::py_thread_id_t RPC::thread_get_python_id(c_thread_id_t tid) {
    {
        auto cmd = make_MICmd(MIDataEvaluateExpressionThread("(bool)pybe::is_python_thread()", tid));

        try_send_command(m_parent_session, *cmd, [=](){
            throw NewErrorEvent(DBGERR_DEBUGGER, "Thread %u is not a python thread.", tid);
        });

        auto result = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
        if (result.get_view() != "true") {
            throw NewErrorEvent(DBGERR_DEBUGGER, "Thread %u is not a python thread.", tid);
        }
    }

    {
        auto cmd = make_MICmd(MIDataEvaluateExpressionThread("(long)pybe::get_python_thread_id()", tid));

        try_send_command(m_parent_session, *cmd, [](){
            throw NewErrorEvent(DBGERR_DEBUGGER,
                "Failed to set up python debugging.\n"
                "This is a bug. Please report it and include the following info:\n"
                "failure: failed to determine thread id");
        });

        auto result = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
        const auto version = ::strtol(result.get(), nullptr, 10);
        if (version == 0) throw todo_write_better_error_message();
        return version;
    }
}

void RPC::thread_interrupt(c_thread_id_t tid) {
    // the interrupt command will generate a "program received signal 0" event
    // if it interrupts a running thread. we hide it from the user.
    // threads that are already stopped don't emit any signal events.

    const auto old_event_callback = m_parent_session.event_callback;
    const auto old_event_callback_ctx = m_parent_session.event_callback_ctx;
    auto restore_event_callback_on_exit = OnScopeExit([&](){
        MISessionRegisterEventCallback(&m_parent_session, old_event_callback, old_event_callback_ctx);
    });

    bool got_one = false;
    bool waited = false; // for logging purposes only TODO remove

    MISessionRegisterEventCallback(&m_parent_session, [](void* ctx, MIEvent* /*e*/) {
        // we don't store the event itself because mi will free it after the callback returns
        DVPRINT("suppressed event\n");
        *static_cast<bool*>(ctx) = true;
    }, &got_one);

    auto cmd = make_MICmd(MIExecInterruptThread(tid));

    try_send_command(m_parent_session, *cmd, [](){
        throw NewErrorEvent(DBGERR_DEBUGGER,
            "Failed to set up python debugging.\n"
            "This is a bug. Please report it and include the following info:\n"
            "failure: failed to switch to GIL thread");
        // TODO include error text from gdb
    });

    // sometimes it takes a moment for gdb to emit the sigzero event
    MISessionProgress(&m_parent_session);
    for (size_t tries_left = 32; tries_left > 0 && !got_one; tries_left--) {
        ::usleep(25000); // very short - we don't want to keep the user waiting too long
                         // in the case that the thread was already stopped and no sigzero is coming
        MISessionProgress(&m_parent_session);
        waited = true;
    }

    // if we still haven't caught an event, the worst that will happen is the user sees
    // a kind of ugly error message. we'll let it slide.
    DVPRINT("was the wait necessary? (finish) waited:%d got_one:%d\n", waited, got_one);
}

void RPC::thread_name(c_thread_id_t tid, std::string_view name) {
    // the thread name command is only available in the console interpreter and
    // only operates on the current thread, so we have to silently switch threads around
    auto cleanup = RestoreCurrentThreadOnScopeExit{*this};

    thread_select(tid);
    auto argv = std::vector<const char*>{};
    argv.push_back("thread");
    argv.push_back("name");
    auto name_buf = std::string{name};
    argv.push_back(name_buf.c_str());

    auto cmd = make_MICmd(CLIBypass(argv.size(), const_cast<char**>(argv.data())));
    try_send_command(m_parent_session, *cmd, [](){
        // don't throw on failure - this is purely aesthetic and shouldn't bring down the whole ship if it fails
        DVPRINT("failed to set gdb4hpc thread name! oh well...\n");
    });
}

void RPC::thread_select(c_thread_id_t tid) {
    // MIThreadSelect is broken and will hang forever if we select a running thread
    // so we use CLIBypass
    auto argv = std::vector<const char*>{};
    argv.push_back("thread");
    auto id_buf = std::to_string(tid);
    argv.push_back(id_buf.c_str());

    auto cmd = make_MICmd(CLIBypass(argv.size(), const_cast<char**>(argv.data())));
    try_send_command(m_parent_session, *cmd, [](){
        throw todo_write_better_error_message();
    });
}

bool RPC::thread_has_GIL(c_thread_id_t tid) {
    auto cmd = make_MICmd(MIDataEvaluateExpressionThread("(bool)PyGILState_Check()", tid));

    try_send_command(m_parent_session, *cmd, [](){
        throw todo_write_better_error_message();
    });

    auto result_buf = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
    return result_buf.get_view() == "true";
}

void RPC::thread_take_GIL(c_thread_id_t tid) {
    // when we `call` the pybe function later, it will raise a SIGTRAP once it has
    // the GIL. by default, gdb will restore the stack back to the state that it was
    // before the `call` unless we turn off unwindonsignal.
    //
    // FIXME: we need to restore this to its previous setting afterwards
    {
        auto cmd = make_MICmd(MIGDBSet("unwindonsignal", "off"));

        try_send_command(m_parent_session, *cmd, [](){
            throw NewErrorEvent(DBGERR_DEBUGGER,
                "Failed to set up python debugging.\n"
                "This is a bug. Please report it and include the following info:\n"
                "failure: failed to disable unwind-on-signal");
            // TODO include error text from gdb
        });
    }

    // we are about to start doing stuff that generates async events.
    // we don't want the user to see any of these, so catch them all while we're here
    const auto old_event_callback = m_parent_session.event_callback;
    const auto old_event_callback_ctx = m_parent_session.event_callback_ctx;
    auto restore_event_callback_on_exit = OnScopeExit([&](){
        MISessionRegisterEventCallback(&m_parent_session, old_event_callback, old_event_callback_ctx);
    });

    MIEvent* sigtrap_event = nullptr;

    MISessionRegisterEventCallback(&m_parent_session, [](void* ctx, MIEvent* e) {
        *static_cast<MIEvent**>(ctx) = e;
    }, &sigtrap_event);

    {   // call the grabbing function and catch the SIGTRAP
        const auto fn = "pybe::get_GIL_then_breakpoint()";
        auto cmd = make_MICmd(MIDataEvaluateExpressionThread(fn, tid));

        SendMICommand(&m_parent_session, cmd.get());
        if (!MICommandResultOK(cmd.get())) {
            // XXX: can't really check for error here because gdb reports the SIGTRAP that
            // the new thread receives as a debugger error despite well defined behavior.
            // but maybe we could do some parsing of the output text.
        }

        // our GIL-grabbing thread is now running. now we wrangle async events until our
        // thread has the GIL. how we do that depends on all-stop vs non-stop mode.

        MISessionProgress(&m_parent_session);
        for (size_t tries_left = 9; tries_left > 0 && !sigtrap_event; tries_left--) {
            ::usleep(250000);
            MISessionProgress(&m_parent_session);
        }

        if (sigtrap_event) {
            DVPRINT("Caught event type %d\n", sigtrap_event->type);
            MIEventFree(sigtrap_event);
            // TODO verify that this is the SIGTRAP
        } else {
            throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to set up python debugging.");
        }
    }
}

std::optional<RPC::c_thread_id_t> RPC::any_thread_has_GIL() {
    {
        // gdb will emit an error for the below thread apply call below if *any* single
        // thread is running, so we have to manually stop threads before the command
        // and restart threads after.

        // TODO make this function side-effect free by storing which threads
        // were running and restoring them after we stop everything
    }

    {
        auto cmd = make_MICmd(MIExecInterruptAll());
        try_send_command(m_parent_session, *cmd, [](){
            throw todo_write_better_error_message();
        });
    }

    c_thread_id_t gil_thread = 0;

    {
        const char* argv[] = { "thread", "apply", "all", "p", "(int)PyGILState_Check()" };
        auto cmd = make_MICmd(CLIBypass(sizeof(argv) / sizeof(argv[0]), const_cast<char**>(argv)));
        try_send_command(m_parent_session, *cmd, [](){
            throw todo_write_better_error_message();
        });

        auto result_buf = MallocedCharPtr(MICommandConsoleOOBMessages(cmd.get()));
        static const auto re = std::regex{R"(Thread (\d+).*:\n\$\d+ = (\d+))"};

        auto match = std::smatch{};

        auto search = result_buf.get_string();
        while (std::regex_search(search, match, re)) {
            if (match[2] == "1") {
                gil_thread = ::strtol(std::string{match[1]}.c_str(), nullptr, 10);
                break;
            }
            search = match.suffix();
        }
    }

    {
        // TODO resume threads that were running
    }

    DVPRINT("thread currently holding gil: %u\n", gil_thread);

    if (gil_thread == 0) return std::nullopt;

    return gil_thread;
}

void RPC::thread_continue(c_thread_id_t tid) {
    auto cmd = make_MICmd(MIExecContinueThread(tid));
    try_send_command(m_parent_session, *cmd, [](){
        throw todo_write_better_error_message();
    });
}

void RPC::thread_finish_with_timeout(c_thread_id_t tid, unsigned int frame) {
    // in all-stop mode, finish events change the current thread
    auto preserver = RestoreCurrentThreadOnScopeExit{*this};

    // the -exec-finish commands below will generate a *stopped event which
    // we don't want the user to see
    auto old_event_callback = m_parent_session.event_callback;
    auto old_event_callback_ctx = m_parent_session.event_callback_ctx;
    auto restore_event_callback_on_exit = OnScopeExit([&](){
        MISessionRegisterEventCallback(&m_parent_session, old_event_callback, old_event_callback_ctx);
    });

    bool got_one = false;
    bool waited = false; // for logging purposes only TODO remove

    MISessionRegisterEventCallback(&m_parent_session, [](void* ctx, MIEvent* /*e*/) {
        // TODO make sure the event is freed by mi somewhere and we are not leaking here
        *static_cast<bool*>(ctx) = true;
    }, &got_one);

    try_send_command(m_parent_session, *make_MICmd(MIExecFinishThreadFrame(tid, frame)), [](){
        throw todo_write_better_error_message();
    });

    // eat the generated *stopped
    MISessionProgress(&m_parent_session);
    for (size_t tries_left = 32; tries_left > 0 && !got_one; tries_left--) {
        ::usleep(25000);
        MISessionProgress(&m_parent_session);
        waited = true;
    }

    // TODO detect and protect from failures to finish the function (e.g. infinite loops)

    // if we still haven't caught an event, the worst that will happen is the user sees
    // a kind of ugly error message. we'll let it slide.
    DVPRINT("was the wait necessary? (finish) waited:%d got_one:%d\n", waited, got_one);
}

pybe::Backtrace RPC::get_elf_backtrace(c_thread_id_t tid) {
    auto cmd = make_MICmd(MIStackListAllFramesThread(tid));
    try_send_command(m_parent_session, *cmd, [](){
        throw todo_write_better_error_message();
    });

    auto miframes = MIGetStackListFramesInfo(cmd.get());
    if (!miframes) throw todo_write_better_error_message();
    auto miframes_cleanup = OnScopeExit([&](){
        MIListFree(miframes, (void (*)())MIFrameFree);
    });

    auto result = Backtrace{};

    MIListSet(miframes);
    for (
        MIFrame* frame = (MIFrame*)MIListGet(miframes);
        frame != nullptr;
        frame = (MIFrame*)MIListGet(miframes)
    ) {
        result.frames.emplace_back(Frame{
            .function_name = frame->func ? frame->func : "",
            .file_name = frame->file ? frame->file : "",
            .line_number = frame->line,
        });
    }

    if (result.frames.empty()) {
        throw todo_write_better_error_message();
    }

    return result;
}

pybe::Backtrace RPC::get_python_backtrace(c_thread_id_t tid, py_thread_id_t py_thread_id) {
    if (!m_pybe_channel) {
        throw todo_write_better_error_message();
    }

    auto& channel = *m_pybe_channel;

    const auto seq = channel.generate_sequence_number();
    {
        const auto fn = std::string{"\"pybe::get_backtrace("} + std::to_string(seq) + ", " + std::to_string(py_thread_id) + ")\"";
        auto cmd = make_MICmd(MIDataEvaluateExpressionThread(fn.c_str(), tid));

        try_send_command(m_parent_session, *cmd, [](){
            throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to get backtrace");
        });
    }

    auto buf = channel.recv_with_timeout(seq);
    if (buf == "") {
        // TODO: be more informative
        throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to get backtrace");
    }

    const auto* cursor = buf.c_str();
    if (auto result = pybe::Backtrace::deserialize(cursor, buf.size())) {
        return *result;
    }

    // TODO: be more informative
    throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to get backtrace");
}

pybe::Frame RPC::get_python_frame(c_thread_id_t tid, py_thread_id_t py_thread_id) {
    if (!m_pybe_channel) {
        throw todo_write_better_error_message();
    }

    auto& channel = *m_pybe_channel;

    const auto seq = channel.generate_sequence_number();
    const auto fn = std::string{"pybe::get_frame("} + std::to_string(seq) + "," + std::to_string(py_thread_id) + ")";

    try_pybe_fn_check_true(m_parent_session, fn.c_str(), tid, [](){
        throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to get frame");
    });

    auto buf = channel.recv_with_timeout(seq);
    if (buf == "") {
        // TODO: be more informative
        throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to get frame");
    }

    const auto* cursor = buf.c_str();
    if (auto result = pybe::Frame::deserialize(cursor, buf.size())) {
        return *result;
    }

    // TODO: be more informative
    throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to get backtrace");
}

std::string RPC::eval_elf_expression(c_thread_id_t tid, size_t frame, std::string_view expr) {
    // note the '"' at the beginning of `gdb_expr` below - in order to properly
    // send an expression with spaces to gdbmi, we have to surround the whole thing
    // in quotes. then we have to surround the char* argument with escaped quotes, looking
    // something like this: "eval_expr(seq, \"<expr>\")". now, if expr *itself* contains
    // quotes, those need to be escaped *twice* since they are two layers of quotes deep.
    auto double_escaped = std::string{expr};
    double_escaped = std::regex_replace(double_escaped, std::regex{R"(")"}, R"(\\\")");
    const auto gdb_expr = std::string{"\""} + double_escaped + "\"";

    auto cmd = make_MICmd(MIDataEvaluateExpressionThreadFrame(gdb_expr.c_str(), tid, frame));
    try_send_command(m_parent_session, *cmd, [](){
        throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to eval");
    });

    const auto result = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
    return result.get_string();
}

std::string RPC::eval_py_expression(c_thread_id_t tid, py_thread_id_t ptid, std::string_view expr) {
    if (!m_pybe_channel) {
        throw todo_write_better_error_message();
    }

    auto& channel = *m_pybe_channel;

    const auto seq = channel.generate_sequence_number();
    {
        // note the '"' at the beginning of `gdb_expr` below - in order to properly
        // send an expression with spaces to gdbmi, we have to surround the whole thing
        // in quotes. then we have to surround the char* argument with escaped quotes, looking
        // something like this: "eval_expr(seq, \"<expr>\")". now, if expr *itself* contains
        // quotes, those need to be escaped *twice* since they are two layers of quotes deep.
        auto double_escaped = std::string{expr};
        double_escaped = std::regex_replace(double_escaped, std::regex{R"(")"}, R"(\\\")");

        const auto gdb_expr = std::string{"\"(bool)pybe::eval_expr("} + std::to_string(seq) + "," + std::to_string(ptid) + ",\\\"" + double_escaped + "\\\")\"";

        try_pybe_fn_check_true(m_parent_session, gdb_expr.c_str(), tid, [](){
            throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to eval");
        });
    }

    auto buf = channel.recv_with_timeout(seq);
    if (buf == "") {
        // TODO: be more informative
        throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to eval");
    }

    return buf;
}

void RPC::start_tracing(c_thread_id_t tid) {
    try_pybe_fn_check_true(m_parent_session, "pybe::init_trace()", tid, [](){
        throw NewErrorEvent(DBGERR_DEBUGGER,
            "Failed to set up python debugging.\n"
            "This is a bug. Please report it and include the following info:\n"
            "failure: failed to set up tracing");
        // TODO include error text from gdb
    });
}

const char* RPC::get_tracing_function_name() {
    return "pybe::trace";
}

const char* RPC::get_GIL_holding_function_name() {
    return "pybe::PybeImpl::get_GIL_then_breakpoint";
}

const char* RPC::get_GIL_releasing_function_name() {
    return "pybe::get_GIL_then_breakpoint";
}

pybe::Breakpoint RPC::set_breakpoint(c_thread_id_t tid, int feid, std::string_view filename, int line, std::string_view funcname) {
    // TODO support spaces
    const auto fn = std::string{"pybe::set_breakpoint("} +
        std::to_string(feid) + "," +
        "\"" + std::string{filename} + "\"," +
        std::to_string(line) + "," +
        "\"" + std::string{funcname} + "\"" +
    ")";

    try_pybe_fn_check_true(m_parent_session, fn.c_str(), tid, [](){
        throw todo_write_better_error_message();
    });

    return pybe::Breakpoint {
        .feid = feid,
        .location = pybe::Frame {
            .function_name = std::string{funcname},
            .file_name = std::string{filename},
            .line_number = line,
        }
    };
}

void RPC::delete_breakpoint(c_thread_id_t tid, int feid) {
    const auto fn = std::string{"pybe::delete_breakpoint("} + std::to_string(feid) +")";
    try_pybe_fn_check_true(m_parent_session, fn.c_str(), tid, [](){
        throw todo_write_better_error_message();
    });
}

bool RPC::at_breakpoint(c_thread_id_t tid) {
    const auto fn = std::string{"pybe::have_breakpoint()"};
    auto cmd = make_MICmd(MIDataEvaluateExpressionThread(fn.c_str(), tid));
    try_send_command(m_parent_session, *cmd, [](){
        throw todo_write_better_error_message();
    });

    auto result = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
    return result.get_view() == "true";
}

pybe::Breakpoint RPC::take_current_breakpoint(c_thread_id_t tid) {
    if (!m_pybe_channel) {
        throw todo_write_better_error_message();
    }
    auto& channel = *m_pybe_channel;

    const auto seq = channel.generate_sequence_number();
    {
        const auto fn = std::string{"pybe::take_current_breakpoint("} + std::to_string(seq) + ")";
        auto cmd = make_MICmd(MIDataEvaluateExpressionThread(fn.c_str(), tid));

        try_send_command(m_parent_session, *cmd, [](){
            throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to get breakpoint");
        });
    }

    auto buf = channel.recv_with_timeout(seq);
    if (buf.empty()) {
        // TODO: be more informative
        throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to get breakpoint");
    }

    const auto* cursor = buf.c_str();
    if (auto result = pybe::Breakpoint::deserialize(cursor, buf.size())) {
        return *result;
    } else {
        return {};
    }
}

void RPC::insert_step_trap(c_thread_id_t tid, py_thread_id_t ptid, bool step_in) {
    const auto fn = std::string{"pybe::install_step_trap("} + std::to_string(ptid) + "," + std::to_string(step_in) + ")";
    try_pybe_fn_check_true(m_parent_session, fn.c_str(), tid, [](){
        throw todo_write_better_error_message();
    });
}

RPC::RestoreCurrentThreadOnScopeExit::RestoreCurrentThreadOnScopeExit(RPC& rpc)
    : parent_rpc{rpc}
{
    this->old_thread_id = this->parent_rpc.current_thread_get_id();
}

RPC::RestoreCurrentThreadOnScopeExit::~RestoreCurrentThreadOnScopeExit() {
    this->parent_rpc.thread_select(this->old_thread_id);
}

} // namespace pybe

TEST_CASE("parse thread apply all p (int)PyGILState_Check()") {
    const std::string check =
        "Thread 1 (Thread 0x7fdea386a700 (LWP 3947601) \"python3.10\"):\n"
        "$1 = 0\n"
        "Thread 2 (Thread 0x7fdea386a700 (LWP 3947601) \"python3.10\"):\n"
        "$2 = 1\n"
    ;

    const auto re = std::regex{R"(Thread (\d+).*:\n\$\d+ = (\d+))"};

    auto match = std::smatch{};

    REQUIRE(std::regex_search(check, match, re));
    CHECK(match[1] == "1");
    CHECK(match[2] == "0");

    auto next = std::string{match.suffix()};
    REQUIRE(std::regex_search(next, match, re));
    CHECK(match[1] == "2");
    CHECK(match[2] == "1");
}
