// Copyright 2023 Hewlett Packard Enterprise Development LP.
//
// Signal handling utilities

#pragma once

#include <signal.h>

using SigHandlerFn = void(*)(int);

class TmpSignalHandler {
public:
    // While this object is in scope, call `handler` when `signal` is received.
    // When this object goes out of scope, the signal handlers that were set at the
    // time of the constructor call are restored.
    TmpSignalHandler(int signal, SigHandlerFn handler);
    ~TmpSignalHandler();

private:
    struct sigaction m_tmp_sigaction;
    struct sigaction m_old_sigaction;
    int m_signal;
};
