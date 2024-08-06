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

    // python 3.6 and below require the GIL to be explicitly created at python startup.
    // this should only be called at launch time just after the python interpreter is initialized.
    void initialize_gil();

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
    int get_python_minor_version();

private:
    pybe::RPC m_rpc;
    int m_gil_thread_id = -1;
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

    // releases the GIL
    ~GILHolder();

    // retreive the current python backtrace.
    //
    // throws dbgevent_t* error on failure.
    pybe::Backtrace get_backtrace();

private:
    friend Pybe;
    GILHolder(pybe::RPC& rpc, int gil_thread_id);

    pybe::RPC& m_parent_rpc;
    int m_gil_thread_id = -1;
    long m_py_thread_id = 0;
};
