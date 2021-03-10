/******************************************************************************\
 * proxy_daemon.c - Debugger proxy daemon
 *
 * Copyright 2013-2020 Hewlett Packard Enterprise Development LP.
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

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common_tools_fe.h"
#include "dbg_client.h"
#include "dbgproxy.h"
#include "defs.h"
#include "proxy.h"
#include "useful.h"

#define RECONNECT_TRIES 10

// These are static in dbg_client, here they are provided by dbg_client_proxy.c
extern dbginvoke_t *NewDbgInvoke(int, DebugClientProtocol * const);
extern void FreeDbgInvoke(dbginvoke_t *);

/*
** global variables
*/
FILE *err_stream = NULL;
list_t *instances = (list_t *)NULL;
int Quit = 0;

/*
** Static global variables
*/
static proxy_session *sess = NULL;
static struct addrinfo *host;  // addrinfo object for hostname
static fd_set dispatch_fds;
static int high_fd;

/*
** long opts definitions
*/
static const struct option proxy_long_opts[] = {
    {"hostname", optional_argument, 0, 'h'},
    {"port", optional_argument, 0, 'p'},
    {0, 0, 0, 0}};

struct proxy_instance {
    int clnt_id;  // id used by the client to identify this instance
    proxy_type
        *inst;  // instance of a proxy_type associated with an attach/invoke
};
typedef struct proxy_instance proxy_instance;

static void NewProxyInstance(int id, proxy_type *inst) {
    // Alloc the obj
    proxy_instance *obj = (proxy_instance *)Alloc(sizeof(proxy_instance));
    // memset everything to 0
    memset(obj, 0, sizeof(proxy_instance));

    obj->clnt_id = id;
    obj->inst = inst;

    AddToList(instances, obj);
}

static void FreeProxyInstance(proxy_type *inst) {
    proxy_instance *obj;

    if (inst == NULL) return;

    // find the obj associated with this instance
    SetList(instances);
    while ((obj = (proxy_instance *)GetListElement(instances)) != NULL) {
        if (obj->inst == inst) {
            RemoveFromList(instances, obj);

            FreeProxyType(obj->inst);
            Free(obj);

            return;
        }
    }
}

static proxy_type *GetProxyTypeFromClntId(int id) {
    proxy_instance *obj;

    SetList(instances);

    while ((obj = (proxy_instance *)GetListElement(instances)) != NULL) {
        if (obj->clnt_id == id) return obj->inst;
    }

    return NULL;
}

static proxy_type *GetProxyTypeFromInstId(int id) {
    proxy_instance *obj;

    SetList(instances);

    while ((obj = (proxy_instance *)GetListElement(instances)) != NULL) {
        if (PROXY_ID(obj->inst) == id) return obj->inst;
    }

    return NULL;
}

static void CleanupOnSocketClose(void) {
    dbginvoke_t *di = NULL;
    proxy_instance *obj = NULL;
    int detach = 0;
    bitset *recv_set;
    proto_msg_set *pmsg_set;

    // find the dbginvoke_t associated with this id
    if (!EmptyList(DbgInvokeList)) {
        // Reset the DbgInvokeList
        SetList(DbgInvokeList);

        // Loop through each dbginvoke entry and call its exit function
        while ((di = (dbginvoke_t *)GetListElement(DbgInvokeList)) !=
               (dbginvoke_t *)NULL) {
            // use default based on attach value
            switch (di->d_type) {
                case DBG_ATTACHED:
                    detach = 1;
                    break;
                case DBG_LAUNCHED:
                    detach = 0;
                    break;
            }

            // Create the quit command
            auto request = CommandToSocProto(DBGCMD_QUIT, detach);

            // send the command to every rank
            recv_set = DBG_PROTO_SW(di)->sendCommand(di, di->d_bitset_full, request.c_str());

            pmsg_set = DBG_PROTO_SW(di)->receiveReply(di, di->d_bitset_full, recv_set);

            // We don't care about actually processing the reply, since
            // this is the cleanup on error case. We want to ensure that
            // we received an asynch reply.

            // cleanup
            FreeProtoMsgSet(pmsg_set);

            bitset_free(recv_set);

            // It is now safe to exit
            DBG_PROTO_SW(di)->exit(di, detach);

            // Cleanup the dbginvoke, we are done with it.
            FreeDbgInvoke(di);
        }
    }

    // Free all the instances
    if (!EmptyList(instances)) {
        SetList(instances);

        while ((obj = (proxy_instance *)GetListElement(instances)) != NULL) {
            FreeProxyInstance(obj->inst);
            // Need to reset the list since it will be messed up in the function
            // call
            SetList(instances);
        }
    }

    _exit(1);
}

static int checkDirPerms(const char *dir) {
    struct stat st;

    if (dir == NULL) return -1;

    // Stat the tmp_dir
    if (stat(dir, &st)) {
        // could not stat the directory
        return -1;
    }

    // check to see if the user id is the same as this directory
    if (st.st_uid == getuid()) {
        // Check the user perms
        return ((st.st_mode & S_IRWXU) ? 0 : -1);
    }

    // check to see if the group id is the same as the directory
    if (st.st_gid == getgid()) {
        // Check the group perms
        return ((st.st_mode & S_IRWXG) ? 0 : -1);
    }

    // Check the other perms
    return ((st.st_mode & S_IRWXO) ? 0 : -1);
}

