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

std::string MiInterpreter::complete_text(std::string_view line) const {
    // Completion is unimplemented for this interpreter
    return std::string{line};
}

bool MiInterpreter::can_interpret(std::string_view line) const {
    auto find_result = m_mi_commands->findCmd(line);
    return std::get_if<Command::Parts>(&find_result) != nullptr
        || m_console_intr.can_interpret(line);
}

bool MiInterpreter::interpret(std::string_view line) {
    m_result.reset();
    m_should_print_running = false;
    m_error = "";

    const auto result = try_interpret(line);

    // TODO implement command prefixes internally, without cli_get_prefix()

    if (result) {
        auto leader = m_should_print_running ? "^running" : "^done";
        if (m_result) {
            PrintOutUnformatted("%s%s,%s\n", cli_get_prefix().c_str(), leader, m_result.get());
        } else {
            PrintOutUnformatted("%s%s\n", cli_get_prefix().c_str(), leader);
        }
    } else {
        if (!m_error.empty()) {
            PrintOutUnformatted("%s^error,msg=\"%s\"\n", cli_get_prefix().c_str(), m_error.c_str());
        } else {
            // ideally this should never happen.
            PrintOutUnformatted("%s^error,msg=\"Unknown error\"\n", cli_get_prefix().c_str());
        }
    }
    PrintOutUnformatted("(gdb) \n");
    return result;
}

