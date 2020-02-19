#pragma once

#include <string>
class UnaryOp;

//! Base interface for a data operator
/*! An operator is a simple data creator or manipulator that
  can be chained together in a DAG to create a complicated expression.
*/
class Op
{
public:
    Op(const std::string& name);
    virtual ~Op() = default;

    //! Return true if the operator does not produce stream output
    virtual bool isTerminator() const; // default to false

    //! Return true if the operator does not take stream input
    virtual bool isSource() const;  // default to false

    //! start a source generator, return true if a start was kicked off
    //! default to no-op / return false
    virtual bool start() const;

    //! default calls done on the output stream
    virtual void done();

    //! Connect the output of this operator to the input of \a dest.
    virtual void pipeTo(std::shared_ptr<UnaryOp> dest);

    //! Get the operator this one is piping to
    std::shared_ptr<UnaryOp> getOutput() const { return m_output; }

    //! Get the name of the operator
    const std::string& getName() const { return m_name; }

private:
    std::string m_name;
    std::shared_ptr<UnaryOp> m_output;
};
