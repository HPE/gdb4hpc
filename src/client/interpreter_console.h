// Copyright 2023 Hewlett Packard Enterprise Development LP.
//
// Console Interpreter
//
// Interprets human readable commands and outputs human readable results
#pragma once

#include <memory>

#include "cmd.h"
#include "cli_funcs.h"
#include "observable.h"

class ConsoleInterpreter : public Interpreter {
public:
    ConsoleInterpreter(cli_stream_t& io);
    ~ConsoleInterpreter() override = default;

    // interpreter interface:

    std::string_view name() const override;
    std::string complete_text(std::string_view line) const override; // unimplemented
    bool can_interpret(std::string_view line) const override;
    bool interpret(std::string_view line) override;
    Interpreter::Printer& get_printer() override;

    // console specific:

    // interpret will use the interpreter on the top of the command stack.
    // push/pop_commands modify the stack and are called by functions that
    // change the command set. examples of such commands are gdbmode, decomp,
    // and build.
    void push_commands(std::shared_ptr<Command> root);
    void pop_commands();

    void attach_observers();
private:
    bool print_help(std::string_view line);

    std::vector<std::shared_ptr<Command>> m_commands_stack;

    // observers
    std::vector<std::unique_ptr<ObservableConnection>> m_obsv_conns;
    void handle_async_event(DbgInvoke::EventSet &ev_set, const Bitset &reply_set);

    class Printer : public Interpreter::Printer {
    public:
        ~Printer() override = default;

        // documented in interpreter.h
        void print_message_v(const char* fmt, va_list args) override;
        std::ostream& get_message_stream() override;
        void print_error_v(const char* fmt, va_list args) override;
        void print_inferior_output(const DbgInvoke& di, std::string_view line, FILE* use_instead = nullptr) override;

        // console specific
        void print_with_set_info(const char* set_name, const Bitset& group, const char* fmt, ...) __attribute__((format (printf,4,5)));
        void print_error_with_set_info(const char* set_name, const Bitset& group, const char* fmt, ...) __attribute__((format (printf,4,5)));
        void print_event_location(const DbgInvoke::EventSet& ev_set);
    private:
        Printer(ConsoleInterpreter& parent, cli_stream_t& io);
        friend ConsoleInterpreter::ConsoleInterpreter(cli_stream_t& io);

        ConsoleInterpreter& m_intr;
        cli_stream_t& m_io;

        class StreamBuf : public std::streambuf {
        public:
            StreamBuf(Printer& printer);
            std::streamsize xsputn(const char* s, std::streamsize n) override;
            int overflow(int c = EOF) override;
        private:
            Printer& m_printer;
        };

        std::unique_ptr<StreamBuf> m_buff;
        std::unique_ptr<std::ostream> m_ostream;
    };

    Printer m_printer;
};

class ConsoleCommandsFactory {
public:
    static std::shared_ptr<Command> make();
private:
    // internal or base commands that don't live in cmd_*.cpp files.
    // e.g. the base "set" node which all other set commands are under.
    static void insert_class_commands(Command& root);

    // these are defined in their respective cmd_*.cpp files
    static void insert_interpreter_commands(Command& root);
    static void insert_frame_commands(Command& root);
    static void insert_step_commands(Command& root);
    static void insert_passthru_commands(Command& root);
    static void insert_script_commands(Command& root);
    static void insert_blockmap_commands(Command& root);
    static void insert_env_commands(Command& root);
    static void insert_python_commands(Command& root);
    static void insert_assign_commands(Command& root);
    static void insert_checkpoint_commands(Command& root);
    static void insert_source_commands(Command& root);
    static void insert_whatis_commands(Command& root);
    static void insert_break_commands(Command& root);
    static void insert_proc_commands(Command& root);
    static void insert_list_commands(Command& root);
    static void insert_iostat_commands(Command& root);
    static void insert_threads_commands(Command& root);
    static void insert_print_commands(Command& root);
    static void insert_comp_commands(Command& root);
    static void insert_go_commands(Command& root);
    static void insert_procset_commands(Command& root);
};
