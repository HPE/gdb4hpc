/******************************************************************************\
 * dbgproxy.c - proxy interface routines
 *
 * Copyright 2013-2015,2020 Hewlett Packard Enterprise Development LP.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dbgproxy.h"
#include "defs.h"
#include "useful.h"

// Used for checking null strings
static const char *__fmt_null_str = "";

#define FMT_CHECK_NULL_STR(s) \
    if (s == NULL) {          \
        s = __fmt_null_str;   \
    }

#define FMT_MAKE_NULL_STR(s) \
    if (strlen(s) == 0) {    \
        Free(s);             \
        s = NULL;            \
    }

// Static prototypes
static char *getstring(char **);
static int getint(char **);
static char **getargv(char **);
static char *ArgvToStr(char **, int *);

static char *getstring(char **res_buf) {
    int len;
    char *endptr = NULL;
    char *res;

    *res_buf = skipspace(*res_buf);

    // First get the length of the string - Note that this can be 0
    len = (int)strtol(*res_buf, &endptr, 10);

    // Skip past the space between the length and the start of the string
    ++endptr;

    // Now create a new buffer and memcpy
    res = (char *)Alloc(len + 1);
    // only copy if len > 0
    if (len > 0) {
        memcpy(res, endptr, len);
    }

    // set the null terminator
    res[len] = '\0';

    // set res_buf past string
    *res_buf = endptr + len;

    return res;
}

static int getint(char **res_buf) {
    int digit;
    char *endptr = NULL;

    *res_buf = skipspace(*res_buf);

    // Get the digit
    digit = (int)strtol(*res_buf, &endptr, 10);

    *res_buf = endptr;

    return digit;
}

static char **getargv(char **res_buf) {
    int argc;
    char *argv_str;
    char **argv;
    char *t;
    int i;

    argc = getint(res_buf);
    argv_str = getstring(res_buf);

    if (argc == 0) return NULL;

    // allocate the argv
    argv = (char **)Alloc(sizeof(char *) * (argc + 1));
    memset(argv, 0, sizeof(char *) * (argc + 1));

    // now process each string in the flattened string
    t = argv_str;
    for (i = 0; i < argc; ++i) {
        argv[i] = getstring(&t);
    }

    // cleanup and return
    Free(argv_str);

    return argv;
}

static char *ArgvToStr(char **argv, int *argc) {
    char **t;
    int cnt = 0;
    char *tmp = NULL;
    char *tmp2 = NULL;

    if (argv == NULL) {
        if (argc != NULL) *argc = 0;
        return strdup("");
    }

    // walk through the array entries to determine the number of elements
    for (t = argv; *t != NULL; ++t) {
        ++cnt;
    }

    if (argc != NULL) *argc = cnt;

    // ensure there is at least one element
    if (cnt == 0) {
        return strdup("");
    }

    // create the flattened string
    for (t = argv; *t != NULL; ++t) {
        // This should happen the first time through
        if (tmp == NULL) {
            Asprintf(&tmp2, "%d %s ", (int)strlen(*t), *t);
        } else {
            Asprintf(&tmp2, "%s%d %s ", tmp, (int)strlen(*t), *t);
            // Free the old tmp
            Free(tmp);
        }
        tmp = tmp2;
    }

    return tmp;
}

proxy_session *NewProxySession(void) {
    // Alloc the return type
    proxy_session *rtn = (proxy_session *)Alloc(sizeof(proxy_session));
    // memset everything to 0
    memset(rtn, 0, sizeof(proxy_session));

    // set each member that needs to be initialized non-zero
    PROXY_SESSION_SLEN(rtn) = sizeof(struct sockaddr_in);
    PROXY_SESSION_LISTENFD(rtn) = -1;
    PROXY_SESSION_CMDFD(rtn) = -1;
    PROXY_SESSION_ERRORFD(rtn) = -1;
    PROXY_SESSION_MSGFD(rtn) = -1;

    return rtn;
}

void CleanupProxySession(proxy_session *sess) {
    if (sess == NULL) return;

    // check to see if we should cleanup this session
    if (!(PROXY_SESSION_VALID(sess)) && !(PROXY_SESSION_REFCNT(sess))) {
        // This session has no more references and its no longer valid
        //
        // If the session is still valid, then more launches/attaches can be
        // associated with it.
        //
        // If there are still outstanding references, the session will get
        // cleaned up when the proxy_type is free'ed.

        // Close the listen fd
        if (PROXY_SESSION_LISTENFD(sess) >= 0)
            close(PROXY_SESSION_LISTENFD(sess));

        // Close the command socket
        if (PROXY_SESSION_CMDFD(sess) >= 0) close(PROXY_SESSION_CMDFD(sess));

        // Close the error socket
        if (PROXY_SESSION_ERRORSTREAM(sess) != NULL)
            fclose(PROXY_SESSION_ERRORSTREAM(sess));

        // Close the message socket
        if (PROXY_SESSION_MSGSTREAM(sess) != NULL)
            fclose(PROXY_SESSION_MSGSTREAM(sess));

        // Unlink the job file and free the string
        // XXX: This will delete the job script file, meaning the job *must*
        //      be finished before calling this function.
        if (PROXY_SESSION_JOBFILE(sess) != NULL) {
            unlink(PROXY_SESSION_JOBFILE(sess));
            Free(PROXY_SESSION_JOBFILE(sess));
        }

        // Free the object
        Free(sess);
    }
}

proxy_type *NewProxyType(proxy_session *sess) {
    if (sess == NULL) return NULL;

    // Alloc the return type
    proxy_type *rtn = (proxy_type *)Alloc(sizeof(proxy_type));
    // memset everything to 0
    memset(rtn, 0, sizeof(proxy_type));

    // set each member that needs to be initialized non-zero
    PROXY_EVENTFD(rtn) = -1;
    PROXY_OUTPUTFD(rtn) = -1;

    // set the session
    PROXY_SESSION(rtn) = sess;
    // increment refCount
    PROXY_SESSION_REFCNT(sess)++;

    // assign the id
    PROXY_ID(rtn) = ++(PROXY_SESSION_IDCNT(sess));

    return rtn;
}

void FreeProxyType(proxy_type *p) {
    if (p == NULL) return;

    // try to close the sockets associated with this instance
    if (PROXY_EVENTFD(p) >= 0) {
        close(PROXY_EVENTFD(p));
    }

    // try to fclose the output stream, otherwise try closing the socket
    if (PROXY_OUTPUTSTREAM(p) != NULL) {
        fclose(PROXY_OUTPUTSTREAM(p));
    } else if (PROXY_OUTPUTFD(p) >= 0) {
        close(PROXY_OUTPUTFD(p));
    }

    // decrement refCount
    PROXY_SESSION_REFCNT(PROXY_SESSION(p))--;

    // Cleanup the session
    CleanupProxySession(PROXY_SESSION(p));

    // Free the object
    Free(p);
}

recv_type *NewRecvType(bitset *target, bitset *recv) {
    // Alloc the return type
    recv_type *rtn = (recv_type *)Alloc(sizeof(recv_type));

    // Set the bitset pointers
    rtn->target = target;
    rtn->received = recv;

    return rtn;
}

void FreeRecvType(recv_type *recv) {
    if (recv == NULL) return;

    bitset_free(recv->target);
    bitset_free(recv->received);

    Free(recv);
}

attach_args *NewAttachArgs(void) {
    attach_args *rtn = (attach_args *)Alloc(sizeof(attach_args));

    // set everything to null/0
    memset(rtn, 0, sizeof(attach_args));

    return rtn;
}

void FreeAttachArgs(attach_args *args) {
    if (args == NULL) return;

    Free(args->app_ident);
    Free(args->arch);
    Free(args->backend);

    Free(args);
}

invoke_args *NewInvokeArgs(void) {
    invoke_args *rtn = (invoke_args *)Alloc(sizeof(invoke_args));

    // set everything to null/0
    memset(rtn, 0, sizeof(invoke_args));

    return rtn;
}

void FreeInvokeArgs(invoke_args *args) {
    char **t;

    if (args == NULL) return;

    Free(args->prog);
    if (args->argv != NULL) {
        for (t = args->argv; *t != NULL; ++t) {
            Free(*t);
        }
    }
    Free(args->arch);
    if (args->arch_args != NULL) {
        for (t = args->arch_args; *t != NULL; ++t) {
            Free(*t);
        }
    }
    Free(args->arch_input);
    Free(args->backend);
    if (args->env != NULL) {
        for (t = args->env; *t != NULL; ++t) {
            Free(*t);
        }
    }
    Free(args->cwd);

    Free(args);
}

proto_message *SocProtoToProtoMsg(char *s) {
    char *bitset_str;
    char *msg_str;
    proto_message *rtn;

    if (s == NULL) {
        return NULL;
    }

    s = skipspace(s);

    if (*s == '\0') {
        return NULL;
    }

    bitset_str = getstring(&s);
    msg_str = getstring(&s);

    rtn = NewProtoMsg(str_to_bitset(bitset_str, NULL), msg_str);

    Free(bitset_str);

    return rtn;
}

recv_type *SocProtoToRecv(char *s) {
    char *target_str;
    char *recv_str;
    recv_type *rtn;

    if (s == NULL) return NULL;

    s = skipspace(s);

    if (*s == '\0') {
        return NULL;
    }

    target_str = getstring(&s);
    recv_str = getstring(&s);

    rtn = NewRecvType(str_to_bitset(target_str, NULL),
                      str_to_bitset(recv_str, NULL));

    Free(target_str);
    Free(recv_str);

    return rtn;
}

attach_args *SocProtoToAttachArgs(char *s) {
    attach_args *rtn;

    if (s == NULL) return NULL;

    s = skipspace(s);

    if (*s == '\0') return NULL;

    // create the return type
    rtn = NewAttachArgs();

    // process the message
    rtn->app_ident = getstring(&s);
    rtn->arch = getstring(&s);
    FMT_MAKE_NULL_STR(rtn->arch);
    rtn->backend = getstring(&s);
    FMT_MAKE_NULL_STR(rtn->backend);
    rtn->gpu_flag = getint(&s);
    rtn->debug_flag = getint(&s);

    return rtn;
}

invoke_args *SocProtoToInvokeArgs(char *s) {
    invoke_args *rtn;

    if (s == NULL) return NULL;

    s = skipspace(s);

    if (*s == '\0') return NULL;

    // create the return type
    rtn = NewInvokeArgs();

    // process the message
    rtn->prog = getstring(&s);
    FMT_MAKE_NULL_STR(rtn->prog);
    rtn->argv = getargv(&s);
    rtn->nproc = getint(&s);
    rtn->arch = getstring(&s);
    FMT_MAKE_NULL_STR(rtn->arch);
    rtn->arch_args = getargv(&s);
    rtn->arch_input = getstring(&s);
    FMT_MAKE_NULL_STR(rtn->arch_input);
    rtn->backend = getstring(&s);
    FMT_MAKE_NULL_STR(rtn->backend);
    rtn->env = getargv(&s);
    rtn->cwd = getstring(&s);
    FMT_MAKE_NULL_STR(rtn->cwd);
    rtn->gpu_flag = getint(&s);
    rtn->debug_flag = getint(&s);
    rtn->early_entry_flag = getint(&s);
    rtn->non_stop_flag = getint(&s);

    return rtn;
}

int SocProtoToExitArgs(char *s) {
    int detach;

    if (s == NULL) return 0;

    s = skipspace(s);

    if (*s == '\0') {
        return 0;
    }

    detach = getint(&s);

    return detach;
}

char *ProtoMsgToSocProto(proto_message *s) {
    char *stres = NULL;
    char *bitset_str = NULL;

    if (s == NULL) return NULL;

    // Convert the group to a string
    bitset_str = bitset_to_str(s->group);

    Asprintf(&stres, "%d %s %d %s", (int)strlen(bitset_str), bitset_str,
             (int)strlen(s->message), s->message);

    Free(bitset_str);

    return stres;
}

char *RecvToSocProto(bitset *target, bitset *recv) {
    char *stres = NULL;
    char *target_str = NULL;
    char *recv_str = NULL;

    if (target == NULL || recv == NULL) return NULL;

    // Convert the bitsets to strings
    target_str = bitset_to_str(target);
    recv_str = bitset_to_str(recv);

    Asprintf(&stres, "%d %s %d %s", (int)strlen(target_str), target_str,
             (int)strlen(recv_str), recv_str);

    Free(target_str);
    Free(recv_str);

    return stres;
}

char *AttachArgsToSocProto(const char *app_ident, const char *arch,
                           const char *backend, int gpu_flag, int debug_flag) {
    char *stres = NULL;

    // null guard the strings
    FMT_CHECK_NULL_STR(arch);
    FMT_CHECK_NULL_STR(backend);

    // Create the res
    Asprintf(&stres, "%d %s %d %s %d %s %d %d", (int)strlen(app_ident),
             app_ident, (int)strlen(arch), arch, (int)strlen(backend), backend,
             gpu_flag, debug_flag);

    return stres;
}

char *InvokeArgsToSocProto(const char *prog, char **argv, int nproc,
                           const char *arch, char **arch_args,
                           const char *arch_input, const char *backend,
                           char **env, const char *cwd, int gpu_flag,
                           int debug_flag, int early_entry_flag, bool isNonStop) {
    char *stres = NULL;
    char *argv_str;
    int argv_num;
    char *arch_args_str;
    int arch_args_num;
    char *env_str;
    int env_num;

    // create the argv style strings
    argv_str = ArgvToStr(argv, &argv_num);
    arch_args_str = ArgvToStr(arch_args, &arch_args_num);
    env_str = ArgvToStr(env, &env_num);

    // null guard the strings
    FMT_CHECK_NULL_STR(prog);
    FMT_CHECK_NULL_STR(arch);
    FMT_CHECK_NULL_STR(arch_input);
    FMT_CHECK_NULL_STR(backend);
    FMT_CHECK_NULL_STR(cwd);

    // Create the res
    Asprintf(
        &stres,
        "%d %s %d %d %s %d %d %s %d %d %s %d %s %d %s %d %d %s %d %s %d %d %d %d",
        (int)strlen(prog), prog, argv_num, (int)strlen(argv_str), argv_str,
        nproc, (int)strlen(arch), arch, arch_args_num,
        (int)strlen(arch_args_str), arch_args_str, (int)strlen(arch_input),
        arch_input, (int)strlen(backend), backend, env_num,
        (int)strlen(env_str), env_str, (int)strlen(cwd), cwd, gpu_flag,
        debug_flag, early_entry_flag, isNonStop);

    // Cleanup
    Free(argv_str);
    Free(arch_args_str);
    Free(env_str);

    return stres;
}

char *ExitArgsToSocProto(int detach) {
    char *stres = NULL;

    Asprintf(&stres, "%d", detach);

    return stres;
}
