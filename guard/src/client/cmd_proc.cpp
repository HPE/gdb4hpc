/*********************************************************************************\
 * Process control commands.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "client.h"
#include "common_tools_fe.h"
#include "gvar.h"
#include "useful.h"

/*
** Static global variables
*/
static int debug_flag = 0;
static int gpu_flag = 0;
static bool non_stop_mode = false;

/*
** static function prototypes
*/
static int DoInvoke(const char *, proto_session *, const char *, char **,
                    dbgprocset_t *, const char *, char **, const char *,
                    const char *, char **, const char *);
static int DoAttach(const char *, proto_session *, dbgprocset_t *, const char *,
                    const char *, const char *);
static void ReleaseProc(dbgprocset_t *, dbgquit_type);

/*
** long opts definitions
*/
static const struct option session_long_opts[] = {
    {"qsub", optional_argument, 0, 'q'},
    {"sbatch", optional_argument, 0, 's'},
    {"end", optional_argument, 0, 'e'},
    {0, 0, 0, 0}};

static const struct option launch_long_opts[] = {
    {"args", optional_argument, 0, 'a'},
    {"launcher-args", optional_argument, 0, 'g'},
    {"launcher-input", optional_argument, 0, 'i'},
    {"workdir", optional_argument, 0, 'd'},
    {"env", optional_argument, 0, 'e'},
    {"qsub", optional_argument, 0, 'q'},
    {"sbatch", optional_argument, 0, 's'},
    {"gpu", no_argument, &gpu_flag, 1},
    {"debug", no_argument, &debug_flag, 1},
    {"launcher", optional_argument, 0, 'l'},
    {0, 0, 0, 0}};

static const struct option attach_long_opts[] = {
    {"gpu", no_argument, &gpu_flag, 1},
    {"debug", no_argument, &debug_flag, 1},
    {0, 0, 0, 0}};

static int class_info_subcmd_application_cmd(rec_set_t *rec_set, int argc,
                                             char **argv) {
    dbgprocset_t *ps;
    int num_ps = 0;
    dbgprocset_t *c_ps;
    dbg_over_set_t *over_set;
    dbg_over_set_t *c_over_set;
    dbginvoke_t *di;
    int found;

    PrintOutMsg("%-10s %-10s %-5s %s\n", "Handle", "Ranks", "Focus",
                "Application");

    SetProcSet();
    while ((ps = GetProcSet()) != NULL) {
        if (ps->s_type != DBGSET_INVOKE) continue;

        ++num_ps;

        // Get the current process set.
        c_ps = CurrentProcSet();

        SetOverlaySetForProcSet(ps);
        while ((over_set = GetNextOverlaySetForProcSet(ps)) != NULL) {
            // point at the di
            di = over_set->s_di;
            PrintOutMsg("$%-9s %-10d ", di->d_handle, di->d_nproc);

            // reset found, we only print n if the di is not in the over_set
            found = 0;
            SetOverlaySetForProcSet(c_ps);
            while ((c_over_set = GetNextOverlaySetForProcSet(c_ps)) != NULL) {
                if (di == c_over_set->s_di) {
                    PrintOutMsg("%-5s ", "y");
                    ++found;
                    break;
                }
            }
            if (!found) {
                PrintOutMsg("%-5s ", "n");
            }

            if (di->d_cmd != NULL) {
                PrintOutMsg("%s\n", di->d_cmd);
            } else {
                PrintOutMsg("\n");
            }
        }
    }

    if (num_ps == 0) {
        PrintOutError(
            "There are currently no lauched/attached applications.\n");
        return 1;
    }

    return 1;
}

