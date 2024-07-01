// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// pybe output. communication with dbgsrv and logs

#pragma once

#include "pybe_stl.h"

// TODO break some of this out into its own header so that dbgsrv can include it
//      without also including Python.h, TINYSTL, etc.

namespace pybe {

struct Frame {
    String serialize() const;
    static Expected<Frame, std::nullptr_t> deserialize(const char*& data, size_t len);

    bool operator==(const Frame& other) const = default;
    bool operator!=(const Frame& other) const = default;

    String function_name;
    String file_name;
    int line_number;
};

struct Backtrace {
    String serialize() const;
    static Expected<Backtrace, std::nullptr_t> deserialize(const char*& data, size_t len);

    bool operator==(const Backtrace& other) const;
    bool operator!=(const Backtrace& other) const = default;

    Vector<Frame> frames;
};

void log(const char *format, ...) __attribute__((format (printf,1,2)));

} // namespace pybe
