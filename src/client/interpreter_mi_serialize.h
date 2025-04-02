// Copyright 2025 Hewlett Packard Enterprise Development LP.
//
// Helper classes and functions for generating mi message output

#pragma once

#include <string>
#include <variant>
#include <memory>
#include <vector>

namespace mi {

// this slightly strange nomenclature matches the gdbmi spec's grammar
struct Value;
struct Result;
struct List;
struct Tuple;

// in gdbmi, a "Const" is quoted string literal. the serialization functions
// handle escaping quotes etc, so here they should be stored as plain strings.
using Const = std::string;

struct Value {
    // base data object in gdbmi. can be a string, a result, a list, or a tuple.
    // see the classes for explanations of what each of those are in the gdb mi format.
    using Variant = std::variant<Const, Result, List, Tuple>;

    Value(Const str);
    Value(Result result);
    Value(List list);
    Value(Tuple tuple);

    // const to enforce immutability, which makes for a simple api. if we ever
    // find ourselves needing to modify a value after its creation, we can
    // revisit this.
    std::shared_ptr<const Variant> data;
};

struct Result {
    // in gdbmi, a "Result" in the context of the output grammar
    // is just a key value pair, not a full mi result. when serialized,
    // it is output as 'key=<value>'.

    std::string key;
    Value value;
};

struct List {
    // a List is a collection of arbitrary Values. when serialized, its
    // contents are surrounded by brackets "[...]"

    std::vector<Value> contents;

    // build up the tuple contents with <<.
    // see unit tests in interpreter_mi_serialize.cpp for examples.
    List& operator<<(Const&& str);
    List& operator<<(Result&& result);
    List& operator<<(List&& list);
    List& operator<<(Tuple&& tuple);
    List& operator<<(Value&& value);
};

struct Tuple {
    // a Tuple is like a List that *only* stores Results. when serialized, its
    // contents are surrounded by braces "{...}"

    std::vector<Result> contents;

    // build up the tuple contents with <<.
    // see unit tests in interpreter_mi_serialize.cpp for examples.
    //
    // tuples *only* store Results, hence the limited API compared with List
    Tuple& operator<<(Result&& result);
};

class MessageContents {
public:
    // contents of an MI result message (async messages not covered here)
    // note that we don't handle the token prefix (the 38 in "38^done") in this class
    // or the message class (^done, ^running, etc) those are handled internally
    // by the MiInterpreter, hence Message*Contents*
    //
    // but maybe they could be moved here eventually

    // build up the message contents with <<.
    // see unit tests in interpreter_mi_serialize.cpp for examples.
    //
    // Results are the only record that can appear at the "top level" of a
    // gdb mi message which is why the API is limited to that.
    MessageContents& operator<<(Result&& result);

    std::string serialize() const;

private:
    std::vector<Value> m_data;
};

} // namespace mi