static int cmd_session_cmd(rec_set_t *rec_set, int argc, char **argv) {
    proto_session *sess = NULL;
    const char *proto = Client.proto;
    WLM_type manager = WLM_none;
    char *batch_script = NULL;
    int opt_ind = 0;
    int opt;

    // reset optind
    optind = 0;

    //
    // Process user provided arguments
    //

    while ((opt = getopt_long(argc, argv, "q:s:e:", session_long_opts,
                              &opt_ind)) != -1) {
        switch (opt) {
            case 0:
                // if this is a flag, do nothing
                break;

            case 'q':
                if (optarg == (char *)NULL) {
                    PrintOutError(
                        "Error: --qsub provided, but missing arguments.\n");
                    goto exit_with_error;
                }

                // Note that we want to convert any relative path name into an
                // absolute pathname
                if ((batch_script = realpath(StripQuotes(optarg), NULL)) ==
                    (char *)NULL) {
                    PrintOutError(
                        "Failed to resolve absolute path of provided batch "
                        "script.\n");
                    goto exit_with_error;
                }

                // set proto to proxy since we are required to use the proxy in
                // conjunction with the batch system
                proto = "proxy";
                // set the manager to qsub
                manager = WLM_qsub;
                break;

            case 's':
                if (optarg == (char *)NULL) {
                    PrintOutError(
                        "Error: --sbatch provided, but missing arguments.\n");
                    goto exit_with_error;
                }

                // Note that we want to convert any relative path name into an
                // absolute pathname
                if ((batch_script = realpath(StripQuotes(optarg), NULL)) ==
                    (char *)NULL) {
                    PrintOutError(
                        "Failed to resolve absolute path of provided batch "
                        "script.\n");
                    goto exit_with_error;
                }

                // set proto to proxy since we are required to use the proxy in
                // conjunction with the batch system
                proto = "proxy";
                // set the manager to sbatch
                manager = WLM_sbatch;
                break;

            case 'e':
                // User wants to end the session. So lets set the current
                // session to NULL. This handles cleanups for us automatically.
                if ((sess = DbgGetSession()) != NULL) {
                    PrintOutMsg(
                        "Ending current session. Future attach/launch commands "
                        "will not be associated with it.\n");
                }
                DbgSetSession(NULL);
                break;

            default:
                PrintOutError(
                    "Invalid session argument provided. See \"help session\" "
                    "for usage.\n");
                return 1;
        }
    }

    // Make sure the user provided us batch arguments, otherwise return.
    if (batch_script == NULL || manager == WLM_none) {
        return 1;
    }

    // Create a new session
    if ((sess = DbgSessionCreate(proto, manager, batch_script, NULL, NULL)) ==
        NULL) {
        PrintOutError("Could not create session for WLM!\n");
        goto exit_with_error;
    }

    // Set this session for all future launch/attach commands
    DbgSetSession(sess);

    PrintOutMsg("Session ready.\n");

    Free(batch_script);

    return 1;

exit_with_error:

    if (batch_script != NULL) Free(batch_script);

    return 0;
}

