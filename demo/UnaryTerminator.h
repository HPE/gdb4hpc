#pragma once

#include "UnaryOp.h"
#include <mutex>
class IStream;

//! Base class for operations that take one input and produce no data
//! output.   
/*! One key extension for the UnaryTerminator operations is that they
  support wait() so support asynchronous operation.
*/
class UnaryTerminator : public UnaryOp
{
public:
    UnaryTerminator(const std::string& name);

    bool isTerminator() const override;
    void pipeTo(std::shared_ptr<UnaryOp> dest) override;
    void done() override;
    void wait();

private:
    std::mutex m_waitMutex;
    std::condition_variable m_doneCondition;
    bool m_isDone;
};
