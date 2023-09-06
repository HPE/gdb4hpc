// Copyright 2023 Hewlett Packard Enterprise Development LP.
//
// Mi Interpreter
//
// Interprets machine readable commands and outputs machine readable results
// formatted to the GDB/MI standard with extensions for ccdb
#pragma once

#include <memory>

#include "interpreter.h"
#include "interpreter_console.h"
#include "cmd.h"

class MiInterpreter : public Interpreter {
public:
    MiInterpreter();
    ~MiInterpreter() override = default;

    std::string_view name() const override;
    std::string complete_text(std::string_view line) const override; // unimplemented
    bool can_interpret(std::string_view line) const override;
    void interpret(std::string_view line) override;

    // XXX: the public member variables here used to be global. the usage pattern should
    // be improved, but just preserving old behavior for now.

    std::string m_launcher_args;
    std::string m_num_ranks;
    std::string m_procset;

private:
    Command::RunContext make_context();
    void print_help();

    std::shared_ptr<Command> m_mi_commands;
    ConsoleInterpreter m_console_intr;
};
