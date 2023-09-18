// Copyright 2023 Hewlett Packard Enterprise Development LP.

#include <string.h>

#include "signal_util.h"
#include "unittest_macros.h"

namespace test {

int global_test_int = 0;

TEST_CASE("TmpSignalHandler: modify data") {
    global_test_int = 0;

    auto changeIt = [](int) {
        global_test_int = 6;
    };

    auto sig_handler = TmpSignalHandler(SIGUSR1, changeIt);

    raise(SIGUSR1);
    CHECK(global_test_int == 6);
}

TEST_CASE("TmpSignalHandler: restore old signal handler") {
    auto oldChangeIt = [](int) { global_test_int = 2; };
    auto newChangeIt = [](int) { global_test_int = 3; };

    global_test_int = 0;

    struct sigaction old_sigaction;
    memset(&old_sigaction, 0, sizeof(old_sigaction));
    old_sigaction.sa_handler = oldChangeIt;
    sigaction(SIGUSR1, &old_sigaction, nullptr);

    raise(SIGUSR1);
    REQUIRE(global_test_int == 2);

    {
        auto sig_handler = TmpSignalHandler(SIGUSR1, newChangeIt);
        raise(SIGUSR1);
        REQUIRE(global_test_int == 3);
    }

    raise(SIGUSR1);
    REQUIRE(global_test_int == 2);
}

TEST_CASE("TmpSignalHandler: don't interfere with unrelated signals") {
    auto oldChangeIt = [](int) { global_test_int = 4; };
    auto newChangeIt = [](int) { global_test_int = 5; };

    global_test_int = 0;

    struct sigaction old_sigaction;
    memset(&old_sigaction, 0, sizeof(old_sigaction));
    old_sigaction.sa_handler = oldChangeIt;
    sigaction(SIGUSR1, &old_sigaction, nullptr);

    raise(SIGUSR1);
    REQUIRE(global_test_int == 4);

    {
        global_test_int = 0;
        auto sig_handler = TmpSignalHandler(SIGUSR2, newChangeIt);
        raise(SIGUSR1); // using old handler
        REQUIRE(global_test_int == 4);
        raise(SIGUSR2); // using tmp handler
        REQUIRE(global_test_int == 5);
    }
}

} // namespace test

TmpSignalHandler::TmpSignalHandler(int signal, SigHandlerFn handler)
    : m_signal{signal}
{
    memset(&m_tmp_sigaction, 0, sizeof(m_tmp_sigaction));
    sigemptyset(&m_tmp_sigaction.sa_mask);
    m_tmp_sigaction.sa_handler = handler;
    sigaction(m_signal, &m_tmp_sigaction, &m_old_sigaction);
}

TmpSignalHandler::~TmpSignalHandler() {
    sigaction(m_signal, &m_old_sigaction, nullptr);
}
