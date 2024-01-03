#include "interpreter_mi.h"
#include "str.h"
#include "client_settings.h"
#include "cmd.h"

#include <charconv>

namespace {

void log(const char* fmt, ...) __attribute__((format (printf,1,2)));

void log(const char* fmt, ...) {
    static const auto log_args = []() {
        struct Args { bool enabled; std::string filename; };
        auto args = Args{
            .enabled = ::getenv("GDB4HPC_DEBUG_INTERPRETER") != nullptr,
            .filename = "",
        };

        if (args.enabled) {
            if (auto log_dir = ::getenv("CRAY_DBG_LOG_DIR")) {
                ::fprintf(stderr, "Saving mi debug log at %s/mi.log.\n", ::getenv("CRAY_DBG_LOG_DIR"));
                args.filename = std::string{log_dir} + "/mi.log";
            }
        }

        return args;
    }();

    if (log_args.enabled) {
        va_list args;
        va_start(args, fmt);

        auto message = MallocedCharPtr(fmt, args);
        va_end(args);
        if (!message) return;

        if (!log_args.filename.empty()) {
            if (auto* f = ::fopen(log_args.filename.c_str(), "a")) {
                ::fprintf(f, "%s", message.get());
                ::fclose(f);
                // the above fclose flushes the *user space* buffers, but not
                // the *kernel space* buffers. we be extra diligent and flush
                // the kernel buffers too.
                //
                // we have to do this because ccdb will kill gdb4hpc with a
                // SIGKILL if we emit something that it can't parse. without
                // ::fsync, it often comes in and kills us before everything
                // gets onto disk. since it's a SIGKILL, the os isn't even nice
                // enough to flush buffers for us when we die.
                ::fsync(::fileno(f));
                return;
            }
        }

        ::fprintf(stderr, "%smi: %s%s", "\u001b[30;1m", message.get(), "\033[0m");
    }
}

} // anonymous namespace

MiInterpreter::MiInterpreter(cli_stream_t& io)
    : m_mi_commands(MiCommandsFactory::make())
    , m_console_intr(io)
    , m_result{}
    , m_should_print_running{}
    , m_error{}
    , m_printer(*this, io)
{}

std::string_view MiInterpreter::name() const {
    return "mi";
}

bool MiInterpreter::can_interpret(std::string_view line) const {
    auto find_result = m_mi_commands->findCmd(line);
    return std::get_if<CommandNode::Parts>(&find_result) != nullptr
        || m_console_intr.can_interpret(line);
}

bool MiInterpreter::interpret(std::string_view line) {
    m_result.reset();
    m_should_print_running = false;
    m_error = "";
    cli_set_prefix("");

    const auto result = try_interpret(line);

    // TODO implement command prefixes internally, without cli_get_prefix()

    // Check that we aren't quitting, otherwise the (gdb) prompt would show
    // after the user quit, during shutdowns
    if (!Client.quit) {
        if (result) {
            auto leader = m_should_print_running ? "^running" : "^done";
            if (m_result) {
                m_printer.print_unfiltered("%s%s,%s\n", cli_get_prefix().c_str(), leader, m_result.get());
            } else {
                m_printer.print_unfiltered("%s%s\n", cli_get_prefix().c_str(), leader);
            }
        } else {
            if (!m_error.empty()) {
                m_printer.print_unfiltered("%s^error,msg=\"%s\"\n", cli_get_prefix().c_str(), m_error.c_str());
            } else {
                // ideally this should never happen.
                m_printer.print_unfiltered("%s^error,msg=\"Unknown error\"\n", cli_get_prefix().c_str());
            }
        }
        m_printer.print_unfiltered("(gdb) \n");
    }
    return result;
}

