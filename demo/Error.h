#pragma once

#include <string>

//! Error class
/* lookat: derive from std::exception */
class Error {
public:
    Error(const std::string& message) : m_message(message) {}
    const std::string& str() const { return m_message; }
private:
    std::string m_message;
};