bool MiInterpreter::try_interpret(std::string_view line) {
    line = stripped(line);
    log("interpreting \"%.*s\"", (int)line.size(), line.data());
    if (line.empty() || line.starts_with("#")) {
        return true;
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
    auto ctx = Command::RunContext{ .caller = this };

    if (auto* parts = std::get_if<Command::Parts>(&find_result)) {
        auto& [cmd, args] = *parts;
        if (!cmd) {
            log("found in tree, but no function pointer?");
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

        if (cmd->execute(ctx, procset, argv.argc(), argv.c_strs().data()) == Command::Success) {
            log("command successfull");
            return true;
        } else {
            log("command returned an error");
            return false;
        }
    } else if (m_console_intr.can_interpret(untagged_line)) {
        log("forwarding to console interpreter");
        return m_console_intr.interpret(untagged_line);
    } else {
        log("unknown to both mi and console interpreter");
        static_assert(std::variant_size_v<decltype(find_result)> == 2, "Assuming there are only two alternatives");
        m_printer.print_error("%s\n", std::get<Command::ErrStr>(find_result).c_str());
        return false;
    }
}

void MiInterpreter::set_result(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char *cstr = nullptr;
    vasprintf(&cstr, fmt, args);
    m_result.reset(cstr);

    va_end(args);
}

void MiInterpreter::attach_observers() {
    m_obsv_conns.emplace_back(
        observers::setable_param_changed.attach(
            [this](std::string_view name, std::string_view value) {
                log("observing setable_param_changed");
                return handle_setable_param_changed(name, value);
            }
        )
    );

    m_obsv_conns.emplace_back(
        observers::async_event.attach(
            [this](DbgInvoke::EventSet& ev_set, const Bitset& reply_set) {
                log("observing async_event");
                return handle_async_event(ev_set, reply_set);
            }
        )
    );

    m_obsv_conns.emplace_back(
        observers::target_resumed.attach(
            [this]() {
                log("observing target_resumed");
                return handle_target_resumed();
            }
        )
    );
}

void MiInterpreter::handle_setable_param_changed(std::string_view name, std::string_view new_value) {
    PrintOutUnformatted("=cmd-param-changed,param=\"%.*s\",value=\"%.*s\"\n",
                        (int)name.size(), name.data(),
                        (int)new_value.size(), new_value.data());
}

void MiInterpreter::print_stopped_msg(StoppedType type,const std::string& file, const std::string& func,
                       int line, const std::string& bkpt_id, const std::string& proc_name,
                       const Bitset * proc_group, const std::string& fullpath){
    std::ostringstream ss;
    std::string group = proc_group->toSetString();
    switch(type){
        case StoppedType::BKPT_HIT:{
            ss << "reason=\"breakpoint-hit\",bkptno=\"" <<bkpt_id
               << "\",proc_set=\"" << proc_name
               << "\",group=\"" << group.substr(1,group.length()-2)
               << "\",frame={func=\"" << func
               << "\",args=[],file=\"" << file
               << "\",fullname=\""<< fullpath
               << "\",line=\"" << line << "\"}";
            break;
        }
        case StoppedType::END_RANGE:{
            ss << "reason=\"end-stepping-range\",proc_set=\""<<proc_name
               << "\",group=\"" << group.substr(1,group.length()-2)
               << "\",frame={func=\"" << func
               << "\",args=[],file=\""<< file
               << "\",fullname=\""<< fullpath
               << "\",line=\""<<line << "\"}";
            break;
        }
        case StoppedType::EXIT:{
            ss << "reason=\"exited-normally\", proc_set=\""<<proc_name
                << "\",group=\"" << group.substr(1,group.length()-2) << "\"";
            break;
        }
    }
    PrintOutUnformatted("*stopped,%s\n", ss.str().c_str());
}

void MiInterpreter::handle_async_event(DbgInvoke::EventSet& ev_set, const Bitset& /*reply_set*/) {
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
                    print_stopped_msg(StoppedType::EXIT, "", "", 0, "",ev_set.di().handle().c_str(),&ev_set.group(),"");
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
                        print_stopped_msg(StoppedType::BKPT_HIT, bpset->file(), bpset->function(), 
                                          bpset->line(), std::to_string(bpset->id()), ev_set.di().handle().name(),
                                          &ev_set.group(), ev_set.di().sourcePaths().getFullPath(bpset->file()));
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
                        print_stopped_msg(StoppedType::BKPT_HIT, bpset->file(), bpset->function(), bpset->line(), 
                                        std::to_string(bpset->id()), ev_set.di().handle().name(),&ev_set.group(),
                                        ev_set.di().sourcePaths().getFullPath(bpset->file()));
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
            const dbgframe_t *frame = ev_set.event()->ev_step.step_frame;
            if (frame) {
                const auto &frameLoc = frame->getLoc();
                auto file = frameLoc.loc_file, frame = frameLoc.loc_func;
                print_stopped_msg(StoppedType::END_RANGE, file ? file : "unknown", frame ? frame : "unknown",
                                  frameLoc.loc_line, "", ev_set.di().handle().name(), &ev_set.group(), ev_set.di().sourcePaths().getFullPath(file));
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
                print_stopped_msg(StoppedType::BKPT_HIT, loc.loc_file,
                                  loc.loc_func, loc.loc_line, "0",
                                  ev_set.di().handle().name(), &ev_set.group(),
                                  ev_set.di().sourcePaths().getFullPath(loc.loc_file));
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
    PrintOutUnformatted("*running,thread-id=\"all\"\n");
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
    char *cstr = nullptr;
    int num_bytes = vasprintf(&cstr, format, arg_list);
    if (num_bytes < 0) return;
    print_message_string(cstr, num_bytes);
    free(cstr);
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
            m_io.cs_out("%s", m_message_ss.str().c_str());
            m_message_ss.str("");
            m_message_ss << "~\"";
            break;
        default: m_message_ss << cstr[i];
        }
    }
}

void MiInterpreter::Printer::print_error_v(const char* format, va_list arg_list) {
    // XXX: the MI standard only allows the output of one error per command, so
    // we only use the first error produced.
    if (!m_intr.m_error.empty()) return;

    // the arg_list may well contain quotes or newlines, so we have to
    // just get the final output form so we can know they're being properly
    // escaped.
    char *char_buf = nullptr;
    if (vasprintf(&char_buf, format, arg_list) < 0) return;
    auto cstr = MallocedCharPtr(char_buf);

    for (const auto *p = cstr.get(); *p; ++p) {
        switch (*p) {
        case '"': m_error_ss << "\\\""; break;
        case '\n':
            // save the result in the parent interpreter
            m_intr.m_error = std::move(m_error_ss).str();
            m_error_ss.str("");
            break;
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

std::shared_ptr<Command> MiCommandsFactory::make() {
    auto root = std::make_shared<Command>("", nullptr, proc_optional, nullptr, nullptr, nullptr);

    insert_class_commands(*root);
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
    insert_print_commands(*root);
    insert_env_commands(*root);
    insert_proc_commands(*root);
    insert_comp_commands(*root);
    insert_go_commands(*root);
    insert_procset_commands(*root);

    return root;
}

static void insert_setshow_commands(Command& root, std::string set, std::string show) {
    root.addSubCmd(set, nullptr, proc_ignore,
                   nullptr, class_set_help,
                   "Set information about the debugger environment.");

    root.addSubCmd(show, nullptr, proc_ignore,
                   nullptr, class_show_help,
                   "Show information about the debugger environment.");
}

void MiCommandsFactory::insert_class_commands(Command& root) {
    // insert a fake help command so we get the niceness of the command tree
    // matching "h", "he", etc. to "help". printing the actual help requires
    // context only available in the interpreter itself, so the interpreter
    // will catch a match to this command and short circuit to the help output.
    auto dummy_help = [](Command::RunContext&, rec_set_t*, int, const char* const []) -> int {
        return Command::Failure;
    };
    root.addSubCmd({
        .name = "-help", .ptype = proc_ignore,
        .fn = dummy_help, .help = cmd_help_help,
        .desc = "Display help information about commands."});

    insert_setshow_commands(root, "-gdb-set", "-gdb-show");
}

void MiCommandsFactory::insert_dummy_commands(Command& root) {
    static const std::pair<std::string, std::string> dummy_map[] = {
         {"-data-list-register-names", "register-names=[]"},
         {"-enable-frame-filters", ""},
         {"-list-features", "features=[]"},
         {"-list-target-features", "features=[async]"},
    };

    const auto make_dummy = [](const std::string& value) {
        return [value](Command::RunContext& c, rec_set_t*, int, const char* const []) -> int {
            if (auto* mi = dynamic_cast<MiInterpreter*>(c.caller)) {
                if (value.empty()) {
                    return Command::Success;
                } else {
                    mi->set_result("%s", value.c_str());
                    return Command::Success;
                }
            } else {
                // unreachable as long as the dummy commands are only attached
                // to an mi interpreter.
                PrintOutError("Interpreter error\n");
                return Command::Failure;
            }
        };
    };

    for (const auto& [key, value] : dummy_map) {
        root.addSubCmd({ .name = key, .ptype = proc_ignore, .fn = make_dummy(value) });
    }
}