bool MiInterpreter::try_interpret(std::string_view line) {
    line = stripped(line);
    log("interpreting \"%.*s\"\n", (int)line.size(), line.data());
    if (line.empty() || line.starts_with("#")) {
        return true;
    }

    // consume tag, if present.
    auto untagged_line = line;
    if (!line.starts_with("-")) {
        auto tag = -1;
        auto result = std::from_chars(line.data(), line.data() + line.size(), tag);

        if (tag != -1) {
            log("got tag: %d\n", tag);
            // TODO use a different mechanism than cli_set_prefix to handle the tag
            cli_set_prefix(std::to_string(tag));
            untagged_line.remove_prefix(result.ptr - line.data());
            log("after tag, interpreting \"%.*s\"\n", (int)untagged_line.size(), untagged_line.data());
        }
    }

    auto find_result = m_mi_commands->findCmd(untagged_line);

    if (auto* parts = std::get_if<CommandNode::Parts>(&find_result)) {
        auto& [cmd, args] = *parts;
        if (!cmd) {
            log("found in tree, but no function pointer?\n");
            return false;
        }

        // short circuit to help function. the command node in the
        // tree doesn't actually point to a command function.
        if (cmd->getShortName() == "-help") {
            return print_help();
        }

        auto* procset = CurrentProcSet();
        auto argv = ArgvStream{};
        argv.parseAndAppendArgs(cmd->getShortName() + " " + args);

        if (cmd->execute(*this, procset, argv.argc(), argv.c_strs().data()) == Command::Success) {
            log("command successfull\n");
            return true;
        } else {
            log("command returned an error\n");
            return false;
        }
    } else {
        log("forwarding to console interpreter\n");

        // emit escaped log message &"..."
        std::ostringstream ss;
        ss << std::quoted(untagged_line);
        auto quoted = std::move(ss).str();

        // the call to stripped above removed the newline character from the line, but
        // that *should* be included in the escaped output.
        //
        // "set \"foo\" bar" -> "set \"foo\" bar\n"
        quoted.replace(quoted.size() - 1, 1, "\\n\"");

        m_printer.print_unfiltered("&%s\n", quoted.c_str());

        return m_console_intr.interpret(untagged_line);
    }
}

void MiInterpreter::set_result(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    m_result = MallocedCharPtr(fmt, args);
    va_end(args);
}

void MiInterpreter::attach_observers() {
    // we intentionally don't attach observers to m_console_intr so that it stays silent
    // during async events. (see #IgnoreAsync)
    m_obsv_conns.emplace_back(
        observers::setable_param_changed.attach(
            [this](std::string_view name, std::string_view value) {
                log("observing setable_param_changed\n");
                return handle_setable_param_changed(name, value);
            }
        )
    );

    m_obsv_conns.emplace_back(
        observers::async_event.attach(
            [this](DbgInvoke::EventSet& ev_set, const Bitset& reply_set) {
                log("observing async_event\n");
                return handle_async_event(ev_set, reply_set);
            }
        )
    );

    m_obsv_conns.emplace_back(
        observers::target_resumed.attach(
            [this]() {
                log("observing target_resumed\n");
                return handle_target_resumed();
            }
        )
    );
}

void MiInterpreter::handle_setable_param_changed(std::string_view name, std::string_view new_value) {
    m_printer.print_unfiltered("=cmd-param-changed,param=\"%.*s\",value=\"%.*s\"\n",
                        (int)name.size(), name.data(),
                        (int)new_value.size(), new_value.data());
}

void MiInterpreter::Printer::print_stopped_breakpoint(
    std::string_view bkpt_id, std::string_view proc_name, const Bitset &proc_group,
    std::string_view func, std::string_view file, std::string_view fullpath,
    int line)
{
    std::ostringstream ss;
    auto group_str = proc_group.toSetString();

    ss << "bkptno=\"" <<bkpt_id
       << "\",proc_set=\"" << proc_name
       << "\",group=\"" << group_str.substr(1,group_str.length()-2)
       << "\",frame={func=\"" << func
       << "\",args=[],file=\"" << file
       << "\",fullname=\""<< fullpath
       << "\",line=\"" << line << "\"}"
    ;

    print_unfiltered("*stopped,reason=\"breakpoint-hit\",%s\n", ss.str().c_str());
}

void MiInterpreter::Printer::print_stopped_step(
    std::string_view proc_name, const Bitset &proc_group, int threadId,
    std::string_view func, std::string_view file, std::string_view fullpath, int line)
{
    std::ostringstream ss;
    auto group_str = proc_group.toSetString();

    ss << "proc_set=\"" << proc_name
       << "\",group=\"" << group_str.substr(1,group_str.length()-2)
       << "\",frame={func=\"" << func
       << "\",args=[],file=\""<< file
       << "\",fullname=\""<< fullpath
       << "\",line=\""<<line << "\"}"
       << ",thread-id=\"" << threadId << '"'
    ;

    print_unfiltered("*stopped,reason=\"end-stepping-range\",%s\n", ss.str().c_str());
}

