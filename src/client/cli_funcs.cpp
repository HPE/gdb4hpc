/******************************************************************************\
 * cli_funcs.c - Command line interface routines.
 *
 * Copyright 2011-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#include "config.h"

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <iostream>
#include <deque>

#include "client.h"
#include "useful.h"

#include "interpreter.h"

/*
** Static internal definitions
*/
static cli_stream_t *NewCLIStream(cli_type_t, interpreter_type, cli_stream_args_t *);
static void cli_handle_error(void);
static void ProgOutLineStream(DbgInvoke *, FILE *);
static void ProgOutLineStrings(DbgInvoke *, const std::vector<std::string> &lines);

/*
 * Static Global Variables
 */
static cli_stream_t *CurrentStream; /* Current cli stream */
static std::vector<cli_stream_t*> AllStreams;
static std::deque<std::string> pendingCommandQueue{}; /* Commands queued, waiting to be executed */

// connection to interpreter printing functions
Interpreter::Printer& cli_get_current_printer() {
    return CurrentStream->cs_current_interpreter->get_printer();
}

void InitCLI(cli_type_t default_type, interpreter_type interpreter,
             cli_stream_args_t *default_args) {
    cli_stream_t *stream = NULL;

    // Create the cli stream
    // Note that we set the default prompt information here, that why we have
    // the need for the switch statement
    switch (default_type) {
        case CLI_NOTTY_STREAM:
            // There are no prompts for tty-less streams
            stream = NewCLIStream(CLI_NOTTY_STREAM, interpreter, default_args);
            break;

        case CLI_TTY_STREAM:
            stream =
                NewCLIStream(CLI_TTY_STREAM, interpreter, default_args);
            break;

        case CLI_SOCKET_STREAM:
            stream = NewCLIStream(CLI_SOCKET_STREAM, interpreter, default_args);
            break;

        case CLI_FILE_STREAM:
            stream = NewCLIStream(CLI_FILE_STREAM, interpreter, default_args);
            break;
    }
    stream->cs_num = 0;

    // Make sure the call didn't fail, if so its fatal. We cannot continue
    // without a default stream
    if (stream == NULL) {
        PrintOutError("Fatal: InitCLI() failed.\n");
        exit(1);
    }

    // Create the stream queue
    AllStreams.push_back(stream);
    DbgSwitchCurrentStream(stream);

    // Install the output callback handler in the dbg_client layer
    DbgHandleOutput(ProgOutLineStream);  // handle FILE *
    DbgHandleOutput(ProgOutLineStrings); // handle vector<string>

    // Install the input callback handler in the dbg_client layer
    DbgHandleInput(CurrentStream->cs_fdin, CurrentStream, cli_handle_input);
}

void DbgSwitchCurrentStream(cli_stream_t* stream){

    if (stream != CurrentStream) {
        if (CurrentStream) CurrentStream->cs_out.flush();
        CurrentStream = stream;
    }
}

static std::shared_ptr<Interpreter> getOrCreateInterpreter(std::string_view name) {
    const auto name_str = std::string{name};

    if (auto iter = CurrentStream->cs_interpreters.find(name_str); iter != CurrentStream->cs_interpreters.end()) {
        return iter->second;
    }

    // not found, so try to create it. we create the new interpreter with async disabled
    // so that it stays silent during async messages (see #IgnoreAsync in client_io.md)
    if (auto new_intr = Interpreter::make(name, *CurrentStream, { .async = false })) {
        auto [iter, success] = CurrentStream->cs_interpreters.insert({name_str, new_intr});
        // success should always be true after the .contains check above
        return iter->second;
    } else {
        return nullptr;
    }
}

bool DbgInterpreterExec(std::string_view interpreter_name, std::string_view text) {
    if (auto intr = getOrCreateInterpreter(interpreter_name)) {
        return intr->interpret(text);
    }

    PrintOutError("No such interpreter: %*s\n", (int)interpreter_name.size(), interpreter_name.data());
    return false;
}

bool DbgInterpret(std::string_view text) {
    // cs_current_interpreter will never be null after the cli_stream_t ctor
    return CurrentStream->cs_current_interpreter->interpret(text);
}

Interpreter::Completions DbgComplete(std::string_view partial_word, std::string_view buffer) {
    // cs_current_interpreter will never be null after the cli_stream_t ctor
    return CurrentStream->cs_current_interpreter->complete_text(partial_word, buffer);
}

std::string DbgGetPrompt() {
    return CurrentStream->cs_current_interpreter->get_prompt();
}

bool DbgInterpreterSet(std::string_view name) {
    if (auto intr = getOrCreateInterpreter(name)) {
        CurrentStream->cs_current_interpreter = intr;
        return true;
    }

    PrintOutError("No such interpreter: %*s\n", (int)name.size(), name.data());
    return false;
}

