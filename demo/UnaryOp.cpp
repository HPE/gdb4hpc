#include "UnaryOp.h"
#include "IStream.h"
#include "OStream.h"
#include "Error.h"

UnaryOp::UnaryOp(std::shared_ptr<IStream> in,
                 std::shared_ptr<OStream> out)
    : m_in(in), m_out(out), 
      m_notifier(new UnaryOpNotifier(this))
{
    in->setNotifier(m_notifier);
}

UnaryOp::~UnaryOp() {
    m_in->setNotifier(nullptr);
}
void UnaryOp::inputNotify(Stream::State state)
{
    switch (state) {
    case Stream::DATA_READY:
        notifyReady();
        break;
    case Stream::END_OF_FILE:
        m_out->eof();
        break;
    case Stream::CANCEL:
        throw Error("not handling cancel");
        break;
    } 
}

void UnaryOp::notifyReady()
{
    // lookat : I probably have a race condition here in the multi-thread case
    while (m_in->hasData()) {
        apply();
    }
}

UnaryOp::UnaryOpNotifier::UnaryOpNotifier(UnaryOp* op) : m_op(op) {}

void UnaryOp::UnaryOpNotifier::notify(Stream::State state)
{
    m_op->inputNotify(state);
}
