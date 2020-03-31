#include "UnaryTerminator.h"
#include "IStream.h"
#include "Error.h"

UnaryTerminator::UnaryTerminator(const std::string& name)
    : UnaryOp(name), m_isDone(false)
{}

bool UnaryTerminator::isTerminator() const
{
    return true;
}

void UnaryTerminator::pipeTo(std::shared_ptr<UnaryOp> dest)
{
    throw Error("piping from a terminator");
}

void UnaryTerminator::done()
{
    std::unique_lock<std::mutex> lock(m_waitMutex);
    m_isDone = true;
    m_doneCondition.notify_all();
}

void UnaryTerminator::wait()
{
    std::unique_lock<std::mutex> lock(m_waitMutex);
    while (!m_isDone) {
        m_doneCondition.wait(lock);
    }
}

