// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// stl-like functions and classes that aren't offered in tinystl.
//
// TODO write in python.md why we use Result<> instead of constructors.
//      short answer: with no stdlib++, we have no exceptions. without
//      exceptions there is no way to error check contructors, so we
//      wrap them in `make` functions that return a Result<> that must
//      be checked.

#pragma once

#include <cstddef>
#include <utility>
#include <pthread.h>

#include <TINYSTL/string.h>
#include <TINYSTL/string_view.h>
#include <TINYSTL/vector.h>

#include "pythoncapi_compat.h"

namespace pybe {

using String = tinystl::string;
using StringView = tinystl::string_view;
template <typename T> using Vector = tinystl::vector<T>;

template <typename T, typename E>
class Expected {
    // based on C++23's std::expected. See unit tests for usage examples.
public:
    Expected(const T& value) : m_contents{value}, m_ok{true} {};
    Expected(const E& error) : m_contents{error}, m_ok{false} {};
    Expected(T&& value) : m_contents{std::move(value)}, m_ok{true} {};
    Expected(E&& error) : m_contents{std::move(error)}, m_ok{false} {};

    ~Expected() {
        if (m_ok) {
            m_contents.value.~T();
        } else {
            m_contents.error.~E();
        }
    }

    // TODO: clean these up. probably don't need all of these

    operator bool() const { return has_value(); };
    bool has_value() const { return m_ok; };

    T& get_value() { return m_contents.value; }
    const T& get_value() const { return m_contents.value; }
    T&& move_value() { return std::move(m_contents.value); }
    const T&& move_value() const { return std::move(m_contents.value); }

    const T& operator*() const& { return get_value(); }
    T& operator*() & { return get_value(); }
    const T&& operator*() const&& { return move_value(); }
    T&& operator*() && { return move_value(); }

    const T* operator->() const { return &get_value(); }
    T* operator->() { return &get_value(); }

    E& get_error() { return m_contents.error; }
    const E& get_error() const { return m_contents.error; }

private:
    union Contents {
        Contents(const T& value) : value{value} {};
        Contents(T&& value) : value{std::move(value)} {};
        Contents(const E& error) : error{error} {};
        Contents(E&& error) : error{std::move(error)} {};
        T value;
        E error;
        ~Contents() { /* handled by ~Expected */ };
    };
    Contents m_contents;
    bool m_ok;
};

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

String to_string(int num);

} // namespace pybe
