/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "script_data.h"
#include "blockmap.h"
#include "gram.h"

#include <unordered_map>
#include <iomanip>

#if DEBUG
#include <iostream>
#include <iomanip>
// backslash sequences are escape codes to print debug messages in dark grey. if
// output is redirected to a file, they should be removed
#define LOG(msg) \
    std::cerr << "\u001b[30;1m" << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << "): " << msg << "\033[0m" << std::endl
#else 
#define LOG(msg)
#endif

namespace script::data {

ProgramVariable::ProgramVariable(const ExpressionHandle::ProgVarExpr& var)
    : m_symbolName{var.symbolName()}
    , m_symbolSubscript(var.symbolSubscript())
    , m_repeat{var.repeat()}
{}

ProgramVariable::ProgramVariable(const ProgramVariable& other)
    : m_symbolName{other.m_symbolName}
    , m_symbolSubscript{other.m_symbolSubscript}
    , m_repeat{other.m_repeat}
{}

ProgramLocation::ProgramLocation(const ExpressionHandle::ProgLocExpr& loc)
    : m_filename{loc.name()}
    , m_linenum{loc.lineNumber()}
{}

std::string ProgramLocation::string() const {
    return m_filename + ":" + std::to_string(m_linenum);
}

Expression::Expression(const ExpressionHandle::ProgVarExpr& var)
    : m_variable(var)
    , m_location(var.location())
    , m_blockmap{FindBlockMap(var.blockmapName().c_str())}
    , m_procsetSlice(var.procsetSubscript())
    , m_procset{var.procsetName()}
{
    using namespace std::string_literals;

    auto* procset = FindProcSet(m_procset.c_str());

    if (!procset) {
        LOG("Missing procset " << var.procsetName());
        throw InvalidAssertionError("Procset doesn't exist: $"s + var.procsetName());
    } else {
        LOG("Matched procset " << procset->s_name);
    }

    if (var.blockmapName() != "" && !m_blockmap) {
        LOG("Missing blockmap " << var.blockmapName());
        throw InvalidAssertionError("Blockmap doesn't exist: $"s + var.blockmapName());
    } else {
        LOG("Matched blockmap " << var.blockmapName());
    }

    // check that if a procset slice is present, it's not out of bounds
    auto ranksInProcset = 0;
    forEachOverlaySet(*procset, [&ranksInProcset](auto *os) {
        ranksInProcset += os->s_di_bs->numSet();
    });

    for (int i = 0; i < m_procsetSlice.getRank(); i++) {
        // no negative indices
        if (m_procsetSlice.getLow(i) < 0 || m_procsetSlice.getHigh(i) < 0) {
            throw InvalidAssertionError("Invalid procset slice; negative index: $"s + m_procset + "::{...}");
        }

        // no indices above the number of ranks in the procset
        if (m_procsetSlice.getLow(i) >= ranksInProcset || m_procsetSlice.getHigh(i) >= ranksInProcset) {
            throw InvalidAssertionError("Invalid procset slice; out of bounds: $"s + m_procset + "::" + m_procsetSlice.createBitset()->toSetString());
        }
    }
}

const dbgprocset_t* Expression::procset() const {
    return FindProcSet(m_procset.c_str());
}

Expression::Expression(const Expression& other)
    : m_variable{other.m_variable}
    , m_location{other.m_location}
    , m_blockmap{other.m_blockmap}
    , m_procsetSlice{other.m_procsetSlice}
    , m_procset{other.m_procset}
{}

std::string Expression::string() const {
    auto beginning = evalString();
    auto hasBlockmap = blockmap() != nullptr;
    auto loc = std::ostringstream{};

    if (hasBlockmap) {
        // evalString will have put an '}' on the end
        beginning.replace(beginning.size() - 1, 1, "");
    }

    loc << "@";
    loc << "\"" << location().filename() << "\"" << ":" << location().lineNumber();

    if (hasBlockmap) {
        loc << "}";
    }
    
    return beginning + loc.str();
}

std::string Expression::evalString() const {
    const auto stringInnerSlice = [&](const slice_t& s) {
        auto iss = std::ostringstream{};

        std::string comma = "";
        for (int i = 0; i < s.getRank(); i++) {
            int low = s.getLow(i);
            int high = s.getHigh(i);
            iss << comma;
            if (low == high) {
                iss << low;
            } else {
                iss << low << ".." << high;
            }
            comma = ",";
        }

        return iss.str();
    };

    auto ss = std::ostringstream{};

    auto hasBlockmap = blockmap() != nullptr;

    if (hasBlockmap) {
        ss << "$" << blockmap()->m_sym->h_key << "{";
    }

    ss << "$" << procsetName();
    if (procsetSlice().getRank() > 0) {
        ss << "{";
        ss << stringInnerSlice(procsetSlice());
        ss << "}";
    }
    ss << "::";
    ss << variable().symbolName();
    const auto& ssub = variable().symbolSubscript(); 
    if (ssub.getRank() > 0) {
        ss << "[";
        ss << stringInnerSlice(ssub);
        ss << "]";
    }

    if (variable().repeat() > 0) {
        ss << "@" << variable().repeat();
    }

    if (hasBlockmap) {
        ss << "}";
    }

    return ss.str();
}

Assertion::Assertion(size_t id, settings::Assertion settings, const ExpressionHandle::AssertExpr& assertExpr) 
    : m_id{id}
    , m_lhs(assertExpr.leftHandSide())
    , m_op(assertExpr.compareOp())
    , m_rhs(assertExpr.rightHandSide())
    , m_settings{std::move(settings)}
{
    using namespace std::string_literals;

    // if lhs ranks overlap rhs ranks but the locations are different, the assertion
    // is impossible to evaluate
    if (m_lhs.procsetName() == m_rhs.procsetName()) {
        // the procsets are the same...
        if (m_lhs.location() != m_rhs.location()) {
            // ...and the locations are different...

            auto leftSlice = m_lhs.procsetSlice().createBitset();
            auto rightSlice = m_rhs.procsetSlice().createBitset();
            if (!leftSlice && !rightSlice) {
                // ..and none of them are sliced, and therefore overlap. impossible!
                throw InvalidAssertionError("Expressions' ranks overlap ($"s + m_lhs.procsetName() + "), but locations are different. Assertion is impossible to evaluate");
            }
            if (!leftSlice || !rightSlice) {
                // ...and at least one is not sliced, therefore containing all 
                // ranks in the procset, so they overlap. impossible!
                throw InvalidAssertionError("Expressions' ranks overlap ("s + (leftSlice ? leftSlice->toSetString() : rightSlice->toSetString()) + "), but locations are different. Assertion is impossible to evaluate");
            }
            // ...and they're both sliced...
            auto intersection = bitset::fromAnd(*leftSlice, *rightSlice);
            if (intersection->numSet() > 0) {
                // ...and there's an overlap. impossible!
                throw InvalidAssertionError("Expressions' ranks overlap ("s + intersection->toSetString() + "), but locations are different. Assertion is impossible to evaluate");
            }
        }
    }
}

std::string Assertion::string() const {
    auto ss = std::ostringstream{};

    ss << leftHandSide().string()
       << " " << op().string() << " "
       << rightHandSide().string()
    ;

    return ss.str();
}

Script::Script(std::string name) 
    : m_name{std::move(name)}
{
    LOG("Creating script $" << m_name);
}

void Script::newAssert(const ExpressionHandle::AssertExpr& assertExpr) {
    LOG("New assert");

    try {
        m_asserts.emplace_back(m_nextId++, m_assertSettings, assertExpr);
    } catch (const InvalidAssertionError &e) {
        LOG(e.what());
        throw e;
    }

    LOG("New assert: success");
}

std::string Script::string() const {
    return string(std::numeric_limits<size_t>::max());
}

std::string Script::string(size_t lines) const {
    auto ss = std::ostringstream{};

    ss << "Assertion script $" << m_name << ":\n";

    size_t count = 0;
    for (const auto& ai : m_asserts) {
        ss << "Assertion " << ai.id() << ": ";
        ss << ai.string();
        ss << "\n";
        if (++count == lines) {
            if (count < m_asserts.size()) {
                ss << "...";
            }
            break;
        };
    }

    return ss.str();
}

settings::Item Script::getSetting(const std::string& name) {
    using namespace settings::token;

    // try script settings
    auto tryScript = m_scriptSettings.getSetting(name);
    if (!std::holds_alternative<nullptr_t>(tryScript)) {
        return tryScript;
    }

    // try assertion settings (might return nullptr)
    return m_assertSettings.getSetting(name);
}

namespace {

// works on anything with .getSetting()
// produces either "configured with default settings" or something like
// "configured with stop, !verbose, error: 1.2, 2.4", where every item listed
// differs from the default settings (item is omitted if it matches default)
template <typename SettingsStruct>
std::string settingsStringHelper(SettingsStruct settings) {
    std::ostringstream ss;
    bool allDefault = true;
    std::string comma = "";

    for (const auto& sett : settings::infoTable()) {
        const auto current = settings.getSetting(sett.name);
        if (std::holds_alternative<nullptr_t>(current)) continue;

        // check if current differs from default
        if (!settings::equal(current, sett.defaultt)) {
            if (allDefault) ss << "configured with ";
            ss << comma;
            if (std::holds_alternative<bool*>(current) || std::holds_alternative<const bool*>(current)) {
                // annotate bools with "!" (or a lack of "!")
                const auto b = std::holds_alternative<const bool*>(current)
                    ? *std::get<const bool*>(current)
                    : *std::get<bool*>(current)
                ;
                if (!b) ss << "!";
                ss << sett.name;
            } else {
                // all other types are just name: value
                ss << sett.name << ": " << settings::shortString(current);
            }
            comma = ", ";
            allDefault = false;
        }

    }

    return ss.str();
}

} // anonymous namespace

std::string Assertion::settingsString() const {
    return settingsStringHelper(m_settings);
}

std::string Script::settingsString() const {
    return settingsStringHelper(m_scriptSettings);
}

} // namespace script::data
