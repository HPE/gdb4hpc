/*********************************************************************************\
 * Exec support routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Cray Inc.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "alloc.h"
#include "defs.h"
#include "error.h"
#include "exec.h"
#include "str.h"

// external error printing function
extern void PrintOutError(const char *, ...);

static void pathexec(char *, char **, char **);
static int do_exec(int, char *, char **, char **);

#ifdef DEBUG
static void dumparg(FILE *, char *[]);

static void dumparg(FILE *fp, char *argv[]) {
    char **ap;

    for (ap = argv; *ap != NULL; ap++) fprintf(fp, "%s ", *ap);
}
#endif /* DEBUG */

int CheckStatus(int pid) {
    int status;
    int rtn_pid;

    rtn_pid = waitpid(pid, &status, WNOHANG);

    // only return true if our pid status has changed, on error we don't
    // want this to return true. This is to handle cases where aprun is
    // a wrapper script which calls the real aprun.
    if (rtn_pid == pid) {
        // Return true only if the child exited.
        return WIFEXITED(status);
    } else {
        return 0;
    }
}

void Exec(char *cmd, char **av, char **ev) { (void)do_exec(0, cmd, av, ev); }

int ForkAndExec(char *cmd, char **av, char **ev) {
    return do_exec(1, cmd, av, ev);
}

void Reap(int sig) {
    int status;

    wait(&status);
}

static void pathexec(char *prog, char **av, char **ev) {
    int n;
    char *p;
    char *s;
    char *t;
    static char path[MAXPATHLEN + 1];

    if (prog == NULL) return;

    if (*prog == '/' || (p = getenv("PATH")) == NULL) {
        execve(prog, av, ev);
        return; /* only on error */
    }

    while (*p != '\0') {
        if ((s = SkipTo(p, ':')) != NULL) *s = '\0';

        strcpy(path, p);

        if (s != NULL) *s = ':';

        n = strlen(path);

        t = &path[n - 1];

        if (*t != '/') {
            strcat(path, "/");
            n++;
        }

        strncat(path, prog, MAXPATHLEN - n);

        execve(path, av, ev);

        p = s + 1;
    }
}

#ifndef ALPS /* Cray_Inc */
static int do_exec(int dofork, char *cmd, char **av, char **ev) {
    int pid;

    /*
#ifdef DEBUG
            fprintf(stderr, "Exec: %s ", cmd);
            dumparg(stderr, av);
            fprintf(stderr, "\n");
#endif
    */

    if (!dofork) {
        pathexec(cmd, av, ev);
        PrintOutError("exec \"%s\" failed", cmd);
        exit(1);
    }

    switch (pid = fork()) {
        case 0:
            close(0);
            dup2(open("/dev/null", O_RDONLY), 0);
            pathexec(cmd, av, ev);
            PrintOutError("exec \"%s\" failed", cmd);
            exit(1);

        case -1:
            PrintOutError("fork failed");
            break;

        default:
            break;
    }

    return pid;
}

#else /* ALPS */
/* Cray_Inc: Re-worked the code a bit to keep the DPRINT message
 * from being printed when dofork is false.  An example of this
 * causing a problem is when do_exec() is called to start gdb--stderr
 * has already been connected to dbgsrv's DBGIn and the DPRINT
 * output is later interpreted as real gdb output.
 */
static int do_exec(int dofork, char *cmd, char **av, char **ev) {
    int pid;

    if (dofork) {
        pid = fork();

        if (pid == -1) {
            PrintOutError("fork failed");
            return pid;
        }
        if (pid != 0) {
            /* parent */
            return pid;
        }

        /* child */
#ifdef DEBUG
        fprintf(stderr, "Exec: %s ", cmd);
        dumparg(stderr, av);
        fprintf(stderr, "\n");
#endif
        /* Note: Only execute the preceding DPRINT if dofork was
         * specified.  If the fork was done by the callee, it could
         * have re-assigned the stdio file descriptors such that
         * writing to stderr would be problematic.
         */
    }

    pathexec(cmd, av, ev);
    PrintOutError("exec \"%s\" failed", cmd);
    exit(1);
}
#endif /* ALPS */
