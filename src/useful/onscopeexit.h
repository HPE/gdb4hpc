/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <functional>

// call a simple function on scope exit

template <typename F>
class OnScopeExit {
public:
    OnScopeExit(const F &toCall) : func{toCall} {};
    ~OnScopeExit() { func(); };
private:
    F func;
};
