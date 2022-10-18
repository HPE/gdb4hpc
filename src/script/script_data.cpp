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
// backslash sequences are escape codes to print debug messages in dark grey. if
// output is redirected to a file, they should be removed
#define LOG(msg) \
    std::cerr << "\u001b[30;1m" << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << "): " << msg << "\033[0m" << std::endl
#else 
#define LOG(msg)
#endif

namespace script::data {

static std::vector<ProgramLocation> extractProgramLocations(const ExpressionHandle& expr);
static void enforceValidExpression(const ExpressionHandle& expr);
static void enforceValidLocations(const std::vector<ProgramLocation>& locations);

ProgramLocation::ProgramLocation(DbgInvoke::Handle di, std::unique_ptr<Bitset>&& bitset,
                                 std::string filename, int linenum)
    : m_dbgInvoke{di}
    , m_bitset{std::move(bitset)}
    , m_filename{std::move(filename)}
    , m_linenum{linenum} {}

ProgramLocation::ProgramLocation(const ProgramLocation& other)
    : m_dbgInvoke{other.m_dbgInvoke}
    , m_bitset{other.m_bitset->clone()}
    , m_filename{other.m_filename}
    , m_linenum{other.m_linenum} {}

DbgInvoke::Handle ProgramLocation::dbgInvoke() const {
    return m_dbgInvoke;
}

const Bitset& ProgramLocation::bitset() const { return *m_bitset; }

const std::string& ProgramLocation::filename() const { return m_filename; }

int ProgramLocation::lineNumber() const { return m_linenum; }

std::string ProgramLocation::getLogString() const {
    return std::string{"$"} + m_dbgInvoke.name() + bitset().toSetString() + "::" + filename() + ":" + std::to_string(lineNumber());
}

bool ProgramLocation::matches(const std::string& file, int lineNum) const
{
    if (lineNum != m_linenum) return false;
    if (file == m_filename) return true;

    // file make be a full path
    if (auto loc = file.rfind(m_filename);
        loc + m_filename.size() == file.size() && loc != 0 && file[loc-1] == '/') return true;
    return false;
}

Assertion::Assertion(size_t id, settings::Assertion settings, std::string&& exprString)
    : m_id{id}
    , m_string{std::move(exprString)}
    , m_settings{std::move(settings)} {}

Assertion::ParsedAssertion Assertion::parse() const {
    try {
        auto expr = ExpressionHandle(m_string);
        enforceValidExpression(expr);

        auto locations = extractProgramLocations(expr);
        enforceValidLocations(locations);

        return ParsedAssertion(std::move(expr), std::move(locations));
    } catch (const ExpressionHandle::ParseError& e) {
        throw InvalidAssertionError(e.what());
    }
    // letting InvalidAssertionError escape
}

const std::string& Assertion::string() const {
    return m_string;
}

static std::vector<ProgramLocation> extractProgramLocations(const ExpressionHandle& expr) {
    // every variable used in the expression is required to specify the procset
    // and file/line number so the assertion script can set breakpoints and know
    // where/when to actually run the evaluation. if we find a variable that is
    // missing some of that information, it's unusable, and we throw InvalidAssertionError.

    using namespace std::string_literals;
    auto result = std::vector<ProgramLocation>{};

    try {
        const auto& rawExpr = expr.raw();
        const auto top = rawExpr.e_lval - 1;

        // scan the stack for <var> or <blockmap> tags, which are the start of variables
        // tagged with locations, and parse them. assemble<ProgVarExpr> will try to
        // extract the variable and the location all at once and fail if it can't satisfy
        // everything.
        for (int i = top; i >= 0; --i) {
            switch (rawExpr.e_expr[i].e_op) {
            case EXPR_BLOCKMAP:
            case EXPR_VAR: {
                auto var = expr.assembleFromOffset<ProgVarExpr>(top - i);

                // successful parse, now validate the procset

                const auto* pset = FindProcSet(var.procsetName().c_str());

                if (!pset) {
                    // procset is no longer valid
                    throw InvalidAssertionError(std::string{"$"} + var.procsetName() +
                                                " in \"" + expr.string() +
                                                "\" is expired");
                }

                auto ranksInProcset = 0;
                forEachOverlaySet(*pset, [&ranksInProcset](auto* os) {
                    ranksInProcset += os->s_di_bs->numSet();
                });

                auto& procSubScr = var.procsetSubscript();
                for (int i = 0; i < procSubScr.getRank(); i++) {
                    // ensure low <= high
                    if (procSubScr.getLow(i) > procSubScr.getHigh(i)) {
                        throw InvalidAssertionError(
                            "Invalid procset slice; low index larger than high index in "
                            "$" + var.procsetName() + "::{...}");
                    }

                    // no negative indices allowed
                    if (procSubScr.getLow(i) < 0 || procSubScr.getHigh(i) < 0) {
                        throw InvalidAssertionError(
                            "Invalid procset slice; negative index: $"s +
                            var.procsetName() + "::{...}");
                    }

                    // no indices above the number of ranks in the procset allowed
                    if (procSubScr.getLow(i) >= ranksInProcset ||
                        procSubScr.getHigh(i) >= ranksInProcset) {
                        throw InvalidAssertionError(
                            "Invalid procset slice; out of bounds: $"s +
                            var.procsetName() +
                            "::" + procSubScr.createBitset()->toSetString());
                    }
                }

                // procset is good. now create ProgramLocations from its dis
                forEachOverlaySet(*pset, [&](const auto* os) {
                    result.emplace_back(
                        os->s_di, sliceBitset(*os, &procSubScr), var.location().name(),
                        var.location().lineNumber());
                });
                break;
            }
            default:
                // not interested in this token
                break;
            }
        }

        LOG("Extracted locations:");
        for ([[maybe_unused]] const auto& loc : result) {
            LOG("- " << loc.getLogString());
        }
    } catch (const ParseError& e) {
        LOG(e.what());
        throw InvalidAssertionError("Failed to extract location; Syntax error: "s + e.what());
    }

    return result;
}

static void enforceValidExpression(const ExpressionHandle& expr) {
    // expression must be a comparison (==, !=, <, <=, >, >=),
    // so check symbol on top of stack
    switch (expr.raw().e_expr[expr.raw().e_lval - 1].e_op) {
    case EXPR_OPEQUAL:
    case EXPR_OPNOTEQUAL:
    case EXPR_OPLT:
    case EXPR_OPLEQ:
    case EXPR_OPGT:
    case EXPR_OPGEQ:
        // ok
        break;
    default:
        throw InvalidAssertionError("Assertion expression must be a "
                                    "comparison (==, !=, <, <=, >, >=)");
    }
}

static void enforceValidLocations(const std::vector<ProgramLocation>& locations) {
    using namespace std::string_literals;

    // expression must include at least one program location to be meaningful in
    // an assertion script. without a location, we have no way to know when to evaluate it.
    if (locations.empty())
        throw InvalidAssertionError(
            "No program locations in assertion, nothing to evaluate. Specify "
            "program variables with $a{0}::var@\"filename\":linenum syntax");

    // expression must use valid dbginvokes
    for (const auto& loc : locations) {
        const auto* di = loc.dbgInvoke().lock();
        if (!di) {
            LOG("expired dbginvoke " << loc.dbgInvoke().name());
            throw InvalidAssertionError("Expired procset: $"s + loc.dbgInvoke().name());
        }
    }

    // if one dbginvoke/bitset overlaps another dbginvoke/bitset but the locations are
    // different, the assertion is impossible to evaluate because the program cannot stop
    // in a state that satisfies both at the same time.
    for (size_t i = 0; i < locations.size(); ++i) {
        auto& lhs = locations[i];
        for (size_t j = i + 1; j < locations.size(); ++j) {
            auto& rhs = locations[j];

            if (lhs.dbgInvoke().name() != rhs.dbgInvoke().name())
                continue; // ok: independent dbginvokes can't conflict.

            // the dbginvokes are the same...

            if (lhs.filename() == rhs.filename() && lhs.lineNumber() == rhs.lineNumber())
                continue; // ok: multiple values at the same location can be evaluated together

            // ...and the locations are different...

            const auto& leftSlice = lhs.bitset();
            const auto& rightSlice = rhs.bitset();
            auto intersection = bitset::fromAnd(leftSlice, rightSlice);
            if (intersection->numSet() > 0) {
                // ...but there's an overlap.
                throw InvalidAssertionError(
                    "Expression contains overlapping ranks ($"s + lhs.dbgInvoke().name() +
                    lhs.bitset().toSetString() + " and $" + rhs.dbgInvoke().name() +
                    rhs.bitset().toSetString() + " with overlap " +
                    intersection->toSetString() +
                    "), but locations are different. Assertion is "
                    "impossible to evaluate");
            }
        }
    }
}

Script::Script(std::string name)
    : m_name{std::move(name)}
{
    LOG("Creating script $" << m_name);
}

void Script::newAssert(std::string&& exprString) {
    LOG("New assert");

    try {
        auto assert = Assertion(m_nextId++, m_assertSettings, std::move(exprString));

        // throws if assertExpr is not fit to be an assert. we don't keep the results of
        // the parse since program state could change between now and script start. the
        // assertion will be re-parsed at script start time. the results of that parse
        // might be different from the results of this one, but to provide useful feedback
        // to the user at writing time, we parse for errors here, too.
        assert.parse();

        m_asserts.emplace_back(std::move(assert));
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

Assertion::ParsedAssertion::ParsedAssertion(ExpressionHandle&& expr, std::vector<ProgramLocation>&& locs)
    : expression{std::move(expr)}
    , locations{std::move(locs)} {}

std::string Script::settingsString() const {
    return settingsStringHelper(m_scriptSettings);
}

} // namespace script::data
