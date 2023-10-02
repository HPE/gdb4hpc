// Copyright 2023 Hewlett Packard Enterprise Development LP.

#include "cmd.h"
#include "args.h"
#include "client.h"

#include <string_view>

namespace {

int cmd_set_interpreter(int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Usage: maint set interpreter console|mi\n");
        return Command::Failure;
    }

    if (!DbgInterpreterSet(argv[1])) {
        PrintOutError("No such interpreter: %s\n", argv[1]);
        return Command::Failure;
    };

    return Command::Success;
}

int interpreter_exec_console(int argc, const char* const argv[]) {
    if (argc < 3) {
        PrintOutError("Usage: %s console|mi <command>\n", argv[0]);
        return Command::Failure;
    }

    auto args = ArgvStream(argc, argv);

    const auto interpreter_name = args[1];
    const auto command = args.asString(2);

    // when running interpreter-exec from the console, we want to swap the
    // current stream so that output is printed through the sub interpreter.
    if (!DbgInterpreterSet(interpreter_name))
        return Command::Failure; // error already printed

    // all error messages handled in DbgInterpreterExec
    auto result = DbgInterpreterExec(interpreter_name, command) ? Command::Success : Command::Failure;
    DbgInterpreterSet("console"); // blindly assuming it won't fail

    return result;
}

int interpreter_exec_mi(int argc, const char* const argv[]) {
    if (argc < 3) {
        PrintOutError("Usage: %s console|mi <command>\n", argv[0]);
        return Command::Failure;
    }

    auto args = ArgvStream(argc, argv);

    const auto interpreter_name = args[1];
    const auto command = args.asString(2);

    // unlike the console version above, we *don't* change the global
    // interpreter for mi. this keeps the output going through the mi
    // interpreter.

    // all error messages handled in DbgInterpreterExec
    return DbgInterpreterExec(interpreter_name, command) ? Command::Success : Command::Failure;
}

}

void ConsoleCommandsFactory::insert_interpreter_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "maint set interpreter", .ptype = proc_ignore, .fn = cmd_set_interpreter,
        .help = cmd_maint_set_interpreter_help,
        .desc = "Set the current input/output interpreter."});
    root.addSubCmd({
        .name = "interpreter-exec", .ptype = proc_ignore, .fn = interpreter_exec_console,
        .help = cmd_interpreter_exec_help,
        .desc = "Execute a command under a specific interpreter."});
}

void MiCommandsFactory::insert_interpreter_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "-interpreter-set", .ptype = proc_ignore, .fn = cmd_set_interpreter});
    root.addSubCmd({
        .name = "-interpreter-exec", .ptype = proc_ignore, .fn = interpreter_exec_mi});
}
