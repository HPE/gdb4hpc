// Copyright 2023 Hewlett Packard Enterprise Development LP.
#include "interpreter_console.h"

#include "str.h"
#include "client_settings.h"
#include "cmd.h"
#include "bttree.h"

using namespace std::string_literals;
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

ConsoleInterpreter::ConsoleInterpreter(cli_stream_t& io)
    : m_printer(*this, io)
{
    m_commands_stack.emplace_back(ConsoleInterpreter::CommandTree{ ConsoleCommandsFactory::make(), "" });
}

std::string_view ConsoleInterpreter::name() const {
    return "console";
}

bool ConsoleInterpreter::can_interpret(std::string_view line) const {
    auto find_result = m_commands_stack.back().tree->findCmd(line);
    return std::get_if<CommandNode::Parts>(&find_result) != nullptr;
}

bool ConsoleInterpreter::interpret(std::string_view line) {
    line = stripped(line);
    log("interpreting \"%.*s\"", (int)line.size(), line.data());
    if (line.empty() || line.starts_with("#")) {
        return true;
    }

    auto find_result = m_commands_stack.back().tree->findCmd(line);

    if (auto* parts = std::get_if<CommandNode::Parts>(&find_result)) {
        auto& [cmd, args] = *parts;
        if (!cmd) {
            // this should never happen. if it does, it's a bug in gdb4hpc, not
            // user error. regardless, we still print an error message to give
            // the user feedback that something is wrong.
            log("found in tree, but no function pointer?");
            PrintOutError("Unknown command: %.*s\n", (int)line.length(), line.data());
            return false;
        }

        // short circuit to help function. the command node in the
        // tree doesn't actually point to a command function.
        if (cmd->getShortName() == "help") {
            return print_help(line);
        }

        auto* procset = CurrentProcSet();
        auto argv = ArgvStream{};
        argv.parseAndAppendArgs(cmd->getShortName() + " " + args);

        if (cmd->execute(*this, procset, argv.argc(), argv.c_strs().data())) {
            log("command successful");
            return true;
        } else {
            log("command returned an error");
            return false;
        }
    } else {
        static_assert(std::variant_size_v<decltype(find_result)> == 2, "Assuming there are only two alternatives");
        const auto err_str = std::get<CommandNode::ErrStr>(find_result);
        PrintOutError("%s\n", err_str.c_str());
        return false;
    }
}

Interpreter::Printer& ConsoleInterpreter::get_printer() {
    return m_printer;
}

void ConsoleInterpreter::push_commands(std::shared_ptr<CommandNode> root) {
    m_commands_stack.emplace_back(CommandTree{ root, "" });
}

