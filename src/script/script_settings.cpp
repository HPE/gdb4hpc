/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "script_settings.h"

#include <iomanip>
#include <sstream>
#include <limits>
#include <iostream>

namespace script::settings {

namespace {

// helper for std::visit below
template<class> inline constexpr bool always_false_v = false;

} // anonymous namespace

std::string shortString(const Item& item) {
    return std::visit([](auto&& item) -> std::string {
        using T = std::decay_t<decltype(item)>;

        if constexpr (std::is_same_v<T, nullptr_t>) {
            return "(nullptr)"; // not intended to be used, but needed to compile
        } else if constexpr (std::is_convertible_v<T, const char*>) {
            return std::string{item};
        } else if constexpr (std::is_convertible_v<T, const bool*>) {
            return *item ? "true" : "false";
        } else if constexpr (std::is_convertible_v<T, const std::pair<double, double>*>) {
            std::ostringstream ss;
            ss << std::setprecision(2) << item->first << ", " << item->second;
            return ss.str();
        } else {
            static_assert(always_false_v<T>, "Incomplete stringify");
        }
    }, item);
}

std::string longString(const Item& item) {
    return std::visit([](auto&& item) -> std::string {
        using T = std::decay_t<decltype(item)>;

        if constexpr (std::is_same_v<T, nullptr_t>) {
            return "(nullptr)"; // not intended to be used, but needed to compile
        } else if constexpr (std::is_convertible_v<T, const char*>) {
            return std::string{item};
        } else if constexpr (std::is_convertible_v<T, const bool*>) {
            return *item ? "true" : "false";
        } else if constexpr (std::is_convertible_v<T, const std::pair<double, double>*>) {
            std::ostringstream ss;
            // no setprecision here
            ss << item->first << ", " << item->second;
            return ss.str();
        } else {
            static_assert(always_false_v<T>, "Incomplete stringify");
        }
    }, item);
}

bool equal(const Item& a, const Item& b) {
    // default std::variant operator== only compares pointer addresses, need
    // to write our own to dereference

    return std::visit([](auto&& a, auto&& b) -> bool {
        using T = std::decay_t<decltype(a)>;
        using RT = std::decay_t<decltype(b)>;

        if constexpr (std::is_same_v<T, nullptr_t> && std::is_same_v<RT, nullptr_t>) {
            // nullptr == nullptr
            return true; // not intended to be used, but needed to compile
        } else if constexpr (std::is_same_v<T, nullptr_t> || std::is_same_v<RT, nullptr_t>) {
            // nullptr != other pointer type
            return false;
        } else if constexpr (!std::is_convertible_v<T, RT> && !std::is_convertible_v<RT, T>) {
            // type mismatch
            // need to check if they are convertible to eachother instead of
            // std::is_same because std::is_same<const *, *> == false
            return false;
        } else if constexpr (std::is_convertible_v<T, const char*>) {
            auto va = std::string_view(a);
            auto vb = std::string_view(b);
            return va == vb;
        } else if constexpr (std::is_convertible_v<T, const bool*>) {
            return *a == *b;
        } else if constexpr (std::is_convertible_v<T, const std::pair<double, double>*>) {
            return *a == *b;
        } else {
            static_assert(always_false_v<T>, "Incomplete compare");
        }
    }, a, b);
}

} // namespace script::settings