static void setupCfgDir(const char *dir) {
    struct stat st;

    if (dir == NULL) return;

    // try to stat the directory
    if (stat(dir, &st)) {
        // the directory doesn't exist so we need to create it
        // use perms 755
        if (mkdir(dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) {
            PrintOutError("Could not create config directory %s.\n", dir);
            PrintOutError("Try setting env var %s and try again. Aborting...\n",
                          GUARD_CFG_DIRECTORY);
            exit(1);
        }
    } else {
        // chmod the existing directory to ensure there isn't any funny business
        // going on
        if (chmod(dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) {
            PrintOutError("Could not chmod config directory %s.\n", dir);
            PrintOutError("Try setting env var %s and try again. Aborting...\n",
                          GUARD_CFG_DIRECTORY);
            exit(1);
        }
    }
}

// All environment setup should happen here
static void SetupEnvironment(void) {
    char *inst_base;
    char *lib_dir;
    char *libexec_dir;
    char *cfg_dir;
    char *home_dir;

    // Adjust the environment to add our install base to LD_LIBRARY_PATH and
    // PATH of our process
    if ((inst_base = getinstallbase()) == NULL) {
        PrintOutError("Could not locate install base. Aborting...\n");
        exit(1);
    }
    // create the path strings
    Asprintf(&lib_dir, "%s/lib", inst_base);
    Asprintf(&libexec_dir, "%s/libexec", inst_base);

    // Set the environment variables
    adjustLibPath(lib_dir);
    adjustExecPath(libexec_dir);

    // get the cfg dir settings - note that slurm can put us on an actual
    // compute node, meaning that we might need to reset the cfg dir.
    if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == NULL) {
        // Not found.
        // We now want to use $HOME since this will contain guard only files
        // like editline history files.

    reinit_directory:

        if ((home_dir = getenv("HOME")) == NULL) {
            PrintOutError("Could not read $HOME. Aborting...\n");
            exit(1);
        }

        // Check if we can write to HOME
        if (checkDirPerms(home_dir)) {
            // We have no where to create a temporary directory...
            PrintOutError("No write permission for $HOME. Aborting...\n");
            exit(1);
        }

        // Create the directory name string
        Asprintf(&cfg_dir, "%s/.gdb4hpc", home_dir);

        // Setup/create this directory
        setupCfgDir(cfg_dir);
        setenv(GUARD_CFG_DIRECTORY, cfg_dir, 1);
        Free(cfg_dir);

    } else {
        // The user set GUARD_CFG_DIRECTORY, so we *ALWAYS* want to use that
        struct passwd *pw;
        char *tmp_dir;

        // Check to see if we can write to this directory
        if (checkDirPerms(cfg_dir)) {
            // fix for slurm
            goto reinit_directory;
        }

        // Lets create a unique directory just in case the user gave us some
        // weird location that others can access

        // Get the pw info, this is used in the unique name part of cfg
        // directories
        if ((pw = getpwuid(getuid())) == NULL) {
            PrintOutError("getpwuid() failed. Aborting...\n");
            exit(1);
        }

        tmp_dir = cfg_dir;
        cfg_dir = NULL;
        Asprintf(&cfg_dir, "%s/.gdb4hpc-%s", tmp_dir, pw->pw_name);

        DPRINT("Using %s as proxy staging area.\n", cfg_dir);

        // Setup/create this directory
        setupCfgDir(cfg_dir);
        setenv(GUARD_CFG_DIRECTORY, cfg_dir, 1);
        Free(cfg_dir);
    }

    Free(lib_dir);
    Free(libexec_dir);
    Free(inst_base);
}

int main(int argc, char **argv) {
    // arguments from argv
    int opt;
    int opt_ind = 0;
    char *hostname = NULL;
    char *port = NULL;
    // other stuff
    int rc, sopt;
    struct addrinfo hints;  // hints object for call to getaddrinfo
    int tries = 0;

    // Setup

    // Ensure the environment has been setup, otherwise set it up and re-exec
    // This is required because otherwise dlopen's will be fubared.
    // This needs to use a different env var than the client, otherwise the
    // env might have been forwarded but directories might not have been
    // created.
    if (getenv(PROXY_ENV_SETUP) == NULL) {
        char *exe_path;

        SetupEnvironment();
        // Done. Set the proper environment variable so that we don't have an
        // infinite loop here.
        setenv(PROXY_ENV_SETUP, "true", 1);
        exe_path = getexepath();
        execve(exe_path, argv, environ);
    }

    // Close stdin/stdout/stderr for the non-debug case
#ifndef DEBUG
    close(0);
    close(1);
    close(2);
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_WRONLY);
#endif

    DPRINT("Proxy daemon started.\n");

#ifdef DEBUG
    char lhost[MAXHOSTNAMELEN];

    if (gethostname(lhost, MAXHOSTNAMELEN - 1) == 0) {
        DPRINT("I am running on %s.\n", lhost);
    }
#endif

    memset(&hints, 0, sizeof(hints));  // ensure the hints struct is cleared
    hints.ai_family = AF_UNSPEC;       // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    //
    // Process user provided arguments
    //
    while ((opt = getopt_long(argc, argv, "h:p:", proxy_long_opts, &opt_ind)) !=
           -1) {
        switch (opt) {
            case 0:
                // if this is a flag, do nothing
                break;
            case 'h':
                if (optarg == (char *)NULL) {
                    PrintOutError(
                        "Error: --hostname provided, but missing argument.\n");
                    return 1;
                }
                hostname = strdup(optarg);
                break;

            case 'p':
                if (optarg == (char *)NULL) {
                    PrintOutError(
                        "Error: --port provided, but missing argument.\n");
                    return 1;
                }
                port = strdup(optarg);
                break;

            default:
                PrintOutError("Invalid argument provided.\n");
                return 1;
        }
    }

    // ensure arguments were provided
    if (hostname == NULL) {
        PrintOutError("Error: Missing argument --hostname.\n");
        return 1;
    }

    if (port == NULL) {
        PrintOutError("Error: Missing argument --port.\n");
        return 1;
    }

    // get the addrinfo of the hostname
    if ((rc = getaddrinfo(hostname, port, &hints, &host)) != 0) {
        fprintf(stderr, "Getaddrinfo failed: %s\n", gai_strerror(rc));
        // Handle error
        return 1;
    }

    // Create a new session
    sess = NewProxySession();

    // Create the cmd socket
    if ((PROXY_SESSION_CMDFD(sess) = socket(host->ai_family, host->ai_socktype,
                                            host->ai_protocol)) < 0) {
        perror("socket");
        // Handle error
        return 1;
    }

    // Set the command socket to TCP_NODELAY
    sopt = 1;
    if (setsockopt(PROXY_SESSION_CMDFD(sess), IPPROTO_TCP, TCP_NODELAY, &sopt,
                   sizeof(sopt)) < 0) {
        // Non-fatal error
        perror("setsockopt(TCP_NODELAY)");
    }

    // Connect the cmd socket to host
    while (
        connect(PROXY_SESSION_CMDFD(sess), host->ai_addr, host->ai_addrlen)) {
        perror("connect");
        sleep(1);
        if (tries++ == RECONNECT_TRIES) {
            // Handle error
            return 1;
        }
    }

    tries = 0;

    // Create the error socket
    if ((PROXY_SESSION_ERRORFD(sess) = socket(
             host->ai_family, host->ai_socktype, host->ai_protocol)) < 0) {
        perror("socket");
        return 1;
    }

    // Set the error socket to TCP_NODELAY
    sopt = 1;
    if (setsockopt(PROXY_SESSION_ERRORFD(sess), IPPROTO_TCP, TCP_NODELAY, &sopt,
                   sizeof(sopt)) < 0) {
        // Non-fatal error
        perror("setsockopt(TCP_NODELAY)");
    }

    // Connect the error socket to host
    while (
        connect(PROXY_SESSION_ERRORFD(sess), host->ai_addr, host->ai_addrlen)) {
        perror("connect");
        sleep(1);
        if (tries++ == RECONNECT_TRIES) {
            return 1;
        }
    }

    // Create a file stream for the error socket.
    PROXY_SESSION_ERRORSTREAM(sess) = fdopen(PROXY_SESSION_ERRORFD(sess), "w");

    // Set the stream to be line buffered
    setlinebuf(PROXY_SESSION_ERRORSTREAM(sess));

    // set the global error stream
    err_stream = PROXY_SESSION_ERRORSTREAM(sess);

    // Create the message socket
    if ((PROXY_SESSION_MSGFD(sess) = socket(host->ai_family, host->ai_socktype,
                                            host->ai_protocol)) < 0) {
        perror("socket");
        return 1;
    }

    // Set the message socket to TCP_NODELAY
    sopt = 1;
    if (setsockopt(PROXY_SESSION_MSGFD(sess), IPPROTO_TCP, TCP_NODELAY, &sopt,
                   sizeof(sopt)) < 0) {
        // Non-fatal error
        perror("setsockopt(TCP_NODELAY)");
    }

    // Connect the message socket to host
    while (
        connect(PROXY_SESSION_MSGFD(sess), host->ai_addr, host->ai_addrlen)) {
        perror("connect");
        sleep(1);
        if (tries++ == RECONNECT_TRIES) {
            return 1;
        }
    }

    // Create a file stream for the message socket.
    PROXY_SESSION_MSGSTREAM(sess) = fdopen(PROXY_SESSION_MSGFD(sess), "w");

    // Set the stream to be line buffered
    setlinebuf(PROXY_SESSION_MSGSTREAM(sess));

    // create a new instance list to hold onto proxy_type instances
    instances = NewList();

    // initialize the protocols

    // TODO: Allow this to select any protocol, other than the hardcoded mrnet.
    DebugClientProtocol * const ptab = findClientProtoByName("mrnet");
    if (ptab == nullptr) {
        // Handle error
        return 1;
    }

    // call the init function
    if (!ptab->init()) {
        // Handle error
        return 1;
    }

    for (;;) {
        switch (DbgCheckEvents()) {
            case -1:
                Quit = 1;

            case 0:
                // we only want to continue if we haven't triggered the Quit
                // condition.
                if (!Quit) {
                    continue;
                }
                break;

            default:
                DbgDispatchEvents();
        }

        if (Quit) break;
    }

    // call the finish function
    ptab->finish();

    return 0;
}