void DbgQueueCommand(const std::string &command) {
    // Don't queue pure whitespace
    if (command.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
        pendingCommandQueue.push_back(command);
    }
}

size_t DbgExecuteQueuedCommands() {
    const auto r = pendingCommandQueue.size();

    if (r == 0) return 0;

    // preserve prompt
    cli_save_stream();

    while (pendingCommandQueue.size()) {
        auto command = std::move(pendingCommandQueue.front());
        pendingCommandQueue.pop_front();
        if (!DbgInterpret(command.c_str())) {
            PrintOutError("Command failed: %s\n", command.c_str());
            if (!CurrentStream->keep_going()) break;
        }
    }

    cli_restore_stream();

    return r;
}

void DestroyCLI(void) {
    // Destroy the stream queue
    for (auto s : AllStreams)
        delete s;
    AllStreams.clear();

    // Null out the global variables
    CurrentStream = nullptr;
}

int SetNewCLIStream(cli_type_t type, interpreter_type interpreter, cli_stream_args_t *args) {
    cli_stream_t *stream;

    // clear the current line this should contain a prompt
    cli_clear_line();

    stream = new cli_stream_t(type, interpreter, args);

    // Make sure the call didn't fail
    if (stream == NULL) {
        return 1;
    }
    stream->cs_num = int(AllStreams.size());
    // Enqueue the existing command stream
    AllStreams.push_back(stream);
    DbgSwitchCurrentStream(stream);

    // call the stream restore function
    cli_reset_stream();
    // Reset the input handler in the dbg_client layer
    DbgHandleInput(CurrentStream->cs_fdin, CurrentStream, cli_handle_input);

    return 0;
}

void DestroyCurrentCLIStream(void) {
    if (!CurrentStream) return;

    // Destroy the current stream
    // clear the current line this should contain a prompt
    cli_clear_line();
    AllStreams.erase(std::remove(AllStreams.begin(), AllStreams.end(), CurrentStream),
                     AllStreams.end());
    delete CurrentStream;
    CurrentStream = nullptr;

    // reset to the default
    SetDefaultCLIStream();
}

void SetDefaultCLIStream(void) {
    if (AllStreams.empty()) {
        Client.quit = 1;
        return;
    }

    // The first stream is the default
    DbgSwitchCurrentStream(AllStreams[0]);

    // LOOKAT: I'm pretty sure this should be happening more generally
    
    // call the stream restore function
    // Note that this should always be done. Editline gets messed up if the
    // reset isn't called after signal handling for some reason (it would be in
    // line edit mode).
    cli_reset_stream();
}

static cli_stream_t *NewCLIStream(cli_type_t type, interpreter_type interpreter, cli_stream_args_t *args) {
    return new cli_stream_t(type, interpreter, args);
}

cli_stream_t::cli_stream_t(cli_type_t type, interpreter_type interpreter, cli_stream_args_t *args)
    : cs_type(type)
    , cs_args(args)
    , cs_logging(cs_out)
{
    // create interpreter
    {
        auto intr = std::shared_ptr<Interpreter>{};

        switch (interpreter) {
            case interpreter_type::CLI: intr = Interpreter::make("console", *this, { .async = true }); break;
            case interpreter_type::MI: intr = Interpreter::make("mi", *this, { .async = true }); break;
        }

        if (!intr) throw std::runtime_error("Invalid interpreter");

        cs_current_interpreter = intr;
        cs_interpreters.insert({std::string{intr->name()}, intr});
    }

    switch (type) {
        case CLI_NOTTY_STREAM:
            // TODO: Create an external definition for no tty if needed.
        case CLI_TTY_STREAM:
            cs_ext_funcs = cli_get_readline_funcs();
            break;

        case CLI_SOCKET_STREAM:
            cs_ext_funcs = cli_get_socket_funcs();
            break;

        case CLI_FILE_STREAM:
            cs_ext_funcs = cli_get_file_funcs();
            break;
    }


    // An external init function is required since these set the the fp and fd
    // members of the stream
    if (!cs_ext_funcs->external_init) {
        PrintOutError(
            "Error: No external_init() function defined for cli stream "
            "type.\n");
        // set the external destroy function to null since it wasn't able to
        // init
        cs_ext_funcs->external_destroy = nullptr;
        return;
    }

    // call to external init function
    auto tp = this;
    if ((*cs_ext_funcs->external_init)(&tp)) {
        PrintOutError("Error: cli external_init() failed.\n");
    }
}