/*
** TODO: This is now called launch, but the source refers to it as invoke
** invoke $proc['['num']',...],... program [--args "prog-args"]
**  [--using arch] [--arch-args "arch-args"] [--arch-input "inputFile"]
**  [--on host] [--with proto] [--backend debugger] [--gpu]
*/
static int cmd_invoke_cmd(rec_set_t *rec_set, int argc, char **argv) {
    proto_session *sess = NULL;
    dbgprocset_t *ps = NULL;
    char *prog = NULL;
    const char *arch = Client.arch;
    const char *proto = Client.proto;
    const char *backend = Client.backend;
    char *arch_input = NULL;
    int set_default_workdir = 0;
    char *workdir = NULL;
    char **env_list = NULL;
    int env_list_num = 0;
    int env_list_size = 0;
    char *env_str = NULL;
    char **args = NULL;
    char **arch_args = NULL;
    WLM_type manager = WLM_none;
    char *batch_script = NULL;
    // TODO: Allow the user to specify hostname/username
    char *hostname = NULL;
    char *username = NULL;
    expr_t *e;
    int opt_ind = 0;
    int opt;
    int i;
    char *elogin_env;

    // reset optind and flags
    optind = 0;
    debug_flag = 0;
    gpu_flag = 0;

    if (argc < 3) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: launch $handle ./program\n");
        return 0;
    }

    //
    // Setup the env_list copying in any environment variables set by calling
    // the set environment command.
    //

    if (SizeOfList(Client.set_env_list) > 1) {
        // there are entires in the list, process them
        SetList(Client.set_env_list);

        // loop through the list
        while ((env_str = (char *)GetListElement(Client.set_env_list)) !=
               (char *)NULL) {
            // realloc memory if env_list_num + 1 is GEQ to env_list_size
            // note that we need at least 1 entry that acts as a null pointer
            if ((env_list_num + 1) >= env_list_size) {
                // increase the list by 10 entries
                env_list_size += 10;
                // BUG 801222 - ensure the list is not null
                if (env_list == NULL) {
                    env_list = (char **)Alloc(env_list_size * sizeof(char *));
                    memset(env_list, 0, env_list_size * sizeof(char *));
                } else {
                    Resize((void *)env_list, env_list_size * sizeof(char *));
                    // set the new entries to null, note that env_list_num is
                    // where the new entry will be placed, the malloced entries
                    // start after this.
                    memset(&env_list[env_list_num], 0, 10 * sizeof(char *));
                }
            }

            // set the new entry
            env_list[env_list_num++] = strdup(env_str);
        }
    }

    //
    // Process user provided arguments
    //

    // Note that invoke/launch used to allow the user to provide the following
    // args:
    //
    // --arch-args --arch-input --using --on --backend --with --path
    //
    // These are no longer supported with longopts. Their use relates back to
    // the pre-MRNet days and legacy stuff. I don't think there is value in
    // maintaining these in the Cray trunk.

    // This will permute the non-option stuff to the end, so parse that stuff
    // last.

    while ((opt = getopt_long(argc, argv, "a:g:i:d::e:q:", launch_long_opts,
                              &opt_ind)) != -1) {
        switch (opt) {
            case 0:
                // if this is a flag, do nothing
                break;
            case 'a':
                if (optarg == (char *)NULL) {
                    PrintOutError(
                        "Error: --args provided, but missing arguments.\n");
                    goto exit_with_error;
                }
                args = Str2Argv(StripQuotes(optarg));
                break;

            case 'g':
                if (optarg == (char *)NULL) {
                    PrintOutError(
                        "Error: --launcher-args provided, but missing "
                        "arguments.\n");
                    goto exit_with_error;
                }
                arch_args = Str2Argv(StripQuotes(optarg));
                break;

            case 'q':
                if (optarg == (char *)NULL) {
                    PrintOutError(
                        "Error: --qsub provided, but missing arguments.\n");
                    goto exit_with_error;
                }

                // Note that we want to convert any relative path name into an
                // absolute pathname
                if ((batch_script = realpath(StripQuotes(optarg), NULL)) ==
                    (char *)NULL) {
                    PrintOutError(
                        "Failed to resolve absolute path of provided batch "
                        "script.\n");
                    goto exit_with_error;
                }

                // set proto to proxy since we are required to use the proxy in
                // conjunction with the batch system
                proto = "proxy";
                // set the manager to qsub
                manager = WLM_qsub;
                break;

            case 's':
                if (optarg == (char *)NULL) {
                    PrintOutError(
                        "Error: --sbatch provided, but missing arguments.\n");
                    goto exit_with_error;
                }

                // Note that we want to convert any relative path name into an
                // absolute pathname
                if ((batch_script = realpath(StripQuotes(optarg), NULL)) ==
                    (char *)NULL) {
                    PrintOutError(
                        "Failed to resolve absolute path of provided batch "
                        "script.\n");
                    goto exit_with_error;
                }

                // set proto to proxy since we are required to use the proxy in
                // conjunction with the batch system
                proto = "proxy";
                // set the manager to sbatch
                manager = WLM_sbatch;
                break;

            case 'i':
                if (optarg == (char *)NULL) {
                    PrintOutError(
                        "Error: --launcher-input provided, but missing "
                        "arguments.\n");
                    goto exit_with_error;
                }
                arch_input = strdup(StripQuotes(optarg));
                break;

            case 'd':
                if (optarg == (char *)NULL) {
                    // --workdir provided without argument, so default to chdir
                    // to the directory that the binary is located in. We need
                    // to post process this since the name of the a.out is not
                    // known yet.
                    ++set_default_workdir;
                } else {
                    // Note that we want to convert any relative path name into
                    // an absolute pathname
                    if ((workdir = realpath(StripQuotes(optarg), NULL)) ==
                        (char *)NULL) {
                        PrintOutError(
                            "Error: --workdir provided, but failed to resolve "
                            "its absolute path.\n");
                        goto exit_with_error;
                    }
                }
                break;

            case 'e':
                if (optarg == (char *)NULL) {
                    PrintOutError(
                        "Error: --env provided, but missing arguments.\n");
                    goto exit_with_error;
                }

                // realloc memory if env_list_num + 1 is GEQ to env_list_size
                // note that we need at least 1 entry that acts as a null
                // pointer
                if ((env_list_num + 1) >= env_list_size) {
                    // increase the list by 10 entries
                    env_list_size += 10;
                    // BUG 801222 - ensure the list is not null
                    if (env_list == NULL) {
                        env_list =
                            (char **)Alloc(env_list_size * sizeof(char *));
                        memset(env_list, 0, 10 * sizeof(char *));
                    } else {
                        Resize((void *)env_list,
                               env_list_size * sizeof(char *));
                        // set the new entries to null, note that env_list_num
                        // is where the new entry will be placed, the malloced
                        // entries start after this.
                        memset(&env_list[env_list_num], 0, 10 * sizeof(char *));
                    }
                }

                // set the new entry
                env_list[env_list_num++] = strdup(StripQuotes(optarg));
                break;
            case 'l':
                if (optarg == (char *)NULL) {
                    PrintOutError(
                        "Error: --launcher provided, but missing arguments.\n");
                    goto exit_with_error;
                }

                setenv(CTI_LAUNCHER_NAME_ENV_VAR, optarg, 1);
                break;
            default:
                PrintOutError(
                    "Invalid launch argument provided. See \"help launch\" for "
                    "usage.\n");
                goto exit_with_error;
        }
    }

    // Ensure the user provided two non-option arguments
    if (argc - optind != 2) {
        PrintOutError(
            "Invalid number of non-option arguments. See \"help launch\" for "
            "usage.\n");
        PrintOutError("Expected 2, found %d.\n", argc - optind);
        goto exit_with_error;
    }

    // parse the non-option arguments

    // The first argument should be the name of the process set
    if ((e = ParseExpr(argv[optind++])) == NULL) {
        PrintError();
        goto exit_with_error;
    }

    // Create the process set for invoke
    if ((ps = CreateProcSetForInvoke(e)) == NULL) {
        FreeExpr(e);
        goto exit_with_error;
    }

    FreeExpr(e);

    // The second argument is the name of the a.out

    // Note that we want to convert any relative path name into an absolute
    // pathname
    if ((prog = realpath(argv[optind++], NULL)) == (char *)NULL) {
        PrintOutError(
            "Failed to resolve absolute path of provided application.\n");
        goto exit_with_error;
    }

    // Process any arguments from above that were dependent on the non-option
    // arguments

    if (set_default_workdir) {
        // --workdir provided without argument, so default to chdir to
        // the directory that the binary is located in.
        char *tmp;
        char *ptr;

        // We want to point at the final "/" in the string
        tmp = strdup(prog);
        ptr = strrchr(tmp, '/');

        if (ptr != NULL) {
            *ptr = '\0';
            workdir = strdup(tmp);
        } else {
            // No path provided, so we can't chdir anywhere. Silently ignore the
            // argument This should never happen since prog points at the
            // realpath of the app
            workdir = NULL;
        }

        Free(tmp);
    }

    // Verify that the arguments are correct

    if (arch == NULL) {
        PrintOutError("Must specify an MP architecure.\n");
        goto exit_with_error;
    }

    if (proto == NULL) {
        PrintOutError("Must specify a protocol.\n");
        goto exit_with_error;
    }

    // Check to see if we need to handle a session for this invoke
    switch (manager) {
        case WLM_none:
            // First check to see if we currently have an open session
            if ((sess = DbgGetSession()) != NULL) {
                // There currently is an open session, so use it in the invoke
                proto = "proxy";
                break;
            }

            // Get the value of the ESWRAP_LOGIN environment variable
            elogin_env = getenv("ESWRAP_LOGIN");
            if (elogin_env == NULL) {
                // Also check value of new EPROXY_LOGIN var
                elogin_env = getenv("EPROXY_LOGIN");
            }

            // If on a Cray system, check to see if we are currently on an
            // eslogin node if using WLM_none.
            //
            // If so, we want to create a new session on the service node using
            // the proxy protocol and forward every command through that.
            //
            // For the cluster system, this is not the case. We do not need a
            // proxy.
            // FIXME: The same is also true for native slurm.

            // Are we on an eslogin node?
            if (elogin_env != NULL) {
                // We are on an eslogin node since the file did not exist

                // set proto to proxy since we are required to use the proxy in
                // conjunction with eslogin nodes
                proto = "proxy";

                hostname = strdup(elogin_env);

                // Create a new session
                if ((sess = DbgSessionCreate(proto, manager, NULL, hostname,
                                             username)) == NULL) {
                    PrintOutError("Could not create session for eslogin!\n");
                    goto exit_with_error;
                }

                // Set this session for all future launch/attach commands
                DbgSetSession(sess);
            }

            // There is no current session and we are not on an eslogin node, so
            // we can use the default protocol.

            break;

        case WLM_qsub:
        case WLM_sbatch:

            // These sessions are only tied to this launch command and are not
            // set for future use. Aka they are temporary in nature and get
            // cleaned up when the dbginvoke gets cleaned up.

            // Create a new session
            if ((sess = DbgSessionCreate(proto, manager, batch_script, NULL,
                                         NULL)) == NULL) {
                PrintOutError("Could not create session for WLM!\n");
                goto exit_with_error;
            }

            break;
    }

    //
    // Invoke the application
    //
    if (!DoInvoke(proto, sess, prog, args, ps, arch, arch_args, arch_input,
                  backend, env_list, workdir)) {
        // If XML mode is enabled, we want to print out a removed message.
        if (Client.XML_mode) {
            XMLPrintRemovedMsg(ps->s_name);
        }
        goto exit_with_error;
    }

    // normal return

    Free(prog);
    FreeArgv(args);
    FreeArgv(arch_args);
    if (batch_script != NULL) Free(batch_script);
    if (hostname != NULL) Free(hostname);
    if (username != NULL) Free(username);
    if (arch_input != NULL) Free(arch_input);
    if (workdir != NULL) Free(workdir);
    if (env_list != NULL) {
        i = 0;
        while (env_list[i] != NULL) Free(env_list[i++]);
        Free(env_list);
    }

    return 1;