/*
** Main debugger event handler.
**
** Returns: -1 if an error occurred
**		0 if there is nothing to do
**		1 if there are callbacks to process
**
** This now will check on dbginvoke_t objects rather than on generic protocol
** layer.
*/
int DbgCheckEvents(void) {
    dbginvoke_t *di;
    int status;
    int *ev_fd;
    struct timeval tv = {1, 0};

    // reset the fd_set
    FD_ZERO(&dispatch_fds);
    high_fd = -1;

    /*
    ** Check if any events are ready.
    */

    // Check the protocols for each dbginvoke if there are any.
    if (!EmptyList(DbgInvokeList)) {
        // Reset the DbgInvokeList
        SetList(DbgInvokeList);

        // Loop through each dbginvoke entry and check for events.
        while ((di = (dbginvoke_t *)GetListElement(DbgInvokeList)) != (dbginvoke_t *)NULL) {
            /*
             Note: This should be used for protocols that do not use an event
             notification fd. If a event notification fd can be used, then 
             it should be added to the sw_evfd list and sw_checkevents should 
             simply return 0.
            */
            if (DBG_PROTO_SW(di)->checkEvents(di) > 0) {
                DPRINT("DbgCheckEvents: sw_checkevents returned 1\n");
                return 1;  // there is a proto event to handle. Return
                           // immediately.
            }

            /*
             If there is a d_evfds fd set, we want to add it to the fd_set to
             select on. This enables protocol events.
            */
            if (di->d_evfds != NULL) {
                SetList(di->d_evfds);

                while ((ev_fd = (int *)GetListElement(di->d_evfds)) !=
                       (int *)NULL) {
                    if (high_fd < *ev_fd) {
                        high_fd = *ev_fd;
                    }
                    FD_SET(*ev_fd, &dispatch_fds);
                }
            }
        }
    }

    /*
    ** Add the cmd stream fd of the main proxy object to the fd_set to select
    *on. This enables commands.
    */
    if (high_fd < PROXY_SESSION_CMDFD(sess)) {
        high_fd = PROXY_SESSION_CMDFD(sess);
    }

    FD_SET(PROXY_SESSION_CMDFD(sess), &dispatch_fds);

    /*
    ** Main select event polling loop.
    */
    for (;;) {
        // from the select manpage, nfds is the highest-numbered file descriptor
        // in any of the three sets, plus 1.
        status = select(high_fd + 1, &dispatch_fds, NULL, NULL, &tv);

        switch (status) {
            case -1:
                switch (errno) {
                    case EINTR:
                        DPRINT("DbgCheckEvents: EINTR\n");
                        continue;

                    default:
                        // DbgSetClntErr(DBGERR_SELECT, strerror(errno));
                        return -1;
                }
                break;

            case 0:
                return 0;

            default:
                return 1;
        }
    }

    // We should never reach here...
    DPRINT("DbgCheckEvents: At final return\n");
    return 1;
}

