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

    // interpreter interface

    std::string_view name() const override;
    std::string complete_text(std::string_view line) const override; // unimplemented
    bool can_interpret(std::string_view line) const override;
    void interpret(std::string_view line) override;

    // console interpreter specific

    void push_commands(std::shared_ptr<Command> root);
    void pop_commands();

private:
    void print_help(std::string_view line);
    Command::RunContext make_context();

    // interpret will use the interpreter on the top of the stack. push/pop_commands
    // modify the stack and are called by functions that change the command set. examples
    // of such commands are gdbmode, decomp, and build.
    std::vector<std::shared_ptr<Command>> m_commands_stack;

};