exit_with_error:

    FreeArgv(args);
    FreeArgv(arch_args);
    if (ps != NULL) FreeProcSet(ps);
    if (prog != NULL) Free(prog);
    if (batch_script != NULL) Free(batch_script);
    if (hostname != NULL) Free(hostname);
    if (username != NULL) Free(username);
    if (arch_input != NULL) Free(arch_input);
    if (workdir != NULL) Free(workdir);
    if (env_list != NULL) {
        i = 0;
        while (env_list[i] != NULL) Free(env_list[i++]);
        Free(env_list);
    }

    return 0;
}

/*
** Invoke helper routine. Invoke one or more processes.
** Note that the invoke process always operates synchronously.
*/
static int DoInvoke(const char *proto, proto_session *sess, const char *prog,
                    char **args, dbgprocset_t *ps, const char *arch,
                    char **arch_args, const char *arch_input,
                    const char *backend, char **Env, const char *cwd) {
    dbginvoke_t *di;
    dbgframe_t *f;
    rec_set_t *rec_set;
    rep_set_t *rep_set;
    rep_set_t *quit_rep;
    int p2pmode = 0;

    // Call the invoke in the dbg_client layer

    // Launches with generic wlm and non srun/aprun launchers use early entry
    char *launcher_name = getenv(CTI_LAUNCHER_NAME_ENV_VAR);
    cti_wlm_type_t current_wlm = cti_current_wlm();
    if (current_wlm == CTI_WLM_SSH && launcher_name != NULL &&
        strcmp(launcher_name, "srun") != 0 &&
        strcmp(launcher_name, "aprun") != 0) {
        Client.early_entry = 1;
    }

    di = DbgInvoke(proto, sess, prog, args, ps->s_name, ps->s_nproc, arch,
                   arch_args, arch_input, backend, Env, cwd, gpu_flag,
                   debug_flag, Client.unsafe, Client.early_entry, non_stop_mode);

    if (di == NULL) {
        // Do not print out an error message here since the lower layer already
        // printed it for us.
        return 0;
    }

    // Create a rec_set based on the di
    rec_set = NewRecSet();
    AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full));

    // Finalize setup of the procset
    if (FinalizeProcSetForInvoke(ps, di)) {
        quit_rep = DbgQuit(rec_set, DBG_KILL);
        ProcessErrorInRepSet(quit_rep);
        FreeRecSet(rec_set);
        FreeRepSet(quit_rep);
        return 0;
    }

    // Set the client event callback handler
    DbgHandleEvent(di, EventCB);

    // Issue the startup command
    rep_set = DbgStartup(rec_set);

    // process error
    if (ProcessErrorInRepSet(rep_set)) {
        PrintOutError("Could not startup application.\n");
        quit_rep = DbgQuit(rec_set, DBG_KILL);
        ProcessErrorInRepSet(quit_rep);
        FreeRecSet(rec_set);
        FreeRepSet(quit_rep);
        FreeRepSet(rep_set);
        return 0;
    }

    // Print the startup frames based on this bitset
    if (Client.print_entry_frame) {
        for (auto&& reply : rep_set) {
            // point at the frame
            f = EVENT_FRAME(reply->rep_event);
            // print out the message
            if (f->frame_loc.loc_line > 0) {
                PrintMsgWithSetInfo(
                    reply->rep_di->d_handle, reply->rep_group,
                    "Initial breakpoint, %s at %s:%d\n", f->frame_loc.loc_func,
                    f->frame_loc.loc_file, f->frame_loc.loc_line);
            } else {
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                    "Initial breakpoint, in %s\n",
                                    f->frame_loc.loc_func);
            }
            // If XML mode is enabled, we want to print out a status message.
            // We don't need to ask the backend for the status since this is
            // implied by printing out the initial breakpoint.
            if (Client.XML_mode) {
                char *set_str;
                char *status_block;
                status_block = XMLStartStatusBlock(reply->rep_di->d_handle);
                // Create the full set string
                Asprintf(&set_str, "%s%s", reply->rep_di->d_handle,
                         bitset_to_set(reply->rep_group));
                XMLAddStatus(&status_block, set_str, DBGSTAT_STOPPED,
                             &(f->frame_loc));
                Free(set_str);
                XMLPrintStatusBlock(status_block);
            }
        }
    }

    // Done with the startup reply
    FreeRepSet(rep_set);

    // Free the created rec_set
    FreeRecSet(rec_set);

    // for P2P here
    GetGraphVar(GRAPH_P2PMODE, (int *)&p2pmode);

    if (p2pmode) {
        // FIXME: andrewg: the getenv call here can fail. This needs to be
        //        fixed. When the p2p is fixed to actually ship the
        //        dbglog_netinfo file, this entire thing should be removed.

        // now, rename the file dbg_netinfo --> dbg_netinfo_<procset_name> file
        char filename[BUFSIZ];
        char filerename[BUFSIZ];
        char *filepath;
        filepath = getenv("CRAY_DBG_LOG_DIR");
        sprintf(filename, "%s/dbglog_netinfo_%s", filepath, ps->s_name);
        sprintf(filerename, "%s/dbglog_netinfo.0.log", filepath);
        if (rename(filerename, filename) < 0) {
            PrintOutError("Error in renaming file.");
        }
    }
    // end

    return 1;
}

