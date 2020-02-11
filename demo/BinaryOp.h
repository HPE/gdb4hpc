#pragma once

#include "Notifier.h"
#include <mutex>

class IStream; class OStream;

class BinaryOp
{
public:
    BinaryOp(std::shared_ptr<IStream> a, std::shared_ptr<IStream> b, std::shared_ptr<OStream> diffs);
    virtual void apply() = 0;

    std::shared_ptr<IStream> getIn1() { return m_in1; }
    std::shared_ptr<IStream> getIn2() { return m_in2; }
    std::shared_ptr<OStream> getOut() { return m_out; }
private:
    class BinaryOpNotifier : public Notifier {
    public:
        BinaryOpNotifier(BinaryOp* op, int w);
        virtual void notify(Stream::State state);
    private:
        BinaryOp* m_op;
        int m_w;
    };

    friend class BinaryOpNotifier;

    void inputNotify(int which, Stream::State state);
    void notifyReady(int which);

    std::shared_ptr<IStream> m_in1;
    std::shared_ptr<IStream> m_in2;
    std::shared_ptr<OStream> m_out;

    std::shared_ptr<BinaryOpNotifier> m_notifier1;
    std::shared_ptr<BinaryOpNotifier> m_notifier2;

    std::mutex m_mutex;
    bool m_ready[2];
    bool m_eof[2];
};
