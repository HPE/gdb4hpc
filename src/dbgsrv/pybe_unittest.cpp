// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// ... TODO ...

#include <string>

#include "unittest_drivers.h"
#include "pybe_stl.h"

TEST_CASE("pybe::Expected: basics") {
    auto slot_machine = [](bool error) -> pybe::Expected<size_t, std::string> {
        if (!error) {
            return 777;
        } else {
            return std::string{"no luck"};
        }
    };

    SUBCASE("Non-error") {
        auto result_e = slot_machine(false);

        CHECK(static_cast<bool>(result_e));
        if (result_e) {
            auto& result = *result_e;
            CHECK(result == 777);
        } else {
            CHECK_MESSAGE(false, "Should never reach here");
        }
    }

    SUBCASE("error") {
        auto result_e = slot_machine(true);

        CHECK(!static_cast<bool>(result_e));
        if (result_e) {
            CHECK_MESSAGE(false, "Should never reach here");
        } else {
            auto& error = result_e.get_error();
            CHECK(error == "no luck");
        }
    }
}

TEST_CASE("pybe::Expected: Contained type constructors and methods are run correctly") {
    struct Widget {
        Widget(size_t spr) : sprockets{spr} {};
        Widget(const Widget&) = delete;
        Widget(Widget&& other) : sprockets{other.sprockets} { other.sprockets = 0; };

        size_t get_sprockets() { return sprockets; }
        size_t sprockets;
    };

    SUBCASE("rvalue ctor") {
        // rvalue ctor
        auto widgetified_expected = pybe::Expected<Widget, int>(Widget(222));
        REQUIRE(static_cast<bool>(widgetified_expected));
        REQUIRE(widgetified_expected.has_value());
        CHECK(widgetified_expected.get_value().sprockets == 222);
        CHECK((*widgetified_expected).sprockets == 222);
        CHECK(widgetified_expected->sprockets == 222);
        CHECK(widgetified_expected->get_sprockets() == 222);
    }

    SUBCASE("move operations") {
        auto original_widget = Widget(1234);
        auto widgetified_expected = pybe::Expected<Widget, int>(std::move(original_widget));
        REQUIRE(original_widget.sprockets == 0);

        REQUIRE(static_cast<bool>(widgetified_expected));
        REQUIRE(widgetified_expected.has_value());
        CHECK(widgetified_expected.get_value().sprockets == 1234);

        auto taken_widget = *std::move(widgetified_expected);
        CHECK(taken_widget.sprockets == 1234);
        CHECK(widgetified_expected.get_value().sprockets == 0);
    }
}

TEST_CASE("pybe::Expected: Destructors") {
    bool everything_is_ok = true;

    struct Bomb {
        Bomb(bool& victim) : m_victim{victim} {};
        ~Bomb() { m_victim = false; }
        bool& m_victim;
    };

    auto get_possible_bomb = [&](bool bomb) -> pybe::Expected<std::string, Bomb> {
        if (bomb) {
            return Bomb(everything_is_ok);
        } else {
            return std::string{":-)"};
        }
    };

    REQUIRE(everything_is_ok);

    SUBCASE("the bomb doesn't explode") {
        {
            auto not_a_bomb = get_possible_bomb(false);
            REQUIRE(static_cast<bool>(not_a_bomb));
            CHECK(*not_a_bomb == ":-)");
        }
        CHECK(everything_is_ok);
    }

    SUBCASE("the bomb explodes!") {
        {
            auto not_a_bomb = get_possible_bomb(true);
            REQUIRE(!static_cast<bool>(not_a_bomb));
        }
        CHECK(!everything_is_ok);
    }
}

int main(int argc, char* argv[]) {
    return runUnitTests(argc, argv);
}