/*
** attach $proc apid [--gpu] [--debug]
*/
static int cmd_attach_cmd(rec_set_t *rec_set, int argc, char **argv) {
    proto_session *sess = NULL;
    dbgprocset_t *ps;
    char *app_ident;
    const char *arch = Client.arch;
    const char *proto = Client.proto;
    const char *backend = Client.backend;
    char *hostname = NULL;
    char *username = NULL;
    expr_t *e;
    int opt_ind = 0;
    int opt;

    // reset optind and flags
    optind = 0;
    debug_flag = 0;
    gpu_flag = 0;

    if (argc < 3) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: attach $handle <app_ident>\n");
        return 0;
    }

    //
    // Process user provided arguments
    //
    // This will permute the non-option stuff to the end, so parse that stuff
    // last.

    while ((opt = getopt_long(argc, argv, "", attach_long_opts, &opt_ind)) !=
           -1) {
        switch (opt) {
            case 0:
                // if this is a flag, do nothing
                break;
            default:
                PrintOutError(
                    "Invalid attach argument provided. See \"help attach\" for "
                    "usage.\n");
                return 0;
        }
    }

    // Ensure the user provided two non-option arguments
    if (argc - optind != 2) {
        PrintOutError(
            "Invalid number of non-option arguments. See \"help attach\" for "
            "usage.\n");
        PrintOutError("Expected 2, found %d.\n", argc - optind);
        return 0;
    }

    // parse the non-option arguments

    // The first argument should be the name of the process set
    if ((e = ParseExpr(argv[optind++])) == (expr_t *)NULL) {
        PrintError();
        return 0;
    }

    // Create the process set for invoke
    if ((ps = CreateProcSetForInvoke(e)) == NULL) {
        FreeExpr(e);
        return 0;
    }

    FreeExpr(e);

    app_ident = strdup(argv[optind++]);

    // Verify that the arguments are correct

    if (proto == NULL) {
        PrintOutError("Must specify a protocol.\n");
        Free(app_ident);
        return 0;
    }

    // Get the value of the ESWRAP_LOGIN environment variable
    char *elogin_env = getenv("ESWRAP_LOGIN");
    if (elogin_env == NULL) {
        // Also check value of new EPROXY_LOGIN var
        elogin_env = getenv("EPROXY_LOGIN");
    }

    // If on a Cray system, check to see if we are currently on an eslogin node
    // if using WLM_none.
    //
    // If so, we want to create a new session on the service node using the
    // proxy protocol and forward every command through that.
    //
    // For the cluster system, this is not the case. We do not need a proxy.
    // FIXME: The same is also true for native slurm.

    // Are we on an eslogin node?
    if (elogin_env != NULL) {
        // We are on an eslogin node since the file did not exist

        // set proto to proxy since we are required to use the proxy in
        // conjunction with eslogin nodes
        proto = "proxy";

        // First check to see if we currently have an open session
        if ((sess = DbgGetSession()) == NULL) {
            // There is no open session, so we need to create one.

            hostname = strdup(elogin_env);

            // Create a new session
            if ((sess = DbgSessionCreate(proto, WLM_none, NULL, hostname,
                                         username)) == NULL) {
                PrintOutError("Could not create session for eslogin!\n");
                Free(app_ident);
                return 0;
            }

            // Set this session for all future launch/attach commands
            DbgSetSession(sess);
        }
    }

    if (!DoAttach(proto, sess, ps, app_ident, arch, backend)) {
        // If XML mode is enabled, we want to print out a removed message.
        if (Client.XML_mode) {
            XMLPrintRemovedMsg(ps->s_name);
        }
        Free(app_ident);
        return 0;
    }

    Free(app_ident);

    return 1;
}

