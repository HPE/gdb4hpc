/******************************************************************************\
 * cli_readline.c - External readline CLI routines.
 *
 * Copyright 2011-2023 Hewlett Packard Enterprise Development LP.
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

#include <unordered_set>

#include <editline/readline.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "client.h"
#include "useful.h"

/*
 * Internal prototypes
 */
static int cli_readline_init(cli_stream_t **);
static int cli_readline_destroy(cli_stream_t **);
static int cli_readline_handle_char(cli_stream_t **);
static int cli_readline_save_stream(cli_stream_t **);
static int cli_readline_restore_stream(cli_stream_t **);
static int cli_readline_reset_stream(cli_stream_t **);
static int cli_readline_clear_line(cli_stream_t **);

static bool should_expand_line(const char *line);
static bool should_repeat_line(const char *line);
static char *expand_line(const char *line);
static void record_and_execute_line(const char *line);
static void rl_handle_line(char *);

static char** generate_completions(const char* text, int start, int end);

cli_externals_t *cli_get_readline_funcs(void) {
    cli_externals_t *rtn;

    rtn = (cli_externals_t *)Alloc(sizeof(cli_externals_t));

    rtn->external_init = cli_readline_init;
    rtn->external_destroy = cli_readline_destroy;
    rtn->external_handle_input = cli_readline_handle_char;
    rtn->external_handle_error =
        NULL;  // We don't handle debugger errors for this stream
    rtn->external_save_stream = cli_readline_save_stream;
    rtn->external_restore_stream = cli_readline_restore_stream;
    rtn->external_reset_stream = cli_readline_reset_stream;
    rtn->external_clear_line = cli_readline_clear_line;

    return rtn;
}

static int cli_readline_init(cli_stream_t **stream) {
    cli_stream_t *cs = (*stream);
    char *config_loc;

    // Set the stream members based on the args

    // Note that this function handles both notty and tty right now so we need
    // to make sure we access the proper members
    switch (cs->cs_type) {
        case CLI_NOTTY_STREAM:
            cs->cs_fpin = cs->cs_notty_in;
            cs->cs_out.setOut(cs->cs_notty_out);
            cs->cs_fpprog = cs->cs_notty_out;
            cs->cs_fperr = cs->cs_notty_err;
            break;

        case CLI_TTY_STREAM:
            cs->cs_fpin = cs->cs_tty_in;
            cs->cs_out.setOut(cs->cs_tty_out);
            cs->cs_fpprog = cs->cs_tty_out;
            cs->cs_fperr = cs->cs_tty_err;
            break;

        case CLI_SOCKET_STREAM:
        case CLI_FILE_STREAM:
            // shouldn't happen - but catch for completeness
            return 1;
    }

    cs->cs_fdin = fileno(cs->cs_fpin);

    // Set the readline input and output streams
    rl_instream = cs->cs_fpin;
    rl_outstream = cs->cs_out.getOut();

    /*
    ** Readline init stuff.
    */
    rl_callback_handler_install(
        "",              // the initial prompt. the real prompt will be obtained from
                         // the current interpreter once the interpreter is set up.
                         // the user will never see this one.
        rl_handle_line); // Setup the readline callback handler.

    // Setup the history
    using_history();

    // Get the default configuration location and read in the history file if
    // possible. We do not care if this fails. Default to /tmp
    if ((config_loc = getenv("GDB4HPC_HIST_DIR")) != NULL) {
        char *histfile;
        Asprintf(&histfile, "%s/gdb4hpc_history", config_loc);
        read_history(histfile);
        Free(histfile);
    } else if ((config_loc = getenv(GUARD_CFG_DIRECTORY)) != NULL) {
        char *histfile;
        Asprintf(&histfile, "%s/gdb4hpc_history", config_loc);
        read_history(histfile);
        Free(histfile);
    }

    // Set history size to 800
    stifle_history(800);

    // Allow gdb4hpc specific branches in .editrc
    rl_readline_name = "gdb4hpc";

    // Read in the editline rc file, this will fail silently.
    struct passwd *pw = getpwuid(getuid());
    if (pw) {
        const char *homedir = pw->pw_dir;
        char *init_file_loc;
        Asprintf(&init_file_loc, "%s/.editrc", homedir);
        rl_read_init_file(init_file_loc);
        Free(init_file_loc);
    }

    // set tab completion hook
    rl_attempted_completion_function = generate_completions;

    return 0;
}

