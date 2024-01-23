// Copyright 2023 Hewlett Packard Enterprise Development LP.
#include "interpreter.h"

// available interpreters are "console" and "mi".
#include "interpreter_console.h"
#include "interpreter_mi.h"

#include <ranges>

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

std::string Interpreter::get_prompt() const {
    return "";
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

using Completions = Interpreter::Completions;

std::string Completions::calc_common_prefix() const {
    if (this->possibilities.size() == 0) return "";

    auto prefix = std::string_view{this->possibilities[0]};

    // skipping 1st since we don't have to compare the first word with itself
    for (std::string_view poss : this->possibilities | std::views::drop(1)) {
        auto c = size_t{0};
        while (c < prefix.size() && c < poss.size() && prefix[c] == poss[c]) c++;
        if (c != prefix.size()) prefix = prefix.substr(0, c);
    }

    return std::string{prefix};
}