/*
** Dispactch pending debugger events.
*/
void DbgDispatchEvents(void) {
    dbginvoke_t *di;
    int *ev_fd;

    /*
    ** Now dispatch any callback routines that are active.
    */
    if (!EmptyList(DbgInvokeList)) {
        // Reset the DbgInvokeList
        SetList(DbgInvokeList);

        // Loop through each dbginvoke entry and check for events.
        while ((di = (dbginvoke_t *)GetListElement(DbgInvokeList)) !=
               (dbginvoke_t *)NULL) {
            // if the proto is shutting down, don't dispatch further pending
            // events
            if (di->d_isShutdown) continue;

            // If there is an d_evfds list, then the protol layer is using
            // event notification fd's. We should try to check to see if
            // each one is set, and if so call the protocol specific
            // dispatch function with the event fd as an argument
            if (di->d_evfds != NULL) {
                SetList(di->d_evfds);

                while ((ev_fd = (int *)GetListElement(di->d_evfds)) !=
                        (int *)NULL) {
                    // if the previous event caused a shutdown, don't
                    // continue processing events
                    if (di->d_isShutdown) break;

                    if (FD_ISSET(*ev_fd, &dispatch_fds)) {
                        // dispatch each of the set notification fds one at
                        // a time.
                        DBG_PROTO_SW(di)->dispatchEvents(di, *ev_fd);
                    }
                }
            } else {
                // event notification fd's not in use by this protocol, so
                // call the generic dispatch function with -1 for event fd
                // arg.
                DBG_PROTO_SW(di)->dispatchEvents(di, -1);
            }
        }
    }

    /*
    ** Process any commands
    */
    if (FD_ISSET(PROXY_SESSION_CMDFD(sess), &dispatch_fds)) {
        proxy_msg_header msg_head;
        char *msg = NULL;
        proto_msg_set *pmsg_set;
        proto_message *pmsg;
        bitset *set;
        recv_type *recv_set;
        attach_args *att_args;
        launch_args *launch_args;
        proxy_type *inst;
        int sopt, tries = 0;
        int detach;
        int n = 0;

        // ensure there is data to read on the cmd session socket. If there
        // isn't, that means the socket was closed. In which case we should
        // force things to quit since we lost our communication mechanism with
        // the client.
        if (ioctl(PROXY_SESSION_CMDFD(sess), FIONREAD, &n) < 0) {
            PrintOutError("ioctl failed.\n");
            CleanupOnSocketClose();
            return;
        }
        if (n < 1) {
            CleanupOnSocketClose();
        }

        // First recv the header
        if (recv(PROXY_SESSION_CMDFD(sess), &msg_head, sizeof(proxy_msg_header),
                 MSG_WAITALL) < 0) {
            PrintOutError("Failed to recv on event socket.\n");
            return;
        }

        // Next recv the message if there is one
        if (msg_head.msg_len > 0) {
            // Create a string buffer to receive the message
            msg = (char *)Alloc(msg_head.msg_len);

            // recv the message
            if (recv(PROXY_SESSION_CMDFD(sess), msg, msg_head.msg_len,
                     MSG_WAITALL) < 0) {
                PrintOutError("Failed to recv on event socket.\n");
                Free(msg);
                return;
            }
        }

        // switch on the msg_type
        switch (msg_head.msg_type) {
            case MSG_SEND_CMD:
                // convert msg to a proto message
                if ((pmsg = SocProtoToProtoMsg(msg)) == NULL) {
                    PrintOutError(
                        "Could not convert cmd stream msg to proto msg.\n");
                    Free(msg);
                    return;
                }

                // cleanup
                Free(msg);

                // find the dbginvoke_t associated with this id
                inst = GetProxyTypeFromClntId(msg_head.msg_id);
                di = NULL;
                if (!EmptyList(DbgInvokeList)) {
                    // Reset the DbgInvokeList
                    SetList(DbgInvokeList);

                    // Loop through each dbginvoke entry and check for events.
                    while ((di = (dbginvoke_t *)GetListElement(
                                DbgInvokeList)) != (dbginvoke_t *)NULL) {
                        if (di->d_proto_id == PROXY_ID(inst)) break;
                    }
                }

                // Send the command
                set = DBG_PROTO_SW(di)->sendCommand(di, pmsg->group, pmsg->message);

                // cleanup
                FreeProtoMsg(pmsg);

                // send a response back containing the set

                pmsg = NewProtoMsg(set, strdup(""));
                msg = ProtoMsgToSocProto(pmsg);

                msg_head.msg_type = MSG_OK;
                msg_head.msg_len = (int)strlen(msg) * sizeof(char);

                // Send the header
                if (send(PROXY_SESSION_CMDFD(sess), &msg_head,
                         sizeof(proxy_msg_header),
                         0) != sizeof(proxy_msg_header)) {
                    PrintOutError("Could not send header on cmd stream.\n");
                    return;
                }

                // Send the msg
                if (send(PROXY_SESSION_CMDFD(sess), msg, msg_head.msg_len, 0) !=
                    msg_head.msg_len) {
                    PrintOutError("Could not send msg on cmd stream.\n");
                    return;
                }

                // cleanup
                FreeProtoMsg(pmsg);
                Free(msg);

                break;

            case MSG_RECV_REPLY:
                // convert msg to a recv type
                if ((recv_set = SocProtoToRecv(msg)) == NULL) {
                    PrintOutError(
                        "Could not convert cmd stream msg to recv type.\n");
                    Free(msg);
                    return;
                }

                // cleanup
                Free(msg);

                // find the dbginvoke_t associated with this id
                inst = GetProxyTypeFromClntId(msg_head.msg_id);
                di = NULL;
                if (!EmptyList(DbgInvokeList)) {
                    // Reset the DbgInvokeList
                    SetList(DbgInvokeList);

                    // Loop through each dbginvoke entry and check for events.
                    while ((di = (dbginvoke_t *)GetListElement(
                                DbgInvokeList)) != (dbginvoke_t *)NULL) {
                        if (di->d_proto_id == PROXY_ID(inst)) break;
                    }
                }

                pmsg_set = DBG_PROTO_SW(di)->receiveReply(di, recv_set->target, recv_set->received);

                // send the result back

                ResetProtoMsgSet(pmsg_set);
                while ((pmsg = NextProtoMsgInSet(pmsg_set)) != NULL) {
                    // Send the header
                    msg_head.msg_type = MSG_OK;
                    msg = ProtoMsgToSocProto(pmsg);
                    msg_head.msg_len = (int)strlen(msg) * sizeof(char);

                    if (send(PROXY_SESSION_CMDFD(sess), &msg_head,
                             sizeof(proxy_msg_header),
                             0) != sizeof(proxy_msg_header)) {
                        PrintOutError("Could not send header on cmd stream.\n");
                        return;
                    }

                    // Send the msg
                    if (send(PROXY_SESSION_CMDFD(sess), msg, msg_head.msg_len,
                             0) != msg_head.msg_len) {
                        PrintOutError("Could not send msg on cmd stream.\n");
                        return;
                    }

                    // cleanup
                    Free(msg);
                }

                // cleanup
                FreeProtoMsgSet(pmsg_set);
                break;

            case MSG_ATTACH: {
                // convert msg to an attach_args type
                if ((att_args = SocProtoToAttachArgs(msg)) == NULL) {
                    PrintOutError(
                        "Could not convert cmd stream msg to recv type.\n");
                    Free(msg);
                    return;
                }

                // Create a new proxy object associated with this invoke
                inst = NewProxyType(sess);

                // Create the event socket
                if ((PROXY_EVENTFD(inst) =
                         socket(host->ai_family, host->ai_socktype,
                                host->ai_protocol)) < 0) {
                    perror("socket");
                    return;
                }

                // Set the event socket to TCP_NODELAY
                sopt = 1;
                if (setsockopt(PROXY_EVENTFD(inst), IPPROTO_TCP, TCP_NODELAY,
                               &sopt, sizeof(sopt)) < 0) {
                    // Non-fatal error
                    perror("setsockopt(TCP_NODELAY)");
                }

                // Create the output socket
                if ((PROXY_OUTPUTFD(inst) =
                         socket(host->ai_family, host->ai_socktype,
                                host->ai_protocol)) < 0) {
                    perror("socket");
                    return;
                }

                // Set the output socket to TCP_NODELAY since this will output
                // fstream data
                sopt = 1;
                if (setsockopt(PROXY_OUTPUTFD(inst), IPPROTO_TCP, TCP_NODELAY,
                               &sopt, sizeof(sopt)) < 0) {
                    // Non-fatal error
                    perror("setsockopt(TCP_NODELAY)");
                }

                /* Connect the sockets */

                // Connect the event socket to host
                while (connect(PROXY_EVENTFD(inst), host->ai_addr,
                               host->ai_addrlen)) {
                    perror("connect");
                    sleep(1);
                    if (tries++ == RECONNECT_TRIES) {
                        return;
                    }
                }

                tries = 0;

                // Connect the output socket to host
                while (connect(PROXY_OUTPUTFD(inst), host->ai_addr,
                               host->ai_addrlen)) {
                    perror("connect");
                    sleep(1);
                    if (tries++ == RECONNECT_TRIES) {
                        return;
                    }
                }

                // Create a file stream for the output socket.
                PROXY_OUTPUTSTREAM(inst) = fdopen(PROXY_OUTPUTFD(inst), "w");

                // Set the stream to be line buffered
                setlinebuf(PROXY_OUTPUTSTREAM(inst));

                // TODO: Allow this to select any protocol, other than the hardcoded mrnet we see now.
                DebugClientProtocol * const ptab = findClientProtoByName("mrnet");
                // Set the protocol based on the proto arg
                if (ptab == nullptr) {
                    // Handle error
                    return;
                }

                // Create the dbginvoke_t object to pass down, nproc is unknown
                // since this is the attach case
                di = NewDbgInvoke(0, ptab);

                // Call the protocol specific attach function
                // TODO: This could be modified to allow for multiple proxy
                // invocations,
                //       to support multiple hops, but that is not required
                //       right now...
                if (ptab->attach(di, NULL, att_args->app_ident,
                                    att_args->arch, att_args->backend,
                                    att_args->gpu_flag, att_args->debug_flag)) {
                    // Handle error
                    FreeAttachArgs(att_args);
                    FreeDbgInvoke(di);
                    return;
                }

                // cleanup attach args
                FreeAttachArgs(att_args);

                // set the proto id in the di
                di->d_proto_id = PROXY_ID(inst);

                // Create a proxy_instance object to associate the client id
                // with our id
                NewProxyInstance(msg_head.msg_id, inst);

                // Send the DBG_OK event for the entire set
                set = bitset_new(di->d_nproc);
                bitset_invert(set);
                pmsg_set = NewProtoMsgSet();
                AddProtoMsgSet(
                    pmsg_set,
                    NewProtoMsg(set, EventToSocProto(NewEvent((dbgev)DBG_OK))));
                DbgDispatchProtoEvents(di, pmsg_set);

                break;
            }

            case MSG_INVOKE: {
                // convert msg to an invoke_args type
                if ((launch_args = SocProtoToLaunchArgs(msg)) == NULL) {
                    PrintOutError(
                        "Could not convert cmd stream msg to recv type.\n");
                    Free(msg);
                    return;
                }

                // Create a new proxy object associated with this invoke
                inst = NewProxyType(sess);

                // Create the event socket
                if ((PROXY_EVENTFD(inst) =
                         socket(host->ai_family, host->ai_socktype,
                                host->ai_protocol)) < 0) {
                    perror("socket");
                    return;
                }

                // Set the event socket to TCP_NODELAY
                sopt = 1;
                if (setsockopt(PROXY_EVENTFD(inst), IPPROTO_TCP, TCP_NODELAY,
                               &sopt, sizeof(sopt)) < 0) {
                    // Non-fatal error
                    perror("setsockopt(TCP_NODELAY)");
                }

                // Create the output socket
                if ((PROXY_OUTPUTFD(inst) =
                         socket(host->ai_family, host->ai_socktype,
                                host->ai_protocol)) < 0) {
                    perror("socket");
                    return;
                }

                // Set the output socket to TCP_NODELAY since this will output
                // fstream data
                sopt = 1;
                if (setsockopt(PROXY_OUTPUTFD(inst), IPPROTO_TCP, TCP_NODELAY,
                               &sopt, sizeof(sopt)) < 0) {
                    // Non-fatal error
                    perror("setsockopt(TCP_NODELAY)");
                }

                /* Connect the sockets */

                // Connect the event socket to host
                while (connect(PROXY_EVENTFD(inst), host->ai_addr,
                               host->ai_addrlen)) {
                    perror("connect");
                    sleep(1);
                    if (tries++ == RECONNECT_TRIES) {
                        return;
                    }
                }

                tries = 0;

                // Connect the output socket to host
                while (connect(PROXY_OUTPUTFD(inst), host->ai_addr,
                               host->ai_addrlen)) {
                    perror("connect");
                    sleep(1);
                    if (tries++ == RECONNECT_TRIES) {
                        return;
                    }
                }

                // Create a file stream for the output socket.
                PROXY_OUTPUTSTREAM(inst) = fdopen(PROXY_OUTPUTFD(inst), "w");

                // Set the stream to be line buffered
                setlinebuf(PROXY_OUTPUTSTREAM(inst));

                // TODO: Allow this to select any protocol, other than the hardcoded mrnet we see now.
                DebugClientProtocol * const ptab = findClientProtoByName("mrnet");
                // Set the protocol based on the proto arg
                if (ptab == nullptr) {
                    // Handle error
                    return;
                }

                // Create the dbginvoke_t object to pass down
                di = NewDbgInvoke(launch_args->nproc, ptab);

                // Call the protocol specific invoke function
                // TODO: This could be modified to allow for multiple proxy
                // invocations,
                //       to support multiple hops, but that is not required
                //       right now...
                if (ptab->launch(di, NULL, launch_args->prog, launch_args->argv,
                                    launch_args->arch, launch_args->arch_args,
                                    launch_args->arch_input, launch_args->backend, launch_args->gdb,
                                    launch_args->env, launch_args->cwd,
                                    launch_args->gpu_flag, launch_args->debug_flag,
                                    launch_args->early_entry_flag,
                                    launch_args->non_stop_flag)) {
                    // Handle error
                    FreeLaunchArgs(launch_args);
                    FreeDbgInvoke(di);
                    return;
                }

                // set the proto id in the di
                di->d_proto_id = PROXY_ID(inst);

                // Create a proxy_instance object to associate the client id
                // with our id
                NewProxyInstance(msg_head.msg_id, inst);

                // cleanup invoke args
                FreeLaunchArgs(launch_args);

                // Send the DBG_OK event for the entire set
                set = bitset_new(di->d_nproc);
                bitset_invert(set);
                pmsg_set = NewProtoMsgSet();
                AddProtoMsgSet(
                    pmsg_set,
                    NewProtoMsg(set, EventToSocProto(NewEvent((dbgev)DBG_OK))));
                DbgDispatchProtoEvents(di, pmsg_set);

                break;
            }

            case MSG_SAVE_SESSION:
                // Set this session as valid
                PROXY_SESSION_VALID(sess) = 1;

                // Send the MSG_OK response
                msg_head.msg_id = -1;
                msg_head.msg_len = 0;
                msg_head.msg_type = MSG_OK;

                if (send(PROXY_SESSION_CMDFD(sess), &msg_head,
                         sizeof(proxy_msg_header),
                         0) != sizeof(proxy_msg_header)) {
                    PrintOutError("Could not send header on cmd stream.\n");
                }

                break;

            case MSG_CLEANUP_SESSION:
                // Set this session as invalid
                PROXY_SESSION_VALID(sess) = 0;

                // Send the MSG_OK response
                msg_head.msg_id = -1;
                msg_head.msg_len = 0;
                msg_head.msg_type = MSG_OK;

                if (send(PROXY_SESSION_CMDFD(sess), &msg_head,
                         sizeof(proxy_msg_header),
                         0) != sizeof(proxy_msg_header)) {
                    PrintOutError("Could not send header on cmd stream.\n");
                }

                // Call the cleanup function to ensure it gets cleaned up if
                // there are no longer any valid instances connected with the
                // session. This might cause us to exit.
                CleanupProxySession(sess);

                break;

            case MSG_EXIT:
                // convert msg to exit args
                detach = SocProtoToExitArgs(msg);

                // cleanup
                Free(msg);

                // find the dbginvoke_t associated with this id
                inst = GetProxyTypeFromClntId(msg_head.msg_id);
                di = NULL;
                if (!EmptyList(DbgInvokeList)) {
                    // Reset the DbgInvokeList
                    SetList(DbgInvokeList);

                    // Loop through each dbginvoke entry and check for events.
                    while ((di = (dbginvoke_t *)GetListElement(
                                DbgInvokeList)) != (dbginvoke_t *)NULL) {
                        if (di->d_proto_id == PROXY_ID(inst)) break;
                    }
                }

                // TODO: Handle error

                // ensure the exit function is implemented
                if (di != NULL) {
                    // call the exit function
                    if (DBG_PROTO_SW(di)->exit(di, detach)) {
                        PrintOutError("Exit failed.\n");
                        msg_head.msg_type = MSG_ERROR;
                    } else {
                        msg_head.msg_type = MSG_OK;
                    }
                } else {
                    if (di == NULL) {
                        msg_head.msg_type = MSG_OK;
                    } else {
                        PrintOutError("Exit not implemented.\n");
                        msg_head.msg_type = MSG_ERROR;
                    }
                }

                // Send just the header back in response
                msg_head.msg_len = 0;

                // Send the header
                if (send(PROXY_SESSION_CMDFD(sess), &msg_head,
                         sizeof(proxy_msg_header),
                         0) != sizeof(proxy_msg_header)) {
                    PrintOutError("Could not send header on cmd stream.\n");
                    return;
                }

                // Cleanup the proxy instance, we are done with it.
                if (di != NULL) FreeDbgInvoke(di);
                FreeProxyInstance(inst);

                break;

            case MSG_OK:
            case MSG_ERROR:
                // These should not be called!
                break;
        }
    }
}