void MiInterpreter::Printer::print_stopped_exit(std::string_view proc_name, const Bitset &proc_group) {
    std::ostringstream ss;
    auto group_str = proc_group.toSetString();

    ss << "procset=\"" << proc_name << "\","
       << "group=\"" << group_str.substr(1, group_str.length() - 2) << '"'
    ;

    print_unfiltered("*stopped,reason=\"exited-normally\",%s\n", ss.str().c_str());
}

void MiInterpreter::handle_async_event(DbgInvoke::EventSet& ev_set, const Bitset& /*reply_set*/) {
    // When handling async output, always use m_printer and never use PrintOut*. This
    // ensures that the async output bypasses the CurrentStream and always gets to its
    // intended destination. See #AsyncPrinting in client_io.md.

    switch (ev_set.event()->ev_event) {
        case DBGEV_EXIT: {
            // TODO
            break;
        }

        case DBGEV_BPHIT: {
            auto bpset = ev_set.di().breakpoints().findByDbgBreakpoint(*ev_set.event()->ev_bp);

            if (!bpset) {
                // if unknown to client, assume it's an exit breakpoint
                if (ev_set.event()->ev_bp->bp_type == DBGBP_EXIT_TYPE) {
                    m_printer.print_stopped_exit(ev_set.di().handle().name(), ev_set.group());
                }
                break;
            }

            switch (bpset->type()) {
                case DBGBP_WATCH_TYPE:
                case DBGBP_AWATCH_TYPE:
                case DBGBP_RWATCH_TYPE:
                    // TODO
                    break;

                case DBGBP_BREAK_TYPE:
                    if (bpset->line() > 0) {
                        m_printer.print_stopped_breakpoint(
                            std::to_string(bpset->id()), ev_set.di().handle().name(),
                            ev_set.group(), bpset->function(),
                            bpset->file(), ev_set.di().sourcePaths().getFullPath(bpset->file()),
                            bpset->line()
                        );
                    }
                    break;

                case DBGBP_CATCH_TYPE:
                    // TODO
                    break;

                default:
                    // TODO
                    break;
            }

            break;
        }

        case DBGEV_BPDEL: {
            auto bpset = ev_set.di().breakpoints().findByDbgBreakpoint(*ev_set.event()->ev_bp);

            if (!bpset) {
                // silently ignore? not sure what else to do...
                break;
            }

            // What we print out is based on if we are at a watchpoint or
            // breakpoint
            switch (bpset->type()) {
                case DBGBP_WATCH_TYPE:
                case DBGBP_AWATCH_TYPE:
                case DBGBP_RWATCH_TYPE:
                    // TODO
                    break;

                case DBGBP_BREAK_TYPE: {
                    if (bpset->line() > 0) {
                        m_printer.print_stopped_breakpoint(
                            std::to_string(bpset->id()), ev_set.di().handle().name(),
                            ev_set.group(), bpset->function(),
                            bpset->file(), ev_set.di().sourcePaths().getFullPath(bpset->file()),
                            bpset->line()
                        );
                    }
                    break;
                }

                case DBGBP_CATCH_TYPE:
                    // TODO
                    break;

                default:
                    // TODO
                    break;
            }

            break;
        }

        case DBGEV_SIGNAL: {
            // TODO
            break;
        }

        case DBGEV_STEP: {
            const auto& step = ev_set.event()->ev_step;
            const dbgframe_t *frame = step.step_frame;
            if (frame) {
                const auto &frameLoc = frame->getLoc();
                auto file = frameLoc.loc_file, frame = frameLoc.loc_func;
                m_printer.print_stopped_step(
                    ev_set.di().handle().name(), ev_set.group(), step.step_threadId,
                    frame ? frame : "unknown", file ? file : "unknown",
                    ev_set.di().sourcePaths().getFullPath(file), frameLoc.loc_line
                );
            }
            break;
        }

        case DBGEV_FRAME: {
            // this is a special case: the only time this event happens
            // asynchronously is at startup. print the initial breakpoint.
            //
            // this is a little hacky and relies on the fact that there are no
            // other situations where an async DBGEV_FRAME event happens.

            auto frame = ev_set.event()->ev_frame;
            const auto &loc = frame->getLoc();

            if (loc.loc_line > 0) {
                // XXX: fake breakpoint id of 0
                m_printer.print_stopped_breakpoint(
                    "0", ev_set.di().handle().name(),
                    ev_set.group(), loc.loc_func,
                    loc.loc_file, ev_set.di().sourcePaths().getFullPath(loc.loc_file),
                    loc.loc_line
                );
            } else {
                // XXX: there should probably be a fallback here, but the old
                // behavior didn't have it, so leaving it for now.
            }
        }

        case DBGEV_ERROR:
            // TODO
            break;

        case DBGEV_OK:
            // nothing to do
            break;

        default:
            // nothing to do
            break;
    }
}

