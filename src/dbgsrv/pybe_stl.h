// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// stl-like functions and classes that aren't offered in tinystl.
//
// why we use Result<> instead of constructors:
// with no stdlib++, we have no exceptions. without
// exceptions there is no way to error check contructors, so we
// wrap them in `make` functions that return a Result<> that must
// be checked.

#pragma once

#include <cstddef>
#include <utility>
#include <pthread.h>

#include <TINYSTL/string.h>
#include <TINYSTL/string_view.h>
#include <TINYSTL/unordered_set.h>
#include <TINYSTL/vector.h>

#include "pythoncapi_compat.h"

#include "pybe_expected.h"

namespace pybe {

using String = tinystl::string;
using StringView = tinystl::string_view;
template <typename T> using Vector = tinystl::vector<T>;
template <typename T> using Set = tinystl::unordered_set<T>;

class Thread {
    // mimics std::thread with pthread
    //
    // TODO: we could probably make this a little more type safe
    //       with some templates
public:
    static Expected<Thread, std::nullptr_t>
        make(void* (*start_fn)(void *), void* start_arg);
    Thread(const Thread& other) = delete;
    Thread(Thread&& other);
    ~Thread();

    [[ nodiscard ]]
    int detach();
private:
    Thread(::pthread_t id, ::pthread_attr_t attrs);

    ::pthread_t m_id;
    ::pthread_attr_t m_attrs;

    bool m_detached = false;
};

template <typename T>
struct StrongRef {
    // wrapper around a strong reference returned by the python API.
    // calls Py_XDECREF on scope exit.
    //
    // https://docs.python.org/3/glossary.html#term-strong-reference

    StrongRef(T* pyobject)
        : ptr{pyobject}
    {}

    StrongRef(const StrongRef& other) = delete;

    StrongRef(StrongRef&& other)
        : ptr{other.ptr}
    {
        other.ptr = nullptr;
    }

    StrongRef& operator=(StrongRef&& other) {
        decref();
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }

    ~StrongRef() {
        decref();
    }

    operator bool() const { return ptr != nullptr; }
    const T& operator*() const { return *ptr; }
    T& operator*() { return *ptr; }
    const T* operator->() const { return ptr; }
    T* operator->() { return ptr; }

    void decref() {
        // XDECREF is safe on nullptr
        Py_XDECREF(ptr);
        ptr = nullptr;
    }

    T* ptr;
};

class LogFile {
public:
    static Expected<LogFile, std::nullptr_t> open(StringView dir_path);
    LogFile(const LogFile& other) = delete;
    LogFile(LogFile&& expiring);
    ~LogFile();

    void write(const char* format, ...) __attribute__((format (printf,2,3)));
    void write_v(const char* format, va_list args);
private:
    LogFile() = default;

    FILE* m_file = nullptr;
};

void log(const char *format, ...) __attribute__((format (printf,1,2)));

} // namespace pybe
