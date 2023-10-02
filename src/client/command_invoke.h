// Copyright 2023 Hewlett Packard Enterprise Development LP.
#pragma once

#include "interpreter.h"

#include <functional>
#include <memory>

// The Command class holds a callable command. They are created from any
// callable object that fits the general mold of
// int(Interpreter&, rec_set_t*, int, const char* const[]), or a subset of that
// signature, e.g. int(int, const char* const[])
//
// HISTORY:
//
// In previous versions of gdb4hpc, a Command was just a function pointer.
// The main problem with this approach was that it meant that every command had
// to have the same function signature. In reality, not all commands are created
// equal. For example, many show commands don't use any of their arguments and
// simply fetch some data, display it, and return.
//
// This limitation was easy enough to deal with by just ignoring the
// arguments in the command implementation. However, once Interpreters were
// implemented, this limitation reached a tipping point. For example, take
// cmd_directmode_cmd (in cmd_passthru.cpp). This is the command that gets
// executed when the user types "gdbmode" with no arguments. The command is only
// meaningful with the ConsoleInterpreter.
//
// This meant that for every command implementation to have the same API, we
// needed to introduce an Interpreter& base class argument. For commands
// only meaningful to certain interpreters, that meant either 1. bloating the
// Interpreter API with every possible function that can be called with no-op
// defaults, or 2. doing a dynamic_cast at the top of every command
// implementation and returning early if it fails.
//
// The Command class was designed to solve this problem by being a drop in
// replacement for the old function pointers while also letting implementation
// code define specific APIs to avoid the problems mentioned above. The Command
// class itself will still accept a full invocation of
// int(Interpreter&, rec_set_t*, argc, argv), called from the interpreter level.
// However, before forwarding the call to the command implentation, it will
// filter it down to only the context that the command implementation wants. The
// Command deduces the desired command implementation API at construction time.
// e.g.
//
//   Command fn = cmd_directmode_cmd;
//
// Will create a command which wraps cmd_directmode_cmd, and the interpreter's
// call to
//
//   fn(Interpreter&, rec_set_t*, argc, argv)
//
// will be filtered down to
//
//   fn(ConsoleInterpreter&, rec_set_t*, argc, argv)
//
// similarly, a call to cmd_set_print_pretty will be filtered down to
//
//   fn(rec_set_t*, argc, argv)
//
// since that command doesn't need an interpreter at all. Some commands are so
// simple that they take zero arguments.
//
// The Command class also supports the notion of a nullptr command to support
// the old function pointer syntax. Null commands are most often used in
// CommandNodes in the middle of a command tree, where the purpose of the node
// is not to run a command, but to parse the command line to find the leaf node
// with the actual command. For example, the string "set print pretty on" has
// the nodes:
//
//   set -> print -> pretty
//
// Where set and print are null command nodes.
//
// See the .cpp file. for tests with examples of usage and a list of all
// supported function signatures.

class rec_set_t;

namespace detail {

// helpers to wrap a callable object in a std::function, with a special case for nullptr
template <typename T> auto get_fn(T &&t) {
    return std::function{std::forward<T>(t)};
}
template <> auto inline get_fn(std::nullptr_t &&) {
    return std::function<void()>{nullptr};
}

} // namespace detail

class Command {
public:
    // function implementations return int for legacy reasons.
    // they should prefer these for better readability.
    static constexpr auto Success = 1;
    static constexpr auto Failure = 0;

    // create a command wrapping any function-like object that fits the general
    // mold of int(Interpreter&, rec_set_t*, int, const char* const[]), or
    // nullptr. the wrapped command will be called with the generic function
    // signature, but will be filtered/casted at runtime to match the
    // destination signature.
    template <typename AnyFunction>
    Command(AnyFunction fn)
        // std::make_shared can't do the template deductions, so we're stuck with new here
        : m_impl{new Impl(detail::get_fn(std::move(fn)))}
    {}

    // call the implementation and return its result. arguments will be
    // filtered/casted to match the implementation function. if the arguments
    // can't be made to match, returns Failure.
    int operator()(Interpreter &intr, rec_set_t* rec_set, int argc, const char* const argv[]) {
        return m_impl->call(intr, rec_set, argc, argv);
    }

    // enables `if (my_command) { ... }`
    operator bool() const {
        return m_impl->to_bool();
    }
private:
    struct Interface {
        virtual ~Interface() = default;
        virtual int call(Interpreter &intr, rec_set_t* rec_set, int argc, const char* const argv[]) const = 0;
        virtual bool to_bool() const = 0;
    };

    template <typename R, typename... Args>
    struct Impl final : Interface {
        Impl(std::function<R(Args...)>&& fn) : fn{std::move(fn)} {}
        virtual int call(Interpreter &intr, rec_set_t* rec_set, int argc, const char* const argv[]) const override;
        virtual bool to_bool() const override {
            return static_cast<bool>(fn);
        }
        std::function<R(Args...)> fn;
    };

    std::shared_ptr<Interface> m_impl;
};
