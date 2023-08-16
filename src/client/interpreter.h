// Copyright 2023 Hewlett Packard Enterprise Development LP.
//
// Interpreter interface
#pragma once

#include <stdarg.h>
#include <string_view>

class Interpreter {
public:
    virtual ~Interpreter() = default;

    // Given `text`, complete_text tries to transform it into a string that the
    // interpreter understands. If that isn't possible, returns a copy of
    // `text`.
    //
    // TODO making a copy of `text` on failure seems a little wasteful. consider
    // changing this api to return a string_view which would save the copy on
    // the failure case and let the caller decide when to copy on the success case.
    virtual std::string complete_text(std::string_view text) = 0;

    // Given `text`, can_interpret returns true if it would be appropriate to
    // call interpret(`text`).
    virtual bool can_interpret(std::string_view text) = 0;

    // Given `text`, interface with gdb4hpc.
    //
    // TODO give this function a more useful return value.
    virtual void interpret(std::string_view text) = 0;
};
