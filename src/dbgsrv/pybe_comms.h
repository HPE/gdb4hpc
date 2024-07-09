// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// pybe output. communication with dbgsrv and logs

#pragma once

#include "pybe_stl.h"

// TODO break some of this out into its own header so that dbgsrv can include it
//      without also including Python.h, TINYSTL, etc. and so that we don't have
//      to put __attribute__ everywhere.

namespace pybe {

struct Frame {
    __attribute__((visibility("default")))
    String serialize() const;

    __attribute__((visibility("default")))
    static Expected<Frame, std::nullptr_t> deserialize(const char*& data, size_t len);

    __attribute__((visibility("default")))
    bool operator==(const Frame& other) const = default;

    __attribute__((visibility("default")))
    bool operator!=(const Frame& other) const = default;

    String function_name;
    String file_name;
    int line_number;
};

struct Backtrace {
    __attribute__((visibility("default")))
    String serialize() const;

    __attribute__((visibility("default")))
    static Expected<Backtrace, std::nullptr_t> deserialize(const char*& data, size_t len);

    __attribute__((visibility("default")))
    bool operator==(const Backtrace& other) const;

    __attribute__((visibility("default")))
    bool operator!=(const Backtrace& other) const = default;

    Vector<Frame> frames;
};

void log(const char *format, ...) __attribute__((format (printf,1,2)));

} // namespace pybe
