#include "UnaryTerminator.h"
#include "IStream.h"
#include "Error.h"

UnaryTerminator::UnaryTerminator(std::shared_ptr<IStream> in)
    : m_in(in),
      m_notifier(new UnaryTerminatorNotifier(this))
{
    in->setNotifier(m_notifier);
}

void UnaryTerminator::inputNotify(Stream::State state)
{
    switch (state) {
    case Stream::DATA_READY:
        notifyReady();
        break;
    case Stream::END_OF_FILE:
        break;
    case Stream::CANCEL:
        throw Error("not handling cancel");
        break;
    } 
}

void UnaryTerminator::notifyReady()
{
    // lookat : I probably have a race condition here in the multi-thread case
    while (m_in->hasData()) {
        apply();
    }
}

UnaryTerminator::UnaryTerminatorNotifier::UnaryTerminatorNotifier(UnaryTerminator* op) : m_op(op) {}

void UnaryTerminator::UnaryTerminatorNotifier::notify(Stream::State state)
{
    m_op->inputNotify(state);
}