static int cli_readline_destroy(cli_stream_t **stream) {
    cli_stream_t *cs = (*stream);
    char *config_loc;

    Free(cs->cs_ext_funcs);

    // remove the prompt
    rl_set_prompt("");
    rl_replace_line("", 0);
    rl_forced_update_display();

    // Get the default configuration location and write the history out to file
    if ((config_loc = getenv("GDB4HPC_HIST_DIR")) != NULL) {
        char *histfile;
        Asprintf(&histfile, "%s/gdb4hpc_history", config_loc);
        write_history(histfile);
        Free(histfile);
    } else if ((config_loc = getenv(GUARD_CFG_DIRECTORY)) != NULL) {
        char *histfile;
        Asprintf(&histfile, "%s/gdb4hpc_history", config_loc);
        write_history(histfile);
        Free(histfile);
    }

    // Remove the readline callback handler
    rl_callback_handler_remove();

    return 0;
}

static int cli_readline_handle_char(cli_stream_t **) {
    rl_callback_read_char();
    return 0;
}

static int cli_readline_save_stream(cli_stream_t **stream) {
    cli_stream_t *cs = (*stream);

    cs->cs_buf_point = rl_point;
    cs->cs_buf = rl_copy_text(0, rl_end);
    rl_set_prompt("");
    rl_replace_line("", 0);
    rl_forced_update_display();

    return 0;
}

static int cli_readline_restore_stream(cli_stream_t **stream) {
    cli_stream_t *cs = (*stream);

    rl_set_prompt(DbgGetPrompt().c_str());
    rl_replace_line(cs->cs_buf, 0);
    rl_point = cs->cs_buf_point;
    rl_forced_update_display();

    Free(cs->cs_buf);

    return 0;
}

static int cli_readline_reset_stream(cli_stream_t **) {
    rl_prep_terminal(0);
    rl_set_prompt(DbgGetPrompt().c_str());
    rl_replace_line("", 0);
    rl_forced_update_display();

    return 0;
}

static int cli_readline_clear_line(cli_stream_t **) {
    rl_set_prompt("");
    rl_replace_line("", 0);
    rl_forced_update_display();

    return 0;
}

// returns true if line has a form similar to "!searchterm" and can be passed to
// history_expand
static bool should_expand_line(const char *line) {
    if (line == nullptr || line[0] != '!' || strnlen(line, 2) < 2) {
        return false;
    }
    // We support expansion, but only "!search". editline is broken in regards 
    // to !? and will eventually segfault. Most of the other operators don't
    // work either.

    // Catch other editline expansion prefixes: !?, !!, !n, etc.
    // If the second character isn't in our banned list, we can do expansion
    if (strchr("0123456789-#?!", line[1])) {
        return false;
    }

    return true;
}

// returns true if line is blank ("") and the most recently executed command
// is suitable to be repeated (step, next...)
static bool should_repeat_line(const char *line) {
    if (!line || *line != '\0')
        return false;

    // we only want to repeat a certain few commands
    const std::unordered_set<std::string> repreatable = {
        "n", "ne", "nex", "next",
        "s", "ste", "step", // st is considered ambigious (start, step, or stop?)
        "l", "li", "lis", "list"
    };

    auto recentEvent = current_history();

    if (!recentEvent || !recentEvent->line) {
        return false;
    }

    auto lastLine = std::string{recentEvent->line};

    return static_cast<bool>(repreatable.count(lastLine));
}

// returns the expanded version of line as expanded by history_expand
static char *expand_line(const char *line) {
    if (line == nullptr) {
        return nullptr;
    }

    char *expandedLine = nullptr;
    if (history_expand(const_cast<char *>(line), &expandedLine) == 1) {
        return expandedLine;
    }

    // history_expand can fail in ways that don't clean up expandedLine
    free(expandedLine);

    return nullptr;
}

