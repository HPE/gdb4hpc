/*********************************************************************************\
 * log.c - Functions used to create log files.
 *
 * Copyright 2011-2013,2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#include "log.h"

#include <execinfo.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

FILE *create_log(int suffix, const char *nodeName) {
    char logfile[PATH_MAX];
    const char *envDir;
    FILE *logfd;

    // sanity checks
    if (nodeName == (char *)NULL) return (FILE *)NULL;

    // determine where to create the log
    if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)NULL) {
        // user didn't set the environment variable, so lets
        // write to /tmp
        envDir = "/tmp";
    }

    // create the fullpath string to the log file using PATH_MAX plus a null
    // term
    snprintf(logfile, PATH_MAX + 1, "%s/dbglog_%s.%d.log", envDir, nodeName,
             suffix);

    if ((logfd = fopen(logfile, "w")) == (FILE *)NULL) {
        // could not open log file for writing at the specififed location
        return (FILE *)NULL;
    }

    return logfd;
}

FILE *open_log(int suffix, const char *nodeName) {
    char logfile[PATH_MAX];
    const char *envDir;
    FILE *logfd;

    // sanity checks
    if (nodeName == (char *)NULL) return (FILE *)NULL;

    // determine where to create the log
    if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)NULL) {
        // user didn't set the environment variable, so lets
        // write to /tmp
        envDir = "/tmp";
    }

    // create the fullpath string to the log file using PATH_MAX plus a null
    // term
    snprintf(logfile, PATH_MAX + 1, "%s/dbglog_%s.%d.log", envDir, nodeName,
             suffix);

    if ((logfd = fopen(logfile, "a")) == (FILE *)NULL) {
        // could not open log file for writing at the specififed location
        return (FILE *)NULL;
    }

    return logfd;
}

int write_log(FILE *log, const char *buf) {
    if (log == (FILE *)NULL || buf == (char *)NULL) return 1;

    fprintf(log, "%s", buf);

    return 0;
}

int write_log2(FILE *log, const char *buf) {
    if (log == (FILE *)NULL || buf == (char *)NULL) return 1;

    fprintf(log, "%s\n", buf);

    return 0;
}

int close_log(FILE *log) { return fclose(log); }

int hook_stdoe(FILE *log) {
    // ensure the file ptr exists
    if (log == (FILE *)NULL) return 1;

    if (dup2(fileno(log), STDOUT_FILENO) < 0) return 1;

    if (dup2(fileno(log), STDERR_FILENO) < 0) return 1;

    return 0;
}

// Print the stacktace at the current location to file
// log.
void print_backtrace(FILE *log, const char *message) {
    void *trace[20];
    int trace_size = backtrace(trace, 20);
    char **frames = backtrace_symbols(trace, trace_size);
    if (frames) {
        fprintf(log, "%s: stack trace\n", (message ? message : ""));
        for (int i = 0; i < trace_size; ++i) {
            fprintf(log, "%s\n", frames[i]);
        }
        free(frames);
    } else {
        fprintf(log, "%s: could not get strack trace\n",
                (message ? message : ""));
    }
}