void MiInterpreter::handle_target_resumed() {
    // faking the thread id for now
    m_printer.print_unfiltered("*running,thread-id=\"all\"\n");
    m_should_print_running = true;
}

Interpreter::Printer& MiInterpreter::get_printer() {
    return m_printer;
}

bool MiInterpreter::print_help() {
    // this is not meant to be a user friendly help command, it's for developers/debugging
    for (auto scmd : m_mi_commands->getSubCmds()) {
        m_printer.print_message("%-30s %s\n", scmd->getShortName().c_str(), scmd->getDesc() ? scmd->getDesc() : "(no description given)");
    }
    return true;
}

MiInterpreter::Printer::Printer(MiInterpreter& parent, cli_stream_t& io)
    : m_intr(parent)
    , m_io(io)
{}

void MiInterpreter::Printer::print_message_v(const char* format, va_list arg_list) {
    // the arg_list may well contain quotes or newlines, so we have to
    // just get the final output form so we can know they're being properly
    // escaped in print_message_string.
    auto cstr = MallocedCharPtr(format, arg_list);
    if (!cstr) return;

    print_message_string(cstr.get(), ::strlen(cstr.get()));
}

void MiInterpreter::Printer::print_message_string(const char* cstr, size_t len) {
    // the output is line oriented to we use m_message_ss to cache any partial lines

    if (!m_message_ss.tellp()) m_message_ss << "~\"";  // the MI form is ~"..."

    for (auto i = size_t{0}; i < len; i++) {
        switch (cstr[i]) {
        case '"': m_message_ss << "\\\""; break;
        case '\n':
            // escape the newline and flush the result
            m_message_ss << "\\n\"\n";
            log("%s", m_message_ss.str().c_str());
            m_io.cs_out("%s", m_message_ss.str().c_str());
            m_message_ss.str("");
            m_message_ss << "~\"";
            break;
        default: m_message_ss << cstr[i];
        }
    }
}

void MiInterpreter::Printer::print_unfiltered(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    auto cstr = MallocedCharPtr(fmt, args);
    va_end(args);
    if (!cstr) return;

    log("%s", cstr.get());
    m_io.cs_out("%s", cstr.get());

}

void MiInterpreter::Printer::print_error_v(const char* format, va_list arg_list) {
    // given the following sequence of PrintOutError calls:
    //
    // 1. PrintOutError("First error\n");
    // 2. PrintOutError("Second \"error\"\n");
    //
    // the mi output should look like this:
    //
    // &"First error\n"
    // &"Second \"error\"\n"
    // ...
    // ^error,msg="First error"
    //
    // note that the ^error part doesn't have the escaped newline on the end, but the &
    // output does.

    // the arg_list may well contain quotes or newlines, so we have to
    // just get the final output form so we can know they're being properly
    // escaped.
    auto cstr = MallocedCharPtr(format, arg_list);
    if (!cstr) return;

    for (const auto *p = cstr.get(); *p; ++p) {
        switch (*p) {
            case '"': m_error_ss << "\\\""; break;
            case '\n': {
                // collect string from and reset sstream without appending the \n
                auto escaped = std::move(m_error_ss).str();
                // std::move does not reset the internal string, so we have to do it manually
                m_error_ss.str("");

                // emit error message as &"...\n" message. note that we go through
                // cs_out, not cs_fperr, so logging works. in mi, cs_out and fperr
                // point to the same FILE*.
                log("&\"%s\\n\"\n", escaped.c_str());
                m_io.cs_out("&\"%s\\n\"\n", escaped.c_str());

                // save the result in the parent interpreter for ^error="..."
                // XXX: the MI standard only allows the output of one error per command, so
                // we only use the first error produced.
                if (m_intr.m_error.empty()) {
                    m_intr.m_error = std::move(escaped);
                }
                break;
            }
            default:
                m_error_ss << *p;
                break;
        }
    }
}

