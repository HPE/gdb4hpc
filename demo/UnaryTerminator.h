#pragma once

#include "Notifier.h"
class IStream;

class UnaryTerminator
{
public:
    UnaryTerminator(std::shared_ptr<IStream> in);
    virtual void apply() = 0;

    std::shared_ptr<IStream> getIn() { return m_in; }
private:
    class UnaryTerminatorNotifier : public Notifier {
    public:
        UnaryTerminatorNotifier(UnaryTerminator* op);
        virtual void notify(Stream::State state);
    private:
        UnaryTerminator* m_op;
    };

    friend class UnaryTerminatorNotifier;

    void inputNotify(Stream::State state);
    void notifyReady();

    std::shared_ptr<IStream> m_in;

    std::shared_ptr<UnaryTerminatorNotifier> m_notifier;
};
