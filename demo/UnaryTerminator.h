#pragma once

#include "Notifier.h"
#include <mutex>
class IStream;

class UnaryTerminator
{
public:
    UnaryTerminator(std::shared_ptr<IStream> in);
    virtual ~UnaryTerminator();

    virtual void apply() = 0;
    virtual void detach();

    std::shared_ptr<IStream> getIn() { return m_in; }
    
    void wait();
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
    void notifyDone();

    std::shared_ptr<IStream> m_in;

    std::shared_ptr<UnaryTerminatorNotifier> m_notifier;

    std::mutex m_waitMutex;
    std::condition_variable m_doneCondition;
    bool m_isDone;
};