/*
** Invoke helper routine. Invoke one or more processes.
** Note that the attach process always operates synchronously.
*/
static int DoAttach(const char *proto, proto_session *sess, dbgprocset_t *ps,
                    const char *app_ident, const char *arch,
                    const char *backend) {
    dbginvoke_t *di;
    dbgframe_t *f;
    dbgframe_t *fp;
    rec_set_t *rec_set;
    rep_set_t *rep_set;
    rep_set_t *quit_rep;
    int p2pmode = 0;

    // Note that we are reusing the invoke callback.
    di = DbgAttach(proto, sess, ps->s_name, app_ident, arch, backend, gpu_flag,
                   debug_flag);

    if (di == NULL) {
        PrintOutError("Could not attach to application.\n");
        return 0;
    }

    // Create the rec_set based on the di
    rec_set = NewRecSet();
    AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full));

    // Finalize setup of the procset
    if (FinalizeProcSetForInvoke(ps, di)) {
        quit_rep = DbgQuit(rec_set, DBG_KILL);
        ProcessErrorInRepSet(quit_rep);
        FreeRecSet(rec_set);
        FreeRepSet(quit_rep);
        return 0;
    }

    // Set the client event callback handler
    DbgHandleEvent(di, EventCB);

    // grab all the frames
    rep_set = DbgShowFrame(rec_set, 0);

    // process error
    if (ProcessErrorInRepSet(rep_set)) {
        // This is fatal as its an indication that things are broken (i.e. we
        // cannot send commands)
        PrintOutError("Could not communicate with application.\n");
        quit_rep = DbgQuit(rec_set, DBG_DETACH);
        ProcessErrorInRepSet(quit_rep);
        FreeRecSet(rec_set);
        FreeRepSet(quit_rep);
        FreeRepSet(rep_set);
        return 0;
    }

    // We print the frames below, but we need to print this message only once,
    // so we do it here
    if (Client.print_entry_frame) {
        PrintOutMsg("Current rank location:\n");
        // Print the current frame based on this bitset
        // Print the startup frames based on this bitset
        for (auto&& reply : rep_set) {
            // point at the frame
            fp = EVENT_FRAME(reply->rep_event);
            // print the frame
            for (f = fp; f != NULL; f = f->frame_next) {
                PrintFrameSet(reply->rep_di->d_handle, reply->rep_group, f);
            }
            // If XML mode is enabled, we want to print out a status message.
            // We don't need to ask the backend for the status since this is
            // implied by printing out the initial breakpoint.
            if (Client.XML_mode) {
                char *set_str;
                char *status_block;
                status_block = XMLStartStatusBlock(reply->rep_di->d_handle);
                // Create the full set string
                Asprintf(&set_str, "%s%s", reply->rep_di->d_handle,
                         bitset_to_set(reply->rep_group));
                // Set the location based on the top frame
                XMLAddStatus(&status_block, set_str, DBGSTAT_STOPPED,
                             (fp == NULL) ? NULL : &(fp->frame_loc));
                Free(set_str);
                XMLPrintStatusBlock(status_block);
            }
        }
    }

    // Done with the frames reply
    FreeRepSet(rep_set);

    // Free the created rec_set
    FreeRecSet(rec_set);

    // for P2P here
    GetGraphVar(GRAPH_P2PMODE, (int *)&p2pmode);

    if (p2pmode) {
        // FIXME: andrewg: the getenv call here can fail. This needs to be
        //        fixed. When the p2p is fixed to actually ship the
        //        dbglog_netinfo file, this entire thing should be removed.

        // now, rename the file dbg_netinfo --> dbg_netinfo_<procset_name> file
        char filename[BUFSIZ];
        char filerename[BUFSIZ];
        char *filepath;
        filepath = getenv("CRAY_DBG_LOG_DIR");
        sprintf(filename, "%s/dbglog_netinfo_%s", filepath, ps->s_name);
        sprintf(filerename, "%s/dbglog_netinfo.0.log", filepath);
        if (rename(filerename, filename) < 0) {
            PrintOutError("Error in renaming file.");
        }
    }
    // end

    return 1;
}

