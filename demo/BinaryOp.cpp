#include "BinaryOp.h"
#include "IStream.h"
#include "OStream.h"
#include "Error.h"

BinaryOp::BinaryOp(std::shared_ptr<IStream> in1, std::shared_ptr<IStream> in2, 
                   std::shared_ptr<OStream> out)
    : m_in1(in1), m_in2(in2), m_out(out), 
      m_notifier1(new BinaryOpNotifier(this,0)),
      m_notifier2(new BinaryOpNotifier(this,1)),
      m_ready('\0'), m_eof('\0')
{
    in1->setNotifier(m_notifier1);
    in2->setNotifier(m_notifier2);
}

void BinaryOp::inputNotify(int which, Stream::State state)
{
    switch (state) {
    case Stream::DATA_READY:
        notifyReady(which);
        break;
    case Stream::END_OF_FILE:
        m_eof |= 1<<which;
        if (m_eof == '\3')
            m_out->eof();
        break;
    case Stream::CANCEL:
        throw Error("not handling cancel");
        break;
    } 
}

void BinaryOp::notifyReady(int which)
{
    m_ready |= (1<<which);
    while (m_ready == '\3') {
        m_ready = '\0';
        apply();

        // apply might have exhausted one of the streams and not the other.
        // Use |= because another notifyRead might have happened during apply
        // (that's also why we reset the flag to zero beforehand.  Also it's
        // we loop on m_ready == 3 - we might have gotten the last notification
        // from either one
        if (m_in1->hasData()) 
            m_ready |= '\1';
        if (m_in2->hasData())
            m_ready |= '\2';
    }
}

BinaryOp::BinaryOpNotifier::BinaryOpNotifier(BinaryOp* op, int w) : m_op(op), m_w(w) {}

void BinaryOp::BinaryOpNotifier::notify(Stream::State state)
{
    m_op->inputNotify(m_w, state);
}
