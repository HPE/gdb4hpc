// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// ... TODO ...

#include <string>

#include "unittest_drivers.h"

#include "pybe_comms.h"

namespace doctest {
// these tell doctest how to stringify pybe types and make for nicer test output

template<> struct StringMaker<pybe::String> {
    static String convert(const pybe::String& str) {
        return str.c_str();
    }
};

pybe::String pybe_convert(const pybe::Frame& frame) {
    auto result = pybe::String{};
    result.append(frame.function_name.c_str());
    result.append(" in ");
    result.append(frame.file_name.c_str());
    result.append(":");
    result.append(std::to_string(frame.line_number).c_str());
    return result.c_str();
}

template<> struct StringMaker<pybe::Frame> {
    static String convert(const pybe::Frame& frame) {
        return pybe_convert(frame).c_str();
    }
};

template<> struct StringMaker<pybe::Backtrace> {
    static String convert(const pybe::Backtrace& bt) {
        auto result = pybe::String{};
        for (const auto& frame : bt.frames) {
            result.append(pybe_convert(frame).c_str());
            result.append(" --- ");
        }
        return result.c_str();
    }
};
}

TEST_CASE("pybe::Expected: basics") {
    auto slot_machine = [](bool error) -> pybe::Expected<size_t, const char *> {
        if (!error) {
            return 777;
        } else {
            return "no luck";
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

    auto get_possible_bomb = [&](bool bomb) -> pybe::Expected<const char *, Bomb> {
        if (bomb) {
            return Bomb(everything_is_ok);
        } else {
            return ":-)";
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

TEST_CASE("frame serialization") {
    const auto frame_in = pybe::Frame{
        .function_name = "pybe::Frame::serialize",
        .file_name = "", // to test empty string serialization
        .line_number = 17,
    };
    const auto frame_different = pybe::Frame{
        .function_name = "pybe::Frame::not_serialize",
        .file_name = "not_pybe_comms.cpp",
        .line_number = 71,
    };

    auto frame_wire = frame_in.serialize();
    CAPTURE(frame_wire);

    auto* cursor = frame_wire.c_str();

    auto maybe_frame_out = pybe::Frame::deserialize(cursor, frame_wire.size());
    REQUIRE(maybe_frame_out);
    auto& frame_out = *maybe_frame_out;

    CHECK(frame_in == frame_out);
    CHECK(frame_different != frame_out);
}

TEST_CASE("backtrace serialization") {
    const auto frame_a = pybe::Frame{
        .function_name = "pybe::Frame::serialize",
        .file_name = "", // test empty string
        .line_number = 17,
    };
    const auto frame_b = pybe::Frame{
        .function_name = "pybe::Backtrace::serialize",
        .file_name = "pybe_unittest.cpp",
        .line_number = 21,
    };
    const auto frame_c = pybe::Frame{
        .function_name = "pybe::Backtrace::deserialize",
        .file_name = "pybe_comms.cpp",
        .line_number = 13,
    };

    auto bt_in = pybe::Backtrace{};
    bt_in.frames.push_back(frame_a);
    bt_in.frames.push_back(frame_b);
    bt_in.frames.push_back(frame_c);

    auto bt_diff = pybe::Backtrace{};
    bt_diff.frames.push_back(frame_a); // 3 copies of frame a
    bt_diff.frames.push_back(frame_a);
    bt_diff.frames.push_back(frame_a);

    auto bt_wire = bt_in.serialize();
    CAPTURE(bt_wire);

    auto* cursor = bt_wire.c_str();

    auto maybe_bt_out = pybe::Backtrace::deserialize(cursor, bt_wire.size());
    REQUIRE(maybe_bt_out);
    auto& bt_out = *maybe_bt_out;

    CHECK(bt_in == bt_out);
    CHECK(bt_diff != bt_out);
}

int main(int argc, char* argv[]) {
    return runUnitTests(argc, argv);
}
