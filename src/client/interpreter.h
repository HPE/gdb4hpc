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
        // Which events are sent are interpreter specific.
        bool async = false;
    };
    static std::shared_ptr<Interpreter>
        make(std::string_view name, cli_stream_t& io_stream, MakeOpts&& opts);

    // name returns the name of the interpreter as it would be typed by a user in the
    // interpreter-exec command.
    virtual std::string_view name() const = 0;

    // Given `text`, complete_text tries to transform it into a string that the
    // interpreter understands. If that isn't possible, it returns a copy of
    // `text`.
    //
    // This function is currently not implemented by any interpreter.
    //
    // TODO making a copy of `text` on failure seems a little wasteful. consider
    // changing this api to return a string_view which would save the copy on
    // the failure case and let the caller decide when to copy on the success case,
    // or maybe just a std::optional<std::string>.
    virtual std::string complete_text(std::string_view text) const = 0;

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
