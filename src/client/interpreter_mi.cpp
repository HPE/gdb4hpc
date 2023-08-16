#include "interpreter_mi.h"

#include <charconv>

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

        fprintf(stderr, "%smi interpreter: %s%s\n", "\u001b[30;1m", message, "\033[0m");
        ::free(message);
    }
}

} // anonymous namespace

MiInterpreter::MiInterpreter()
    : m_mi_commands(MiCommandsFactory::make())
    , m_console_intr()
{}

std::string MiInterpreter::complete_text(std::string_view line) {
    // Completion is unimplemented for this interpreter
    return std::string{line};
}

bool MiInterpreter::can_interpret(std::string_view line) {
    auto find_result = m_mi_commands->findCmd(line);
    return std::get_if<Command::Parts>(&find_result) != nullptr
        || m_console_intr.can_interpret(line);
}

void MiInterpreter::interpret(std::string_view line) {
    if (auto first = line.find_first_not_of(" \n\r\t"); first != std::string_view::npos) {
        line.remove_prefix(first);
    }
    if (auto last = line.find_last_not_of(" \n\r\t"); last != std::string_view::npos) {
        line.remove_suffix(line.size() - (last + 1));
    }

    log("interpreting \"%.*s\"", (int)line.size(), line.data());

    // consume tag, if present.
    if (!line.starts_with("-")) {
        auto tag = -1;
        auto result = std::from_chars(line.data(), line.data() + line.size(), tag);

        if (tag != -1) {
            log("got tag: %d", tag);
            // TODO use a different mechanism than cli_set_prefix to handle the tag
            cli_set_prefix(std::to_string(tag));
            line.remove_prefix(result.ptr - line.data());
            log("after tag, interpreting \"%.*s\"", (int)line.size(), line.data());
        }
    }

    auto find_result = m_mi_commands->findCmd(line);

    if (auto* parts = std::get_if<Command::Parts>(&find_result)) {
        auto& [cmd, args] = *parts;
        if (!cmd) {
            log("found in tree, but no function pointer?");
            return;
        }

        // short circuit to help function. the command node in the
        // tree doesn't actually point to a command function.
        if (cmd->getShortName() == "-help") {
            print_help();
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
    } else if (m_console_intr.can_interpret(line)) {
        log("forwarding to console interpreter");
        m_console_intr.interpret(line);
    } else {
        // fall back to execute command. even if we aren't deaing with a console
        // command, we still might have a valid command here. some unconverted
        // mi commands, e.g. "-break-after", won't show up in any command tree
        // until they've been transformed by ExecuteCommand.
        //
        // XXX: there is no guarantee that .data() is valid here. the
        // string_view might not be null terminated. in practice, that is never
        // a problem though.
        log("falling back to ExecuteCommand");
        ExecuteCommand(line.data());
    }
}

void MiInterpreter::print_help() {
    // this is not meant to be a user friendly help command, it's for developers/debugging
    for (auto scmd : m_mi_commands->getSubCmds()) {
        PrintOutMsg("%-30s %s\n", scmd->getShortName().c_str(), scmd->getDesc() ? scmd->getDesc() : "(no description given)");
    }
    return;
}
