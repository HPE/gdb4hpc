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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include "client.h"
#include "useful.h"

/*
** Static internal definitions
*/
static cli_stream_t *NewCLIStream(cli_type_t, interpreter_type, cli_stream_args_t *, int, ...);
static void cli_handle_error(void);
static void ProgOutLineStream(DbgInvoke *, FILE *);
static void ProgOutLineStrings(DbgInvoke *, const std::vector<std::string> &lines);
static void vPrintOutMIMsg(const char *format, va_list arg_list);
static void vPrintOutMIError(const char *format, va_list arg_list);

/*
 * Static Global Variables
 */
static cli_stream_t *CurrentStream; /* Current cli stream */
static std::vector<cli_stream_t*> AllStreams;
static bool MIPrintToConsole = false;
static bool MIPrintConsoleToMI = false;

void InitCLI(cli_type_t default_type, interpreter_type interpreter,
             cli_stream_args_t *default_args) {
    cli_stream_t *stream = NULL;

    // Create the cli stream
    // Note that we set the default prompt information here, that why we have
    // the need for the switch statement
    switch (default_type) {
        case CLI_NOTTY_STREAM:
            // There are no prompts for tty-less streams
            stream = NewCLIStream(CLI_NOTTY_STREAM, interpreter, default_args, 0);
            break;

        case CLI_TTY_STREAM:
            stream =
                NewCLIStream(CLI_TTY_STREAM, interpreter,
                             default_args, 2, "dbg %s> ", "> ");
            break;

        case CLI_SOCKET_STREAM:
            stream = NewCLIStream(CLI_SOCKET_STREAM, interpreter, default_args, 0);
            break;

        case CLI_FILE_STREAM:
            stream = NewCLIStream(CLI_FILE_STREAM, interpreter, default_args, 0);
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
        Client.cli_out = &CurrentStream->cs_out;
        Client.MI_mode = (CurrentStream->cs_interpreter == interpreter_type::MI);
    }
}

void DestroyCLI(void) {
    // Destroy the stream queue
    for (auto s : AllStreams)
        delete s;
    AllStreams.clear();

    // Null out the global variables
    CurrentStream = nullptr;

    Client.cli_out->reset();
}