cli_stream_t::~cli_stream_t() {
    DbgUnhandleInput(this);

    // call to external destroy function
    if (cs_ext_funcs->external_destroy) {
        auto cs = this;
        if ((*cs_ext_funcs->external_destroy)(&cs)) {
            PrintOutError("Warning: external_destroy() failed.\n");
        }
    } else {
        // Free it
        free(cs_ext_funcs);
    }

    DestroyCLIArgs(cs_type, cs_args);
}

bool cli_stream_t::echo() const {
    // the only streams that implement echoing are file streams
    switch (cs_type) {
        case CLI_FILE_STREAM: return cs_args->file_args.echo;
        default: return false;
    }
}

bool cli_stream_t::keep_going() const {
    // the only streams that implement keep going are file streams
    switch (cs_type) {
        case CLI_FILE_STREAM: return cs_args->file_args.keep_going;
        default: return false;
    }
}

cli_stream_args_t *NewCLIArgs(cli_type_t type, ...) {
    cli_stream_args_t *rtn;
    va_list args;
    char *tmpstr;

    // Create and initialize the return type
    rtn = (cli_stream_args_t *)Alloc(sizeof(cli_stream_args_t));
    memset(rtn, 0, sizeof(cli_stream_args_t));

    va_start(args, type);

    switch (type) {
        case CLI_NOTTY_STREAM:
            rtn->notty_in = va_arg(args, FILE *);
            rtn->notty_out = va_arg(args, FILE *);
            rtn->notty_err = va_arg(args, FILE *);
            break;

        case CLI_TTY_STREAM:
            rtn->tty_in = va_arg(args, FILE *);
            rtn->tty_out = va_arg(args, FILE *);
            rtn->tty_err = va_arg(args, FILE *);
            break;

        case CLI_SOCKET_STREAM:
            if ((tmpstr = va_arg(args, char *)) != NULL) {
                rtn->socket_host = strdup(tmpstr);
            }
            if ((tmpstr = va_arg(args, char *)) != NULL) {
                rtn->socket_port = strdup(tmpstr);
            }
            if ((tmpstr = va_arg(args, char *)) != NULL) {
                rtn->socket_error_port = strdup(tmpstr);
            }
            break;

        case CLI_FILE_STREAM:
            if ((tmpstr = va_arg(args, char *)) != NULL) {
                rtn->file_in_path = strdup(tmpstr);
            }
            if ((tmpstr = va_arg(args, char *)) != NULL) {
                rtn->file_out_path = strdup(tmpstr);
            }
            if ((tmpstr = va_arg(args, char *)) != NULL) {
                rtn->file_err_path = strdup(tmpstr);
            }

            rtn->file_args.fallback_stream = CurrentStream; // nullptr ok

            // cli_option is promoted to int when passed in va_arg
            auto echo_opt = static_cast<cli_option>(va_arg(args, int));
            auto keep_going_opt = static_cast<cli_option>(va_arg(args, int));

            auto echo_default = CurrentStream ? CurrentStream->echo() : false;
            auto keep_going_default = CurrentStream ? CurrentStream->keep_going() : false;

            rtn->file_args.echo = echo_opt == CLI_INHERIT ? echo_default : static_cast<bool>(echo_opt);
            rtn->file_args.keep_going = keep_going_opt == CLI_INHERIT ? keep_going_default : static_cast<bool>(keep_going_opt);
            break;
    }

    va_end(args);

    return rtn;
}

void DestroyCLIArgs(cli_type_t type, cli_stream_args_t *args) {
    switch (type) {
        case CLI_NOTTY_STREAM:
        case CLI_TTY_STREAM:
            Free(args);
            break;

        case CLI_SOCKET_STREAM:
            Free(args->socket_host);
            Free(args->socket_port);
            Free(args->socket_error_port);
            Free(args);
            break;

        case CLI_FILE_STREAM:
            if (args->file_in_path != NULL) Free(args->file_in_path);
            if (args->file_out_path != NULL) Free(args->file_out_path);
            if (args->file_err_path != NULL) Free(args->file_err_path);
            Free(args);
            break;
    }
}

void cli_handle_input(void) {
    int rtn;

    if (CurrentStream == NULL) return;

    // ensure that there is a handle input function, otherwise we have no way
    // of reading input and should quit
    if (CurrentStream->cs_ext_funcs->external_handle_input == NULL) {
        PrintOutError(
            "Fatal: external_handle_input() is null. Avoiding deadlock.\n");
        Client.quit = 1;
        return;
    }

    // call to external handle input function
    rtn = (*CurrentStream->cs_ext_funcs->external_handle_input)(&CurrentStream);

    if (rtn) {
        // Call the error handling function to figure out what to do
        cli_handle_error();
    }
}