int DbgDispatchProtoEvents(dbginvoke_t *di, proto_msg_set *set) {
    proto_message *pmsg;
    char *msg;
    proxy_msg_header msg_head;
    proxy_type *inst;

    if (set == NULL) return 0;

    // get the proxy_type from the instance id
    inst = GetProxyTypeFromInstId(di->d_proto_id);

    ResetProtoMsgSet(set);
    while ((pmsg = NextProtoMsgInSet(set)) != NULL) {
        // Convert this to a string
        msg = ProtoMsgToSocProto(pmsg);

        // Send the header
        msg_head.msg_len = (int)strlen(msg) * sizeof(char);
        msg_head.msg_type = MSG_OK;
        if (send(PROXY_EVENTFD(inst), &msg_head, sizeof(proxy_msg_header), 0) !=
            sizeof(proxy_msg_header)) {
            PrintOutError("Could not send header on event stream.\n");
            return 0;
        }

        // Send the msg
        if (send(PROXY_EVENTFD(inst), msg, msg_head.msg_len, 0) !=
            msg_head.msg_len) {
            PrintOutError("Could not send msg on event stream.\n");
            return 0;
        }

        // Done with msg
        Free(msg);
    }

    return 1;
}

void DbgDispatchProtoOutput(dbginvoke_t *di, FILE *p_o_stream) {
    proxy_type *inst;
    char *line;

    if (di == NULL || p_o_stream == NULL) return;

    // get the proxy_type from the instance id
    inst = GetProxyTypeFromInstId(di->d_proto_id);

    // Output a program line recieved on the program output pipe
    line = (char *)Alloc(BUFSIZ);

    while (fgets(line, BUFSIZ, p_o_stream) > 0) {
        fprintf(PROXY_OUTPUTSTREAM(inst), "%s", line);
    }

    Free(line);
}

/*
** fprintf a line to stderr.
*/
void PrintOutError(const char *format, ...) {
    va_list arg_list;

    // sanity
    if (PROXY_SESSION_ERRORSTREAM(sess) == NULL) return;

    va_start(arg_list, format);

    // vfprintf the line we recieved
    vfprintf(PROXY_SESSION_ERRORSTREAM(sess), format, arg_list);

    // end the va_list
    va_end(arg_list);
}

/*
** fprintf a line to stdout.
*/
void PrintOutMsg(const char *format, ...) {
    va_list arg_list;

    // sanity
    if (PROXY_SESSION_MSGSTREAM(sess) == NULL) return;

    va_start(arg_list, format);

    // vfprintf the line we recieved
    vfprintf(PROXY_SESSION_MSGSTREAM(sess), format, arg_list);

    // end the va_list
    va_end(arg_list);
}

/*
** get a line from stdin - We want to force this to fail here...
*/
char *cli_get_line(void) {
    PrintOutError("cli_get_line should not be called from proxy interface.\n");
    return NULL;
}

void ForceQuit(void) { _exit(1); }
