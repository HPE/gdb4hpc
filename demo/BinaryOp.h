#pragma once

#include "Op.h"
#include "UnaryOp.h"
#include "Value.h"
#include <mutex>
#include <deque>

class Value;

//! Interface for an operator taking two input value and 
//! producing a single output value.
/*! This class handles the logic for dealing with two independent
  data streams; triggering the apply function on the derived 
  type only when output is available from both inputs.
*/
class BinaryOp : public Op
{
public:
    BinaryOp(const std::string& name);
    virtual ~BinaryOp();
    
    //! Primary operation interface
    virtual void apply(const Value& a, const Value& b) = 0;

    //! Access to bind the lhs or rhs input to the output
    //! of another operator.
    std::shared_ptr<UnaryOp> getLHS() { return m_lhs; }
    std::shared_ptr<UnaryOp> getRHS() { return m_rhs; }
private:
    static const size_t MAX_QUEUE_SIZE = 10000;

    // Adapter to treat the lhs or rhs as its own unary
    // operator.
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
