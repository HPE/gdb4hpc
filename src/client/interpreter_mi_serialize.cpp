// Copyright 2025 Hewlett Packard Enterprise Development LP.
//
// Helper classes and functions for generating mi message output
//
#include "interpreter_mi_serialize.h"
#include "unittest_macros.h"

#include <iomanip>
#include <sstream>
#include <variant>

TEST_CASE("Value and Message serialization") {
    SUBCASE("Simple string value") {
        auto msg = mi::MessageContents()
            << mi::Result{"key", mi::Const{"test"}}
        ;

        CHECK(msg.serialize() == "key=\"test\"");
    }

    SUBCASE("Quoted string value") {
        auto msg = mi::MessageContents()
            << mi::Result{"key", mi::Const{"value with \"quotes\" and spaces"}}
        ;

        CHECK(msg.serialize() == "key=\"value with \\\"quotes\\\" and spaces\"");
    }

    SUBCASE("Empty string value") {
        auto msg = mi::MessageContents()
            << mi::Result{"key", mi::Const{""}}
        ;

        CHECK(msg.serialize() == "key=\"\"");
    }

    SUBCASE("Multiple values") {
        auto msg = mi::MessageContents()
            << mi::Result{"key1", mi::Const{"value1"}}
            << mi::Result{"key2", mi::Const{"value2"}}
        ;

        CHECK(msg.serialize() == "key1=\"value1\",key2=\"value2\"");
    }

    SUBCASE("Nested values - tuple") {
        auto msg = mi::MessageContents()
            << mi::Result{"dict", mi::Tuple{}
                << mi::Result{"inner1", mi::Const{"value1"}}
                << mi::Result{"inner2", mi::Const{"value2"}}}
        ;

        CHECK(msg.serialize() == "dict={inner1=\"value1\",inner2=\"value2\"}");
    }

    SUBCASE("Nested values - list") {
        auto msg = mi::MessageContents()
            << mi::Result{"list", mi::List{}
                << mi::Const{"item1"}
                << mi::Const{"item2"}
                << mi::Const{"item3"}}
        ;

        CHECK(msg.serialize() == "list=[\"item1\",\"item2\",\"item3\"]");
    }

    SUBCASE("Complex nested structure") {
        // Create a complex structure like:
        // ^done,bkpts=[{number="1",type="breakpoint",disp="keep",enabled="y"},
        //              {number="2",type="breakpoint",disp="keep",enabled="n"}]

        auto msg = mi::MessageContents()
            << mi::Result{"bkpts", mi::List{}
                << (mi::Tuple{}
                    << mi::Result{"number", mi::Const{"1"}}
                    << mi::Result{"type", mi::Const{"breakpoint"}}
                    << mi::Result{"disp", mi::Const{"keep"}}
                    << mi::Result{"enabled", mi::Const{"y"}})
                << (mi::Tuple{}
                    << mi::Result{"number", mi::Const{"2"}}
                    << mi::Result{"type", mi::Const{"breakpoint"}}
                    << mi::Result{"disp", mi::Const{"keep"}}
                    << mi::Result{"enabled", mi::Const{"n"}})}
        ;

        CHECK(msg.serialize() == "bkpts=[{number=\"1\",type=\"breakpoint\",disp=\"keep\",enabled=\"y\"},{number=\"2\",type=\"breakpoint\",disp=\"keep\",enabled=\"n\"}]");
    }

    SUBCASE("Empty list and tuple") {
        auto msg = mi::MessageContents()
            << mi::Result{"empty_list", mi::List{}}
            << mi::Result{"empty_dict", mi::Tuple{}}
        ;

        CHECK(msg.serialize() == "empty_list=[],empty_dict={}");
    }

    SUBCASE("Deeply nested structure") {
        // Create a deeply nested structure to test handling of complex hierarchies
        auto msg = mi::MessageContents()
            << mi::Result{"complex", mi::Tuple{}
                << mi::Result{"nested_list", mi::List{}
                    << mi::Const{"item1"}
                    << (mi::Tuple{}
                        << mi::Result{"key1", mi::Const{"val1"}}
                        << mi::Result{"key2", mi::Const{"val2"}})
                    << mi::Const{"item3"}}
                << mi::Result{"simple_key", mi::Const{"simple_value"}}}
        ;

        CHECK(msg.serialize() == "complex={nested_list=[\"item1\",{key1=\"val1\",key2=\"val2\"},\"item3\"],simple_key=\"simple_value\"}");
    }
}

namespace mi {

Value::Value(Const str)
    : data(std::make_shared<const Value::Variant>(std::move(str))) {}

Value::Value(List list)
    : data(std::make_shared<const Value::Variant>(std::move(list))) {}

Value::Value(Tuple tuple)
    : data(std::make_shared<const Value::Variant>(std::move(tuple))) {}

Value::Value(Result result)
    : data(std::make_shared<const Value::Variant>(std::move(result))) {}

List& List::operator<<(Const&& str) {
    contents.push_back(str);
    return *this;
}

List& List::operator<<(Result&& result) {
    contents.push_back(std::move(result));
    return *this;
}

List& List::operator<<(List&& list) {
    contents.push_back(std::move(list));
    return *this;
}

List& List::operator<<(Tuple&& tuple) {
    contents.push_back(std::move(tuple));
    return *this;
}

List& List::operator<<(Value&& value) {
    contents.push_back(std::move(value));
    return *this;
}

Tuple& Tuple::operator<<(Result&& result) {
    contents.push_back(std::move(result));
    return *this;
}

MessageContents& MessageContents::operator<<(Result&& result) {
    m_data.push_back(result);
    return *this;
}

namespace {
    // Helper functions to serialize different types of values
    void serialize_value(const Value::Variant& value, std::ostringstream& ss);

    void serialize_const(const std::string& str, std::ostringstream& ss) {
        // strings need to be quoted
        ss << std::quoted(str);
    }

    void serialize_list(const List& list, std::ostringstream& ss) {
        ss << '[';
        bool first = true;
        for (const auto& item : list.contents) {
            if (!first) ss << ',';
            first = false;
            serialize_value(*item.data, ss);
        }
        ss << ']';
    }

    void serialize_tuple(const Tuple& tuple, std::ostringstream& ss) {
        ss << '{';
        bool first = true;
        for (const auto& item : tuple.contents) {
            if (!first) ss << ',';
            first = false;
            ss << item.key << '=';
            serialize_value(*item.value.data, ss);
        }
        ss << '}';
    }

    void serialize_result(const Result& result, std::ostringstream& ss) {
        ss << result.key << '=';
        serialize_value(*result.value.data, ss);
    }

    void serialize_value(const Value::Variant& value, std::ostringstream& ss) {
        std::visit([&ss](const auto& v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::string>) {
                serialize_const(v, ss);
            } else if constexpr (std::is_same_v<T, List>) {
                serialize_list(v, ss);
            } else if constexpr (std::is_same_v<T, Tuple>) {
                serialize_tuple(v, ss);
            } else if constexpr (std::is_same_v<T, Result>) {
                serialize_result(v, ss);
            }
        }, value);
    }
}

std::string MessageContents::serialize() const {
    std::ostringstream ss;

    std::string sep = "";

    // Add results
    for (const auto& value : m_data) {
        ss << sep;
        sep = ",";
        serialize_value(*value.data, ss);
    }

    return ss.str();
}

} // namespace mi
