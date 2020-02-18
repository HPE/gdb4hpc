#pragma once

#include "Op.h"
#include "UnaryOp.h"
#include "Value.h"
#include <mutex>
#include <deque>

class Value;

class BinaryOp : public Op
{
public:
    BinaryOp(const std::string& name);
    virtual ~BinaryOp();

    virtual void apply(const Value& a, const Value& b) = 0;

    std::shared_ptr<UnaryOp> getLHS() { return m_lhs; }
    std::shared_ptr<UnaryOp> getRHS() { return m_rhs; }
private:
    static const size_t MAX_QUEUE_SIZE = 10000;

    class HandSide : public UnaryOp {
    public:
        HandSide(BinaryOp& binOp, int w);
        void apply(const Value& val) override;
        void done() override;
    private:
        BinaryOp& m_binOp;
        int m_w;
    };
    friend class HandSide;
    
    void apply1(const Value& v, int w);
    void done1(int w);

    std::shared_ptr<HandSide> m_lhs;
    std::shared_ptr<HandSide> m_rhs;

    std::mutex m_valueMutex;
    std::condition_variable m_canTakeValue;

    std::deque<Value> m_queue;
    int m_queue_w;
    bool m_isDone[2];
};
