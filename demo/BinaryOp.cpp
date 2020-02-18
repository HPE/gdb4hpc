#include "BinaryOp.h"
#include "IStream.h"
#include "OStream.h"
#include "Error.h"

BinaryOp::BinaryOp(const std::string& name)
    : Op(name), 
      m_lhs(new HandSide(*this, 0)),
      m_rhs(new HandSide(*this, 1)),
      m_queue_w(0)
{
    m_isDone[0] = m_isDone[1] = false;
}

BinaryOp::~BinaryOp() {}

void BinaryOp::apply1(const Value& v, int w)
{

    bool didPop = true;
    {
        // wait until I have a spot to store this value
        std::unique_lock<std::mutex> lock(m_valueMutex);
        while (m_queue_w == w && 
               m_queue.size() == MAX_QUEUE_SIZE && 
               !m_isDone[1-w]) {
            m_canTakeValue.wait(lock);
        }

        if (m_queue_w == w) {
            // We don't have a value for the other argument
            m_queue.push_back(v);
            return;
        } else if (m_queue.empty()) {
            // We don't have a value for either argument
            m_queue.push_back(v);
            m_queue_w = w;
        } else { 
            // We not have v for argument w and and at 
            // least one available for the other
            const Value& a = (!w ? v : m_queue.front());
            const Value& b = ( w ? v : m_queue.front());
            apply(a,b);
            
            m_queue.pop_front();
            didPop = true;
        }
    }
    
    if (didPop)
        m_canTakeValue.notify_all();
}

void BinaryOp::done1(int w)
{
    {
        std::unique_lock<std::mutex> lock(m_valueMutex);
        m_isDone[w] = true;
        if (m_isDone[1-w])
            Op::done();
    }
    m_canTakeValue.notify_all();
}

BinaryOp::HandSide::HandSide(BinaryOp& binOp, int w) 
    : UnaryOp("BinaryOp::HandSide"), m_binOp(binOp), m_w(w) {}

void BinaryOp::HandSide::apply(const Value& val)
{
    m_binOp.apply1(val, m_w);
}

void BinaryOp::HandSide::done()
{
    m_binOp.done1(m_w);
}