static void cli_handle_error(void) {
    int rtn;

    if (CurrentStream == NULL) return;

    // call to external handle error function
    // Note that this may close the CurrentStream and reset it to the
    // DefaultStream.
    if (CurrentStream->cs_ext_funcs->external_handle_error != NULL) {
        rtn = (*CurrentStream->cs_ext_funcs->external_handle_error)(
            &CurrentStream);
        if (rtn) {
            PrintOutError("Warning: external_handle_error() failed.\n");
        }
    }
}

void cli_save_stream(void) {
    if (CurrentStream == NULL) return;

    // not thread safe
    // If we currently have a stream saved, return.
    // We don't want to save a stream without first restoring.
    if (CurrentStream->cs_saved) return;

    CurrentStream->cs_saved = true;

    // call to external save stream function
    if (CurrentStream->cs_ext_funcs->external_save_stream != NULL) {
        int rtn = (*CurrentStream->cs_ext_funcs->external_save_stream)(
            &CurrentStream);
        if (rtn) {
            // hopefully unreachable. his needs to go directly through the
            // printer since PrintOutMsg calls cli_save_stream.
            cli_get_current_printer().print_message("Warning: external_save_stream() failed.\n");
        }
    }
}

void cli_restore_stream(void) {
    if (CurrentStream == NULL) return;

    // ensure there is a saved stream.
    if (!CurrentStream->cs_saved) return;

    // call to external restore stream function
    if (CurrentStream->cs_ext_funcs->external_restore_stream != NULL) {
        int rtn = (*CurrentStream->cs_ext_funcs->external_restore_stream)(
            &CurrentStream);
        if (rtn) {
            // hopefully unreachable. this needs to go directly through the
            // printer since PrintOutMsg calls cli_restore_stream.
            cli_get_current_printer().print_message("Warning: external_restore_stream() failed.\n");
        }
    }

    CurrentStream->cs_saved = false;
}

void cli_reset_stream(void) {
    int rtn;
    if (CurrentStream == NULL) return;

    // call to external reset stream function
    if (CurrentStream->cs_ext_funcs->external_reset_stream != NULL) {
        rtn = (*CurrentStream->cs_ext_funcs->external_reset_stream)(
            &CurrentStream);
        if (rtn) {
            PrintOutError("Warning: external_reset_stream() failed.\n");
        }
    }
}

void cli_clear_line(void) {
    int rtn;

    if (CurrentStream == NULL) return;

    // call to external clear line function
    if (CurrentStream->cs_ext_funcs->external_clear_line != NULL) {
        rtn =
            (*CurrentStream->cs_ext_funcs->external_clear_line)(&CurrentStream);
        if (rtn) {
            PrintOutError("Warning: external_clear_line() failed.\n");
        }
    }
}

bool cli_is_interactive() {
    // return true if we have a stream, the stream has an input, and it's a tty
    return CurrentStream != NULL &&
        CurrentStream->cs_fpin != NULL &&
        CurrentStream->cs_type == CLI_TTY_STREAM;
}

static const char* cli_query_prompt_fmt_string(cli_query_default default_val) {
    switch (default_val) {
    case cli_query_default::NO:
        return "%s (y or [n]) ";
    case cli_query_default::YES:
        return "%s ([y] or n) ";
    case cli_query_default::NONE:
    default:
        return "%s (y or n) ";
    }
}

static bool cli_query_interactive(const char* s, cli_query_default default_val) {
    // no newline
    PrintOutMsg(cli_query_prompt_fmt_string(default_val), s);

    int rval = -1;
    for (; rval < 0;) {
        int resp = fgetc(CurrentStream->cs_fpin);

        if (resp == 'n' || resp == 'N') {
            rval = 0;
        } else if (resp == 'y' || resp == 'Y') {
            rval = 1;
        } else if (resp == '\n' && default_val != cli_query_default::NONE) {
            rval = (default_val == cli_query_default::YES);
        } else {
            PrintOutMsg("\nPlease answer y or n:\n");
        }

        // don't leave the a dangling CR in the input stream.
        while (resp != '\n' && resp != EOF) {
            resp = fgetc(CurrentStream->cs_fpin);
        }
    }

    PrintOutMsg("\n");
    return rval;
}

static bool cli_query_noninteractive(const char* s, cli_query_default default_val) {
    // no newline
    PrintOutMsg(cli_query_prompt_fmt_string(default_val), s);

    // print a fake answer for logging purposes
    switch (default_val) {
    case cli_query_default::NO:
        PrintOutMsg("n\n");
        break;
    case cli_query_default::YES:
        PrintOutMsg("y\n");
        break;
    case cli_query_default::NONE:
        // if this message is ever printed, it's a bug and the calling function should be
        // fixed to provide a sane default for non-interactive.
        PrintOutMsg("Warning: No default given, assuming y\n");
        break;
    }

    return default_val != cli_query_default::NO;
}

