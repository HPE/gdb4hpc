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
    // the only place a pybe function_name should be written down as a magic
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
    // for other versions, it's a no-op.
    void init_GIL(c_thread_id_t tid);

    // create the socket used for delivering large pybe results (e.g. backtraces).
    // use `tid` to call the initialization functions.
    void open_pybe_channel(c_thread_id_t tid);

    // return the thread id of the currently selected thread.
    c_thread_id_t current_thread_get_id();

    // give thread `tid` a new `name`. the new name will be visible to the user
    // in the output of commands like info thread.
    void thread_set_name(c_thread_id_t tid, std::string_view name);

    // get the python id of the current thread `tid`. if `tid` is not a python thread
    // (e.g. a plain old pthread not registered with python), throws an error.
    py_thread_id_t thread_get_python_id(c_thread_id_t tid);

    // interrupt and stop thread `tid`. safe to call on already stopped threads.
    void thread_interrupt(c_thread_id_t tid);

    // start thread `tid` running.
    void thread_continue(c_thread_id_t tid);

    // return true if thread `tid` currently holds the GIL.
    bool thread_has_GIL(c_thread_id_t tid);

    // run thread `tid` until it returns from frame number `frame`.
    // frame 0 is the deepest frame, but not necessarily the current frame.
    //
    // XXX: the following is intended but not currently implemented:
    // if the thread doesn't finish the function after a short time, stop the
    // thread, give up, and throw an error.
    void thread_finish_with_timeout(c_thread_id_t tid, unsigned int frame);

    // check if *any* thread currently has the GIL. if one does, return its thread id.
    std::optional<c_thread_id_t> get_GIL_holding_thread();

    // run thread `tid` and have it compete for the GIL. if it doesn't get the GIL in
    // a short amount of time, the function gives up and throws an error.
    void thread_take_GIL(c_thread_id_t tid);

    // get elf backtrace of thread `tid`
    pybe::Backtrace get_elf_backtrace(c_thread_id_t tid);

    // use thread `tid` to call pybe and generate a backtrace for python thread `ptid`.
    pybe::Backtrace get_python_backtrace(c_thread_id_t tid, py_thread_id_t ptid);

    // use thread `tid` to call pybe and get the current frame of python thread `ptid`.
    pybe::Frame get_python_frame(c_thread_id_t tid, py_thread_id_t ptid);

    // use thread `tid` to call pybe and change the current python frame level.
    //
    // if `dir` is Up or Down and `count` exceeds the end of the stack, the
    // operation will stop at the end of the stack. if `dir` is Absolute and `count`
    // is invalid, no change will take place. The python stack on `ptid` will be used
    // for the checks.
    void move_python_frame_level(c_thread_id_t tid, py_thread_id_t ptid, int count, pybe::FrameDirection dir);

    // evaluate expression `expr` in frame `frame` on thread `tid` and return the result.
    std::string eval_elf_expression(c_thread_id_t tid, size_t frame, std::string_view expr);

    // use thread 'tid' to call pybe and evaluate 'expr' in the context of python thread `ptid`.
    std::string eval_py_expression(c_thread_id_t tid, py_thread_id_t ptid, std::string_view expr);

    // use thread 'tid' to call pybe and initialize the pybe trace/breakpoint system.
    void install_breakpoint_trace(c_thread_id_t tid);

    // some python debugging functionality requires knowing pybe function names.
    // for example, getting the GIL safely requires knowing if pybe is stopped
    // in the python tracing callback that it installs because in that case it
    // already has the GIL.
    //
    // to avoid hard coding these function names anywhere, we provide some
    // functions to get them here.

    // return the name of the tracing function that `start_tracing` will install.
    const char* get_tracing_function_name();

    // return the name of the function that will call PyGILState_Ensure when
    // `thread_take_GIL` is called.
    const char* get_GIL_holding_function_name();

    // return the name of the function that will call PyGILState_Release to
    // complete the sequence that `thread_take_GIL` initializes.
    const char* get_GIL_releasing_function_name();

    // use thread `tid` to call pybe and insert a breakpoint for `funcname` in
    // `filename` or `filename`:`line`. `filename`, `line`, and `funcname` can
    // be left unspecified as empty strings or 0. an omitted field is simply
    // ignored during matching. for example setting a breakpoint at line 9 with
    // no filename or function name will match line 9 in any file.
    //
    // the breakpoint will be saved saved with id `feid`.
    //
    // XXX: pybe does not verify that the passed arguments are valid. e.g. if a
    // file/function actually exists
    pybe::Breakpoint set_breakpoint(c_thread_id_t tid, py_thread_id_t ptid, int feid, std::string_view filename, int line, std::string_view funcname);

    // use thread `tid` to call pybe and delete the breakpoint with id `feid`.
    void delete_breakpoint(c_thread_id_t tid, int feid);

    // use thread `tid` to call pybe and check if the reason that pybe just
    // raised the GIL signal is that pybe detected a breakpoint.
    bool at_breakpoint(c_thread_id_t tid);

    // if `at_breakpoint` returns true, this function can be called to use
    // thread `tid` to call pybe and obtain the breakpoint that was just hit.
    // this will also clear the breakpoint from the internal pybe buffer that
    // holds it. i.e. this function can only be called once per breakpoint hit.
    //
    // XXX: maybe at_breakpoint and this function can be merged into one.
    pybe::Breakpoint take_current_breakpoint(c_thread_id_t tid);

    // use thread `tid` to call pybe and insert a trap that will be activated
    // when python thread `ptid` starts executing a new line. if `step_in` is
    // true, the trap will be activated by function calls. if it's false,
    // function calls will be skipped (mimics the gdb commands step and next,
    // respectively).
    //
    // note that this function does not start any threads - that needs to be
    // done separately.
    void insert_step_trap(c_thread_id_t tid, py_thread_id_t ptid, bool step_in);

    // use thread `tid` to call pybe and insert a trap that will be activated
    // when python thread `ptid` returns from its current function.
    //
    // note that this function does not start any threads - that needs to be
    // done separately.
    void insert_finish_trap(c_thread_id_t tid, py_thread_id_t ptid);

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
