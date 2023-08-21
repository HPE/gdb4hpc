// Copyright 2023 Hewlett Packard Enterprise Development LP.
#include "interpreter.h"

#include "interpreter_console.h"
#include "interpreter_mi.h"

// available interpreters are "console" and "mi".

std::shared_ptr<Interpreter> Interpreter::make(std::string_view name) {
    if (name == "console") return std::make_shared<ConsoleInterpreter>();
    if (name == "mi") return std::make_shared<MiInterpreter>();
    return nullptr;
}