// print prompt s and get a yes/no response - return true if "yes"
// default value indicates which response to use if the user just hits return
//    or NONE if the user must type y or n
// nonInteractiveDefaultVal governs the response if the context it non-interactive
//    i.e. in batch mode or in a "source" command.   In that case return:
//    true if YES,  false if NO, or use defaultVal if nonInteractiveDefaultVal is NONE.
//    If the context is non-interactive and *both* default values are NONE, return true.
bool cli_query(const char *s, cli_query_default defaultVal,
              cli_query_default nonInteractiveDefaultVal) {
    if (!Client.confirm || !cli_is_interactive()) {
        return cli_query_noninteractive(s, nonInteractiveDefaultVal);
    }
    return cli_query_interactive(s, defaultVal);
}

// common code between ProgOutLineStream and ProgOutLineStrings
static void ProgOutLine(const DbgInvoke& di, const char* line) {
    cli_get_current_printer().print_inferior_output(di, line, Client.cli_fpprog);
}

/*
** Output a program line recieved on a program pipe.
*/
static void ProgOutLineStream(DbgInvoke *di, FILE *o_stream) {
    // ensure that there is an output stream to print to in the current stream
    if (!CurrentStream || !CurrentStream->cs_fpprog) return;

    char line[BUFSIZ];

    if (di) {
        while (fgets(line, BUFSIZ, o_stream)) {
            ProgOutLine(*di, line);
        }
    } else {
        while (fgets(line, BUFSIZ, o_stream)) {
            // hopefully unreachable. line is assumed to have newline
            PrintOutMsg("%s", line);
        }
    }
}

static void ProgOutLineStrings(DbgInvoke *di, const std::vector<std::string> &lines) {
    // ensure that there is an output stream to print to in the current stream
    if (!CurrentStream || !CurrentStream->cs_fpprog) return;

    if (di) {
        for (const auto &line : lines) {
            ProgOutLine(*di, line.c_str());
        }
    } else {
        for (const auto &line : lines) {
            // hopefully unreachable. line is assumed to have newline
            PrintOutMsg("%s", line.c_str());
        }
    }
}

void PrintOutUnformatted(const char *format, ...) {
    va_list arg_list;

    va_start(arg_list, format);
    if (CurrentStream) {
        CurrentStream->cs_out.vprintf(format, arg_list);
    } else {
        ::vfprintf(stdout, format, arg_list);
    }
    va_end(arg_list);
}

/*
** fprintf a line to stderr.
*/
void PrintOutError(const char *format, ...) {
    va_list arg_list;

    // setup the va_list
    va_start(arg_list, format);

    vPrintOutError(format, arg_list);

    // end the va_list
    va_end(arg_list);
}

void vPrintOutError(const char *format, va_list arg_list) {
    // ensure that there is an output stream to print to in the current stream. we might
    // not have one yet if this function is called in main before setting up streams.
    if (!CurrentStream || !CurrentStream->cs_fperr) {
        // otherwise default to stderr
        vfprintf(stderr, format, arg_list);
    } else {
        cli_get_current_printer().print_error_v(format, arg_list);
    }
}


/*
** fprintf a line to stdout without saving input
*/
void PrintOutMsg(const char *format, ...) {
    va_list arg_list;

    // setup the va_list
    va_start(arg_list, format);

    vPrintOutMsg(format, arg_list);

    // end the va_list
    va_end(arg_list);
}

void vPrintOutMsg(const char *format, va_list arg_list) {
    // ensure that there is an output stream to print to in the current stream. we might
    // not have one yet if this function is called in main before setting up streams.
    if (CurrentStream) {
        cli_get_current_printer().print_message_v(format, arg_list);
    } else {
        // otherwise default to stdout
        vfprintf(stdout, format, arg_list);
    }
}

std::ostream& OutMsgStream() {
    if (!CurrentStream) return std::cout;
    return cli_get_current_printer().get_message_stream();
}

// The prefix is currently used in MI mode where each input has an
// optional numeric tag this is prepended to any output from that
// command.
const std::string& cli_get_prefix() {
    return CurrentStream->cs_prefix;
}
void cli_set_prefix(std::string prefix)
{
    CurrentStream->cs_prefix = std::move(prefix);
}

cli_out_t::cli_out_t() : m_out(nullptr), m_log(nullptr) {}

void cli_out_t::operator()(const char *format, ...) {
    va_list arg_list;

    // setup the va_list
    va_start(arg_list, format);

    this->vprintf(format, arg_list);

    // end the va_list
    va_end(arg_list);
}