int SetNewCLIStream(cli_type_t type, interpreter_type interpreter,
                    cli_stream_args_t *args, int nprom, ...) {
    va_list pargs;
    cli_stream_t *stream;

    // clear the current line this should contain a prompt
    cli_clear_line();

    // First create the new stream
    va_start(pargs, nprom);

    stream = new cli_stream_t(type, interpreter, args, nprom, pargs);

    va_end(pargs);

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

static cli_stream_t *NewCLIStream(cli_type_t type, interpreter_type interpreter,
                                  cli_stream_args_t *args,
                                  int nprom, ...) {
    va_list pargs;

    va_start(pargs, nprom);

    auto rtn = new cli_stream_t(type, interpreter, args, nprom, pargs);

    va_end(pargs);

    return rtn;
}

cli_stream_t::cli_stream_t(cli_type_t type, interpreter_type interpreter,cli_stream_args_t *args,
                           int nprom, va_list pargs) :
    cs_type(type), cs_interpreter(interpreter), cs_args(args), cs_logging(cs_out)
{
    // Set the prompts if the user supplied any
    if (nprom > 0) {
        cs_nprom = nprom;
        cs_prompts = (char **)Alloc(sizeof(char *) * nprom);

        for (int n = 0; n < nprom; n++) {
            cs_prompts[n] = strdup(va_arg(pargs, char *));
        }
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

    if (cs_prompts) {
        for (int n = 0; n < cs_nprom; n++) Free(cs_prompts[n]);
        free(cs_prompts);
    }

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

void cli_set_error(void) {
    if (CurrentStream == NULL) return;

    // set the error condition flag on the CurrentStream
    CurrentStream->cs_err++;
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
    int rtn;
    if (CurrentStream == NULL) return;

    // not thread safe
    // If we currently have a stream saved or if we are handling a line, return.
    // We don't want to save a stream without first restoring.
    // We don't want to save a stream if we are handling a completed line.
    if (CurrentStream->cs_saved || CurrentStream->cs_handling) return;

    ++CurrentStream->cs_saved;

    // we need to disable input processing while we are printing
    DbgMaskInput();

    // call to external save stream function
    if (CurrentStream->cs_ext_funcs->external_save_stream != NULL) {
        rtn = (*CurrentStream->cs_ext_funcs->external_save_stream)(
            &CurrentStream);
        if (rtn) {
            PrintOutError("Warning: external_save_stream() failed.\n");
        }
    }
    // restore input processing
    DbgUnmaskInput();
}

void cli_restore_stream(void) {
    int rtn;
    if (CurrentStream == NULL) return;

    // ensure there is a saved stream and we are not handling a line.
    // If we are handling a line, we need to wait until the line has been
    // parsed and handled before restoring.
    if (!CurrentStream->cs_saved || CurrentStream->cs_handling) return;

    // call to external restore stream function
    if (CurrentStream->cs_ext_funcs->external_restore_stream != NULL) {
        rtn = (*CurrentStream->cs_ext_funcs->external_restore_stream)(
            &CurrentStream);
        if (rtn) {
            PrintOutError("Warning: external_restore_stream() failed.\n");
        }
    }

    CurrentStream->cs_saved = 0;
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

void cli_inc_prompt(void) {
    if (CurrentStream == NULL) return;

    if (CurrentStream->cs_plevel >= CurrentStream->cs_nprom - 1) return;

    CurrentStream->cs_plevel++;
}

void cli_dec_prompt(void) {
    if (CurrentStream == NULL) return;

    if (CurrentStream->cs_plevel == 0) return;

    CurrentStream->cs_plevel--;
}

bool cli_is_interactive() {
    // return true if we have a stream, the stream has an input, and it's a tty
    return CurrentStream != NULL &&
        CurrentStream->cs_fpin != NULL &&
        CurrentStream->cs_type == CLI_TTY_STREAM;
}

char *cli_get_prompt(void) {
    cli_stream_t *cs = CurrentStream;
    dbgprocset_t *s = CurrentProcSet();
    const char *set_name;
    int plen = 0;

    if (cs == NULL) return NULL;

    if (cs->cs_nprom <= 0) return NULL;

    // free the old prompt
    if (cs->cs_prompt != NULL) Free(cs->cs_prompt);

    if (s != (dbgprocset_t *)NULL)
        set_name = s->s_name.c_str();
    else
        set_name = "none";

    plen = strlen(cs->cs_prompts[cs->cs_plevel]) + strlen(set_name) + 1;

    // Alloc the new prompt. This will be slightly longer than we actually need
    // if there are format characters in the CommandStream prompt string.
    cs->cs_prompt = (char *)Alloc(sizeof(char) * plen);

    // Create the new prompt and return it
    snprintf(cs->cs_prompt, plen, cs->cs_prompts[cs->cs_plevel], set_name);

    return cs->cs_prompt;
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

char *cli_get_line(void) {
    char *res = NULL;
    size_t n;

    // Make sure we currently have a stream, otherwise reading is impossible
    if (CurrentStream == NULL || CurrentStream->cs_fpin == NULL) {
        return NULL;
    }

    // read a line of input
    if (getline(&res, &n, CurrentStream->cs_fpin) < 0) {
        PrintOutError("getline failed.\n");
        perror("getline");
        return NULL;
    }

    return res;
}

static void ProgOutMITarget(DbgInvoke *di, const char* line) {
    // ignoring embedded quotes for the moment
    auto f = CurrentStream->cs_fpprog;
    auto new_line=std::string{line};
    for (std::size_t pos=0; (pos=new_line.find_first_of("\"\n\r",pos)) != std::string::npos; ) {
        new_line.erase(pos,1);
    }
    fprintf(f, "@\"%s\"\n%s", new_line.c_str(), GetMIEpilogue());
}


// common code between ProgOutLineStream and ProgOutLineStrings
static void ProgOutLine(DbgInvoke *di, const char* line) {
    auto f = CurrentStream->cs_fpprog;
    if (Client.MI_mode && !Client.cli_fpprog) {
        ProgOutMITarget(di,line);
    } else {
        if (Client.cli_fpprog) f = Client.cli_fpprog;
        fprintf(f, "<$%s>: %s", di->handle().c_str(), line);
    }
}

/*
** Output a program line recieved on a program pipe.
*/
static void ProgOutLineStream(DbgInvoke *di, FILE *o_stream) {
    // ensure that there is an output stream to print to in the current stream
    if (!CurrentStream || !CurrentStream->cs_fpprog) return;

    char line[BUFSIZ];

    cli_save_stream();

    while (fgets(line, BUFSIZ, o_stream)) {
        ProgOutLine(di, line);
    }

    cli_restore_stream();
}

static void ProgOutLineStrings(DbgInvoke *di, const std::vector<std::string> &lines) {
    // ensure that there is an output stream to print to in the current stream
    if (!CurrentStream || !CurrentStream->cs_fpprog) return;

    cli_save_stream();

    for (const auto &line : lines) {
        ProgOutLine(di, line.c_str());
    }

    cli_restore_stream();
}

/*
** fprintf a line to stdout.
*/
void PrintOutLine(const char *format, ...) {
    va_list arg_list;

    // ensure that there is an output stream to print to in the current stream
    if (!CurrentStream) return;

    cli_save_stream();

    va_start(arg_list, format);
    CurrentStream->cs_out.vprintf(format, arg_list);
    va_end(arg_list);

    cli_restore_stream();
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
     // we need to disable input processing while we are printing
    DbgMaskInput();

    // ensure that there is an error stream to print to in the current stream
    if ((!CurrentStream || !CurrentStream->cs_fperr) && !Client.MI_mode) {
        // otherwise default to stderr
        vfprintf(stderr, format, arg_list);

        // restore input processing
        DbgUnmaskInput();

        return;
    }

    // clear the current line this should contain a prompt
    cli_clear_line();

    if (Client.MI_mode) {
        vPrintOutMIError(format, arg_list);
    } else if (CurrentStream->cs_fperr) {
        // vfprintf the line we recieved
        vfprintf(CurrentStream->cs_fperr, format, arg_list);
    }

    // restore input processing
    DbgUnmaskInput();
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

    // ensure that there is an output stream to print to in the current stream
    if (!CurrentStream) {
        // we need to disable input processing while we are printing
        DbgMaskInput();

        // otherwise default to stdout
        vfprintf(stdout, format, arg_list);

        // restore input processing
        DbgUnmaskInput();

        return;
    }

    if (Client.MI_mode && MIPrintToConsole && !MIPrintConsoleToMI) {
        int currStream=CurrentStream->cs_num;
        DbgSwitchCurrentStream(AllStreams[0]);
        // vfprintf the line we recieved
        cli_clear_line();
        CurrentStream->cs_out.vprintf(format, arg_list);
        cli_reset_stream();
        DbgSwitchCurrentStream(AllStreams[currStream]);
        return;
    } else if (Client.MI_mode || MIPrintConsoleToMI) {
        vPrintOutMIMsg(format, arg_list);
        return;
    }

    // vfprintf the line we recieved
    CurrentStream->cs_out.vprintf(format, arg_list);
}

void PrintOutOk() {
    if (Client.MI_mode) {
        //CurrentStream->cs_out("^done\n(gdb)\n");
        CurrentStream->cs_out("%s^done\n%s", cli_get_prefix().c_str(), GetMIEpilogue());
    }
}

// Print out synchronous debugging output messages in MI format.  This includes
// things like the output from print, info, frame and backtrace.  This should
// not include asynchronous messages (like "breakpoint hit" or "stepping completed")
// or output from the program itself.  Those are output with a different prefix.
static void vPrintOutMIMsg(const char *format, va_list arg_list) {
    // the output is line oriented to we use os to cache any partial lines
    static auto os = std::ostringstream{};

    // the arg_list may well contain quotes or newlines, so we have to
    // just get the final output form so we can know they're being properly
    // escaped.
    char *cstr = nullptr;
    if (vasprintf(&cstr, format, arg_list) < 0) return;

    if (!os.tellp()) os << "~\"";  // the MI form is ~"..."

    for (const auto *p = cstr; *p; ++p) {
        switch (*p) {
        case '"': os << "\\\""; break;   
        case '\n':
            // escape the newline and flush the result
            os << "\\n\"\n";  
            CurrentStream->cs_out("%s", os.str().c_str());
            // GetMIEpilogue();
            os.str("");
            os << "~\"";
            break;
        default: os << *p;
        }
    }

    free(cstr);
}

static void vPrintOutMIError(const char *format, va_list arg_list) {
    static auto os = std::ostringstream{};

    // the arg_list may well contain quotes or newlines, so we have to
    // just get the final output form so we can know they're being properly
    // escaped.
    char *cstr = nullptr;
    if (vasprintf(&cstr, format, arg_list) < 0) return;

    mi_log("in vPrintOutMIError %s : %s\n", cli_get_prefix().c_str(), cstr);

    if (!os.tellp()) os << cli_get_prefix() << "^error,msg=\"" ;  // the MI error form

    for (const auto *p = cstr; *p; ++p) {
        switch (*p) {
        case '"': os << "\\\""; break;   
        case '\n':
            // escape the newline and flush the result
            os << "\\n\"\n";
            if (CurrentStream->cs_fperr) fprintf(CurrentStream->cs_fperr, "%s", os.str().c_str());
            mi_log("output mi: %s", os.str().c_str());
            os.str("");
            break;
        default:
            if (!os.tellp()) os << cli_get_prefix() << "^error,msg=\"" ;
            os << *p;
            break;
        }
    }

    free(cstr);
}

void PrintOutMIResult(MIResult res, const char* format, ...)
{
    if (res == MIResult::DONE) CurrentStream->cs_out("%s^done,", cli_get_prefix().c_str());
    else if (res == MIResult::STOPPED) CurrentStream->cs_out("%s*stopped,", cli_get_prefix().c_str());


    va_list args;
    va_start(args, format);
    CurrentStream->cs_out.vprintf(format, args);
    va_end(args);
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
    // we need to disable input processing while we are printing
    DbgMaskInput();

    // clear the current line this should contain a prompt
    cli_clear_line();

    if (Client.MI_mode || MIPrintConsoleToMI) {
        static const char* tag0 = "output MI:";
        static const char* tag = tag0;
        va_list spargs;
        va_copy(spargs, args);
        mi_logv2(tag, format, spargs);

        auto len = strlen(format);
        if (len && format[len-1] == '\n') tag = tag0;
        else tag = "";
        va_end(spargs);
    }

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

    // restore input processing
    DbgUnmaskInput();
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

const char* GetMIEpilogue() {
    static const char* epilogue = nullptr;
    if (!epilogue) {
        if (!AllStreams.empty() && AllStreams[0]->cs_interpreter == interpreter_type::MI) {
            // This is probably a hack, but emacs-mi does not like the (gdb) at the
            // end of response, so I'm using this criteria as a stand-in because
            // gdb-mi uses -i=mi, while gdb-gui adds a separate tty for mi
            epilogue = "";
        } else {
            epilogue = "(gdb)\n";
        }
    }
    return epilogue;
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
    
SuppressPrintOutError::SuppressPrintOutError() : stream(CurrentStream), prev(nullptr) {
    if (stream) std::swap(stream->cs_fperr, prev);
}
SuppressPrintOutError::~SuppressPrintOutError() { if (stream) stream->cs_fperr=prev; };

PrintOutMItoCli::PrintOutMItoCli() { MIPrintToConsole = true; }
PrintOutMItoCli::~PrintOutMItoCli() { MIPrintToConsole = false; };

PrintOutCliToMI::PrintOutCliToMI() { MIPrintConsoleToMI = true; }
PrintOutCliToMI::~PrintOutCliToMI() { MIPrintConsoleToMI = false; };


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