void MiInterpreter::Printer::print_inferior_output(const DbgInvoke& /*di*/, std::string_view line, FILE* use_instead) {
    FILE* output = [&](){
        if (use_instead) return use_instead;
        return m_io.cs_fpprog;
    }();

    if (!output) return;

    // ignoring embedded quotes for the moment
    auto new_line=std::string{line};
    for (std::size_t pos=0; (pos=new_line.find_first_of("\"\n\r",pos)) != std::string::npos;) {
        new_line.erase(pos,1);
    }

    log("@\"%s\"\n", new_line.c_str());
    ::fprintf(output, "@\"%s\"\n", new_line.c_str());
}

std::ostream& MiInterpreter::Printer::get_message_stream() {
    if (!m_ostream) {
        m_buff = std::make_unique<StreamBuf>(*this);
        m_ostream = std::make_unique<std::ostream>(m_buff.get());
    }
    return *m_ostream;
}

MiInterpreter::Printer::StreamBuf::StreamBuf(MiInterpreter::Printer& printer)
    : m_printer(printer)
{}

std::streamsize MiInterpreter::Printer::StreamBuf::xsputn(const char* s, std::streamsize n) {
    m_printer.print_message_string(s, n);
    return n;
}

int MiInterpreter::Printer::StreamBuf::overflow(int c) {
    if (c != EOF) m_printer.print_message("%c", c);
    return c;
}

std::shared_ptr<CommandNode> MiCommandsFactory::make() {
    auto root = std::make_shared<CommandNode>("", nullptr, proc_optional, nullptr, nullptr, nullptr);

    insert_class_commands(*root);
    // insert_print_commands includes the base "set" command, make sure it stays near
    // the top of the order so that later commands can add set subcommands.
    insert_print_commands(*root);
    insert_dummy_commands(*root);
    insert_mi_commands(*root);
    insert_interpreter_commands(*root);
    insert_frame_commands(*root);
    insert_step_commands(*root);
    insert_whatis_commands(*root);
    insert_blockmap_commands(*root);
    insert_break_commands(*root);
    insert_script_commands(*root);
    insert_list_commands(*root);
    insert_threads_commands(*root);
    insert_env_commands(*root);
    insert_proc_commands(*root);
    insert_comp_commands(*root);
    insert_go_commands(*root);
    insert_procset_commands(*root);
    insert_utils_commands(*root);

    return root;
}

void MiCommandsFactory::insert_class_commands(CommandNode& root) {
    // insert a fake help command so we get the niceness of the command tree
    // matching "h", "he", etc. to "help". printing the actual help requires
    // context only available in the interpreter itself, so the interpreter
    // will catch a match to this command and short circuit to the help output.
    auto dummy_help = []() -> int {
        return Command::Failure;
    };
    root.addSubCmd({
        .name = "-help", .ptype = proc_ignore,
        .fn = dummy_help, .help = cmd_help_help,
        .desc = "Display help information about commands."});

    root.addSubCmd("-gdb-show", nullptr, proc_ignore,
                   nullptr, class_show_help,
                   "Show information about the debugger environment.");
}

void MiCommandsFactory::insert_dummy_commands(CommandNode& root) {
    static const std::pair<std::string, std::string> dummy_map[] = {
         {"-data-list-register-names", "register-names=[]"},
         {"-enable-frame-filters", ""},
         {"-list-features", "features=[]"},
         {"-list-target-features", "features=[async]"},
    };

    const auto make_dummy = [](const std::string& value) {
        return [value](MiInterpreter& mi) -> int {
            if (value.empty()) {
                return Command::Success;
            } else {
                mi.set_result("%s", value.c_str());
                return Command::Success;
            }
        };
    };

    for (const auto& [key, value] : dummy_map) {
        root.addSubCmd({ .name = key, .ptype = proc_ignore, .fn = make_dummy(value) });
    }
}

Interpreter::Completions
MiInterpreter::complete_text(std::string_view /*partial_word*/, std::string_view /*buffer*/) const {
    // unimplemented
    return {};
}
