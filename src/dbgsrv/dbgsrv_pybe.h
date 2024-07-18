// Copyright (c) 2024 Hewlett Packard Enterprise Development LP.
//
// interface to pybe used by dbgsrv

#pragma once

#include <memory>

#include "MISession.h"

#include "pybe_comms_dbgsrv.h"

class Pybe {
    // persistent interface to pybe
public:
    // inject and set up pybe. throws on failure. should only be called once for each inferior.
    Pybe(MISession& mi);

    class GILHolder;
    // coerce the inferior into a state where the pybe thread has the global interpreter lock
    // and return an API to actions that are only available under such conditions. when the
    // returned object is destructed, coerce the inferior into a state where the GIL is released.
    //
    // throws dbgevent_t* error on failure.
    GILHolder coerce_gil();

private:
    MISession& m_parent_session;
    std::unique_ptr<pybe::Server> m_pybe_server;
    std::unique_ptr<pybe::Channel> m_pybe_channel; // never null
    int m_gil_thread_id = -1;
};

class Pybe::GILHolder {
    // interface to pybe actions that are only available when pybe holds the global interpreter lock
public:
    // create via Pybe::coerce_gil
    ~GILHolder();

    // retreive the current python backtrace.
    //
    // throws dbgevent_t* error on failure.
    pybe::Backtrace get_backtrace();

private:
    friend Pybe;
    GILHolder(MISession& mi, int gil_thread_id, pybe::Channel& channel);

    MISession& m_parent_session;
    pybe::Channel& m_parent_pybe_channel;
    int m_gil_thread_id = -1;
};