// record line in command history and then execute it
static void record_and_execute_line(const char *line) {
    if (line == nullptr) return;

    // If there is something other than a null terminator, maybe add it to the
    // history. Note that we do not want to allow immediate repeat history entries.
    if (*line) {
        // Get the current history and check if it matches current line
        if (auto entry = current_history()) {
            // compare line to the history line, if true the two differ
            // If they match, do nothing. We don't want repeat history
            // entries since that is annoying to key through.
            if (strcmp(line, entry->line) != 0) {
                // Add the line to the history
                add_history(line);
            }
        } else {
            // Add the line to the history
            add_history(line);
        }
    }

    // ignore return value. there is nothing we need to do at the cli level upon an error.
    DbgInterpret(line);
}

static void rl_handle_line(char *line) {
    // If line is NULL, something went very very wrong as readline triggered
    // that there was a complete line of text, but it returned nothing to us.
    if (!line) {
        ForceQuit();
        return;
    }

    // replace the current line entry with a blank line. This is so that output
    // will look correct.
    rl_replace_line("", 0);

    // Execute the command
    if (should_repeat_line(line)) {
        // "" was passed, so execute the most recent command again
        // should_repeat_line has already made sure current_history() won't be null
        record_and_execute_line(current_history()->line);
    } else if (!should_expand_line(line)) {
        // No expansion
        record_and_execute_line(line);
    } else if (auto expanded = expand_line(line)) {
        // Try a reverse search if the first character is !
        PrintOutMsg("> %s\n", expanded);
        record_and_execute_line(expanded);
        free(expanded);
    } else {
        // User tried to search, but it failed.
        // They probably don't want to try to run a failed search.
        PrintOutError("Couldn't find a command in history matching \"%s\".\n", line);
    }

    // done with line
    free(line);

    // Check for quit condition, on quit we reset prompt to empty.
    if (!Client.quit) {
        rl_set_prompt(DbgGetPrompt().c_str());
    } else {
        rl_set_prompt("");
    }
}

// `generate_completions` asks the current interpreter to complete the word
// fragment `partial_word` which is a substring in `rl_line_buffer` at indices [`start`, `end`).
//
// readline wants a peculiarly formatted array of char* as the result:
// - result[0]: the shortest common substring - this will *replace* `partial_word` when tab is pressed.
// - result[1]: the first completion option
// - result[2]: the second completion option
// - ...
// - result[n]: nullptr
//
// the returned array is deeply freed by readline and must be allocated with malloc.
//
// if nullptr is returned, readline will fall back to its internal filesystem based
// completions.
static char** generate_completions(const char* /*partial_word*/, int start, int end) {
    // the DbgComplete API expects that the passed partial word is part of the passed buffer.
    // the `partial_word` passed by readline here is actually the result of ::strdup and
    // doesn't satisfy this, so we create a string view ourselves.
    auto completions = DbgComplete(
        std::string_view{rl_line_buffer + start, rl_line_buffer + end},
        std::string_view{rl_line_buffer, static_cast<size_t>(rl_end)}
    );

    if (!completions.possibilities.empty()) {
        const auto& possibilities = completions.possibilities;
        auto replacement = completions.calc_common_prefix();

        // the replacement + all other possibilities + nullptr
        const auto result_size = possibilities.size() + 2;

        auto result = static_cast<char**>(::malloc(sizeof(char*) * result_size));
        result[0] = ::strdup(replacement.c_str());
        for (auto i = size_t{0}; i < possibilities.size(); i++) {
            result[i + 1] = ::strdup(possibilities[i].c_str());
        }
        result[result_size - 1] = nullptr;

        return result;
    } else if (completions.include_filesystem) {
        // no completions provided by interpreter, fall back to readline's filesystem completion
        return nullptr;
    } else {
        // no completions provided by interpreter, and we shouldn't even show filenames
        rl_attempted_completion_over = true;
        return nullptr;
    }
}
