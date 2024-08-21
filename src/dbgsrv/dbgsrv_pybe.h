// Copyright (c) 2024 Hewlett Packard Enterprise Development LP.
//
// interface to pybe used by dbgsrv

#pragma once

#include "dbgsrv_pybe_rpc.h"

class Pybe {
    // Pybe is a singleton-like interface for dbgsrv to use to interact with the
    // injected pybe shared library. Constructing Pybe will inject the pybe shared
    // library into the inferior at which point dbgsrv is able to use Pybe to start
    // interacting with the so.
    //
    // most shared library functions require the GIL and are behind a different API, `GILHolder`.
    // use `Pybe::coerce_gil` to arrange that the pybe shared library has the GIL and
    // is ready for further commands.
public:
    // inject and set up pybe. throws on failure. should only be called once for each inferior.
    Pybe(MISession& mi);

    class GILHolder;
    // coerce the inferior into a state where the pybe thread has the global
    // interpreter lock and return an API to actions that are only available
    // under such conditions. this might mean running user python threads until
    // they release the GIL. when the returned object is destructed, coerce the
    // inferior into a state where the GIL is released.
    //
    // this function is marked [[ nodiscard ]] because side effects happen when
    // the returned GILHolder is destructed. it is probably a programmer error
    // to call this function and not store the return value. if you have such a
    // use case, use trivialize_gil.
    //
    // throws dbgevent_t* error on failure.
    [[ nodiscard ]]
    GILHolder coerce_gil();

    // coerce the inferior into a state where calling coerce_gil would not
    // require any user threads to run.
    //
    // this function simply calls coerce_gil and then immediately discards the
    // GIL. this function is mainly for expressing intent in situations where
    // you are arranging that the GIL be trivial to grab in the future, but
    // don't care about the GIL right now.
    //
    // throws dbgevent_t* error on failure.
    void trivialize_gil();

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
    // no constructor - create via Pybe::coerce_gil
    GILHolder(const GILHolder& other) = delete;
    GILHolder& operator=(const GILHolder& other) = delete;
    GILHolder(GILHolder&& expiring) = delete; // TODO implement this

    // releases the GIL
    ~GILHolder();

    // retreive the current python frame.
    //
    // throws dbgevent_t* error on failure.
    pybe::Frame get_frame();

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
    pybe::Breakpoint set_breakpoint(int feid, std::string_view filename, int line, std::string_view funcname);

    // this should be called after receiving a stop signal from pybe to check if
    // a pybe breakpoint has been hit. if one has been hit, return it. if we're
    // not at a breakpoint, return nullopt.
    std::optional<pybe::Breakpoint> find_current_breakpoint();

    // delete breakpoint with id `feid`.
    void delete_breakpoint(int feid);

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

    // different scenarios require different actions to restore the inferior to
    // the state it was in before aquiring the GIL. GILHolder will decide on its
    // escape strategy at construction time and fill in m_escape if needed.
    //
    // this is an implementation detail that's not really part of the public API.
    // it needs to be public so that it can be inherited from elsewhere though.
    class Escape {
    public:
        virtual void escape() = 0;
    };

private:
    friend Pybe::Pybe(MISession&); // XXX: pybe ctor needs the gil to initialize tracing/breakpoints.
                                   //      would be nice if it didn't!
    friend GILHolder Pybe::coerce_gil();
    GILHolder(pybe::RPC& rpc, int gil_thread_id, long py_thread_id);

    pybe::RPC& m_parent_rpc;
    int m_gil_holding_thread_id = -1;
    long m_py_thread_id = 0;

    std::unique_ptr<Escape> m_escape = nullptr;

};
