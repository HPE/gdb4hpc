// Copyright 2023 Hewlett Packard Enterprise Development LP.
#include "interpreter.h"

// available interpreters are "console" and "mi".
#include "interpreter_console.h"
#include "interpreter_mi.h"

std::shared_ptr<Interpreter> Interpreter::make(std::string_view name, cli_stream_t& io, MakeOpts&& opts) {
    if (name == "console") {
        auto intr = std::make_shared<ConsoleInterpreter>(io);
        if (opts.async) intr->attach_observers();
        return intr;
    }

    if (name == "mi") {
        auto intr = std::make_shared<MiInterpreter>(io);
        if (opts.async) intr->attach_observers();
        return intr;
    }

    return nullptr;
}

void Interpreter::Printer::print_message(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_message_v(fmt, args);
    va_end(args);
}

void Interpreter::Printer::print_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_error_v(fmt, args);
    va_end(args);
}

