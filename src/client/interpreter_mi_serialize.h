// Copyright 2025 Hewlett Packard Enterprise Development LP.
//
// Helper classes and functions for generating mi message output

#pragma once

#include <string>
#include <variant>
#include <memory>
#include <vector>

namespace mi {

struct Value;
struct Const;
struct Result;
struct List;
struct Tuple;

struct Const {
    // in gdbmi, a "Const" is a string literal and is the base data type that
    // all other types are composed of.

    std::string str;

    // serialized as '"<str>"'. the function handles escaping
    // quotes etc, so `str` should be stored as an unescaped string.
    std::string serialize() const;
};

struct Value {
    // base data object in gdbmi. can be a string, a result, a list, or a tuple.
    // see the classes for explanations of what each of those are in the gdb mi format.
    using Variant = std::variant<Const, Result, List, Tuple>;

    Value(Const str);

    // helpers for Const
    Value(std::string str) : Value(Const{str}) {}
    Value(std::string_view str) : Value(Const{std::string{str}}) {}
    Value(const char* str) : Value(str ? std::string{str} : "") {}
    Value(double val, int prec=16) : Value(Const{doubleToString(val, prec)}) {}

    Value(Result result);
    Value(List list);
    Value(Tuple tuple);

    // const to enforce immutability, which makes for a simple api. if we ever
    // find ourselves needing to modify a value after its creation, we can
    // revisit this.
    std::shared_ptr<const Variant> data;

    // serialize the underlying value
    std::string serialize() const;
    std::string doubleToString(double value, int prec=16);
};

struct Result {
    // in gdbmi, a "Result" in the context of the output grammar
    // is just a key value pair, not a full mi response (a full response is a list
    // of zero or more Results).

    std::string key;
    Value value;

    // serialize as 'key=<value>'.
    std::string serialize() const;
};

struct List {
    // a List is a collection of arbitrary Values.

    std::vector<Value> contents;

    // build up the list contents with <<.
    // see unit tests in interpreter_mi_serialize.cpp for examples.
    List& operator<<(Const&& str);
    List& operator<<(Result&& result);
    List& operator<<(List&& list);
    List& operator<<(Tuple&& tuple);
    List& operator<<(Value&& value);

    // serialize as "[<value>, <value>, ...]"
    std::string serialize() const;
};

struct Tuple {
    // a Tuple is like a List that *only* stores Results. when serialized, its
    // contents are surrounded by braces "{...}"

    std::vector<Result> contents;

    // build up the tuple contents with <<.
    // see unit tests in interpreter_mi_serialize.cpp for examples.
    Tuple& operator<<(Result&& result);

    // serialize as "{<result>, <result>, ...}"
    std::string serialize() const;
};

struct MessageContents {
    // MessageContents contains the contents of an MI response.

    // captital R Results are the only record that can appear at the top level of a
    // gdb mi message which is why the API is limited to that.
    std::vector<Result> results;

    // helper to build up the message contents with <<,
    // but manually editing `results` is valid too.
    // see unit tests in interpreter_mi_serialize.cpp for examples.
    MessageContents& operator<<(Result&& result);

    // serialize the contents into a string.
    //
    // note that we don't handle the token prefix (the 38 in "38^done")
    // or the message class (^done, ^running, *stopped, etc).
    // those are handled internally by the MiInterpreter,
    // hence "MessageContents" rather than "Message"
    std::string serialize() const;
};

} // namespace mi
