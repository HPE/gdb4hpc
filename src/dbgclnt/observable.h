// Copyright 2023 Hewlett Packard Enterprise Development LP.
//
// An Observable is an arbitrary event. Observers can attach (i.e. subscribe) to
// the Observable and have a function called when the event happens.
//
// The Observable::attach function returns a special ObservableConnection object.
// The installed callback function will only be called by the Observable as long
// as the ObservableConnection is alive (i.e. in scope). This is motivated by
// the fact that subscribers to Observable events often want to add extra
// context to the function call, often a this pointer. The fact that
// ObservableConnection will disconnect the observer on destruction makes it
// easy to keep the scopes of the extra data the same as the observer
// connection.
//
// See the unit tests in the .cpp file for examples.

#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>

#include "dbginvoke.h"
#include "gdb4_bitset.h"

namespace detail {

// uniquely identifies a connection to an observable.
class ObservableToken {
public:
    explicit ObservableToken();
    bool operator==(const ObservableToken& other);

private:
    size_t m_value;
    static size_t next_value;
};

} // namespace detail

class ObservableConnection {
    // a connection to an observable. when this object goes out of scope, the
    // connection is removed.
    //
    // to create a connection, use Observable::attach.
  public:
    virtual ~ObservableConnection() = default;
};

template <typename... Args>
class Observable {
private:
    struct ConnectionImpl;
public:
    using Func = std::function<void (Args...)>;

    // `attach` connects the observer `fn` to this observable. the connection is
    // only active as long as the returned ObservableConnection is in scope.
    [[nodiscard]]
    std::unique_ptr<ObservableConnection> attach(Func fn) {
        auto conn = std::make_unique<ConnectionImpl>(*this);
        m_observers.emplace_back(conn->m_token, fn);
        return conn;
    }

    // `notify` calls each attached observer.
    void notify(Args... args) {
        for (auto entry : m_observers) {
            entry.func(args...);
        }
    }
private:
    struct ConnectionImpl : public ObservableConnection {
        explicit ConnectionImpl(Observable& observable)
            : m_token()
            , m_observable{observable}
        {}

        virtual ~ConnectionImpl() override {
            m_observable.detach(m_token);
        };

        detail::ObservableToken m_token;
        Observable& m_observable;
    };

    // this is called by ConnectionImpl on destruction.
    void detach(detail::ObservableToken token) {
        const auto [first, last] =
            std::ranges::remove_if(m_observers, [token](auto e){ return token == e.token; });
        m_observers.erase(first, last);
        return;
    }

    struct Entry {
        explicit Entry(detail::ObservableToken t, Func f) : token{t}, func{f} {}
        detail::ObservableToken token;
        Func func;
    };
    std::vector<Entry> m_observers;
};

namespace observers {

// a parameter was changed with the set command.
extern Observable<std::string_view /* name */, std::string_view /* new value */>
    setable_param_changed;

// an async debugger event arrived.
extern Observable<DbgInvoke::EventSet& /* ev_set */, const Bitset& /* reply_set */>
    async_event;

// a thread started running after being stopped.
extern Observable<>
    target_resumed;

}

