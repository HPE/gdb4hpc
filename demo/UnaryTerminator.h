#pragma once

#include "UnaryOp.h"
#include "Notifier.h"
#include <mutex>
class IStream;

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