static int cmd_halt_cmd(rec_set_t *rec_set, int argc, char **argv) {
    rep_set_t *rep_set;
    dbgloc_t *halt_loc;

    rep_set = DbgInterrupt(rec_set);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process location
    for (auto&& reply : rep_set) {
        // point at location data
        halt_loc = EVENT_LOC(reply->rep_event);
        PrintHaltedLocationSet(reply->rep_di->d_handle, reply->rep_group,
                               halt_loc);
    }

    FreeRepSet(rep_set);

    return 1;
}

// external exposure
void PrintHaltedLocationSet(char *setname, bitset *group, dbgloc_t *loc) {
    if (!loc) {
        PrintMsgWithSetInfo(setname, group, "Halt could not report a location\n");
    } else if (loc->loc_line > 0) {
        PrintMsgWithSetInfo(setname, group,
                            "Application halted in %s at %s:%d\n",
                            loc->loc_func, loc->loc_file, loc->loc_line);
    } else {
        PrintMsgWithSetInfo(setname, group, "Application halted in %s\n",
                            loc->loc_func);
    }
}

// Non-stop mode must generally be enable before the process is running,
// thus it is managed as a launch time option.  
void SetLaunchNonStopMode(bool isNonStop) { non_stop_mode = isNonStop; }
bool GetLaunchNonStopMode() { return non_stop_mode; }


static int cmd_kill_cmd(rec_set_t *rec_set, int argc, char **argv) {
    expr_t *e;
    val_t *val;
    dbgprocset_t *ps;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: kill $proc\n");
        return 0;
    }

    if ((e = ParseExpr(argv[1])) == (expr_t *)NULL) {
        PrintError();
        return 0;
    }

    if (EvalExpr(&val, e, rec_set) < 0) {
        PrintError();
        FreeExpr(e);
        return 0;
    }

    FreeExpr(e);

    if (RVAL_TYPE(val) != rval_proc) {
        PrintOutError("Invalid process set argument.\n");
        return 0;
    }

    // point at the procset
    ps = RVAL_PROCSET(val);

    // Ensure this is a DBGSET_INVOKE type, otherwise fail
    if (ps->s_type != DBGSET_INVOKE) {
        PrintOutError(
            "Cannot kill provided procset. Use the handle provided to "
            "launch/attach.\n");
        return 0;
    }

    // We always want to kill, not detach, the application when the user
    // issues the kill command.
    ReleaseProc(ps, DBG_KILL);

    return 1;
}

