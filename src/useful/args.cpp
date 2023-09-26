/*********************************************************************************\
 * Argument routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013, 2020-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#include "args.h"

#include "unittest_macros.h"

TEST_CASE("ArgsToBuf") {
    char* hello = strdup("hello");
    char* world = strdup("world");
    char* args[] = {hello, world};

    auto buf = ArgsToBuf(args, 2);
    CHECK(std::string(buf) == "hello world");
    free(buf);

    free(hello);
    free(world);
}

TEST_CASE("ArgsToString") {
    char* hello = strdup("hello");
    char* world = strdup("world");
    char* args[] = {hello, world};

    CHECK(ArgsToString(args, 2) == "hello world");

    free(hello);
    free(world);
}

TEST_CASE("ArgvStream picks correct constructor when dealing with argc and char**") {
    char* hello = strdup("hello");
    char* world = strdup("world");

    SUBCASE("With char**") {
        char* args[] = {hello, world};
        CHECK(ArgvStream(2, args).asString() == "hello world");
    }

    SUBCASE("With const char**") {
        const char* args[] = {hello, world};
        CHECK(ArgvStream(2, args).asString() == "hello world");
    }

    SUBCASE("With const char* const*") {
        const char* const args[] = {hello, world};
        CHECK(ArgvStream(2, args).asString() == "hello world");
    }

    free(hello);
    free(world);
}

TEST_CASE("ArgvStream.c_strs() always ends with a nullptr") {
    ArgvStream args;

    SUBCASE("with empty args") {
        CHECK(args.c_strs().back() == nullptr);
    }

    SUBCASE("with multiple args") {
        args << "hello" << "world";
        CHECK(args.c_strs().back() == nullptr);
    }

    SUBCASE("with mutliple args; subset") {
        args << "hello" << 1 << "world" << 2;
        CHECK(args.c_strs(2).back() == nullptr);
        CHECK(std::string(args.c_strs(2).front()) == "world");
    }
}

TEST_CASE("ArgvStream.parseAndAppendArgs()") {
    auto args = ArgvStream("hello", "world");

    SUBCASE("No quotes") {
        args.parseAndAppendArgs("my name is scooby");
        CHECK(args.argc() == 6);
        CHECK(args.asString() == "hello world my name is scooby");
    }

    SUBCASE("With quotes") {
        args.parseAndAppendArgs(R"(my name is "scooby doo")");
        CHECK(args.argc() == 6);
        // asString escapes the quotes and adds another layer around them
        CHECK(args.asString() == R"(hello world my name is "\"scooby doo\"")");
        CHECK(std::string(args[5]) == R"("scooby doo")");
    }
}

TEST_CASE("ArgvStream.argc(), .clear(), []") {
    auto args = ArgvStream("hello", "world");
    REQUIRE(args.argc() == 2);
    CHECK(args[0] == "hello");
    CHECK(args[1] == "world");
    args.clear();
    CHECK(args.argc() == 0);
}

TEST_CASE("ArgvStream << string-like") {
    auto args = ArgvStream();
    auto world = "world";

    args << std::string("hello");
    args << std::string_view(world);
    args << "my"; // char *

    const char* const argv[] = {"name", "is", nullptr};
    args << argv;

    auto scoobyArgs = ArgvStream("scooby");
    args << scoobyArgs;

    CHECK(args.argc() == 6);
    CHECK(args.asString() == "hello world my name is scooby");
}

TEST_CASE("ArgvStream << number") {
    auto args = ArgvStream();
    args << 1 << 2.2 << 3u << 4.4d;
    CHECK(args[0] == "1");
    CHECK(args[1] == "2.200000");
    CHECK(args[2] == "3");
    CHECK(args[3] == "4.400000");
}

TEST_CASE("ArgvStream << ArgvStream") {
    auto args1 = ArgvStream("hello", "world");
    auto args2 = ArgvStream("hello2", "world2");
    auto args3 = ArgvStream{};

    args3 << args1 << args2;

    // args1 should not have changed
    CHECK(args1.argc() == 2);
    CHECK(args1[0] == "hello");
    CHECK(args1[1] == "world");

    // args2 should not have changed
    CHECK(args2.argc() == 2);
    CHECK(args2[0] == "hello2");
    CHECK(args2[1] == "world2");

    CHECK(args3.argc() == 4);
    CHECK(args3[0] == "hello");
    CHECK(args3[1] == "world");
    CHECK(args3[2] == "hello2");
    CHECK(args3[3] == "world2");
}

char *ArgsToBuf(const char * const * argv, int argc) {
    return ArgvStream(argc, argv).createBuffer(0, argc);
}

std::string ArgsToString(const char * const * argv, int argc) {
    return ArgvStream(argc, argv).asString(0, argc);
}

std::string ArgsToStringUnquoted(const char * const * argv, int argc) {
    auto os = std::ostringstream{};
    for (int i=0; i<argc; ++i) {
        os << (i ? " " : "") << argv[i];
    }
    return os.str();
}

std::vector<const char *> ArgvStream::c_strs(size_t start) const {
    std::vector<const char *> c_strs;
    std::transform(m_argv.begin() + start, m_argv.end(), 
                   std::back_inserter(c_strs), [](const std::string &s){return s.c_str();});
    c_strs.push_back(nullptr);
    return c_strs;
};

void ArgvStream::parseAndAppendArgs(std::string_view args) {
    std::stringstream arg;

    for (size_t i = 0; i < args.size(); i++) {
        if (isspace(args[i])) continue;
            
        arg.str("");
        arg.clear();
            
        bool single_quote = false;
        bool double_quote = false;
        bool backslash = false;

        for (; i < args.size(); i++) {
            if (isspace(args[i]) && !single_quote && !double_quote && !backslash) break;

            if (backslash) {
                backslash = false;
            } else if (args[i] == '\\') {
                backslash = true;
            } else if (single_quote && args[i] == '\'') {
                single_quote = false;
            } else if (double_quote && args[i] == '"') {
                double_quote = false;
            } else {
                if (args[i] == '\'')
                    single_quote = true;
                else if (args[i] == '"')
                    double_quote = true;
            }

            arg << args[i];
        }

        if (arg.tellp() != 0) {
            m_argv.push_back(arg.str());
        }
    }
}

std::string ArgvStream::asString(size_t start, size_t stop_after, char delim) const {
    std::stringstream ss;
    std::string delimStr = "";
        
    for (size_t i = start; i < m_argv.size() && stop_after > 0; i++, stop_after--) {
        if (std::string_view(m_argv[i]).find(delim) != std::string::npos) {
            ss << delimStr << std::quoted(m_argv[i]);
        } else {
            ss << delimStr << m_argv[i];
        }
        delimStr = delim;
    }

    return ss.str();
}
