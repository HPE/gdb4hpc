// Copyright 2023 Hewlett Packard Enterprise Development LP.

#include "cmd.h"
#include "args.h"
#include "client.h"

#include <string_view>

namespace {

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

int interpreter_complete(int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Usage: maint complete ...<>\n");
        return Command::Failure;
    }

    auto command = ArgvStream(argc, argv).asString(1);
    auto cursor_pos = command.find("<>");

    if (cursor_pos == std::string::npos) {
        PrintOutError("Include \"<>\" where you want completion to happen from.\n");
        return Command::Failure;
    }

    auto cursor_word = command.rfind(" ", cursor_pos);
    if (cursor_word != std::string::npos) {
        cursor_word += 1;
    } else {
        cursor_word = 0;
    }

    command.erase(cursor_pos, 2);

    const auto comps = DbgComplete(std::string_view{&command[cursor_word], cursor_pos - cursor_word}, command);

    for (const auto& comp : comps.possibilities) {
        PrintOutMsg("%s\n", comp.c_str());
    }

    return Command::Success;
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
        .name = "interpreter-exec", .ptype = proc_ignore, .fn = interpreter_exec_console,
        .help = cmd_interpreter_exec_help,
        .desc = "Execute a command under a specific interpreter."});

    root.addSubCmd({
        .name = "maint complete", .ptype = proc_ignore, .fn = interpreter_complete,
        .help = nullptr, .desc = "Text command completion"});
}

void MiCommandsFactory::insert_interpreter_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "-interpreter-exec", .ptype = proc_ignore, .fn = interpreter_exec_mi});
}
