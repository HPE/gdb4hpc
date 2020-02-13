#pragma once

#include <string>

class Op
{
public:
    Op(const std::string& name);
    virtual ~Op() = default;
    virtual bool isTerminator() const; // default to false
    virtual bool isSource() const;  // default to false

    // start a source generator, return true if a start was kicked off
    // default to no-op / return false
    virtual bool start() const;     

    const std::string& getName() const { return m_name; }
private:
    std::string m_name;
};
