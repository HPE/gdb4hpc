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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "args.h"
#include "doctest.h"

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

char *ArgsToBuf(const char * const * argv, int argc) {
    return ArgvStream(argc, argv).createBuffer(0, argc);
}

std::string ArgsToString(const char * const * argv, int argc) {
    return ArgvStream(argc, argv).asString(0, argc);
}
