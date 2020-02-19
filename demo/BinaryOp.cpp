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

// Accept an value from one of the inputs to this operator.
// w is 0 for lhs and 1 for rhs.
void BinaryOp::apply1(const Value& v, int w)
{
    // Data may arrive from the inputs at different rates. 
    // For the stream that is ahead, we keep a fixed sized
    // queue of results in m_queue which is storing values 
    // for w = m_queue_w.
    bool didPop = true;
    {
        // wait until I have a spot to store this value.
        // If we have to block, wait on the condition variable
        // m_canTakeValue until input from the other argment arrives 
        // which will allow us to process a pair and free up 
        // space in the buffer
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
            // We now have v for argument w and and at 
            // least one available for the other
            const Value& a = (!w ? v : m_queue.front());
            const Value& b = ( w ? v : m_queue.front());
            apply(a,b);
            
            m_queue.pop_front();
            didPop = true;
        }
    }

    // unblock the other thread once we've popped off a value
    // lookat: not sure if there is a performance penalty for 
    // notifying when no one is waiting.
    // lookat: I'm also not sure if this will behave very poorly 
    // once the buffer fills up because it reverts to blocking 
    // after accepting every input.  We might instead allow the 
    // buffer to - e.g. get half empty before we start accepting
    // input again...
    if (didPop)
        m_canTakeValue.notify_all();
}

// register the end of input from one of the input streams.
// w = 0 is lhs, w = 1 rhs
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