void ConsoleInterpreter::push_commands(std::shared_ptr<CommandNode> root, std::string_view prompt) {
    m_commands_stack.emplace_back(CommandTree{ root, std::string{prompt} });
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

std::string ConsoleInterpreter::get_prompt() const {
    const auto prompt = m_commands_stack.back().prompt;
    if (!prompt.empty()) return prompt;

    const auto prefix = [&](){
        bool any_python = false;
        for (const auto* oset : CurrentProcSet()->s_over_sets) {
            // being extra paranoid since this function runs after every single command
            if (!oset || !oset->s_di_bs) continue;

            // di.lock() might be null if current procset contains expired di
            if (const auto* di = oset->s_di.lock()) {
                any_python |= oset->s_di_bs->hasCommonBits(*di->getPythonBitset());
            }
        }
        return std::string{any_python ? "python " : "dbg "};
    }();

    if (const auto* current_procset = CurrentProcSet()) {
        return prefix + current_procset->s_name + "> ";
    }

    // should never get here because current_procset should never be null,
    // but let's not segfault in the prompt function of all places if it is.
    return "dbg >";
}

void ConsoleInterpreter::attach_observers() {
    m_obsv_conns.emplace_back(
        observers::async_event.attach(
            [this](DbgInvoke::EventSet& ev_set, const Bitset& reply_set) {
                log("observing async_event");
                return handle_async_event(ev_set, reply_set);
            }
        )
    );
}

bool ConsoleInterpreter::print_help(std::string_view line) {
    auto argv = ArgvStream{};
    argv.parseAndAppendArgs(line);

    // show all available help
    if (argv.argc() < 2) {
        for (auto cmd : m_commands_stack.back().tree->getSubCmds()) {
            // we mark undocumented commands as undocumented by setting their description
            // to nullptr, e.g. the select-frame command.
            if (cmd && cmd->getDesc()) {
                PrintOutMsg("%-16s %s\n", cmd->getShortName().c_str(), cmd->getDesc());
            }
        }
        return true;
    }

    // show specific help
    auto cmdStr = argv.asString(1);
    auto find_result = m_commands_stack.back().tree->findCmd(cmdStr);

    if (std::holds_alternative<CommandNode::Parts>(find_result)) {
        auto [cmd, argv] = std::get<CommandNode::Parts>(find_result);
        if (cmd && cmd->getHelp()) {
            PrintHelpEntry(cmd->getHelp());
        } else if (cmd) {
            PrintOutMsg("No help for %s\n", cmdStr.c_str());
        }

        if (cmd->getSubCmds().size() > 0) {
            PrintOutMsg("Subcommands:\n\n");
            for (auto subcmd : cmd->getSubCmds()) {
                PrintOutMsg("%-20s %s\n", subcmd->getShortName().c_str(), subcmd->getDesc());
            }
        }

        return true;
    } else {
        static_assert(std::variant_size_v<decltype(find_result)> == 2, "Assuming there are only two alternatives");
        const auto err_str = std::get<CommandNode::ErrStr>(find_result);
        PrintOutError("%s\n", err_str.c_str());
        return false;
    }
}

// we need to implement PrintErrorWithSetInfo etc. ourselves here, because
// PrintErrorWithSetInfo will use PrintOutMsg to print its messages, which will
// go out through the current stream, which is not guaranteed to be our printer.

void ConsoleInterpreter::Printer::print_with_set_info(const char* set_name, const Bitset& group, const char* fmt, ...) {
    const auto prefix = GetProcSetString(set_name, &group, nullptr);
    print_message("%s: ", prefix.c_str());

    va_list args;
    va_start(args, fmt);
    print_message_v(fmt, args);
    va_end(args);
}

void ConsoleInterpreter::Printer::print_error_with_set_info(const char* set_name, const Bitset& group, const char* fmt, ...) {
    const auto prefix = GetProcSetString(set_name, &group, nullptr);
    print_error("%s: ", prefix.c_str());

    va_list args;
    va_start(args, fmt);
    print_error_v(fmt, args);
    va_end(args);
}

// Prints program location after a step event is received
void ConsoleInterpreter::Printer::print_event_location(const DbgInvoke::EventSet &ev_set) {
    if (!ev_set.event()) {
        return;
    }

    const dbgframe_t *frame = ev_set.event()->ev_step.step_frame;
    bool frame_changed = ev_set.event()->ev_step.step_frame_changed;

    if (frame) {
        const auto &frameLoc = frame->getLoc();

        // Print location info
        if (frameLoc.loc_file && frameLoc.loc_func && frameLoc.loc_line > 0) {
            // See if we can get the source line

            auto lineArg = std::string{frameLoc.loc_file} + ":" + std::to_string(frameLoc.loc_line);
            lineArg += "," + lineArg; // filename:xx,filename:xx
            dbgevent_t *listResult = nullptr;

            if (auto fe = ev_set.di().frontendGdb()) {
                listResult = fe->list(nullptr, lineArg.c_str());
            }

            if (listResult && listResult->ev_event == DBGEV_LIST &&
                listResult->ev_list && listResult->ev_list->list_lno == frameLoc.loc_line) {
                // Print a frame if the last step event changed functions
                if (frame_changed) {
                    print_with_set_info(
                        ev_set.di().handle().c_str(), ev_set.group(),
                        "%s at %s:%d\n", frameLoc.loc_func, frameLoc.loc_file, frameLoc.loc_line
                    );
                }

                auto line = std::string{listResult->ev_list->list_line};
                formatSourceLine(line);

                print_with_set_info(
                    ev_set.di().handle().c_str(), ev_set.group(),
                    "%d\t%s\n", frameLoc.loc_line, line.c_str()
                );
            } else {
                // For some reason we didn't get the right line, just print the frame
                print_with_set_info(
                    ev_set.di().handle().c_str(), ev_set.group(),
                    "%s at %s:%d\n", frameLoc.loc_func, frameLoc.loc_file, frameLoc.loc_line
                );
            }
            FreeEvent(listResult);

        } else if (frameLoc.loc_func) {
            // No file/line, just print function name
            print_with_set_info(
                ev_set.di().handle().c_str(), ev_set.group(), "in %s\n", frameLoc.loc_func);
        } else {
            // Shouldn't ever happen
            print_with_set_info(ev_set.di().handle().c_str(), ev_set.group(),
                "Step finished with no location information.\n"
            );
        }
    } else {
        // Got a null frame. Can happen in rare cases where
        // gdb doesn't give us any info about why the application stopped
        print_with_set_info(ev_set.di().handle().c_str(), ev_set.group(),
            "Step finished with no location information.\n"
        );
    }
}

void ConsoleInterpreter::handle_async_event(DbgInvoke::EventSet& ev_set, const Bitset& /*reply_set*/) {
    // When handling async output, always use m_printer and never use PrintOut*. This
    // ensures that the async output bypasses the CurrentStream and always gets to its
    // intended destination. See #AsyncPrinting in client_io.md.

    switch (ev_set.event()->ev_event) {
        case DBGEV_EXIT: {
            if (ev_set.event()->ev_exit == 0) {
                m_printer.print_with_set_info(ev_set.di().handle().c_str(), ev_set.group(),
                                    "Program exited normally.\n");
            } else {
                m_printer.print_with_set_info(ev_set.di().handle().c_str(), ev_set.group(),
                                    "Program exited with code 0%o.\n",
                                    ev_set.event()->ev_exit);
            }
            break;
        }

        case DBGEV_BPHIT: {
            // ensure the recv bp has a corresponding bp_set. If not, this is an
            // unknown breakpoint. If its unknown either its a breakpoint
            // encountered from ending the graph (graph breakpoints are hidden)
            // or an exit breakpoint that was set by the dbg servers.

            auto bpset = ev_set.di().breakpoints().findByDbgBreakpoint(*ev_set.event()->ev_bp);

            if (!bpset) {
                if (ev_set.event()->ev_bp->bp_type == DBGBP_EXIT_TYPE) {
                    m_printer.print_with_set_info(
                        ev_set.di().handle().c_str(), ev_set.group(),
                        "The application has reached an exit breakpoint.\n");
                } else if (Client.print_bp_frames) {
                    if (ev_set.event()->ev_bp->bp_loc.loc_line > 0) {
                        m_printer.print_with_set_info(
                            ev_set.di().handle().c_str(), ev_set.group(),
                            "In %s at %s:%d\n",
                            ev_set.event()->ev_bp->bp_loc.loc_func,
                            ev_set.event()->ev_bp->bp_loc.loc_file,
                            ev_set.event()->ev_bp->bp_loc.loc_line);
                    } else {
                        m_printer.print_with_set_info(
                            ev_set.di().handle().c_str(), ev_set.group(), "In %s\n",
                            ev_set.event()->ev_bp->bp_loc.loc_func);
                    }
                }
                break;
            }

            // What we print out is based on if we are at a watchpoint or
            // breakpoint
            // TODO: What should this default to? Right now I am having it
            // default to the breakpoint message.
            auto bpThread = std::string{};
            if (!bpset->threadId().empty()) bpThread = " thread " + bpset->threadId();

            switch (bpset->type()) {
                case DBGBP_WATCH_TYPE:
                case DBGBP_AWATCH_TYPE:
                case DBGBP_RWATCH_TYPE:
                    // Print out the updated locale information for the watchpoint.
                    // This was updated in EventCB before calling this function.
                    for (const auto &watch_loc : bpset->watchLocaleSet()) {
                        m_printer.print_with_set_info(
                            ev_set.di().handle().c_str(), *watch_loc.group,
                            "Watchpoint %d, variable expression: %s\n",
                            bpset->id(), bpset->watchExpression().c_str());
                        if (watch_loc.line > 0) {
                            m_printer.print_with_set_info(
                                ev_set.di().handle().c_str(), *watch_loc.group,
                                "In %s at %s:%d\n", watch_loc.func.c_str(),
                                watch_loc.file.c_str(), watch_loc.line);
                        } else {
                            m_printer.print_with_set_info(
                                ev_set.di().handle().c_str(), *watch_loc.group,
                                "In %s\n", watch_loc.func.c_str());
                        }
                    }
                    // clear out the watchpoint locale to avoid printing multiple times
                    // XXX: this async event handler probably shouldn't have side effects,
                    // considering that other observers might want to read this data!
                    bpset->clearWatchLocaleSet();
                    break;

                case DBGBP_BREAK_TYPE:
                    if (Client.print_bp_frames) {
                        if (bpset->line() > 0) {
                            m_printer.print_with_set_info(
                                ev_set.di().handle().c_str(), ev_set.group(),
                                "Breakpoint %d, %s at %s:%d%s\n", bpset->id(),
                                bpset->function().c_str(), bpset->file().c_str(),
                                bpset->line(), bpThread.c_str());
                        } else {
                            m_printer.print_with_set_info(ev_set.di().handle().c_str(),
                                                ev_set.group(),
                                                "Breakpoint %d, in %s%s\n",
                                                bpset->id(), bpset->function().c_str(),
                                                bpThread.c_str());
                        }
                    }
                    break;

                case DBGBP_CATCH_TYPE:
                    if (!bpset->watchExpression().empty()) {
                        m_printer.print_with_set_info(
                            ev_set.di().handle().c_str(), ev_set.group(),
                            "Catchpoint %d (%s)\n",
                            bpset->id(), bpset->watchExpression().c_str());
                    } else {
                        m_printer.print_with_set_info(ev_set.di().handle().c_str(),
                            ev_set.group(),
                            "Catchpoint %d\n",
                            bpset->id());
                    }
                    break;

                default:
                    if (bpset->line() > 0) {
                        m_printer.print_with_set_info(
                            ev_set.di().handle().c_str(), ev_set.group(),
                            "Unknown break type %d, %s at %s:%d\n",
                            bpset->id(), bpset->function().c_str(),
                            bpset->file().c_str(), bpset->line());
                    } else {
                        m_printer.print_with_set_info(ev_set.di().handle().c_str(),
                                            ev_set.group(),
                                            "Unknown break type %d, in %s\n",
                                            bpset->id(), bpset->function().c_str());
                    }
                    break;
            }

            break;
        }

        case DBGEV_BPDEL: {
            // ensure the recv bp has a cooresponding bp_set. If not, this is an
            // unknown breakpoint. If its unknown either its a breakpoint
            // encountered from ending the graph (graph breakpoints are hidden)
            // or an exit breakpoint that was set by the dbg servers.
            auto bpset = ev_set.di().breakpoints().findByDbgBreakpoint(*ev_set.event()->ev_bp);

            if (!bpset) {
                m_printer.print_error_with_set_info(
                    ev_set.di().handle().c_str(), ev_set.group(),
                    "ERROR: cannot find breakpoint in BPSetList.\n");
                break;
            }

            // ranks removed from breakpoint bitset in
            // src/dbgclnt/dbg_client.cpp

            // What we print out is based on if we are at a watchpoint or
            // breakpoint
            switch (bpset->type()) {
                case DBGBP_WATCH_TYPE:
                case DBGBP_AWATCH_TYPE:
                case DBGBP_RWATCH_TYPE:
                    // Print out the updated locale information for the watchpoint.
                    // This was updated in EventCB before calling this function.
                    for (const auto &watch_loc : bpset->watchLocaleSet()) {
                        m_printer.print_with_set_info(
                            ev_set.di().handle().c_str(), *watch_loc.group,
                            "Watchpoint %d deleted because the program has left "
                            "the block in which its expression is valid.\n",
                            bpset->id());
                        if (watch_loc.line > 0) {
                            m_printer.print_with_set_info(
                                ev_set.di().handle().c_str(), *watch_loc.group,
                                "In %s at %s:%d\n", watch_loc.func.c_str(),
                                watch_loc.file.c_str(), watch_loc.line);
                        } else {
                            m_printer.print_with_set_info(
                                ev_set.di().handle().c_str(), *watch_loc.group,
                                "In %s\n", watch_loc.func.c_str());
                        }
                    }
                    // clear out the watchpoint locale to avoid printing multiple times
                    // XXX: this function probably shouldn't have side effects like this.
                    bpset->clearWatchLocaleSet();
                    break;

                case DBGBP_BREAK_TYPE:
                    if (bpset->line() > 0) {
                        m_printer.print_with_set_info(
                            ev_set.di().handle().c_str(), ev_set.group(),
                            "Breakpoint %d deleted, %s at %s:%d\n",
                            bpset->id(), bpset->function().c_str(),
                            bpset->file().c_str(), bpset->line());
                    } else {
                        m_printer.print_with_set_info(ev_set.di().handle().c_str(),
                                            ev_set.group(),
                                            "Breakpoint %d deleted, in %s\n",
                                            bpset->id(), bpset->function().c_str());
                    }
                    break;

                case DBGBP_CATCH_TYPE:
                    if (!bpset->watchExpression().empty()) {
                        m_printer.print_with_set_info(
                            ev_set.di().handle().c_str(), ev_set.group(),
                            "Catchpoint %d deleted (%s)\n",
                            bpset->id(), bpset->watchExpression().c_str());
                    } else {
                        m_printer.print_with_set_info(ev_set.di().handle().c_str(),
                            ev_set.group(),
                            "Catchpoint %d deleted.\n",
                            bpset->id());
                    }
                    break;

                default:
                    if (bpset->line() > 0) {
                        m_printer.print_with_set_info(
                            ev_set.di().handle().c_str(), ev_set.group(),
                            "Unknown break type %d deleted, %s at %s:%d\n",
                            bpset->id(), bpset->function().c_str(),
                            bpset->file().c_str(), bpset->line());
                    } else {
                        m_printer.print_with_set_info(
                            ev_set.di().handle().c_str(), ev_set.group(),
                            "Unknown break type %d deleted, in %s\n",
                            bpset->id(), bpset->function().c_str());
                    }
                    break;
            }

            break;
        }

        case DBGEV_SIGNAL: {
            auto is_cuda_error = (::strncmp(ev_set.event()->ev_sig_type,
                "Cuda API error detected: ", 24) == 0);
            m_printer.print_with_set_info(ev_set.di().handle().c_str(), ev_set.group(),
                (is_cuda_error) ? "%s.\n" : "Program received signal %s.\n",
                ev_set.event()->ev_sig_type);

            if (ev_set.event()->ev_sig_frame != (dbgframe_t *)NULL) {
                const auto &frameLoc = ev_set.event()->ev_sig_frame->getLoc();
                if (frameLoc.loc_file != NULL) {
                    m_printer.print_with_set_info(
                        ev_set.di().handle().c_str(), ev_set.group(),
                        "In %s at %s:%d\n",
                        frameLoc.loc_func, frameLoc.loc_file, frameLoc.loc_line);
                } else {
                    m_printer.print_with_set_info(
                        ev_set.di().handle().c_str(), ev_set.group(), "In %s\n",
                        frameLoc.loc_func);
                }
            } else if (ev_set.event()->ev_sig_line != NULL) {
                m_printer.print_with_set_info(ev_set.di().handle().c_str(), ev_set.group(),
                                    "%s\n", ev_set.event()->ev_sig_line);
            }

            break;
        }

        case DBGEV_STEP:
            if (Client.print_source_on_step)
                m_printer.print_event_location(ev_set);
            break;

        case DBGEV_FRAME: {
            // this is a special case: the only time this event happens
            // asynchronously is at startup. print the initial breakpoint.
            //
            // this is a little hacky and relies on the fact that there are no
            // other situations where an async DBGEV_FRAME event happens.

            auto* frame = ev_set.event()->ev_frame;
            const auto &loc = frame->getLoc();

            if (loc.loc_line > 0) {
                m_printer.print_with_set_info(
                    ev_set.di().handle().c_str(), ev_set.group(),
                    "Initial breakpoint, %s at %s:%d\n", loc.loc_func, loc.loc_file, loc.loc_line);
            } else {
                m_printer.print_with_set_info(ev_set.di().handle().c_str(), ev_set.group(),
                                    "Initial breakpoint, in %s\n", loc.loc_func);
            }
            break;
        }

        case DBGEV_ERROR:
            m_printer.print_error_with_set_info(ev_set.di().handle().c_str(), ev_set.group(),
                                  "%s\n", DbgEventErrorStr(ev_set.event()));
            break;

        case DBGEV_PYEXCEPTION: {
            auto fatal = ev_set.event()->ev_u.ev_u_pyexception->fatal;

            m_printer.print_with_set_info(
                ev_set.di().handle().c_str(), ev_set.group(),
                "Application raised an %sexception:\n",
                fatal ? "uncaught " : "");

            // print error type and message
            m_printer.print_message("%s: %s\n",
                ev_set.event()->ev_u.ev_u_pyexception->type_name.c_str(),
                ev_set.event()->ev_u.ev_u_pyexception->err_str.c_str());

            // print backtrace
            if (const auto* frame = ev_set.event()->ev_u.ev_u_pyexception->backtrace) {
                for(; frame; frame = frame->getNext()) {
                    m_printer.print_message("  %s\n", frame->getLocationString(false, false).c_str());
                }
            }

            if (fatal) {
                m_printer.print_message(
                    "The python stack has been unwound and python will now exit.\n"
                    "Use the `catch raise` command to stop on exceptions before the stack is unwound.\n");
            }

            break;
        }

        case DBGEV_STRING:
            m_printer.print_with_set_info(ev_set.di().handle().c_str(), ev_set.group(),
                                  "%s\n", ev_set.event()->ev_u.ev_u_string);
            break;

        case DBGEV_OK:
            break;

        default:
            m_printer.print_error_with_set_info(ev_set.di().handle().c_str(), ev_set.group(),
                                  "Can't print event %d\n",
                                  ev_set.event()->ev_event);
            break;
    }
}

ConsoleInterpreter::Printer::Printer(ConsoleInterpreter& parent, cli_stream_t& io)
    : m_intr(parent)
    , m_io(io)
{}

void ConsoleInterpreter::Printer::print_message_v(const char* fmt, va_list args) {
    m_io.cs_out.vprintf(fmt, args);
}

std::ostream& ConsoleInterpreter::Printer::get_message_stream() {
    if (!m_ostream) {
        m_buff = std::make_unique<StreamBuf>(*this);
        m_ostream = std::make_unique<std::ostream>(m_buff.get());
    }
    return *m_ostream;
}

void ConsoleInterpreter::Printer::print_error_v(const char* fmt, va_list args) {
    ::vfprintf(m_io.cs_fperr, fmt, args);
}

void ConsoleInterpreter::Printer::print_inferior_output(const DbgInvoke& di, std::string_view line, FILE* use_instead) {
    FILE* output = [&](){
        if (use_instead) return use_instead;
        return m_io.cs_fpprog;
    }();

    if (!output) return;

    // surprisingly no newline here. i guess it's assumed it will come with the line?
    ::fprintf(output, "<$%s>: %.*s", di.handle().c_str(), (int)line.size(), line.data());
}

ConsoleInterpreter::Printer::StreamBuf::StreamBuf(ConsoleInterpreter::Printer& printer)
    : m_printer(printer)
{}

std::streamsize ConsoleInterpreter::Printer::StreamBuf::xsputn(const char* s, std::streamsize n) {
    // forward to out stream untouched
    return m_printer.m_io.cs_out.fwrite(s, n);
}

int ConsoleInterpreter::Printer::StreamBuf::overflow(int c) {
    // forward to out stream untouched
    return m_printer.m_io.cs_out.putc(c);
}

int ConsoleInterpreter::Printer::StreamBuf::sync() {
    m_printer.m_io.cs_out.flush();
    return 0;
}

std::shared_ptr<CommandNode> ConsoleCommandsFactory::make() {
    auto root = std::make_shared<CommandNode>("", nullptr, proc_optional, nullptr, nullptr, nullptr);

    insert_class_commands(*root);
    // insert_print_commands includes the base "set" command, make sure it stays near
    // the top of the order so that later commands can add set subcommands.
    insert_print_commands(*root);
    insert_interpreter_commands(*root);
    insert_frame_commands(*root);
    insert_step_commands(*root);
    insert_passthru_commands(*root);
    insert_script_commands(*root);
    insert_blockmap_commands(*root);
    insert_env_commands(*root);
    insert_python_commands(*root);
    insert_assign_commands(*root);
    insert_checkpoint_commands(*root);
    insert_source_commands(*root);
    insert_whatis_commands(*root);
    insert_break_commands(*root);
    insert_proc_commands(*root);
    insert_list_commands(*root);
    insert_iostat_commands(*root);
    insert_threads_commands(*root);
    insert_comp_commands(*root);
    insert_go_commands(*root);
    insert_procset_commands(*root);
    insert_utils_commands(*root);

    return root;
}

void ConsoleCommandsFactory::insert_class_commands(CommandNode& root) {
    // insert a fake help command so we get the niceness of the command tree
    // matching "h", "he", etc. to "help". printing the actual help requires
    // context only available in the interpreter itself, so the interpreter
    // will catch a match to this command and short circuit to the help output.
    auto dummy_help = []() -> int {
        return Command::Failure;
    };
    root.addSubCmd("help", "h", proc_ignore,
                    dummy_help, cmd_help_help,
                    "Display help information about commands.");

    root.addSubCmd("info", "i", proc_ignore,
                   nullptr, class_info_help,
                   "Display information about the application being debugged.");

    root.addSubCmd("maint", nullptr, proc_ignore,
                   nullptr, class_maint_help,
                   "Commands for use by debugger maintainers.");

    root.addSubCmd("maint set", nullptr, proc_ignore,
                   nullptr, class_maint_set_help,
                   "Set information about the debugger environment.");

    root.addSubCmd("maint show", nullptr, proc_ignore,
                   nullptr, class_maint_show_help,
                   "Show information about the debugger environment.");

    root.addSubCmd("maint pybe", nullptr, proc_ignore,
                   nullptr, nullptr,
                   "Control the python debugger backend.");

    root.addSubCmd("unset", nullptr, proc_ignore,
                   nullptr, class_unset_help,
                   "Unset information in the debugger environment.");

    root.addSubCmd("show", nullptr, proc_ignore,
                   nullptr, class_show_help,
                   "Show information about the debugger environment.");

    root.addSubCmd("save", nullptr, proc_ignore,
                   nullptr, cmd_save_help,
                   "Save debugger state to a file.");

}

Interpreter::Completions
ConsoleInterpreter::complete_text(std::string_view partial_word, std::string_view buffer) const {
    auto [cmd, args] = m_commands_stack.back().tree->findCmdBestMatch(stripped(buffer));

     // should never happen. findCmdBestMatch will return the root node at worst.
    if (!cmd) return {};

    auto argv = ArgvStream{};
    argv.parseAndAppendArgs(cmd->getShortName() + " " + args);

    if (cmd->getShortName() == "help") {
        // short circuit on help - it needs more global context than normal
        // commands to do completion.
        return complete_help(partial_word, buffer);
    } else if (!partial_word.empty() && argv.argc() == 1 && cmd->getShortName().size() >= partial_word.size()) {
        // the user is still typing the command name. we complete the command
        // name before deferring to command specific impls
        //
        // this branch is chosen even for some commands that appear at first to
        // have an argc() > 1 like "set print elem" because we use the short
        // name when constructing argv above, which peels off "set print" for us
        // and argv[0] becomes "elem".
        auto comps = Interpreter::Completions{};
        comps.possibilities.emplace(cmd->getShortName());
        return comps;
    } else {
        // do command-specific completion
        return cmd->completeText(partial_word, buffer, *this, CurrentProcSet(), argv);
    }
}

Interpreter::Completions ConsoleInterpreter::complete_help(std::string_view partial_word, std::string_view buffer) const {
    // pretend we're completing a normal command that starts after "help".
    // this recursive strategy has a side effect of eventually running
    // command-specific completions. e.g. "help launch <tab>" will run the launch
    // specific completions and might populate the line with more arguments meant
    // for the real command. this might look a little weird, but it won't break
    // anything. once the user presses enter, the help command is smart enough
    // to ignore the extra stuff.

    // the API of complete_text requires that `partial_word` live within `buffer` so
    // we need to be a little fancy in crafting the new arguments for it.
    const auto help_chars = buffer.substr(0, buffer.find_first_of(" \n\r\t"));
    // should never happen, but prevent infinite recursion/exception throws if it does
    if (help_chars.size() == 0 || help_chars.size() > buffer.size()) return {};

    return complete_text(partial_word, buffer.substr(help_chars.size()));
}