void cli_out_t::vprintf(const char *format, va_list args) {
    // vfprintf the line we received
    if (m_out && m_log) {
        va_list args2;
        va_copy(args2, args);
        vfprintf(m_out.get(), format, args);
        vfprintf(m_log.get(), format, args2);
    } else if (m_out) {
        vfprintf(m_out.get(), format, args);
    } else if (m_log) {
        vfprintf(m_log.get(), format, args);
    }
}

size_t cli_out_t::fwrite(const char* str, size_t len) {
    // forward to main printing function to correctly handle logging
    this->operator()("%.*s", (int)len, str);
    return len;
}

int cli_out_t::putc(int c) {
    // forward to main printing function to correctly handle logging
    this->operator()("%c", (char)c);
    return c;
}

void cli_out_t::reset() {
    m_out = m_log = nullptr;
}

void cli_out_t::flush() {
    if (m_ostream) m_ostream->flush();
    if (m_out) fflush(m_out.get());
    if (m_log) fflush(m_log.get());
}

void cli_out_t::close() {
    m_out.reset();
    m_log.reset();
    m_ostream.reset();
}

void cli_out_t::setOut(FILE* f) {
    auto p = std::shared_ptr<FILE>{};
    if (f) p.reset(f, ::fclose); // without the if (f) it will try to fclose(nullptr)
    setOut(p);
}

void cli_out_t::setOut(std::shared_ptr<FILE> f) {
    flush();
    m_out = f;
    m_ostream.reset();
}

void cli_out_t::setLog(FILE* f) {
    auto p = std::shared_ptr<FILE>{};
    if (f) p.reset(f, &::fclose); // without the if (f) it will try to fclose(nullptr)
    setLog(p);
}

void cli_out_t::setLog(std::shared_ptr<FILE> f) {
    flush();
    m_log = f;
    m_ostream.reset();
}


/*
// Little adapter that takes C++ stream style input and sends it
// to a C file handle
class FileAdapter : public std::streambuf
{
public:
    FileAdapter(FILE* f) : m_f(f) {}
    std::streamsize xsputn(const char* s, std::streamsize n) override;
    int overflow(int c = EOF) override;
private:
    FILE* m_f;
};

std::streamsize FileAdapter::xsputn(const char* s, std::streamsize n) { return fwrite(s,1,n,m_f); }
int FileAdapter::overflow(int c) { return putc(c, m_f); }
*/

// Little adapter that takes C++ stream style input and sends it
// to a C file handle
class FileAdapter2 : public std::streambuf
{
public:
    FileAdapter2(FILE* f1, FILE* f2) : m_f1(f1), m_f2(f2) {}
    std::streamsize xsputn(const char* s, std::streamsize n) override;
    int overflow(int c = EOF) override;
private:
    FILE *m_f1, *m_f2;
};

std::streamsize FileAdapter2::xsputn(const char* s, std::streamsize n) {
    auto r1 = (m_f1 ? fwrite(s,1,n,m_f1) : 0);
    auto r2 = (m_f2 ? fwrite(s,1,n,m_f2) : 0);
    return (m_f1 ? r1 : r2);
}
int FileAdapter2::overflow(int c) {
    int r1 = (m_f1 ? putc(c, m_f1) : 0);
    int r2 = (m_f2 ? putc(c, m_f2) : 0);
    return (m_f1 ? r1 : r2);
}

std::ostream& cli_out_t::ostream()
{
    if (!m_ostream) {
        m_buff = std::make_shared<FileAdapter2>(m_out.get(), m_log.get());
        m_ostream = std::make_shared<std::ostream>(m_buff.get());
    }
    return *m_ostream;
}

/*
** fprintf the current saved error string
*/
void PrintError() { PrintOutError("%s\n", GetError()); }

cli_logging_t& GetLogging()
{
    return CurrentStream->cs_logging;
}

cli_logging_t::cli_logging_t(cli_out_t& out) : m_out(out) {}

void cli_logging_t::setEnabled(bool enabled) {
    if (enabled == m_enabled) {
        return;
    } else if (!enabled) {
        m_out.flush();
        m_out.setLog(nullptr);
        if (m_redirected) {
            // return output to cli
            m_out.setOut(m_redirected);
            m_redirected.reset();
        }
    } else if (!m_filename.empty()) {
        startLog();
    }

    m_enabled = enabled;
}

void cli_logging_t::setFile(const std::string& filename) {
    m_filename = filename;
    if (m_enabled && !filename.empty())
        startLog();
}
void cli_logging_t::startLog()
{
    if (FILE* f = fopen(m_filename.c_str(), m_overwrite ? "w" : "a")) {
        m_out.flush();
        m_out.setLog(f);
        m_enabled = true;
        setRedirect(m_redirect);
    } else {
        PrintOutError("Could not open logfile: %s for writing\n", m_filename.c_str());
    }
}

