// Copyright 2023 Hewlett Packard Enterprise Development LP.

#include "cmd.h"
#include "args.h"

#include <string_view>

namespace {

int cmd_set_interpreter(Command::RunContext& ctx, rec_set_t* _, int argc, const char* const argv[]) {
    // this argc value will change when "interpreter-set" is changed to "set interpreter"
    if (argc < 2) {
        PrintOutError("Usage: %s console|mi\n", argv[0]);
        return Command::Failure;
    }

    if (!DbgInterpreterSet(argv[1])) {
        PrintOutError("No such interpreter: %s\n", argv[1]);
        return Command::Failure;
    };

    return Command::Success;
}

int cmd_interpreter_exec(Command::RunContext& ctx, rec_set_t* _, int argc, const char* const argv[]) {
    if (argc < 3) {
        PrintOutError("Usage: %s console|mi <command>\n", argv[0]);
        return Command::Failure;
    }

    auto args = ArgvStream(argc, argv);

    const auto interpreter_name = args[1];
    const auto command = args.asString(2);

    if (!DbgInterpreterExec(interpreter_name, command)) {
        PrintOutError("No such interpreter: %s\n", interpreter_name.c_str());
        return Command::Failure;
    };

    return Command::Success; // TODO change this once DbgInterpret has a better return value
}

}

void ConsoleCommandsFactory::insert_interpreter_commands(Command& root) {
    // TODO write user docs (help, desc) for these commands
    root.addSubCmd({
        .name = "interpreter-set", .ptype = proc_ignore, .fn = cmd_set_interpreter});
    root.addSubCmd({
        .name = "interpreter-exec", .ptype = proc_ignore, .fn = cmd_interpreter_exec});
}

void MiCommandsFactory::insert_interpreter_commands(Command& root) {
    // TODO write user docs (help, desc) for these commands
    root.addSubCmd({
        .name = "-interpreter-set", .ptype = proc_ignore, .fn = cmd_set_interpreter});
    root.addSubCmd({
        .name = "-interpreter-exec", .ptype = proc_ignore, .fn = cmd_interpreter_exec});
}
