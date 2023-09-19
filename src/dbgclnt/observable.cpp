#include "observable.h"
#include "unittest_macros.h"

namespace observers {

Observable<std::string_view /* name */, std::string_view /* new value */> setable_param_changed{};
Observable<DbgInvoke::EventSet& /* ev_set */, const Bitset& /* reply_set */> async_event{};
Observable<> target_resumed{};

}

namespace detail {

size_t ObservableToken::next_value{};

ObservableToken::ObservableToken()
    : m_value{ObservableToken::next_value++}
{}

bool ObservableToken::operator==(const ObservableToken& other) {
    return m_value == other.m_value;
}

} // namespace detail

TEST_CASE("Observers: basics") {
    Observable<int&> observable;
    auto num = int{0};
    auto inc = [](int& data) { data += 1; };
    auto inc2 = [](int& data) { data += 2; };

    {
        auto conn = observable.attach(inc);
        observable.notify(num);
        CHECK(num == 1);

        auto conn2 = observable.attach(inc2);
        observable.notify(num);
        // inc and inc2 each run, resulting in a net += 3. it's not defined by
        // the Observable API if inc or inc2 happens first.
        CHECK(num == 4);
    }
}

TEST_CASE("Observers: connection lifetimes") {
    Observable<int&> observable;
    auto num = int{0};
    auto inc = [](int& data) { data += 1; };
    auto inc2 = [](int& data) { data += 2; };

    {
        auto conn = observable.attach(inc);
        observable.notify(num);
        REQUIRE(num == 1);
    }

    // conn died with the scope, so nothing should happen here
    observable.notify(num);
    REQUIRE(num == 1);

    {
        auto conn = observable.attach(inc2);
        observable.notify(num);
        REQUIRE(num == 3);
    }

    // conn died with the scope, so nothing should happen here
    observable.notify(num);
    REQUIRE(num == 3);
}

TEST_CASE("Observers: adding extra context to observers") {
    Observable<> observable;

    class Num {
    private:
        void inc() { m_value += 1; }
    public:
        void attach_observable(Observable<>& ob) {
            const auto fn = [this](){
                inc();
            };
            // storing the connection as a member variable is an easy way to ensure
            // that the this pointer captured above is never used after being freed.
            m_conn = ob.attach(fn);
        }

        int m_value = 0;
        std::unique_ptr<ObservableConnection> m_conn = nullptr;
    };

    {
        auto num = Num{};
        REQUIRE(num.m_value == 0);

        // nothing happens since the observer has not been attached
        observable.notify();
        REQUIRE(num.m_value == 0);

        num.attach_observable(observable);
        observable.notify();
        REQUIRE(num.m_value == 1);

        // end of scope: Num dies, taking its m_conn with it.
    }

    // the connection died with the scope, so this is safe, even after capturing
    // a this pointer in Num::attach_observable.
    observable.notify();
}
