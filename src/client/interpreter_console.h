// Copyright 2023 Hewlett Packard Enterprise Development LP.
//
// Console Interpreter
//
// Interprets human readable commands and outputs human readable results
#pragma once

#include <memory>

#include "interpreter.h"
#include "cmd.h"

class ConsoleInterpreter : public Interpreter {
public:
    ConsoleInterpreter();
    ~ConsoleInterpreter() override = default;

    std::string complete_text(std::string_view line) override; // unimplemented
    bool can_interpret(std::string_view line) override;
    void interpret(std::string_view line) override;

private:
    std::variant<Command::Parts, Command::ErrStr> find_command(std::string_view line);
    void print_help(std::string_view line);

    std::shared_ptr<Command> m_console_commands;

};
