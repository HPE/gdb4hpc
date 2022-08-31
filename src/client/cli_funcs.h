/******************************************************************************\
 * cli_funcs.c - Command line interface definitions.
 *
 * Copyright 2013-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 ******************************************************************************/

#ifndef _CLI_FUNCS_H
#define _CLI_FUNCS_H

#include <stdarg.h>
#include <stdio.h>

#include "dbgevent.h"
#include "list.h"

/*
** Types of cli streams
*/
enum cli_type {
    CLI_NOTTY_STREAM,
    CLI_TTY_STREAM,
    CLI_SOCKET_STREAM,
    CLI_FILE_STREAM
};
typedef enum cli_type cli_type_t;

enum class interpreter_type { CLI, MI, XML };

/*
** defined arg types for each stream type
*/
struct io_stream_args {
    FILE *in;
    FILE *out;
    FILE *err;
};
typedef struct io_stream_args io_stream_args_t;

// Note that the host, port, and error_port strings will be free'ed when the
// stream is destroyed. These should be created with malloc/strdup or related
// function.
struct socket_stream_args {
    char *host;
    char *port;
    char *error_port;
};
typedef struct socket_stream_args socket_stream_args_t;

// Note that all of the strings in the file stream args will be free'ed when the
// stream is destroyed. These too should be malloced/strduped etc.
struct file_stream_args {
    char *in_path;   // file path for input or NULL to use alt
    char *out_path;  // file path for output or NULL to use alt
    char *err_path;  // file path for error or NULL to use alt
};
typedef struct file_stream_args file_stream_args_t;

/*
** Arguments used when creating a new cli stream.
** These will be passed to the external definitions.
*/
union cli_stream_args {
    io_stream_args_t notty_args;
    io_stream_args_t tty_args;
    socket_stream_args_t socket_args;
    file_stream_args_t file_args;
#define notty_in notty_args.in
#define notty_out notty_args.out
#define notty_err notty_args.err
#define tty_in tty_args.in
#define tty_out tty_args.out
#define tty_err tty_args.err
#define socket_host socket_args.host
#define socket_port socket_args.port
#define socket_error_port socket_args.error_port
#define file_in_path file_args.in_path
#define file_out_path file_args.out_path
#define file_err_path file_args.err_path
};
typedef union cli_stream_args cli_stream_args_t;

class FileAdapter2;

// class for managing gdb4hpc ouput, which can be sent to 0, 1, or 2
// file locations.
class cli_out_t {
public:
    cli_out_t();

    // printf formatted output
    void operator()(const char* format, ...) __attribute__((format (printf,2,3)));
    void vprintf(const char* format, va_list);

    // C++ output
    std::ostream& ostream();

    // other stream operations 
    void reset();
    void flush();
    void close();


    // manage the streams
    void setOut(FILE* f);  // for a newly constructed file
    void setOut(std::shared_ptr<FILE> f);
    FILE* getOut() const { return m_out.get(); }
    std::shared_ptr<FILE> getOutSP() const { return m_out; }
    void setLog(FILE* f);

private:
    std::shared_ptr<FILE> m_out;
    std::shared_ptr<FILE> m_log;
    std::shared_ptr<FileAdapter2> m_buff;
    std::shared_ptr<std::ostream> m_ostream;
};

// interface for managing logging, which sends gdb4hpc output to
// file either in addition to the terminal or instead of terminal
class cli_logging_t {
public:
    cli_logging_t(cli_out_t& out);
    cli_logging_t(const cli_logging_t&) = delete;
    cli_logging_t& operator=(const cli_logging_t&) = delete;
    
    void setEnabled(bool enabled);
    void setFile(const std::string& filename);
    void setOverwrite(bool overwrite);
    void setRedirect(bool redirect);

    bool isEnabled() const;
    std::string getFile() const;
    bool isOverwrite() const;
    bool isRedirect() const;

    FILE* getOrigOut() const;
private:
    void startLog();
    cli_out_t& m_out;
    std::string m_filename = "gdb4hpc.txt";
    bool m_enabled = false;
    bool m_overwrite = false;
    bool m_redirect = false;
    std::shared_ptr<FILE> m_redirected;
};

/*
** CLI stream for I/O
*/
struct cli_stream_t {
    cli_stream_t(cli_type_t type, interpreter_type interpreter, cli_stream_args_t *args,
                 int nprom, va_list pargs);
    ~cli_stream_t();
    
