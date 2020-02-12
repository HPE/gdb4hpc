#pragma once

#include "Notifier.h"
class IStream; class OStream;

class UnaryOp
{
public:
    UnaryOp(std::shared_ptr<IStream> in, std::shared_ptr<OStream> out);
    virtual ~UnaryOp();

    virtual void apply() = 0;

    std::shared_ptr<IStream> getIn() { return m_in; }
private:
    class UnaryOpNotifier : public Notifier {
    public:
        UnaryOpNotifier(UnaryOp* op);
        virtual void notify(Stream::State state);
    private:
	UnaryOp* m_op;
    };

    friend class UnaryOpNotifier;

    void inputNotify(Stream::State state);
    void notifyReady();

    std::shared_ptr<IStream> m_in;
    std::shared_ptr<OStream> m_out;

    std::shared_ptr<UnaryOpNotifier> m_notifier;
};
