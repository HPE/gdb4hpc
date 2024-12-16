// Copyright (c) 2024 Hewlett Packard Enterprise Development LP.
//
// interface to pybe used by dbgsrv

#pragma once

#include "dbgsrv_pybe_rpc.h"
#include "dbgcmd.h"

class Pybe {
    // Pybe is a singleton-like interface for dbgsrv to use to interact with the
    // injected pybe shared library. Constructing Pybe will inject the pybe shared
    // library into the inferior at which point dbgsrv is able to use Pybe to start
    // interacting with the so.
    //
    // most shared library functions require the GIL and are behind a different API, `GILHolder`.
    // use `Pybe::run_until_gil` to arrange that the pybe shared library has the GIL and
    // is ready for further commands.
public:
    // inject and set up pybe. throws on failure. should only be called once for each inferior.
    Pybe(MISession& mi);

    class GILHolder;
    // coerce the inferior into a state where the pybe thread has the global
    // interpreter lock by running the necessary threads until the lock is held.
    // return an API to actions that are only available under such conditions.
    // this might mean running user python threads until they release the GIL.
    //
    // when the returned object is destructed, coerce the
    // inferior into a state where the GIL is released by
    // running it a short while.
    //
    // this function is marked [[ nodiscard ]] because side effects happen when
    // the returned GILHolder is destructed. it is probably a programmer error
    // to call this function and not store the return value. if you have such a
    // use case, use trivialize_gil.
    //
    // throws dbgevent_t* error on failure.
    [[ nodiscard ]]
    GILHolder run_until_gil(pybe::RPC::c_thread_id_t tid);

    // coerce the inferior into a state where calling run_until_gil would not
    // require any user threads to run.
    //
    // this function simply calls run_until_gil and then immediately discards the
    // GIL. this function is mainly for expressing intent in situations where
    // you are arranging that the GIL be trivial to grab in the future, but
    // don't care about the GIL right now.
    //
    // throws dbgevent_t* error on failure.
    //
    // FIXME: there should be a different function available than run_until_gil
    // for using after this function that checks that the assumption that the
    // gil is trivial to get is true.
    void trivialize_gil();

    // return true if `tid` is stopped at the python interpreter entry point.
    // the python interpreter entry point is the function that gdb4hpc stops
    // python at after a launch and is before any lines of python code are
    // interpreted. the exact function changes between python versions.
    bool thread_at_python_entry_point(pybe::RPC::c_thread_id_t tid);

    // return the minor version of the python inferior.
    // e.g. if attached to python 3.6.9, this function returns 6.
    static int get_python_minor_version(MISession& sess);

private:
    pybe::RPC m_rpc;
    pybe::RPC::c_thread_id_t m_gil_thread_id = 0;
    int m_cached_py_minor_version = -1;
};

class Pybe::GILHolder {
    // interface to pybe actions that are only available when
    // pybe holds the global interpreter lock.
    //
    // it has RAII behavior in that constructing GILHolder aquires the GIL, and
    // destructing GILHolder releases it.
public:
    // no constructor - create via Pybe::run_until_gil
    GILHolder(const GILHolder& other) = delete;
    GILHolder& operator=(const GILHolder& other) = delete;
    GILHolder(GILHolder&& expiring) = delete; // this is a well defined operation, just not needed yet

    // releases the GIL, might run user python threads for a bit in all stop mode
    ~GILHolder();

    // retreive the current python frame.
    //
    // throws dbgevent_t* error on failure.
    pybe::Frame get_frame();

    // change which python frame is currently in focus (implements the up/down commands).
    //
    // returns newly focused frame.
    // throws dbgevent_t* error on failure.
    pybe::Frame move_frame_level(int count, DbgFrameDir dir);

    // retreive the current python backtrace.
    //
    // throws dbgevent_t* error on failure.
    pybe::Backtrace get_backtrace();

    // evaluate the python language expression `expr` at the python location of
    // the current thread.
    //
    // throws dbgevent_t* error on failure.
    std::string eval_py_expression(std::string_view expr);

    // set a breakpoint with id `feid`. `filename`, `line`, and `funcname`
    // specify where to set it and each can be omitted (via empty string or 0).
    // if omitted, the data is simply ignored during matching. e.g. setting a
    // breakpoint on line 9 with no file or function specified will match line
    // 9 in *any* file.
    //
    // pybe will attempt to verify that the specified breakpoint is valid (refers
    // to a location that is known). if the location is invalid, this function
    // returns std::nullopt. set `allow_pending` to skip this validation.
    std::optional<pybe::Breakpoint> set_breakpoint(int feid, std::string_view filename, int line, std::string_view funcname, bool allow_pending);

    // this should be called after receiving a stop signal from pybe to check if
    // a pybe breakpoint has been hit. if one has been hit, return it. if we're
    // not at a breakpoint, return nullopt.
    std::optional<pybe::Breakpoint> find_current_breakpoint();

    // delete breakpoint with id `feid`.
    void delete_breakpoint(int feid);

    // this should be called after receiving a stop signal from pybe to check if
    // python is stopped at an uncaught exception. if it is, retreive the
    // exception. if there isn't an exception, return nullopt.
    std::optional<pybe::Exception> find_current_uncaught_exception();

    // set up pybe and the inferior for a single step. if `step_in` is true,
    // pybe will follow function calls (like the "step" command). if `step_in`
    // is false, it will step over function calls (like the "next" command)
    //
    // this function has a funny name to try to make it clear that this function
    // does not start the inferior. it only sets up the trap for the inferior to
    // stop in *once something else starts it again*. this is because setting
    // the step trap requires the GIL (which is implicitly held by the presence
    // of the GILHolder) but advancing requires the GIL be released (the
    // GILHolder must go out of scope).
    void insert_step_trap(bool step_in);

    // set up pybe and the inferior for a finish (run until a return)
    //
    // this function has a funny name to try to make it clear that this function
    // does not start the inferior. it only sets up the trap for the inferior to
    // stop in *once something else starts it again*. this is because setting
    // the step trap requires the GIL (which is implicitly held by the presence
    // of the GILHolder) but advancing requires the GIL be released (the
    // GILHolder must go out of scope).
    void insert_finish_trap();

    // different scenarios require different actions to restore the inferior to
    // the state it was in before aquiring the GIL. GILHolder will decide on its
    // escape strategy at construction time and fill in m_escape if needed.
    //
    // this is an implementation detail that's not really part of the public API.
    // it needs to be public so that it can be inherited from elsewhere though.
    class Escape {
    public:
        virtual ~Escape() = default;
        virtual void escape() = 0;
    };

private:
    friend Pybe::Pybe(MISession&); // XXX: pybe ctor needs the gil to initialize tracing/breakpoints.
                                   //      would be nice if it didn't!
    friend GILHolder Pybe::run_until_gil(pybe::RPC::c_thread_id_t);
    GILHolder(pybe::RPC& rpc, int gil_thread_id, long py_thread_id);

    pybe::RPC& m_parent_rpc;
    int m_gil_holding_thread_id = -1;
    long m_py_thread_id = 0;

    std::unique_ptr<Escape> m_escape = nullptr;

};
