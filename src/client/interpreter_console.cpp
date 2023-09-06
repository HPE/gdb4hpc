// Copyright 2023 Hewlett Packard Enterprise Development LP.
#include "interpreter_console.h"

namespace {

void log(const char* fmt, ...) __attribute__((format (printf,1,2)));

void log(const char* fmt, ...) {
    static const auto enable_log = []() {
        return ::getenv("GDB4HPC_DEBUG_INTERPRETER") != nullptr;
    }();

    if (enable_log) {
        va_list args;
        va_start(args, fmt);

        char* message = nullptr;
        vasprintf(&message, fmt, args);
        va_end(args);

        fprintf(stderr, "%sconsole interpreter: %s%s\n", "\u001b[30;1m", message, "\033[0m");
        ::free(message);
    }
}

} // anonymous namespace

ConsoleInterpreter::ConsoleInterpreter() {
    m_commands_stack.emplace_back(ConsoleCommandsFactory::make());
}

std::string_view ConsoleInterpreter::name() const {
    return "console";
}

std::string ConsoleInterpreter::complete_text(std::string_view line) const {
    // Completion is unimplemented for this interpreter
    return std::string{line};
}

bool ConsoleInterpreter::can_interpret(std::string_view line) const {
    auto find_result = m_commands_stack.back()->findCmd(line);
    return std::get_if<Command::Parts>(&find_result) != nullptr;
}

void ConsoleInterpreter::interpret(std::string_view line) {
    line = stripped(line);
    log("interpreting \"%.*s\"", (int)line.size(), line.data());
    if (line.empty() || line.starts_with("#")) {
        return;
    }

    auto find_result = m_commands_stack.back()->findCmd(line);

    if (auto* parts = std::get_if<Command::Parts>(&find_result)) {
        auto& [cmd, args] = *parts;
        if (!cmd) {
            // this should never happen. if it does, it's a bug in gdb4hpc, not
            // user error. regardless, we still print an error message to give
            // the user feedback that something is wrong.
            log("found in tree, but no function pointer?");
            PrintOutError("Unknown command: %.*s\n", (int)line.length(), line.data());
            return;
        }

        // short circuit to help function. the command node in the
        // tree doesn't actually point to a command function.
        if (cmd->getShortName() == "help") {
            print_help(line);
            return;
        }

        auto* procset = CurrentProcSet();
        auto argv = ArgvStream{};
        argv.parseAndAppendArgs(cmd->getShortName() + " " + args);
        auto ctx = make_context();

        if (!cmd->execute(ctx, procset, argv.argc(), const_cast<char**>(argv.c_strs().data()))) {
            log("command returned an error");
            // XXX to maintain passing tests that specifically look for this behaviour,
            // set the stream error. but eventually this mechanism should be removed in
            // favor of plain old return values. it's actually only utilized in the file
            // input stream.
            cli_set_error();
        }
    } else {
        static_assert(std::variant_size_v<decltype(find_result)> == 2, "Assuming there are only two alternatives");
        PrintOutError("%s\n", std::get<Command::ErrStr>(find_result).c_str());
    }
}

void ConsoleInterpreter::push_commands(std::shared_ptr<Command> root) {
    m_commands_stack.push_back(root);
}

void ConsoleInterpreter::pop_commands() {
    if (m_commands_stack.size() > 1) {
        m_commands_stack.pop_back();
    } else {
        // XXX: if this happens, it's a bug, not user error. the error
        // message isn't very helpful, but there's not much that we *can*
        // communicate to the user other than "something is wrong"
        PrintOutError("Internal error: couldn't change commands.\n");
    }
}

Command::RunContext ConsoleInterpreter::make_context() {
    return {
        .caller = this
    };
}

void ConsoleInterpreter::print_help(std::string_view line) {
    auto argv = ArgvStream{};
    argv.parseAndAppendArgs(line);

    // show all available help
    if (argv.argc() < 2) {
        for (auto cmd : m_commands_stack.back()->getSubCmds()) {
            // we mark undocumented commands as undocumented by setting their description
            // to nullptr, e.g. the select-frame command.
            if (cmd && cmd->getDesc()) {
                PrintOutMsg("%-15s %s\n", cmd->getShortName().c_str(), cmd->getDesc());
            }
        }
        return;
    }

    // show specific help
    auto cmdStr = argv.asString(1);
    auto find_result = m_commands_stack.back()->findCmd(cmdStr);

    if (std::holds_alternative<Command::Parts>(find_result)) {
        auto [cmd, argv] = std::get<Command::Parts>(find_result);
        if (cmd && cmd->getHelp()) {
            PrintHelpEntry(cmd->getHelp());
        } else if (cmd) {
            PrintOutError("No help for %s\n", cmdStr.c_str());
        }

        if (cmd->getSubCmds().size() > 0) {
            PrintOutMsg("Subcommands:\n\n");
            for (auto subcmd : cmd->getSubCmds()) {
                PrintOutMsg("%-20s %s\n", subcmd->getShortName().c_str(), subcmd->getDesc());
            }
        }
    } else {
        static_assert(std::variant_size_v<decltype(find_result)> == 2, "Assuming there are only two alternatives");
        PrintOutError("%s\n", std::get<Command::ErrStr>(find_result).c_str());
    }
}