static int cmd_release_cmd(rec_set_t *rec_set, int argc, char **argv) {
    expr_t *e;
    val_t *val;
    dbgprocset_t *ps;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: release $proc\n");
        return 0;
    }

    if ((e = ParseExpr(argv[1])) == (expr_t *)NULL) {
        PrintError();
        return 0;
    }

    if (EvalExpr(&val, e, rec_set) < 0) {
        PrintError();
        FreeExpr(e);
        return 0;
    }

    FreeExpr(e);

    if (RVAL_TYPE(val) != rval_proc) {
        PrintOutError("Invalid process set argument.\n");
        return 0;
    }

    // point at the procset
    ps = RVAL_PROCSET(val);

    // Ensure this is a DBGSET_INVOKE type, otherwise fail
    if (ps->s_type != DBGSET_INVOKE) {
        PrintOutError(
            "Cannot release provided procset. Use the handle provided to "
            "launch/attach.\n");
        return 0;
    }

    // We always want to detach, not kill, the application when the user
    // issues the release command.
    ReleaseProc(ps, DBG_DETACH);

    return 1;
}

static void ReleaseProc(dbgprocset_t *p, dbgquit_type quit_type) {
    dbg_over_set_t *over_set;
    dbginvoke_t *di;
    rec_set_t *rec_set;
    rep_set_t *rep_set;

    SetOverlaySetForProcSet(p);
    while ((over_set = GetNextOverlaySetForProcSet(p)) != NULL) {
        // point at the dbginvoke for this set
        di = over_set->s_di;

        switch (quit_type) {
            case DBG_KILL:
                PrintOutMsg(
                    "Shutting down debugger and killing application for "
                    "\'%s\'.\n",
                    di->d_handle);
                break;
            case DBG_DETACH:
                PrintOutMsg(
                    "Shutting down debugger and resuming application for "
                    "\'%s\'.\n",
                    di->d_handle);
                break;
            case DBG_DEFAULT:
                // use di instead
                switch (di->d_type) {
                    case DBG_ATTACHED:
                        PrintOutMsg(
                            "Shutting down debugger and resuming application "
                            "for \'%s\'.\n",
                            di->d_handle);
                        break;
                    case DBG_LAUNCHED:
                        PrintOutMsg(
                            "Shutting down debugger and killing application "
                            "for \'%s\'.\n",
                            di->d_handle);
                        break;
                }
                break;
        }

        // If XML mode is enabled, we want to print out a removed message.
        if (Client.XML_mode) {
            XMLPrintRemovedMsg(di->d_handle);
        }

        // Create the rec_set based on the di
        rec_set = NewRecSet();
        AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full));

        rep_set = DbgQuit(rec_set, quit_type);

        ProcessErrorInRepSet(rep_set);

        FreeRecSet(rec_set);
        FreeRepSet(rep_set);
    }

    if (p == CurrentProcSet()) SetCurrentProcSet(NULL);

    FreeProcSet(p);
}

// external exposure
void EndAll(void) {
    dbgprocset_t *ps;

    SetProcSet();
    while ((ps = GetProcSet()) != NULL) {
        if (ps->s_type != DBGSET_INVOKE) continue;

        ReleaseProc(ps, DBG_DEFAULT);
    }
}

void InitCmd_proc(void) {
    // info commands
    AddInfoCommand(
        "application", proc_ignore, 0,
        class_info_subcmd_application_cmd,
        class_info_subcmd_application_help,
        "Show attached/launched applications under debugger control.");

    // regular commands
    AddCommand("session", NULL, proc_ignore, 0, cmd_session_cmd,
               cmd_session_help, "Create a new WLM session.");
    AddCommand("launch", NULL, proc_ignore, 0, cmd_invoke_cmd,
               cmd_invoke_help, "Launch an application.");
    AddCommand("attach", NULL, proc_ignore, 0, cmd_attach_cmd,
               cmd_attach_help, "Attach to an application.");
    AddCommand("halt", NULL, proc_required, 0, cmd_halt_cmd,
               cmd_halt_help,
               "Halt execution of attached/launched application.");
    AddCommand("release", NULL, proc_ignore, 0, cmd_release_cmd,
               cmd_release_help,
               "Release attached/launched application from debugger control.");
    AddCommand("kill", NULL, proc_ignore, 0, cmd_kill_cmd,
               cmd_kill_help, "Kill attached/launched application.");
}