void cli_logging_t::setOverwrite(bool overwrite) {
    // only takes effect the next time we start logging
    m_overwrite = overwrite;
}

void cli_logging_t::setRedirect(bool redirect) {
    if (m_enabled) {
        // not checking m_redirect because startLog is using this
        // to update the m_out state
        if (redirect && !m_redirected) {
            m_redirected = m_out.getOutSP();
            m_out.setOut(nullptr);
        } else if (!redirect && m_redirected) {
            m_out.setOut(m_redirected);
            m_redirected = nullptr;
        }
    }

    m_redirect = redirect;
}

bool cli_logging_t::isEnabled() const { return m_enabled; }
std::string cli_logging_t::getFile() const { return m_filename; }
bool cli_logging_t::isOverwrite() const { return m_overwrite; }
bool cli_logging_t::isRedirect() const { return m_redirect; }

FILE* cli_logging_t::getOrigOut() const {
    return m_redirected ? m_redirected.get() : m_out.getOut();
}

bool SetProgramOutputFile(const char* file) {
    if (file) {
        if (FILE* f = fopen(file, "a")) {
            if (Client.cli_fpprog) fflush(Client.cli_fpprog);
            else if (CurrentStream->cs_fpprog) fflush(CurrentStream->cs_fpprog);
            Client.cli_fpprog = f;
            return true;
        } else {
            return false;
        }
    } else {
        if (Client.cli_fpprog) fflush(Client.cli_fpprog);
        Client.cli_fpprog = nullptr;
        return true;
    }
}

cli_pipe::cli_pipe(const std::string& command) : m_stream(CurrentStream) {
    if (!m_stream) return;

    // primary output
    auto& out = m_stream->cs_out;
    if (auto prev= out.getOutSP()) {
        if (auto [f,pid] = open(prev.get(), command); f) {
            m_prev = prev;
            m_forkPid = pid;
            out.setOut(f);
        }
    }

    // logging output - forking a separate process for each isn't super
    // elegant, but doing a pipe while logging seems a pretty obscure case
    // to start with.
    if (auto prev = out.getLogSP()) {
        if (auto [f,pid] = open(prev.get(), command); f) {
            m_prevLog = prev;
            m_logForkPid = pid;
            out.setLog(f);
        }
    }
}

cli_pipe::~cli_pipe() {
    if (!m_stream) return;
    auto& out = m_stream->cs_out;
    out.close();
    if (m_prev) {
        int status = 0;
        waitpid(m_forkPid, &status, 0);
        out.setOut(m_prev);
    }
    if (m_prevLog) {
        int status = 0;
        waitpid(m_logForkPid, &status, 0);
        out.setLog(m_prevLog);
    }
}

bool cli_pipe::ok() const {
    return !!m_prev;
}

// Create a stream that injects a application command e.g. "grep"
// into a stream.  Any data written to the returned stream is
// piped into command and the stdout from command is then
// sent to "out".
// Return a (file, pid) pair, the fiel is nullptr on failure.
// The pid is the pid of the forked process to run the command, and
// should be waited on to avoid zombies.
std::pair<FILE*,int> cli_pipe::open(FILE* out, const std::string& command)
{
    auto fail = std::pair<FILE*,int>{};
    if (!out) return fail;    

    // create the new pipe
    int cmdPipes[2];
    if (pipe(cmdPipes) < 0) {
        setError("create pipe");
        return fail;
    }

    if (auto cmdPid = fork(); cmdPid < 0) {
        setError("fork");
        close(cmdPipes[0]);
        close(cmdPipes[1]);
        return fail;
    } else if (cmdPid == 0) {
        // child fork - we're not using the input side of the pipe
        close(cmdPipes[1]);
        cmdPipes[1] = -1;

        // connect pipe | command > out
        dup2(cmdPipes[0], STDIN_FILENO);
        fflush(out);
        dup2(fileno(out), STDOUT_FILENO);

        char const* args[] = { "/bin/sh", "-c", command.c_str(), nullptr };
        execvp("/bin/sh", (char* const*) args);
        perror("execvp");
        close(cmdPipes[0]);
        return fail;
    } else {
        close(cmdPipes[0]);
        cmdPipes[0] = -1;

        // connect retFile | pipe
        return std::make_pair(fdopen(cmdPipes[1], "w"), cmdPid);;
    } 
    
    return fail;
}

void cli_pipe::setError(const char* msg) {
    m_error = std::string{msg} + ":" + strerror(errno);
}

