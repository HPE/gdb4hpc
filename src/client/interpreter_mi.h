// Copyright 2023 Hewlett Packard Enterprise Development LP.
//
// Mi Interpreter
//
// Interprets machine readable commands and outputs machine readable results
// formatted to the GDB/MI standard with extensions for ccdb
#pragma once

#include <memory>

#include "interpreter.h"
#include "interpreter_console.h"
#include "cmd.h"
#include "cli_funcs.h"
#include "str.h"

class MiInterpreter : public Interpreter {
public:
    MiInterpreter(cli_stream_t& io);
    ~MiInterpreter() override = default;

    // interpreter interface:

    std::string_view name() const override;
    std::string complete_text(std::string_view line) const override; // unimplemented
    bool can_interpret(std::string_view line) const override;
    bool interpret(std::string_view line) override;
    Interpreter::Printer& get_printer() override;

    // mi specific:

    // Set the result of the mi command. This is different from print_message.
    // print_message will immediately output a message with message format (~...),
    // this function sets the result that is attached to ^done.
    void set_result(const char* fmt, ...) __attribute__((format (printf,2,3)));

    void attach_observers();

    // XXX: the public member variables here used to be global. the usage pattern should
    // be improved, but just preserving old behavior for now.

    std::string m_launcher_args;
    std::string m_num_ranks;
    std::string m_procset;
private:
    bool try_interpret(std::string_view line);
    bool print_help();

    std::shared_ptr<CommandNode> m_mi_commands;
    ConsoleInterpreter m_console_intr;

    // set by set_result/print_error, used to output ^done/^error,... after interpret.
    MallocedCharPtr m_result;    // for ^done
    bool m_should_print_running; // for ^running
    std::string m_error;         // for ^error

    // observers
    std::vector<std::unique_ptr<ObservableConnection>> m_obsv_conns;
    void handle_setable_param_changed(std::string_view name, std::string_view new_value);
    void handle_async_event(DbgInvoke::EventSet &ev_set, const Bitset &reply_set);
    void handle_target_resumed();

    class Printer : public Interpreter::Printer {
    public:
        ~Printer() override = default;

        // documented in interpreter.h
        void print_message_v(const char* fmt, va_list args) override;
        void print_error_v(const char* fmt, va_list args) override;
        void print_inferior_output(const DbgInvoke& di, std::string_view line, FILE* use_instead = nullptr) override;
        std::ostream& get_message_stream() override;

        // forward a string to print_message
        void print_message_string(const char* str, size_t len);

        void print_unfiltered(const char* fmt, ...) __attribute__((format (printf,2,3)));

        // *stopped,reason="breakpoint-hit",...
        void print_stopped_breakpoint(std::string_view bkpt_id,
                                      std::string_view proc_name,
                                      const Bitset &group,
                                      std::string_view func,
                                      std::string_view file,
                                      std::string_view fullpath, int line);

        // *stopped,reason="end-stepping-range",...
        void print_stopped_step(std::string_view proc_name, const Bitset &group, int threadId,
                                std::string_view func, std::string_view file,
                                std::string_view fullpath, int line);

        // *stopped,reason="exited-normally",...
        void print_stopped_exit(std::string_view proc_name, const Bitset &proc_group);

    private:
        Printer(MiInterpreter& parent, cli_stream_t& io);
        friend MiInterpreter::MiInterpreter(cli_stream_t& io);

        class StreamBuf : public std::streambuf {
        public:
            StreamBuf(Printer& printer);
            std::streamsize xsputn(const char* s, std::streamsize n) override;
            int overflow(int c = EOF) override;
        private:
            Printer& m_printer;
        };

        MiInterpreter& m_intr;
        cli_stream_t& m_io;
        std::unique_ptr<StreamBuf> m_buff;
        std::unique_ptr<std::ostream> m_ostream;
        std::ostringstream m_message_ss;
        std::ostringstream m_error_ss;
    };

    Printer m_printer;
};

class MiCommandsFactory {
public:
    static std::shared_ptr<CommandNode> make();
private:
    // internal or base commands that don't live in cmd_*.cpp files.
    // e.g. the base "maint" node which all other maint commands are under.
    static void insert_class_commands(CommandNode& root);

    // dummy commands that don't actually do anything except return a fake "ok" result
    static void insert_dummy_commands(CommandNode& root);

    // in mi_funcs.cpp
    static void insert_mi_commands(CommandNode& root);

    // these are defined in their respective cmd_*.cpp files
    static void insert_interpreter_commands(CommandNode& root);
    static void insert_frame_commands(CommandNode& root);
    static void insert_step_commands(CommandNode& root);
    static void insert_whatis_commands(CommandNode& root);
    static void insert_blockmap_commands(CommandNode& root);
    static void insert_break_commands(CommandNode& root);
    static void insert_list_commands(CommandNode& root);
    static void insert_threads_commands(CommandNode& root);
    static void insert_script_commands(CommandNode& root);
    static void insert_print_commands(CommandNode& root);
    static void insert_env_commands(CommandNode& root);
    static void insert_proc_commands(CommandNode& root);
    static void insert_comp_commands(CommandNode& root);
    static void insert_go_commands(CommandNode& root);
    static void insert_procset_commands(CommandNode& root);
    static void insert_utils_commands(CommandNode& root);
};
