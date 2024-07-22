// Copyright (c) 2024 Hewlett Packard Enterprise Development LP.

#include <dlfcn.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <future>
#include <regex>

#include "dbgevent.h"
#include "dbgsrv_defs.h"
#include "mi_utils.h"
#include "onscopeexit.h"
#include "str.h"

#include "dbgsrv_pybe.h"

namespace {

std::string generate_socket_path() {
    // this needs to start with @ - the communication implementation will replace it
    // with '\0' which will result in an anonymous namespace unix domain socket.
    // the anonymous namespace does not interact with the disk so we don't have to worry
    // about building legit/unused/permissioned paths, but the namespace is shared by
    // the whole os so this needs to be unique to each dbgsrv.
    return std::string{"@gdb4_pybe_"} + std::to_string(::getpid());
}

int g_pybe_thread_id = -1;
MIEvent* g_gil_sigtrap_event = nullptr;

} // anon namespace

Pybe::Pybe(MISession& mi)
    : m_parent_session{mi}
{
    // TODO: confirm that g4h's /lib directory is in the inferior's LD_LIBRARY_PATH.
    //       this might not be automatic if the user loaded gdb4hpc after launching
    //       their app or has their app clear its environment.
    //
    // NOTE: not bothering to check that the inferior has loaded libdl because this
    //       is only used on python inferiors and it always has libdl.
    //
    // TODO: make sure this doesn't explode when it's run on a non-cpython binary
    //       (notably on a binary without libdl)
    //
    // TODO: make a LIBPYBE.SO constant instead of hard coding the library name

    // hopefully the RTLD_NOW constant is the same on the build machine and the target machine...
    auto open_exp = std::string{"\"(void *)dlopen(\\\"libgdb4_pybe.so\\\", "} + std::to_string(RTLD_NOW) + ")\"";
    auto cmd = make_MICmd(MIDataEvaluateExpression(open_exp.c_str()));
    SendMICommand(&mi, cmd.get());
    if (!MICommandResultOK(cmd.get())) {
        DVPRINT("pythonBEInject: got error from [%s]\n", open_exp.c_str());
        throw NewErrorEvent(DBGERR_DEBUGGER,
            "failed to set up python debugging.\n"
            "This is a bug. Please report it and include the following info:\n"
            "failure: dlopen expression: [%s]\n",
            open_exp.c_str());
    }

    auto lib_handle_addr = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
    if (lib_handle_addr.get_view() == "0x0") {
        DVPRINT("pythonBEInject: dlopen failed\n");

        auto dlerror_result = std::string{"unspecified"};
        auto dlerror_cmd = make_MICmd(MIDataEvaluateExpression("(char*)dlerror()"));
        SendMICommand(&mi, dlerror_cmd.get());

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

    {
        if (auto maybe_pybe_server = pybe::Server::make(generate_socket_path())) {
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
            const auto expr = "(bool)pybe::connect_to_dbgsrv(\"" + generate_socket_path() + "\")";
            auto cmd = make_MICmd(MIDataEvaluateExpression(expr.c_str()));
            SendMICommand(&mi, cmd.get());

            if (!MICommandResultOK(cmd.get())) {
                DVPRINT("Failed to start connection in pybe\n");
                throw NewErrorEvent(DBGERR_INTERNAL, "Connection to python failed.");
            }

            // TODO check the actual return value of the command. !MICommandResultOK
            // only indicates that the gdb action of calling the function worked.
        }

        // letting any thrown exceptions propogate out here
        m_pybe_channel = channel_f.get();
    }

    {
        // XXX: we are about to spawn a thread in the user's app, and we need the new
        // thread id to be able to switch to it and run pybe functions. to get the
        // id, we catch the `thread-created` notification that gdbmi will emit when we
        // call the function to start the thread.
        //
        // the only way to get at this information is through the libmi callbacks. the
        // callbacks don't return anything, so we need to get the information out through
        // a side channel. unfortunately the callback is stored as a raw function pointer
        // which means we can't give it a capturing lambda with a properly scoped
        // side channel variable. we have to resort to a global (g_pybe_thread_id).
        //
        // we also need to do the parsing of the thread id directly in the callback. i.e. we
        // can't just save the MIList* and then get back to it. this is because the MIList*
        // only lives as long as the callback and is immediately destroyed once the callback returns.
        //
        // okay, let's get it over with.

        g_pybe_thread_id = -1;

        {
            auto old_callback = m_parent_session.notify_callback;
            auto restore_callback_on_exit = OnScopeExit([&](){
                MISessionRegisterNotifyCallback(&m_parent_session, old_callback);
            });

            MISessionRegisterNotifyCallback(&m_parent_session, [](char *class_name, MIList *notif) {
                // assuming that there will only be one thread-created variable over
                // the course of this
                if (!class_name || std::string_view{class_name} != "thread-created") {
                    // this isn't the notification with the thread id
                    return;
                }

                MIListSet(notif);
                for (auto* keyval = (MIResult*)MIListGet(notif); keyval; keyval = (MIResult*)MIListGet(notif)) {
                    if (::strcmp(keyval->variable, "id") == 0 && keyval->value && keyval->value->cstring) {
                        g_pybe_thread_id = ::atoi(keyval->value->cstring);
                        return;
                    }
                }

                // never found it
                g_pybe_thread_id = -1;
            });

            // TODO: don't make this function name a magic string
            const auto fn = "pybe::spawn_GIL_grabbing_thread()";
            auto cmd = make_MICmd(MIDataEvaluateExpression(fn));

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
            if (g_pybe_thread_id == -1) {
                DVPRINT("PythonBEGetGIL: error getting thread id\n");
                throw NewErrorEvent(DBGERR_DEBUGGER,
                    "Failed to set up python debugging.\n"
                    "This is a bug. Please report it and include the following info:\n"
                    "failure: failed to get thread ID");
            }

            m_gil_thread_id = g_pybe_thread_id;
        }
    }
}

Pybe::GILHolder Pybe::coerce_gil() {
    return GILHolder(m_parent_session, m_gil_thread_id, *m_pybe_channel);
}

Pybe::GILHolder::GILHolder(MISession& mi, int gil_thread_id, pybe::Channel& channel)
    : m_parent_session{mi}
    , m_parent_pybe_channel{channel}
    , m_gil_thread_id{gil_thread_id}
{
    // TODO check if we already have it and/or verify that we're in the right fn

    DVPRINT("getting GIL handle...\n");

    {   // remember old thread
        const char* argv[] = { "thread" };
        auto cmd = make_MICmd(CLIBypass(1, const_cast<char**>(argv)));

        SendMICommand(&m_parent_session, cmd.get());
        if (!MICommandResultOK(cmd.get())) {
            DVPRINT("PythonBEGetGIL: getting current thread\n");
            throw NewErrorEvent(DBGERR_DEBUGGER,
                "Failed to set up python debugging.\n"
                "This is a bug. Please report it and include the following info:\n"
                "failure: failed to get current thread");
            // TODO include error text from gdb
        }

        // std::regex_match below *requires* a std::string for whatever reason
        auto result = MallocedCharPtr(MICommandConsoleOOBMessages(cmd.get())).get_string();
        static std::regex current_thread_re{R"(.*thread is (\d+).*\n)"};
        std::smatch match;
        if (std::regex_match(result, match, current_thread_re)) {
            m_old_thread_id = ::strtoul(match[1].str().c_str(), nullptr, 10);
        }
    }

    {   // check that we are in a python thread
        auto cmd = make_MICmd(MIDataEvaluateExpression("(bool)pybe::is_python_thread()"));

        SendMICommand(&m_parent_session, cmd.get());
        if (!MICommandResultOK(cmd.get())) {
            DVPRINT("PythonBEGetGIL: error calling is_python_thread\n");
            throw NewErrorEvent(DBGERR_DEBUGGER,
                "Failed to set up python debugging.\n"
                "This is a bug. Please report it and include the following info:\n"
                "failure: failed to determine thread category");
            // TODO include error text from gdb
        }

        auto result = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
        if (result.get_view() != "true") {
            throw NewErrorEvent(DBGERR_DEBUGGER, "Current thread has no python state.");
        }
    }

    {   // get the python thread id
        auto cmd = make_MICmd(MIDataEvaluateExpression("(long)pybe::get_python_thread_id()"));

        SendMICommand(&m_parent_session, cmd.get());
        if (!MICommandResultOK(cmd.get())) {
            DVPRINT("PythonBEGetGIL: error calling get_python_thread_id\n");
            throw NewErrorEvent(DBGERR_DEBUGGER,
                "Failed to set up python debugging.\n"
                "This is a bug. Please report it and include the following info:\n"
                "failure: failed to determine thread id");
        }

        auto result = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
        m_py_thread_id = ::strtol(result.get(), nullptr, 10);
    }

    {   // switch to GIL grabber thread
        auto cmd = make_MICmd(MIThreadSelect(gil_thread_id));

        SendMICommand(&m_parent_session, cmd.get());
        if (!MICommandResultOK(cmd.get())) {
            DVPRINT("PythonBEGetGIL: error switching to GIL thread\n");
            throw NewErrorEvent(DBGERR_DEBUGGER,
                "Failed to set up python debugging.\n"
                "This is a bug. Please report it and include the following info:\n"
                "failure: failed to switch to GIL thread");
            // TODO include error text from gdb
        }
    }

    // we are about to start doing stuff that generates async events.
    // we don't want the user to see any of these, so catch them all while we're here
    auto old_event_callback = m_parent_session.event_callback;
    auto restore_event_callback_on_exit = OnScopeExit([&](){
        MISessionRegisterEventCallback(&m_parent_session, old_event_callback);
    });

    MISessionRegisterEventCallback(&m_parent_session, [](MIEvent* e) {
        std::swap(g_gil_sigtrap_event, e);
        if (e) MIEventFree(e);
    });

    {   // make sure grabber thread is stopped so we can run commands
        const char* argv[] = { "interrupt" };
        auto cmd = make_MICmd(CLIBypass(1, const_cast<char**>(argv)));

        SendMICommand(&m_parent_session, cmd.get());
        if (!MICommandResultOK(cmd.get())) {
            DVPRINT("PythonBEGetGIL: error switching to GIL thread\n");
            throw NewErrorEvent(DBGERR_DEBUGGER,
                "Failed to set up python debugging.\n"
                "This is a bug. Please report it and include the following info:\n"
                "failure: failed to switch to GIL thread");
            // TODO include error text from gdb
        }
    }

    // when we `call` the pybe function later, it will raise a SIGTRAP once it has
    // the GIL. by default, gdb will restore the stack back to the state that it was
    // before the `call` unless we turn off unwindonsignal.
    //
    // FIXME: we need to restore this to its previous setting afterwards
    {
        auto cmd = make_MICmd(MIGDBSet("unwindonsignal", "off"));

        SendMICommand(&m_parent_session, cmd.get());

        if (!MICommandResultOK(cmd.get())) {
            DVPRINT("PythonBEGetGIL: error disabling unwind on signal\n");
            throw NewErrorEvent(DBGERR_DEBUGGER,
                "Failed to set up python debugging.\n"
                "This is a bug. Please report it and include the following info:\n"
                "failure: failed to disable unwind-on-signal");
            // TODO include error text from gdb
        }
    }

    {   // call the grabbing function and catch the SIGTRAP
        const auto fn = "pybe::get_GIL_then_breakpoint()";
        auto cmd = make_MICmd(MIDataEvaluateExpression(fn));

        if (g_gil_sigtrap_event) {
            MIEventFree(g_gil_sigtrap_event);
            g_gil_sigtrap_event = nullptr;
        }

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

        // our GIL-grabbing thread is now running. now we wrangle async events until our
        // thread has the GIL. how we do that depends on all-stop vs non-stop mode.
        //
        // in non-stop mode, the call above immediately starts the thread and
        // a SIGTRAP is generated immediately. in all-stop mode, the call above
        // somehow makes it to the SIGTRAP and stops, but gdb doesn't report
        // it until a continue is issued (?, more investigation needed)
        //
        // XXX: the below code only handles non-stop mode.

        MISessionProgress(&m_parent_session);
        for (size_t tries_left = 9; tries_left > 0 && !g_gil_sigtrap_event; tries_left--) {
            ::usleep(250000);
            MISessionProgress(&m_parent_session);
        }

        if (g_gil_sigtrap_event) {
            MIEvent* evt = nullptr;
            std::swap(g_gil_sigtrap_event, evt);
            DVPRINT("Caught event type %d\n", evt->type);
            MIEventFree(evt);
            // TODO verify that this is the SIGTRAP
        } else {
            throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to set up python debugging.");
        }
    }
}

Pybe::GILHolder::~GILHolder() {
    // reset state to what it was before GILHolder.
    // assume we are in the sigtrap func called from wait loop

    try {
        {   // switch to pybe thread
            auto cmd = make_MICmd(MIThreadSelect(m_gil_thread_id));
            SendMICommand(&m_parent_session, cmd.get());
            if (!MICommandResultOK(cmd.get())) {
                // can't do much
                DVPRINT("In ~GILHolder, thread command failed!\n");
            }
        }

        // escape from inner pybe functions
        auto old_event_callback = m_parent_session.event_callback;
        auto restore_event_callback_on_exit = OnScopeExit([&](){
            MISessionRegisterEventCallback(&m_parent_session, old_event_callback);
        });

        // the -exec-finish commands below will generate a *stopped event which
        // we don't want the user to see
        MISessionRegisterEventCallback(&m_parent_session, [](MIEvent* e) {
            std::swap(g_gil_sigtrap_event, e);
            if (e) MIEventFree(e);
        });

        if (g_gil_sigtrap_event) {
            MIEventFree(g_gil_sigtrap_event);
            g_gil_sigtrap_event = nullptr;
        }

        {   // escape std::raise
            auto cmd = make_MICmd(MIExecFinish());
            SendMICommand(&m_parent_session, cmd.get());
            if (!MICommandResultOK(cmd.get())) {
                // can't do much
                DVPRINT("In ~GILHolder, finish command failed!\n");
            }
        }

        {   // escape get_GIL_then_breakpoint
            auto cmd = make_MICmd(MIExecFinish());
            SendMICommand(&m_parent_session, cmd.get());
            if (!MICommandResultOK(cmd.get())) {
                // can't do much
                DVPRINT("In ~GILHolder, finish command failed!\n");
            }
        }

        // eat the generated *stopped
        MISessionProgress(&m_parent_session);
        for (size_t tries_left = 9; tries_left > 0 && !g_gil_sigtrap_event; tries_left--) {
            // I don't think this sleep/loop is actually necessary, but just in case
            ::usleep(250000);
            MISessionProgress(&m_parent_session);
        }

        if (g_gil_sigtrap_event) {
            MIEvent* evt = nullptr;
            std::swap(g_gil_sigtrap_event, evt);
            DVPRINT("Caught event type %d\n", evt->type);
            MIEventFree(evt);
            // TODO verify that this is the *stopped from exec-finish
        } else {
            // not much we can do about that
            DVPRINT("Never got the *stopped...\n");
        }

        {   // switch back to old thread
            auto cmd = make_MICmd(MIThreadSelect(m_old_thread_id));
            SendMICommand(&m_parent_session, cmd.get());
            if (!MICommandResultOK(cmd.get())) {
                // can't do much
                DVPRINT("In ~GILHolder, thread command failed!\n");
            }
        }
    } catch (const std::exception& e) {
        DVPRINT("In ~GILHolder, caught exception! %s\n", e.what());
    } catch (...) {
        DVPRINT("In ~GILHolder, caught unknown exception!\n");
    }
}

pybe::Backtrace Pybe::GILHolder::get_backtrace() {
    const auto seq = m_parent_pybe_channel.generate_sequence_number();
    {
        const auto fn = std::string{"\"pybe::get_backtrace("} + std::to_string(seq) + ", " + std::to_string(m_py_thread_id) + ")\"";
        auto cmd = make_MICmd(MIDataEvaluateExpression(fn.c_str()));

        SendMICommand(&m_parent_session, cmd.get());
        if (!MICommandResultOK(cmd.get())) {
            // TODO better error message
            throw NewErrorEvent(DBGERR_DEBUGGER, "Failed to get backtrace");
        }
    }

    auto buf = m_parent_pybe_channel.recv_with_timeout(seq);
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