    cli_type_t cs_type;  /* cli stream type */
    interpreter_type cs_interpreter = interpreter_type::CLI;  // or MI, XML
    int cs_fdin = -1;         /* fd number for input to select on */
    FILE *cs_fpin = nullptr;       /* file to read from */
    cli_out_t cs_out;      /* stream to output to */
    FILE *cs_fperr = nullptr;      /* stream to send errors to */
    FILE *cs_fpprog = nullptr;     // stream for program output
    int cs_nprom = 0;        /* number of prompt levels */
    int cs_plevel = 0;       /* current prompt level */
    char **cs_prompts = nullptr;   /* prompts to display */
    char *cs_prompt = nullptr;     /* current prompt */
    char *cs_tmp_prompt = nullptr; /* used to temporarily save a prompt */
    char *cs_buf = nullptr;        /* current line buffer */
    size_t cs_buf_size = 0;  /* size of current line buffer in elements */
    int cs_buf_point = 0;    /* current index into line buffer */
    int cs_line = 1;         /* current line */
    int cs_err = 0;          /* error control variable */
    int cs_saved = 0;        /* stream save control variable */
    int cs_handling = 0;     /* stream handling line control variable */
    int cs_seq = 0;          /* current command sequence number */
    int cs_in_cmd = 0;       /* currently processing command */
    int cs_num = 0;          // stream number
    list_t *cs_deferred = NewList(); /* list of deferred lines to print after command
                                       completes */
    struct cli_externals
        *cs_ext_funcs = nullptr;          /* cli externals associated with stream */
    cli_stream_args_t *cs_args; /* cli arguments used to initialize stream */
    cli_logging_t cs_logging;   // setting for loggind output
#define cs_notty_in cs_args->notty_args.in
#define cs_notty_out cs_args->notty_args.out
#define cs_notty_err cs_args->notty_args.err
#define cs_tty_in cs_args->tty_args.in
#define cs_tty_out cs_args->tty_args.out
#define cs_tty_err cs_args->tty_args.err
#define cs_socket_host cs_args->socket_args.host
#define cs_socket_port cs_args->socket_args.port
#define cs_socket_error_port cs_args->socket_args.error_port
#define cs_file_in_path cs_args->file_args.in_path
#define cs_file_out_path cs_args->file_args.out_path
#define cs_file_err_path cs_args->file_args.err_path
};

/*
** External CLI interface
*/
struct cli_externals {
    int (*external_init)(cli_stream_t **);
    int (*external_destroy)(cli_stream_t **);
    int (*external_handle_input)(cli_stream_t **);
    int (*external_handle_error)(cli_stream_t **);
    int (*external_save_stream)(cli_stream_t **);
    int (*external_restore_stream)(cli_stream_t **);
    int (*external_reset_stream)(cli_stream_t **);
    int (*external_clear_line)(cli_stream_t **);
};
typedef struct cli_externals cli_externals_t;

/*
** External CLI interface definitions for each CLI type
** TODO: Add any new external interface definitions here.
*/
extern cli_externals_t *cli_get_readline_funcs(void);
extern cli_externals_t *cli_get_socket_funcs(void);
extern cli_externals_t *cli_get_file_funcs(void);


extern list_t * InputCBList;
/*
** CLI stream init functions
*/
extern void InitCLI(cli_type_t, interpreter_type, cli_stream_args_t *);
extern void DestroyCLI(void);
extern int SetNewCLIStream(cli_type_t, interpreter_type, cli_stream_args_t *, int, ...);
extern void DestroyCurrentCLIStream(void);
extern void SetDefaultCLIStream(void);
extern cli_stream_args_t *NewCLIArgs(cli_type_t, ...);
extern void DestroyCLIArgs(cli_type_t, cli_stream_args_t *);


/*
** Output functions
*/
extern void PrintOutLine(const char *, ...) __attribute__((format (printf,1,2)));
extern void PrintOutError(const char *, ...) __attribute__((format (printf,1,2)));
extern void vPrintOutError(const char *, va_list);
extern void PrintOutMsg(const char *, ...) __attribute__((format (printf,1,2)));
extern void vPrintOutMsg(const char *, va_list);
extern void PrintToFd(FILE *, const char *, ...) __attribute__((format (printf,2,3)));
extern void vPrintToFd(FILE *, const char *, va_list);
extern void PrintError();

// Get/set the logging state 
extern cli_logging_t&  GetLogging();
extern bool SetProgramOutputFile(const char* file);

/*
** CLI helper functions
*/
// Used to increment/decrement the current prompt
extern void cli_inc_prompt(void);
extern void cli_dec_prompt(void);
// Returns the current prompt
extern char *cli_get_prompt(void);
// Used to handle input on the CLI stream cs_fpin
extern void cli_handle_input(void);
// Used to set the error condition flag for the current stream
extern void cli_set_error(void);
// Used to save the current state of a stream
extern void cli_save_stream(void);
// Used to restore a saved stream
extern void cli_restore_stream(void);
// Used to reset the stream to its default state
extern void cli_reset_stream(void);
// Used to clear the current line of its input
extern void cli_clear_line(void);

enum class cli_query_default { NONE, NO, YES };
// Used to confirm dangerous instructions
extern bool cli_query(const char *prompt,
                      cli_query_default defVal=cli_query_default::NONE,
                      cli_query_default nonInteractiveDefVal=cli_query_default::NONE);

// Used to get a line of input. This should be avoided. It is needed for ssh
// keyboard-interactive authentication
extern char *cli_get_line(void);
// Used to start and end a response for XML output
extern int cli_start_response(int);
extern void cli_end_response(void);
// Used to print out XML status messages
extern char *XMLStartStatusBlock(const char *set_name);
extern void XMLAddStatus(char **status_block, char *set, dbgstat stat,
                         const dbgloc_t *location);
extern void XMLPrintStatusBlock(char *status_block);
// Used to print out when a graph has been started
extern void XMLPrintStartGraphMsg(const char *set_name, const char *set_str);
// Used to print out when a process set has been removed
extern void XMLPrintRemovedMsg(const char *set_name);

#endif /* _CLI_FUNCS_H */
