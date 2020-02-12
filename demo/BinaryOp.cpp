#include "BinaryOp.h"
#include "IStream.h"
#include "OStream.h"
#include "Error.h"

BinaryOp::BinaryOp(std::shared_ptr<IStream> in1, std::shared_ptr<IStream> in2, 
                   std::shared_ptr<OStream> out)
    : m_in1(in1), m_in2(in2), m_out(out), 
      m_notifier1(new BinaryOpNotifier(this,0)),
      m_notifier2(new BinaryOpNotifier(this,1))
{
    m_ready[0] = m_ready[1] = false;
    m_eof[0] = m_eof[1] = false;

    in1->setNotifier(m_notifier1);
    in2->setNotifier(m_notifier2);
}

BinaryOp::~BinaryOp() {
    detach();
}

void BinaryOp::detach() {
    m_in1->setNotifier(nullptr);
    m_in2->setNotifier(nullptr);
}    

void BinaryOp::inputNotify(int which, Stream::State state)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    switch (state) {
    case Stream::DATA_READY:
        notifyReady(which);
        break;
    case Stream::END_OF_FILE:
        m_eof[which] = true;
        if (m_eof[0] && m_eof[1])
            m_out->eof();
        break;
    case Stream::CANCEL:
        throw Error("not handling cancel");
        break;
    } 
}

void BinaryOp::notifyReady(int which)
{
    m_ready[which] = true;
    while (m_ready[0] && m_ready[1]) {
        apply();

        // apply might have exhausted one of the streams and not the other
        m_ready[0] = m_in1->hasData();
        m_ready[1] = m_in2->hasData();
    }
}

BinaryOp::BinaryOpNotifier::BinaryOpNotifier(BinaryOp* op, int w) : m_op(op), m_w(w) {}

void BinaryOp::BinaryOpNotifier::notify(Stream::State state)
{
    m_op->inputNotify(m_w, state);
}
