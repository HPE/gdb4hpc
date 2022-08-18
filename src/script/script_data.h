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

class Op {
    // maps to CompOpExpr
public:
    Op(const CompOpExpr& op): m_op {op} {};
    constexpr const char* string() const {
        using namespace ExpressionHandleShorthands;
        switch(m_op) {
            case CompOpExpr::None: return "None";
            case CompOpExpr::Equal: return "==";
            case CompOpExpr::NotEqual: return "!=";
            case CompOpExpr::LessThan: return "<";
            case CompOpExpr::LessThanEqual: return "<=";
            case CompOpExpr::GreaterThan: return ">";
            case CompOpExpr::GreaterThanEqual: return ">=";
            // we don't use "default: ..." so the compiler will warn us
            // if we're missing one...
        }

        // ...but we still suppress the "control reaches end of non-void
        // function" warning

        return ""; // should never get here
    }
private:
    CompOpExpr m_op;
};

class ProgramVariable {
    // Read-only data. Represents a variable in the underlying program
public:
    ProgramVariable(const ProgVarExpr& var);
    ProgramVariable(const ProgramVariable& other);

    const auto& symbolName() const { return m_symbolName; }
    // if no subscript, returned slice_t will have a getRank() of 0
    const auto& symbolSubscript() const { return m_symbolSubscript; }
    // returns 0 if no repeat
    const auto& repeat() const { return m_repeat; }

private:
    std::string m_symbolName;
    slice_t m_symbolSubscript;
    unsigned int m_repeat;
};

class ProgramLocation {
    // Read-only data. Represents a time of execution during the underlying program
public:
    ProgramLocation(const ProgLocExpr& loc);

    const auto& filename() const { return m_filename; }
    const auto& lineNumber() const { return m_linenum; }

    std::string string() const;

    bool operator==(const ProgramLocation& other) const {
        return m_filename == other.m_filename && m_linenum == other.m_linenum;
    }

    bool operator!=(const ProgramLocation& other) const {
        return !(*this == other);
    }

private:
    std::string m_filename;
    int m_linenum;
};

class Expression {
    // Read-only data. Represents one half of an assertion script.
public:
    Expression(const ProgVarExpr& var);

    Expression(const Expression& other);
    Expression& operator=(const Expression& other) = delete;
    
    const auto& variable() const { return m_variable; }
    const auto& location() const { return m_location; }
    const auto* blockmap() const { return m_blockmap; }
    const auto& procsetSlice() const { return m_procsetSlice; }
    const auto* procset() const { return m_procset; }

    // full string with location, e.g. $bmap{$pset::varName@"file.c":XX}
    std::string string() const;
    // substring without location, e.g. $bmap{$pset::varName}
    std::string evalString() const;

private:
    ProgramVariable m_variable;
    ProgramLocation m_location;
    g_block_map_t* m_blockmap; // non-owning, points to data in global blockmap table
    slice_t m_procsetSlice; // if no slice, getRank() returns 0
    dbgprocset_t* m_procset; // non-owning, points to data in global procset table
};

class Assertion {
    // Read only data. Represents a single assertion statement.

public:
    Assertion(size_t id, settings::Assertion settings, const AssertExpr& assertExpr);
    Assertion(Assertion&& other) = default;

    Assertion(const Assertion& other) = delete;
    Assertion& operator=(const Assertion& other) = delete;

    auto id() const { return m_id; }
    const auto& leftHandSide() const { return m_lhs; }
    auto op() const { return m_op; }
    const auto& rightHandSide() const { return m_rhs; }
    const auto& settings() const { return m_settings; }

    std::string string() const;
    // human readable description of settings that differ from default
    std::string settingsString() const;

private:
    const size_t m_id;
    Expression m_lhs;
    Op m_op;
    Expression m_rhs;
    settings::Assertion m_settings;
};

class Script {
    // Read/append only data, and settings. A list of Assertion objects. When
    // newAssert is called, the current internal Assertion::Settings is applied
    // to it.

public:
    Script(std::string name);

    const auto& name() const { return m_name; };
    // Construct a script::Expression from the assertExpr and add it to the
    // internal list of asserts
    void newAssert(const AssertExpr &assertExpr);
    // Return list of assertions that make up the script
    const auto& asserts() const { return m_asserts; }
    
    // returns pointer to item representing the setting with passed name. if such
    // a setting doesn't exist, the returned variant contains nullptr_t
    settings::Item getSetting(const std::string& name);
    const auto& assertionSettings() const { return m_assertSettings; }
    const auto& scriptSettings() const { return m_scriptSettings; }

    std::string string() const;
    // Create a string from at most n assertions
    std::string string(size_t lines) const;
    // human readable description of settings that differ from default
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
