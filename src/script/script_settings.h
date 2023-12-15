/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

// assertion script settings api

#include <tuple>
#include <variant>
#include <string>
#include <array>
#include <map>

namespace script::settings {

namespace token {
// assertion/script settings tokens. these are the words the user actually uses
// to get at various settings.
// extra const keywords needed for "converting string constant to char*" warning

// setting names
constexpr const char* Stop = "stop";
constexpr const char* Halt = "halt";
constexpr const char* VbAsserts = "verbose-asserts";
constexpr const char* VbBPs = "verbose-breakpoints";
constexpr const char* Error = "error";

// used in "scope" column of summary table
constexpr const char* ScopeAssert = "per assert";
constexpr const char* ScopeScript = "per script";

} // namespace token

// returned by the settings api
using Item = std::variant<
    std::nullptr_t,  // used when a setting doesn't exist 
    const char*, // helper to allow passing Item to table printing functions. not actually used as a setting
    // ptr returned to user, const ptr used to allow defaults to be used in constexpr functions
    bool*, const bool*, 
    std::pair<double, double>*, const std::pair<double, double>*
>;

struct Assertion {
    // settings that can differ between assertion scripts.
    // set by user via "set xyz ..." during assertion script writing

    constexpr Assertion()
        : stopOnFailure{false}
        , verboseEval{false}
        , error{0, 0}
    {}

    // map a settings token to data
    Item getSetting(const std::string& name) {
        using namespace token; // settings tokens

        if (name == Stop) return &stopOnFailure;
        if (name == VbAsserts) return &verboseEval;
        if (name == Error) return &error;

        return nullptr;
    }

    const Item getSetting(const std::string& name) const {
        return const_cast<Assertion&>(*this).getSetting(name);
    }

    // return a map of name -> Item. Items are references to data in the assertion and are
    // only valid as long as the script is valid.
    //
    // FIXME: this should just copy the Items. no reason to return mutable Items - Items
    // just don't have value semantics yet.
    std::map<const char*, Item> mapView() const {
        return {
            { token::Stop, &this->stopOnFailure },
            { token::VbAsserts, &this->verboseEval },
            { token::Error, &this->error },
        };
    }

    bool stopOnFailure;              // stop the script if this assertion fails
    bool verboseEval;                // print results of this assertion as it's evaluated
    std::pair<double, double> error; // warn, error tolerances used when calling compare
};
// defaults
constexpr auto defAssertSettings = Assertion{};

struct Script {
    // settings that apply to an assertion script as a whole.
    // set by user via "set xyz ..." during assertion script writing

    constexpr Script()
        : haltUponEnd{false}
        , verboseBreakpoints{false}
    {}

    // map a settings token to data
    Item getSetting(const std::string& name) {
        using namespace token; // settings tokens

        if (name == Halt) return &haltUponEnd;
        if (name == VbBPs) return &verboseBreakpoints;

        return nullptr;
    }

    const Item getSetting(const std::string& name) const {
        return const_cast<Script&>(*this).getSetting(name);
    }

    // return a map of name -> Item. Items are references to data in the script and are
    // only valid as long as the script is valid.
    //
    // FIXME: this should just copy the Items. no reason to return mutable Items - Items
    // just don't have value semantics yet.
    std::map<const char*, Item> mapView() const {
        return {
            { token::Halt, &this->haltUponEnd },
            { token::VbBPs, &this->verboseBreakpoints },
        };
    }

    bool haltUponEnd;        // send halt to all involved ranks when script stops
    bool verboseBreakpoints; // print breakpoints as they are hit
};
// defaults
constexpr auto defScriptSettings = Script{};

// return a short stringified version of item. useful for summary tables.
std::string shortString(const Item& item);
// return a verbose stringified version of item. useful for detailed views.
std::string longString(const Item& item);
// return true if a == b
bool equal(const Item& a, const Item& b);

// used for help/info/show/etc lookups
struct Info {
    const char* name;
    const char* description;
    const char* scope;
    Item defaultt; // "default" is reserved C++ keyword
};

namespace detail {

using I = Info;

// table of all settings accessed by set/show commands while writing an assertion script.
// includes both assert and script settings.
// this table is informational only, used for displaying help/summary/etc commands.
// actual settings are stored in script::settings::Assertion and 
// script::settings::Script objects owned by script::data::Assertion and script::data::Script
constexpr std::array avail {
    I{
        token::Stop,                                          // token used to set/show this setting
        "Assertion script will stop upon assertion failure.", // short description
        token::ScopeAssert,                                   // scope keyword (assert or script)
        &defAssertSettings.stopOnFailure                      // ptr to default value
    }, 
    // and the rest...
    I{token::Halt, "Assertion script will halt all involved ranks upon script end.",token::ScopeScript, &defScriptSettings.haltUponEnd},
    I{token::VbAsserts, "Assertion results will be printed as they are evaluated.", token::ScopeAssert, &defAssertSettings.verboseEval},
    I{token::VbBPs, "Breakpoints will be printed as they are hit.", token::ScopeScript, &defScriptSettings.verboseBreakpoints},
    I{token::Error, "Assertion warn/error tolerances.", token::ScopeAssert, &defAssertSettings.error}
};

// returned in cases where a search in the above table fails
constexpr auto notfound = Info {"not found", "not found", "not found", nullptr};

} // namespace detail

// returns a table of information about all available settings
constexpr auto& infoTable() { return detail::avail; };

// find information about a setting with a given name. if no setting exists,
// returned Info's "defaultt" member will hold nullptr_t
constexpr const Info& findInfo(const char* name) {
    for (const auto& item : detail::avail) {
        if (std::string_view(item.name) == std::string_view(name)) {
            return item;
        }
    }

    return detail::notfound;
}

} // namespace script::settings
