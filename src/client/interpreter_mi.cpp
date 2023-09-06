#include "interpreter_mi.h"
#include "str.h"

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

std::string_view MiInterpreter::name() const {
    return "mi";
}

std::string MiInterpreter::complete_text(std::string_view line) const {
    // Completion is unimplemented for this interpreter
    return std::string{line};
}

bool MiInterpreter::can_interpret(std::string_view line) const {
    auto find_result = m_mi_commands->findCmd(line);
    return std::get_if<Command::Parts>(&find_result) != nullptr
        || m_console_intr.can_interpret(line);
}

void MiInterpreter::interpret(std::string_view line) {
    line = stripped(line);
    log("interpreting \"%.*s\"", (int)line.size(), line.data());
    if (line.empty() || line.starts_with("#")) {
        return;
    }

    // consume tag, if present.
    auto untagged_line = line;
    if (!line.starts_with("-")) {
        auto tag = -1;
        auto result = std::from_chars(line.data(), line.data() + line.size(), tag);

        if (tag != -1) {
            log("got tag: %d", tag);
            // TODO use a different mechanism than cli_set_prefix to handle the tag
            cli_set_prefix(std::to_string(tag));
            untagged_line.remove_prefix(result.ptr - line.data());
            log("after tag, interpreting \"%.*s\"", (int)untagged_line.size(), untagged_line.data());
        }
    }

    auto find_result = m_mi_commands->findCmd(untagged_line);

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
            PrintOutOk();
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
    } else if (m_console_intr.can_interpret(untagged_line)) {
        log("forwarding to console interpreter");
        m_console_intr.interpret(untagged_line);
        // TODO print ^done/^running/^error/etc
        // TODO print (gdb)
    } else {
        log("unknown to both mi and console interpreter");
        static_assert(std::variant_size_v<decltype(find_result)> == 2, "Assuming there are only two alternatives");
        PrintOutError("%s\n", std::get<Command::ErrStr>(find_result).c_str());
    }
}

Command::RunContext MiInterpreter::make_context() {
    return Command::RunContext{
        .caller = this
    };
}

void MiInterpreter::print_help() {
    // this is not meant to be a user friendly help command, it's for developers/debugging
    for (auto scmd : m_mi_commands->getSubCmds()) {
        PrintOutMsg("%-30s %s\n", scmd->getShortName().c_str(), scmd->getDesc() ? scmd->getDesc() : "(no description given)");
    }
    return;
}
