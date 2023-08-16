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

ConsoleInterpreter::ConsoleInterpreter()
    : m_console_commands(ConsoleCommandsFactory::make())
{}

std::string ConsoleInterpreter::complete_text(std::string_view line) {
    // Completion is unimplemented for this interpreter
    return std::string{line};
}

bool ConsoleInterpreter::can_interpret(std::string_view line) {
    auto find_result = find_command(line);
    return std::get_if<Command::Parts>(&find_result) != nullptr;
}

void ConsoleInterpreter::interpret(std::string_view line) {
    log("interpreting \"%.*s\"", (int)line.size(), line.data());

    auto find_result = find_command(line);

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

        if (!cmd->execute(procset, argv.argc(), const_cast<char**>(argv.c_strs().data()))) {
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

std::variant<Command::Parts, Command::ErrStr> ConsoleInterpreter::find_command(std::string_view line) {
    auto console_result = m_console_commands->findCmd(line);
    auto legacy_result = shim::GetCommands()->findCmd(line);
    auto* console_parts = std::get_if<Command::Parts>(&console_result);
    auto* legacy_parts = std::get_if<Command::Parts>(&legacy_result);

    if (console_parts && console_parts->command && legacy_parts && legacy_parts->command) {
        // the command matched something in both trees, so we have to make an
        // educated guess to which one the user meant.
        //
        // a command is most commonly found in both trees when the user is using
        // a single letter alias. for example, "n" will match "next" in the
        // console tree, but also "new-ui" in the legacy tree. "b" will match
        // "break" in the legacy tree and "backtrace" in the console tree. notice that in
        // these two examples, the correct choice is the console tree in one, and the
        // legacy tree in the other, so we can't just blindly favor one tree.
        //
        // we try to detect if a single letter alias was used and prefer that match. if
        // not, prefer the console tree.
        //
        // this logic can be removed when all commands are converted to the new tree.
        log("found in both trees (console: %s) (legacy: %s)",
            console_parts->command->getShortName().c_str(), legacy_parts->command->getShortName().c_str());

        auto first_word = line;
        // calling function already trimmed off leading spaces for us
        if (auto first_space = line.find_first_of(" \t\r\n"); first_space != std::string_view::npos) {
            first_word = line.substr(0, first_space);
        }

        log("first word is %.*s", (int)first_word.size(), first_word.data());

        if (console_parts->command->getAlias() && std::string_view{console_parts->command->getAlias()} == first_word) {
            log("matched console alias: %s (%s)", console_parts->command->getAlias(), console_parts->command->getShortName().c_str());
            return *console_parts;
        } else if (legacy_parts->command->getAlias() && std::string_view{legacy_parts->command->getAlias()} == first_word) {
            log("matched legacy alias: %s (%s)", legacy_parts->command->getAlias(), legacy_parts->command->getShortName().c_str());
            return *legacy_parts;
        } else {
            log("no alias, best guess is the console tree (%s)", console_parts->command->getShortName().data());
            return *console_parts;
        }
    } else if (console_parts && console_parts->command) {
        log("found in console tree (%s)", console_parts->command->getShortName().c_str());
        return *console_parts;
    } else if (legacy_parts && legacy_parts->command) {
        log("found in legacy tree (%s)", legacy_parts->command->getShortName().c_str());
        return *legacy_parts;
    }

    // TODO make this more specific about how many commands and sub commands were matched.
    // i.e. improve the error reporting of "Unknown command: set foo" to "Unknown command: foo"
    auto error = std::string{"Unknown command: "};
    error.append(line);
    return Command::ErrStr(std::move(error));
}

void ConsoleInterpreter::print_help(std::string_view line) {
    auto argv = ArgvStream{};
    argv.parseAndAppendArgs(line);

    // show all available help
    if (argv.argc() < 2) {
        struct Blurb { std::string name; const char* desc; };
        auto all_help_blurbs = std::vector<Blurb>{};

        // collect help text from legacy commands
        std::transform(shim::GetCommands()->getSubCmds().begin(),
                       shim::GetCommands()->getSubCmds().end(),
                       std::back_inserter(all_help_blurbs),
                       [](auto cmd_ptr){
                           return Blurb{.name = cmd_ptr->getShortName(),
                                        .desc = cmd_ptr->getDesc()};
                       });
        // collect help text from console commands
        std::transform(m_console_commands->getSubCmds().begin(),
                       m_console_commands->getSubCmds().end(),
                       std::back_inserter(all_help_blurbs),
                       [](auto cmd_ptr){
                           return Blurb{.name = cmd_ptr->getShortName(),
                                        .desc = cmd_ptr->getDesc()};
                       });

        // XXX: commands are already sorted in their trees, but since we're
        // combining two trees naively, we need to sort again. this can be
        // removed once we no longer have to gather commands from multiple
        // trees.
        std::sort(all_help_blurbs.begin(), all_help_blurbs.end(),
                  [](auto l, auto r){ return l.name < r.name; });

        for (auto [name, desc] : all_help_blurbs) {
            // we mark undocumented commands as undocumented by setting their description
            // to nullptr, e.g. the select-frame command.
            if (desc) {
                PrintOutMsg("%-15s %s\n", name.c_str(), desc);
            }
        }
        return;
    }

    // show specific help
    auto cmdStr = argv.asString(1);
    auto find_result = find_command(cmdStr);

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
