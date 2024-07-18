// Copyright 2024 Hewlett Packard Enterprise Development LP.

#pragma once

#include <utility>

namespace pybe {

template <typename T, typename E>
class Expected {
    // based on C++23's std::expected. See unit tests for usage examples.
public:
    Expected(const T& value) : m_contents{value}, m_ok{true} {};
    Expected(const E& error) : m_contents{error}, m_ok{false} {};
    Expected(T&& value) : m_contents{std::move(value)}, m_ok{true} {};
    Expected(E&& error) : m_contents{std::move(error)}, m_ok{false} {};
    Expected& operator=(T&& value) {
        this->~Expected();
        m_contents.value = std::move(value);
        m_ok = true;
        return *this;
    }
    Expected& operator=(E&& error) {
        this->~Expected();
        m_contents.error = std::move(error);
        m_ok = false;
        return *this;
    }

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

} // namespace pybe
