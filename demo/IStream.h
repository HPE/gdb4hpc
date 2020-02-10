#pragma once

#include "Stream.h"
#include "Notifier.h"
#include <any>

class Notifier; class OStream;

class IStream : public Stream
{
public:
    IStream(std::shared_ptr<ByteStream> byteStream);

    double getDouble();
    int getInt();

    bool read(std::any& val);

    bool hasData() const; // temporary?

    void setNotifier(std::shared_ptr<Notifier> notifier);

    void connectToHead(OStream& ostream);

private:
    class HeadNotifier : public Notifier {
    public:
        HeadNotifier(IStream* stream);
        virtual void notify(Stream::State state);
    private:
        IStream* m_stream;
    };
    friend class HeadNotifier;

    void readType();
    int getIntL();
    void notifyFromHead(Stream::State state);

    Type m_currentType;
    size_t m_remainingOfType;
    bool m_headIsClosed;

    std::shared_ptr<Notifier> m_notifier;
};
