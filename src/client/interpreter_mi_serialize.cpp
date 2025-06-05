// Copyright 2025 Hewlett Packard Enterprise Development LP.
//
// Helper classes and functions for generating mi message output
//
#include "interpreter_mi_serialize.h"
#include "unittest_macros.h"

#include <iomanip>
#include <sstream>
#include <regex>
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

    SUBCASE("Breakpoint-like response") {
        auto msg = mi::MessageContents{}
            << mi::Result{"bkpts", mi::List{}
                << (mi::Tuple{}
                    << mi::Result{"proc_set", "$a"}
                    << mi::Result{"number", "1"}
                    << mi::Result{"type", "breakpoint"}
                    << mi::Result{"disp", "keep"}
                    << mi::Result{"enabled", "y"}
                    << mi::Result{"file", "interpreter_mi_serialize.cpp"}
                    << mi::Result{"line", "42"}
                    << mi::Result{"fullpath", "/path/to/interpreter_mi_serialize.cpp"}
                    << mi::Result{"addr", "0x12345678"}
                    << mi::Result{"function", "main"}
                )
                << (mi::Tuple{}
                    << mi::Result{"proc_set", "$b"}
                    << mi::Result{"number", "2"}
                    << mi::Result{"type", "breakpoint"}
                    << mi::Result{"disp", "del"}
                    << mi::Result{"enabled", "n"}
                    << mi::Result{"file", "another_file.cpp"}
                    << mi::Result{"line", "100"}
                    << mi::Result{"fullpath", "/path/to/another_file.cpp"}
                    << mi::Result{"addr", "0x87654321"}
                    << mi::Result{"function", "another_function"}
                    << mi::Result{"pending", ""} // intentionally empty
                )
            }
        ;

        CHECK(msg.serialize() == "bkpts=[{proc_set=\"$a\",number=\"1\",type=\"breakpoint\",disp=\"keep\",enabled=\"y\",file=\"interpreter_mi_serialize.cpp\",line=\"42\",fullpath=\"/path/to/interpreter_mi_serialize.cpp\",addr=\"0x12345678\",function=\"main\"},{proc_set=\"$b\",number=\"2\",type=\"breakpoint\",disp=\"del\",enabled=\"n\",file=\"another_file.cpp\",line=\"100\",fullpath=\"/path/to/another_file.cpp\",addr=\"0x87654321\",function=\"another_function\",pending=\"\"}]");
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

//Value::Value(double val, int prec=16)
//    : data(std::make_shared<
    
std::string Value::doubleToString(double value, int prec) {
    std::ostringstream msg;
    msg << std::setprecision(prec);
    msg << value;
    return msg.str();
}

std::string Value::serialize() const {
    // we intentionally leave `data` as a public member of the struct, so it's
    // technically possible the user sets it to nullptr... for some reason
    if (!data) [[ unlikely ]] {
        return "";
    }

    return std::visit([](const auto& v) -> std::string {
        return v.serialize();
    }, *data);
}

std::string Const::serialize() const {
    std::ostringstream oss;
    oss << std::quoted(str);
    // the output of cmd_compare_cmd_mi contains "\n". std::quoted converts
    // this to \\n. This needs to be replaced by 
    // a single n for the output to have "\n" as it did using the 
    // deprecated method.
    std::string serialized = oss.str();
    serialized = std::regex_replace(serialized, std::regex(R"(\\n)"), "n");
    return serialized;
}

std::string Result::serialize() const {
    return key + "=" + value.serialize();
}

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

std::string List::serialize() const {
    std::string result = "[";
    bool first = true;
    for (const auto& item : contents) {
        if (!first) result += ",";
        first = false;
        result += item.serialize();
    }
    result += "]";
    return result;
}

Tuple& Tuple::operator<<(Result&& result) {
    contents.push_back(std::move(result));
    return *this;
}

std::string Tuple::serialize() const {
    std::string result = "{";
    bool first = true;
    for (const auto& item : contents) {
        if (!first) result += ",";
        first = false;
        result += item.serialize();
    }
    result += "}";
    return result;
}

MessageContents& MessageContents::operator<<(Result&& result) {
    results.push_back(result);
    return *this;
}

std::string MessageContents::serialize() const {
    std::string result;
    std::string sep = "";

    // Add results
    for (const auto& r : results) {
        result += sep;
        sep = ",";
        result += r.serialize();
    }

    return result;
}

} // namespace mi
