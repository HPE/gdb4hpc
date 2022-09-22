/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

// assertion-specific extensions of the pure text/primitive classes
// in expression_handle.h. these classes don't know how to communicate with
// the backend, but do know a little more than the expression_handle.h classes.
// for example, these classes point to actual instances of procsets and blockmaps
// instead of simply holding on to their names.

#include "script_settings.h"

#include "blockmap.h"
#include "expression_handle.h"
#include "procset.h"

#include <string>
#include <variant>

namespace script::data {

using namespace ExpressionHandleShorthands;

class InvalidAssertionError : public std::runtime_error {
public:
    InvalidAssertionError(const std::string& msg) : std::runtime_error(msg) {};
};

class ProgramLocation {
    // Read-only data. Represents a time (filename, line number) and place (rank(s)) of
    // execution during the underlying program
public:
    ProgramLocation(DbgInvoke::Handle di, std::unique_ptr<Bitset>&& bitset,
                    std::string filename, int linenum);
    explicit ProgramLocation(const ProgramLocation& other);

    DbgInvoke::Handle dbgInvoke() const;
    const Bitset& bitset() const;
    const std::string& filename() const;
    int lineNumber() const;
    std::string getLogString() const;

private:
    DbgInvoke::Handle m_dbgInvoke;
    std::unique_ptr<Bitset> m_bitset;
    std::string m_filename;
    int m_linenum;
};

class Assertion {
    // Read only data. Represents a single assertion statement.

public:
    struct ParsedAssertion;

    // Constructs an Assertion from exprString. If the passed expression is not
    // fit for an assert, throws InvalidAssertionError. To be fit for an assert,
    // an expression must:
    // 1. Be a comparison. i.e. Top operator is ==, <, etc.
    // 2. Include procset/location data for every program variable used.
    //    e.g. "foo == bar" is wrong,
    //    "$a{0..1}::foo@"src.c":10 == $b{2..3}::bar@"src.c":20" is right.
    Assertion(size_t id, settings::Assertion settings, std::string&& exprString);
    Assertion(Assertion&& other) = default;

    Assertion(const Assertion& other) = delete;
    Assertion& operator=(const Assertion& other) = delete;

    auto id() const { return m_id; }
    // parse contained string for validity and return a ParsedAssertion if it is
    // determined to be valid. if the contained expression string isn't parsable, throws
    // InvalidAssertionError
    ParsedAssertion parse() const;
    const auto& settings() const { return m_settings; }

    // assertion expression as it was originally typed by the user
    const std::string& string() const;
    // human readable description of settings that differ from default
    std::string settingsString() const;

private:

    const size_t m_id;
    std::string m_string;
    settings::Assertion m_settings;
};

class Assertion::ParsedAssertion {
    // a "live" assertion expression that has been checked to contain valid data (pointers
    // to DbgInvokes, procsets, etc.) at creation time. Created by Assertion::parse().
public:
    ExpressionHandle expression;
    std::vector<ProgramLocation> locations;

private:
    friend Assertion;
    ParsedAssertion(ExpressionHandle&& expr, std::vector<ProgramLocation>&& locations);
};

class Script {
    // Read/append only data, and settings. A list of Assertion objects. When
    // newAssert is called, the current internal Assertion::Settings is applied
    // to it.

public:
    Script(std::string name);

    const auto& name() const { return m_name; };

    // Constructs an Assertion from exprString and adds it to the
    // internal list of assertions. If the passed expression is not fit for an
    // assert, throws InvalidAssertionError. See script::Assertion constructor.
    // To be fit for an assert, an expression must:
    // 1. Be a comparison. i.e. Top operator is ==, <, etc.
    // 2. Include procset/location data for every program variable used.
    //    e.g. "foo == bar" is wrong,
    //    "$a{0..1}::foo@"src.c":10 == $b{2..3}::bar@"src.c":20" is right.
    void newAssert(std::string&& exprString);

    // Return list of assertions that make up the script
    const auto& asserts() const { return m_asserts; }

    // returns pointer to item representing the setting with passed name. if such
    // a setting doesn't exist, the returned variant contains nullptr_t
    settings::Item getSetting(const std::string& name);
    const auto& assertionSettings() const { return m_assertSettings; }
    const auto& scriptSettings() const { return m_scriptSettings; }

    // Create a human readable string showing all assertions in the script
    std::string string() const;
    // Create a human readable string from at most n assertions from the script, starting
    // with the first
    std::string string(size_t numAsserts) const;
    // Create a human readable description of settings that differ from default
    std::string settingsString() const;

private:
    std::string m_name;
    std::vector<Assertion> m_asserts;
    size_t m_nextId = 1;

    // applied to asserts as they are created
    settings::Assertion m_assertSettings;
    settings::Script m_scriptSettings;
};

} // namespace script::data
