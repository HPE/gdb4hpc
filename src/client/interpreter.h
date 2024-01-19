// Copyright 2023 Hewlett Packard Enterprise Development LP.
//
// Interpreter interface
#pragma once

#include <stdarg.h>
#include <string_view>
#include <memory>

#include "dbginvoke.h"

struct cli_stream_t;

class Interpreter {
public:
    // make returns a pointer to a newly created interpreter implementation.
    //
    // Only certain `name`s will return an implementation - if `name` doesn't refer to an
    // interpreter implementation, make will return nullptr. See interpreter.cpp for
    // available interpreters.
    //
    // Interpreters are tightly coupled with cli_stream_t, and should be thought of as
    // part of the owning stream instead of as an independent entity. `io_stream` is the
    // owning stream. After interpreting text or an async event (see `opt` below), the
    // interpreter will emit output to `io_stream`.
    //
    // `opts` specifies additional options for the created interpreter.
    struct MakeOpts {
        // If async is true, and the interpreter implementation supports it, async
        // debugger events will be sent to the interpreter for it to handle and/or print.
        // Which events are sent are interpreter specific. This option exists to support
        // interpreters that have other silent interpreters as implementation details. See
        // #IgnoreAsync in client_io.md.
        bool async = false;
    };
    static std::shared_ptr<Interpreter>
        make(std::string_view name, cli_stream_t& io_stream, MakeOpts&& opts);

    // name returns the name of the interpreter as it would be typed by a user in the
    // interpreter-exec command.
    virtual std::string_view name() const = 0;

    // Given a `partial_word` in `buffer`, complete_text returns possible
    // Completions of the `partial_word`. If no completions are possible,
    // complete_text returns an empty Completions object.
    //
    // `partial_word` must be contained within `buffer`. i.e. the implementation
    // is allowed to compare memory addresses of `partial_word` with `buffer`.
    class Completions;
    virtual Completions complete_text(std::string_view partial_word, std::string_view buffer) const = 0;

    // Given `text`, can_interpret returns true if it would be appropriate to
    // call interpret(`text`).
    virtual bool can_interpret(std::string_view text) const = 0;

    // Given `text`, interpret the text and interface with gdb4hpc. Returns true if the
    // text was successfully interpreted with no errors.
    virtual bool interpret(std::string_view text) = 0;

    virtual ~Interpreter() = default;

    // a Printer is an interpreter's interface for formatting and delivering output. It is
    // the API that connects functions in cli_funcs.cpp to the interpreter for output
    // formatting.
    //
    // The *only* supported ways to access an interpreter's printer is through:
    //
    // - PrintOutMsg
    // - PrintOutError
    // - ProgOutLine
    //
    // Much like how the Interpreter is highly coupled to an owning cli_stream_t and can
    // be thought of as part of the owning stream rather than an independent entity, a
    // Printer is highly coupled to an Interpreter can can be thought of as part of the
    // Interpreter itself.
    class Printer {
    public:
        // print_message receives data from PrintOutMsg. The received data is a plaintext
        // message to be delivered to the user. print_message is free to format the data
        // as it needs before sending it to the owning stream.
        //
        // Note that print_message is marked final - it simply forwards to print_message_v.
        //
        // get_message_stream returns an std::ostream that will forward streamed data to
        // print_message for formatting.
        virtual void print_message(const char* fmt, ...) final __attribute__((format (printf,2,3)));
        virtual void print_message_v(const char* fmt, va_list args) = 0;
        virtual std::ostream& get_message_stream() = 0;

        // print_error receives data from PrintOutError. The received data is a plaintext
        // error message. print_error is free to format the data as it needs before
        // sending it to the owning stream.
        //
        // Note that print_error is marked final - it simply forwards to print_error_v
        virtual void print_error(const char* fmt, ...) final __attribute__((format (printf,2,3)));
        virtual void print_error_v(const char* fmt, va_list args) = 0;

        // print_inferior_output receives data from ProgOutLine. The received data is the
        // raw output bytes from the inferior, one `line` at a time. No newlines are
        // removed. print_inferior_output is free to format the data as it needs before
        // sending it to the owning stream.
        //
        // `di` is the DbgInvoke that the output came from.
        //
        // If `use_instead` is not null, the printer will send the formatted output to
        // `use_instead` and not to the owning stream. This is support for the
        // implementation of the "set inferior tty" command.
        //
        // Note that ProgOutLine is not a publicly exported function from cli_funcs.cpp.
        // It's called by setting program output callbacks on the dbgclnt proto.
        virtual void print_inferior_output(const DbgInvoke& di, std::string_view line, FILE* use_instead = nullptr) = 0;

        virtual ~Printer() = default;
    };
private:
    // Connection to print functions in cli_funcs.cpp.
    friend Printer& cli_get_current_printer();

    // get_printer returns a printer that is connected to the interpreter (and thus
    // connected to the interpreter's owning stream).
    //
    // get_printer is intentionally private and only accessible from cli_funcs.
    virtual Printer& get_printer() = 0;
};

struct Interpreter::Completions {
    // a representation of completion possibilities for a partially typed-in interpreter command.

    // `calc_common_prefix` calculates and returns the longest string of
    // characters common among all `possibilities`.
    //
    // if there are no possibilities in the list, this function returns "".
    std::string calc_common_prefix() const;

    // `possibilities` can and should be modified from the outside.
    std::vector<std::string> possibilities;

    // if set to true, the function consuming the Completions should fall back to
    // completing the partially typed string with filesystem files/directories if
    // `possibilities` is empty.
    bool include_filesystem = false;
};