cli_pager::cli_pager()
    : m_cli_stream(CurrentStream)
{
    // Don't try to page if:
    // - We don't have a CurrentStream,
    // - CurrentStream doesn't have an output,
    // - The CLI is noninteractive,
    // - Or the user has disabled paging via Client.height == 0 or .pager = ""
    if (!m_cli_stream || !m_cli_stream->cs_out.getOut() || !cli_is_interactive()
        || Client.height == 0 || Client.pager.empty())
    {
        m_state = State::DoNothing;
        return;
    }

    auto* fstream = fopencookie((void*)this, "w", cli_pager::cookie_funcs);
    if (!fstream) {
        m_state = State::DoNothing;
        return;
    }

    {
        // the check for cli_is_interactive above will keep us from getting into
        // weird non-interactive situations where this returns 0.
        struct winsize ws;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
        m_terminal_width = ws.ws_col;
    }

    m_original_fstream = m_cli_stream->cs_out.getOutSP();
    m_cli_stream->cs_out.setOut(fstream);
}

cli_pager::~cli_pager() {
    // The FILE* layer above us does buffering, so make sure we get all of the data in
    // transit before we swap out the streams. This might call forwardToWrite.
    ::fflush(m_cli_stream->cs_out.getOut());

    switch (m_state) {
        case State::Buffer: {
            // height threshold never hit, print out the data now
            dumpBufferedDataToOriginalStream();
        }
        break;

        case State::ForwardToPager: {
            // wait for user to exit pager
            m_pager_process->stdin_pipe.close(Pipe::End::Write);
            m_pager_process->waitForExit();
        }
        break;

        case State::ForwardToOriginalStream: {
            // implies paging was attempted, but it failed
            ::fprintf(m_original_fstream.get(), "\nWarning: pager \"%s\" failed.\n", Client.pager.c_str());
        }
        break;

        case State::DoNothing: return;
    }

    // calls ::fclose on the custom stream that was created in the ctor
    m_cli_stream->cs_out.setOut(m_original_fstream);
}

void cli_pager::dumpBufferedDataToOriginalStream() {
    for (const auto& saved_buf : m_saved_text) {
        ::fwrite(saved_buf.data(), 1, saved_buf.size(), m_original_fstream.get());
    }
    m_saved_text.clear();
}

ssize_t cli_pager::bufferModeWrite(const char* buf, size_t size) {
    const auto handleFailedToStartPager = [&]() {
        dumpBufferedDataToOriginalStream();
        m_state = State::ForwardToOriginalStream;
        return ::fwrite(buf, 1, size, m_original_fstream.get());
    };

    // count lines. count plain old '\n' and also attempt to break extremly long
    // lines into mutliple lines based on m_terminal_width.
    {
        size_t i = 0;
        size_t this_line_length = 1;

        for (; i < size && m_newline_running_total < Client.height; i++, this_line_length++) {
            if (buf[i] == '\n' || this_line_length > m_terminal_width) {
                m_newline_running_total++;
                this_line_length = 0;
            }
        }
    }

    if (m_newline_running_total < Client.height) {
        // newline threshold not met; save data for later
        m_saved_text.emplace_back(buf, buf + size);
        return size;
    } else {
        // newline threshold met; launch pager and switch to pager mode
        auto argv = ArgvStream{};
        argv.parseAndAppendArgs(Client.pager);

        m_pager_process = SharedForegroundSubprocess::make(argv);
        if (!m_pager_process) {
            return handleFailedToStartPager();
        }

        // dump previously saved data into pager
        auto bytes_written = size_t{0};
        auto total_saved_bytes = size_t{0};
        for (const auto& saved_buf : m_saved_text) {
            total_saved_bytes += saved_buf.size();
            bytes_written += m_pager_process->stdin_pipe.write(saved_buf.data(), saved_buf.size());
        }

        if (bytes_written != total_saved_bytes) {
            m_pager_process.reset(); // SIGKILL pager
            return handleFailedToStartPager();
        }

        // dump current data into pager
        if (m_pager_process->stdin_pipe.write(buf, size) != size) {
            m_pager_process.reset(); // SIGKILL pager
            return handleFailedToStartPager();
        }

        m_state = State::ForwardToPager;
        return size;
    }
}

ssize_t cli_pager::handleFILEWrite(void* cookie, const char* buf, size_t size) {
    auto* ths = reinterpret_cast<cli_pager*>(cookie);

    switch (ths->m_state) {
        case State::Buffer: return ths->bufferModeWrite(buf, size);
        case State::ForwardToPager: return ths->m_pager_process->stdin_pipe.write(buf, size);
        case State::ForwardToOriginalStream: return fwrite(buf, 1, size, ths->m_original_fstream.get());

        // forwardToWrite should never be called in the DoNothing state
        // because DoNothing implies that the custom file stream was never installed.
        case State::DoNothing: return 0;
    }

    throw std::runtime_error("unreachable");
    return 0;
}
