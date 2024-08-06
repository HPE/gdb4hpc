// Copyright (c) 2024 Hewlett Packard Enterprise Development LP.
//
// remote procedure calls into the pybe shared library.

#include <memory>
#include <optional>

#include "MISession.h"
#include "pybe_comms_dbgsrv.h"

namespace pybe {

class RPC {
    // remote procedure calls into the pybe shared library.
    //
    // the only place a pybe::function_name() should be written down as a magic
    // string, anywhere in gdb4hpc, is in this class.
    //
    // the class has almost no invariants. it is meant to be a very low level
    // collection of logically atomic pybe operations. these actions are meant to
    // be composed into versions with stronger invariants elsewhere (e.g. in the
    // Pybe class). for example, `get_python_backtrace` requires
    // `open_pybe_channel` to be called first - `get_python_backtrace` will not
    // call it for you. however, `get_python_backtrace` *will* check for it and
    // prevent itself from waiting forever on a closed socket. in cases like this,
    // RPC will throw a dbgevent_t* error as an exception.
    //
    // all that being said, RPC does have a few basic invariants:
    //
    // 1. it won't segfault or hang forever
    // 2. after finishing a function, the current thread is the
    //    same as it was when the function was initially called
    //
    // XXX: this one is not implemented yet!
    // 3. after finishing a function, the threads that were running
    //    when the function was called are still running and vice versa
    //    for stopped threads.
    //
    // almost all actions are parameterized by a thread id. it is important to
    // choose the right thread id to execute the RPC, especially in all-stop mode.
    // most of the time, the easy choice is to use the injected pybe thread, but
    // there are times when you want to use other threads - for example, if you
    // are holding a python thread at a sensitive location in all stop mode and
    // run an RPC on a *different* thread, the python thread will slip! but running
    // with the python thread will return it to its original position
    // (other threads will still slip though!)
public:
    typedef unsigned int c_thread_id_t; // as obtained in gdb e.g. info thread
    typedef long py_thread_id_t; // special python-only thread id - obtain with thread_get_python_id

    RPC(MISession& session);

    // all of the RPC functions throw dbgevent_t* error events on error.
    // if the function returns without throwing, it was successful.

    // return the minor version of the python inferior. e.g. if we are attached
    // to python 3.6.9, this returns 6. uses thread `tid` to obtain the result.
    //
    // this does not require pybe to be injected first.
    int get_python_minor_version(c_thread_id_t tid);

    // inject the pybe shared library of the specified `minor_version` into the
    // python inferior. the thread specified with `tid` will be used to call
    // dlopen etc.
    //
    // this function is required to be called before using any other RPC functions.
    void inject_pybe_so(int minor_version, c_thread_id_t tid);

    // the rest of the functions require that pybe has been injected.

    // create the pybe internal thread. use `tid` to call the initialization functions.
    // return the thread id of the newly created thread.
    c_thread_id_t spawn_pybe_gil_thread(c_thread_id_t tid);

    // initialize python internals and create the GIL. `tid` *must* be the
    // main python thread. this is only meaningful on python versions <= 3.6.
    void init_GIL(c_thread_id_t tid);

    // create the socket used for delivering large pybe results (e.g. backtraces).
    // use `tid` to call the initialization functions.
    void open_pybe_channel(c_thread_id_t tid);

    // return the thread id of the currently selected thread.
    c_thread_id_t current_thread_get_id();

    // give thread `tid` a new `name`. the new name will be visible to the user
    // in the output of commands like info thread.
    void thread_name(c_thread_id_t tid, std::string_view name);

    // get the python id of the current thread `tid`. if `tid` is not a python thread
    // (e.g. a plain old pthread not registered with python), throws an error.
    py_thread_id_t thread_get_python_id(c_thread_id_t tid);

    // interrupt and stop thread `tid`. safe to call on already stopped threads.
    void thread_interrupt(c_thread_id_t tid);

    // start thread `tid` running.
    void thread_continue(c_thread_id_t tid);

    // return true if thread `tid` currently holds the GIL.
    bool thread_has_GIL(c_thread_id_t tid);

    // run thread `tid` until it finishes its current function.
    //
    // XXX: the following is intended but not currently implemented:
    // if the thread doesn't finish the function after a short time, stop the
    // thread, give up, and throw an error.
    void thread_finish_with_timeout(c_thread_id_t tid);

    // check if *any* thread currently has the GIL. if one does, return its thread id.
    std::optional<c_thread_id_t> any_thread_has_GIL();

    // run thread `tid` and have it compete for the GIL. if it doesn't get the GIL in
    // a short amount of time, the function gives up and throws an error.
    void thread_take_GIL(c_thread_id_t tid);

    // use thread `tid` to call pybe and generate a backtrace for python thread `ptid`.
    pybe::Backtrace get_python_backtrace(c_thread_id_t tid, py_thread_id_t ptid);

private:
    // this is private because RPC is supposed to be side-effect free, but some
    // implementations require a
    // 1. push thread
    // 2. do operation on `tid`
    // 3. pop thread
    // implmentation.
    void thread_select(c_thread_id_t tid);

    struct RestoreCurrentThreadOnScopeExit {
        // in all stop mode, the current thread is automatically changed to
        // any thread that encounters a breakpoint/signal/etc. use this to
        // hide that change from the user.

        RestoreCurrentThreadOnScopeExit(RPC& rpc);
        ~RestoreCurrentThreadOnScopeExit();

        RPC& parent_rpc;
        RPC::c_thread_id_t old_thread_id;
    };

    MISession& m_parent_session;

    // after successfully calling open_pybe_channel, these will be non-null
    std::unique_ptr<pybe::Server> m_pybe_server;
    std::unique_ptr<pybe::Channel> m_pybe_channel;
};

} // namespace pybe
