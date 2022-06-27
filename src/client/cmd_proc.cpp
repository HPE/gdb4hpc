/*********************************************************************************\
 * Process control commands.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <getopt.h>
#include <cassert>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include <optional>
#include <filesystem>
#include <regex>

#include "client.h"
#include "common_tools_fe.h"
#include "useful.h"

static dbgprocset_t* DoLaunch(dbginvoke::LaunchOpts&& opts);
static dbgprocset_t* DoRelaunchInvoke(dbgprocset_t* procset, const char* cloneName);
static int DoRelaunch(dbgprocset_t* procset, const char* cloneName);
static int DoAttach(const char *, proto_session *, dbgprocset_t *, const char *,
                    const char *, const char *, bool gpu_flag, bool debug_flag);
static void ReleaseProc(dbgprocset_t *, dbgquit_type);

int class_info_subcmd_application_cmd(rec_set_t *rec_set, int argc,
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

            PrintOutMsg("%s\n", di->d_launch.app_argv.asString().c_str());
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
    const struct option session_long_opts[] = {
        {"qsub", optional_argument, 0, 'q'},
        {"sbatch", optional_argument, 0, 's'},
        {"end", optional_argument, 0, 'e'},
        {0, 0, 0, 0}
    };

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

static dbgprocset_t* getNewProcSet(const char* procName) {

    auto e = ParseExpr(procName);
    if (!e) {
        PrintError();
        return nullptr;
    }
    
    dbgprocset_t* existingProcSet = nullptr;

    auto procSet = CreateProcSetForInvoke(e, existingProcSet);

    // e is destructively modified by CreateProcSetForInvoke, so we cannot
    // reuse it.   
    FreeExpr(e); 
    e = nullptr;

    if (!procSet && existingProcSet) {
        // give the user a chance to release the existing launch and re-use the
        // launch id.   Release and reuse is assumed in non-interactive contexts.
        if (!Client.confirm ||
            cli_query("A process set with this name is in use, release?",
                      cli_query_default::NO, cli_query_default::YES)) {

            // Out with the old
            ReleaseProc(existingProcSet, DBG_DETACH);
            existingProcSet = nullptr;

            // In with the new
            e = ParseExpr(procName);  // assume to work since it's the same string
            procSet = CreateProcSetForInvoke(e, existingProcSet);
            FreeExpr(e);
        }
    }

    return procSet;
}

/*
    Launch an application.

    launch <app_handle> <application>"
                  [--args=\"<args>\" OR -a \"<args>\"]
                  [--launcher="<launcher_name>"]
                  [--launcher-args="<launcher_args>"]
                  [--launcher-input=<path_to_file>]
                  [--workdir=<path>]
                  [--env="<name=value>"]
                  [--gpu]
                  [--debug]
*/
static int cmd_launch_cmd(rec_set_t *rec_set, int argc, char **argv) {
    try {
        // throws if arguments are malformed
        auto args = dbginvoke::LaunchOpts(argc, argv, Client);
        return DoLaunch(std::move(args)) ? 1 : 0;
    } catch (const std::runtime_error& e) {
        PrintOutError("Error: %s\n", e.what());
        return 0;
    }
}

/*
** Invoke helper routine. Invoke one or more processes.
** Note that the invoke process always operates synchronously.
*/
static dbgprocset_t* DoLaunch(dbginvoke::LaunchOpts&& args) {
    dbginvoke_t *di = nullptr;;
    rec_set_t *rec_set = nullptr;
    rep_set_t *rep_set = nullptr;
    rep_set_t *quit_rep = nullptr;
    auto ps = getNewProcSet(args.procset_name.c_str());

    const auto exitWithFailure = [&](){
        // If XML mode is enabled, we want to print out a removed message.
        if (Client.XML_mode) {
            // procset_name will be of the form $a{n}.
            // XMLPrintRemoveMsg only wants the $a part.
            auto procset_name = args.procset_name.substr(0, args.procset_name.find('{'));
            XMLPrintRemovedMsg(procset_name.c_str());
        }

        FreeRecSet(rec_set);
        FreeRepSet(quit_rep);
        FreeRepSet(rep_set);
        FreeProcSet(ps);

        return nullptr;
    };

    if (!ps) return exitWithFailure();

    // if the user has set any environment variables via the set env command, add them
    if (SizeOfList(Client.set_env_list) > 1) {
        SetList(Client.set_env_list);

        char* env_str;
        while ((env_str = (char *)GetListElement(Client.set_env_list)) != nullptr) {
            args.env << env_str;
        }
    }

    if (args.launcher_argv[0] != "") {
        setenv(CTI_LAUNCHER_NAME_ENV_VAR, args.launcher_argv[0].c_str(), 1);
    }

    // Call the invoke in the dbg_client layer
    // moves args into the returned di as the d_launch member
    if (!(di = DbgLaunch(ps->s_name, ps->s_nproc, std::move(args)))) {
        // Do not print out an error message here since the lower layer already
        // printed it for us.
        return exitWithFailure();
    }

    // Create a rec_set based on the di
    rec_set = NewRecSet();
    AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full.get()));

    // Finalize setup of the procset
    if (FinalizeProcSetForInvoke(ps, di)) {
        quit_rep = DbgQuit(rec_set, DBG_KILL);
        ProcessErrorInRepSet(quit_rep);
        return exitWithFailure();
    }

    // Set the client event callback handler
    DbgHandleEvent(di, EventCB);

    PrintOutMsg("Launch complete.\n");

    // Issue the startup command
    rep_set = DbgStartup(rec_set);

    // process error
    if (ProcessErrorInRepSet(rep_set)) {
        PrintOutError("Could not startup application.\n");
        quit_rep = DbgQuit(rec_set, DBG_KILL);
        ProcessErrorInRepSet(quit_rep);
        return exitWithFailure();
    }

    // Print the startup frames based on this bitset
    if (Client.print_entry_frame) {
        for (auto&& reply : rep_set) {
            // point at the frame
            auto f = EVENT_FRAME(reply->rep_event);
            const auto &loc = f->getLoc();

            // print out the message
            if (loc.loc_line > 0) {
                PrintMsgWithSetInfo(
                    reply->rep_di->d_handle, reply->rep_group.get(),
                    "Initial breakpoint, %s at %s:%d\n", loc.loc_func, loc.loc_file, loc.loc_line);
            } else {
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group.get(),
                                    "Initial breakpoint, in %s\n",
                                    loc.loc_func);
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
                    reply->rep_group->toSetString().c_str());
                XMLAddStatus(&status_block, set_str, DBGSTAT_STOPPED,
                             &(f->getLoc()));
                Free(set_str);
                XMLPrintStatusBlock(status_block);
            }
        }
    }

    // ensure the new process has the current print elements setting
    // 0 is the default value set up by the dbgsrv, so if the setting is already
    // 0, we don't have to do anything
    if (Client.print_elements != 0) {
        ClientSetPrintElements(Client.print_elements, rec_set, true);
    }

    // Done with the startup reply
    FreeRepSet(rep_set);

    // Free the created rec_set
    FreeRecSet(rec_set);

    return ps;
}

// relaunch procsets where s_type == DBGSET_INVOKE.
// if `keepFlag` is false, the old procset will be deleted after relaunch.
// returns newly launched procset.
static dbgprocset_t* DoRelaunchInvoke(dbgprocset_t* oldProcset, const char* cloneName) {
    if (!oldProcset) {
        PrintOutError("Error: null procset\n");
        return nullptr;
    }

#if DEBUG
    assert(oldProcset->s_type == DBGSET_INVOKE);
#endif

    // get convenience pointer to old procset's internal di
    SetOverlaySetForProcSet(oldProcset);
    auto* oldDi = GetNextOverlaySetForProcSet(oldProcset)->s_di;

#if DEBUG
    // DBGSET_INVOKE procsets should only have one di
    assert(GetNextOverlaySetForProcSet(oldProcset) == nullptr);
#endif

    // validate that this di can be relaunched
    if (!oldDi) {
        PrintOutError("Error: null di in procset\n");
        return nullptr;
    }

    if (oldDi->d_type == DBG_ATTACHED) {
        PrintOutError("Can't re-launch attached programs.\n");
        return nullptr;
    }

    auto newArgs = dbginvoke::LaunchOpts(oldDi->d_launch);
    newArgs.procset_name = (cloneName ? cloneName : "$tmp_clone");
    newArgs.procset_name += "{" + std::to_string(oldProcset->s_nproc) + "}";
    
    newArgs.app_argv = oldDi->d_launch.run_argv;

    dbgprocset_t* newProcset = nullptr;

    // launch new app
    if (!(newProcset = DoLaunch(std::move(newArgs)))) {
        PrintOutError("Launch failed.\n");
        return nullptr;
    }

    bool printedHeader = false;
    for (const auto& oldBp : oldDi->d_breakpoints) {
        if (!printedHeader) {
            PrintOutMsg("Re-setting breakpoints...\n");
            printedHeader = true;
        }

        rep_set_t *repSet = nullptr;
        auto* recSet = CreateRecSetForProcset(newProcset);

        // mask recSet to only the ranks that the breakpoint was on
        auto tmpGroup = group_recv{
            .rec_di = getFirstDbgInvoke(*newProcset), 
            .rec_group = oldBp.ranksSet()->clone(),
            ._cmd_sent = nullptr
        };

        recSet->intersectReceiver(&tmpGroup);

        if (oldBp.line() > 0) {
            repSet = DbgSetLineBreak(recSet, 
                    oldBp.isPending(), oldBp.file().c_str(), 
                    oldBp.line(), oldBp.threadId().c_str()
            );
        } else {
            repSet = DbgSetFuncBreak(recSet, 
                    oldBp.isPending(), oldBp.file().c_str(), 
                    oldBp.function().c_str(), oldBp.threadId().c_str()
            );
        }

        FreeRecSet(recSet);

        if (!repSet || ProcessErrorInRepSet(repSet)) {
            FreeRepSet(repSet);
            PrintOutError("Warning: failed to set breakpoint for $%s.\n", newProcset->s_name);
            continue;
        }

        for (const auto* reply : repSet) {
            auto newDbgBp = EVENT_BP(reply->rep_event);
            auto newBp = reply->rep_di->d_breakpoints.findByDbgBreakpoint(*newDbgBp);

            if (!newBp) {
                PrintOutError("Warning: Failed to set breakpoint for $%s.\n", reply->rep_di->d_handle);
                continue;
            }

            PrintOutMsg("Re-set breakpoint $%s::%d as new breakpoint $%s::%d.\n",
                oldDi->d_handle, oldBp.id(), newProcset->s_name, newBp->id());
        }
    }

    if (!cloneName) {
        // replace old procset with new one by making the new procset look
        // identical to the old
        // TODO: do this in background thead?

        // claim old name and release old procset
        auto oldName = strdup(oldProcset->s_name);
        ReleaseProc(oldProcset, DBG_DEFAULT); // frees oldProcset
        oldProcset = nullptr;

        // remove name of newly created tmp procset from the symbol tables
        if (auto* val = FindVal(newProcset->s_name)) {
            RemoveVal(val);
        } else {
            PrintOutError("Warning: didn't find val for %s.\n", newProcset->s_name);
        }

        if (auto* sym = FindSym(newProcset->s_name)) {
            RemoveSym(sym);
        } else {
            PrintOutError("Warning: didn't find symbol for %s\n", newProcset->s_name);
        }

        free(newProcset->s_name);
        newProcset->s_name = oldName; // procset name...
        {
            // ...and di name. newly launched procsets should only have one di
            auto di = getFirstDbgInvoke(*newProcset);
            free(di->d_handle);
            di->d_handle = strdup(newProcset->s_name);
        }

        // get new symbol/val into global symbol/val table. both are handled in
        // the ParseExpr/EvalExpr functions.
        std::string exprString = std::string("$") + newProcset->s_name;
        // adds entry in symbol table
        auto* expr = ParseExpr(exprString.c_str()); 
        if (!expr) {
            PrintError(); 
            return nullptr;
        }

        val_t* newProcsetVal = nullptr;
        // creates entry in val table
        if (EvalExpr(&newProcsetVal, expr, nullptr) < 0) {
            PrintError();
            return nullptr;
        }

#if DEBUG
        assert(LVAL_TYPE(newProcsetVal) == lval_internal);
        assert(RVAL_ACTUAL_TYPE(newProcsetVal) == rval_none);
#endif
        // val returned above is a generic "internal value" val
        // fix it up so that it is specifically a procset val
        DestroyRval(RVAL_TYPE(newProcsetVal), RVAL_VAL(newProcsetVal));
        RVAL_TYPE(newProcsetVal) = rval_proc;
        RVAL_PROCSET(newProcsetVal) = newProcset;
        RVAL_DESTROY(newProcsetVal) = 0;
    }

    return newProcset;
}

static int DoRelaunch(dbgprocset_t* oldProcset, const char* clone) {
    if (!oldProcset) return 0;

    switch (oldProcset->s_type) {
        case DBGSET_INVOKE: {
            auto newProcset = DoRelaunchInvoke(oldProcset, clone);
            if (!newProcset) {
                PrintOutError("Relaunch failed!\n");
                return 0;
            }
            return 1;
        }

        case DBGSET_DEFSET: 
            // fall through
        case DBGSET_TEMP:
            // fall through
        case DBGSET_INTERNAL: {
            bool validProcset = true;
            forEachOverlaySet(*oldProcset, [oldProcset, &validProcset](const auto* os){
                // we can't restart only part of a di, so a procset
                // is invalid if it only includes a subset of a di.
                if (validProcset && *os->s_di_bs != *os->s_di->d_bitset_full) {
                    PrintOutError("Error: $%s contains only a subset (%s) of $%s.\n",
                        oldProcset->s_name, os->s_di_bs->toSetString().c_str(), os->s_di->d_handle);
                    validProcset = false;
                }
            });

            if (!validProcset) return 0;

            // create a list of procsets to relaunch. if we were to relaunch within
            // forEachOverlaySet, the overlay set list would get a new entry upon
            // each iteration and the loop would never end!
            std::vector<dbgprocset_t*> toRelaunch;

            bool success = true;
            forEachOverlaySet(*oldProcset, [&toRelaunch, &success](const auto* os) {
                if (!success) return;

                if (auto* diProcset = FindProcSet(os->s_di->d_handle)) {
                    toRelaunch.push_back(diProcset);
                } else {
                    PrintOutError("Error: failed to find procset for $%s!\n", os->s_di->d_handle);
                    success = false;
                }  
            });

            if (!success) return 0;

            for (auto* diProcset : toRelaunch) {
                auto* newProcset = DoRelaunchInvoke(diProcset, clone);
                if (!newProcset) {
                    success = false;
                    PrintOutError("Error: relaunching $%s failed!\n", diProcset->s_name);
                    break;
                }
            }

            return success;
        }
    }

    // should never get here
    return 0;
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
    int opt_ind = 0;
    int opt;

    optind = 0;

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
    
    int debug_flag = 0;
    int gpu_flag = 0;
    const struct option attach_long_opts[] = {
        {"gpu", no_argument, &gpu_flag, 1},
        {"debug", no_argument, &debug_flag, 1},
        {0, 0, 0, 0}
    };

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
    if ((ps = getNewProcSet(argv[optind++])) == NULL) {
        return 0;
    }

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

    if (!DoAttach(proto, sess, ps, app_ident, arch, backend, gpu_flag, debug_flag)) {
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
                    const char *backend, bool gpu_flag, bool debug_flag) {
    dbginvoke_t *di;
    dbgframe_t *fp;
    rec_set_t *rec_set;
    rep_set_t *rep_set;
    rep_set_t *quit_rep;

    // Note that we are reusing the invoke callback.
    di = DbgAttach(proto, sess, ps->s_name, app_ident, arch, backend, gpu_flag,
                   debug_flag);

    if (di == NULL) {
        PrintOutError("Could not attach to application.\n");
        return 0;
    }

    // Create the rec_set based on the di
    rec_set = NewRecSet();
    AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full.get()));

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

    // cuda-gdb doesn't report the threads as stopped at attachment time.
    // The debugger should not quit since since this does not inicate "Could not
    // communicate with application".
    bool isRunning = false;
    if (rep_set && rep_set->hasError()) {
        isRunning = true;
        for (auto&& err : rep_set->getErrs()) {
            if (err->rep_event->ev_errno != DBGERR_ISRUNNING) {
                isRunning = false;
            }
        }
    }

    // process error
    if (!isRunning && ProcessErrorInRepSet(rep_set)) {
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
            for (const dbgframe_t* f = fp; f != nullptr; f = f->getNext()) {
                PrintFrameSet(reply->rep_di->d_handle, reply->rep_group.get(), f);
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
                         reply->rep_group->toSetString().c_str());
                // Set the location based on the top frame
                XMLAddStatus(&status_block, set_str, DBGSTAT_STOPPED,
                             (fp == NULL) ? NULL : &(fp->getLoc()));
                Free(set_str);
                XMLPrintStatusBlock(status_block);
            }
        }
    }

    // Done with the frames reply
    FreeRepSet(rep_set);

    // Free the created rec_set
    FreeRecSet(rec_set);

    return 1;
}

static int cmd_halt_cmd(rec_set_t *rec_set, int argc, char **argv) {
    rep_set_t *rep_set;
    dbgloc_t *halt_loc;

    bool all = false;
    if (argc == 2 && strcmp(argv[1],"-a") == 0) {
        all = true;
    } else if (argc > 1) {
        PrintOutError("unrecognized command arguments.\n");
        PrintOutError("expected halt or halt -a.\n");
        return 0;
    }

    rep_set = DbgInterrupt(rec_set, all);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process location
    for (auto&& reply : rep_set) {
        // point at location data
        halt_loc = EVENT_LOC(reply->rep_event);
        PrintHaltedLocationSet(reply->rep_di->d_handle, reply->rep_group.get(),
                               halt_loc);
    }

    FreeRepSet(rep_set);

    return 1;
}

// external exposure
void PrintHaltedLocationSet(char *setname, const Bitset *group, dbgloc_t *loc) {
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
        AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full.get()));

        rep_set = DbgQuit(rec_set, quit_type);

        ProcessErrorInRepSet(rep_set);

        FreeRecSet(rec_set);
        FreeRepSet(rep_set);
    }

    if (p == CurrentProcSet()) SetCurrentProcSet(NULL);

    FreeProcSet(p);
}

static int cmd_rerun_cmd(rec_set_t* recSet, int argc, char* argv[]) {
    using namespace std::string_literals;
    dbgprocset_t* procSet = nullptr;
    const char* cloneName = nullptr;

    if (argc == 1) {
        if (procSet = CurrentProcSet(); !procSet || !recSet) {
            PrintOutError("No current process set to re-launch.\n");
            return 1;
        }
    } else {
        procSet = FindProcSet(argv[1]+1);
        if (!procSet) {
            PrintOutError("No process set %s to re-launch.\n", argv[1]);
        }
    }

    if (argc > 2 && strcmp(argv[1],argv[2]) != 0) {
        cloneName = argv[2];
    }


    if (!cloneName) {
        auto prompt = "Re-launch $"s + procSet->s_name + "? Original will be released.";
        if (!cli_query(prompt.c_str(), cli_query_default::NO, cli_query_default::YES)) {
            PrintOutMsg("Cancelled.\n");
            return 1;
        }
    } else if (auto cloneSet = FindProcSet(cloneName)) {
        auto prompt = "Re-use "s + cloneName + "? Original will be released.";
        if (cli_query(prompt.c_str(), cli_query_default::NO, cli_query_default::YES)) {
            ReleaseProc(cloneSet, DBG_DEFAULT);
        } else {
            PrintOutMsg("Cancelled.\n");
            return 1;
        }
    }

    if (!DoRelaunch(procSet, cloneName)) {
        PrintOutError("Relaunch failed!\n");
        return 0;
    }

    return 1;
}


static int cmd_run_cmd(rec_set_t* recSet, int argc, char* argv[]) {

    // the command handles launches and attach in addition to rerun.   Infer the
    // case from the arguments we get.
    static auto procName = std::regex{R"(\$([[:alpha:]][[:w:]]*))"};
    static auto procArray = std::regex{R"(\$([[:alpha:]][[:w:]]*)\{[[:d:]]+\})"};

    auto procs = std::vector<std::string>{};
    auto numProcArrs = 0, numNonProcs = 0;

    for (int i=1; i<argc; ++i) {
        auto match = std::cmatch{};
        if (std::regex_match(argv[i], match, procName)) {
            procs.push_back(match[1]);
        } else if (std::regex_match(argv[i], match, procArray)) {
            ++numProcArrs;
        } else {
            ++numNonProcs;
        }
    }

    if (numProcArrs) {
        return cmd_launch_cmd(recSet, argc, argv);
    } else if (procs.size() == 1 && numNonProcs) {
        return cmd_attach_cmd(recSet, argc, argv);
    } else if (!numNonProcs && procs.size() < 3) {
        return cmd_rerun_cmd(recSet, argc, argv);
    } else if (!numProcArrs && procs.empty()) {
        PrintOutError("A procset handle is required, see \"help run\".\n");
    } else {
        PrintOutError("Unrecognized run arguments, see \"help run\".\n");
    }
    
    return 1;
}

static int set_subcmd_args_cmd(rec_set_t* recSet, int argc, char* argv[]) {
    bool onlyReset = (argc == 1);

    // collect dis to modify
    auto dis = std::set<dbginvoke_t*, dbginvokeMapper>{};
    for (const auto* rec : recSet) {
        dis.insert(rec->rec_di);
    }

    // set args
    for (auto* di : dis) {
        // remember argv[0]
        auto argv0 = di->d_launch.run_argv[0];
        di->d_launch.run_argv.clear();
        di->d_launch.run_argv << argv0;
        if (!onlyReset) {
            for (int i = 1 /* because argv[0] == "args" */; i < argc; i++) {
                // the command layer above will correctly parse quoted
                // multi-word strings as a single entry in argv, but will pass
                // them down to this layer with the quotes still there.
                // ArgvStream has its own way to handle multi-word
                // strings, so we chop off the existing quotes from the input.
                std::string_view window(argv[i]);
                if (window.size() > 1 && window.front() == '"' && window.back() == '"') {
                    window = window.substr(1, window.size() - 2);
                }
                di->d_launch.run_argv << window;
            }
        }
    }

    if (onlyReset) {
        PrintOutMsg("Arguments reset.\n");
    }

    return 1;
}

static int show_subcmd_args_cmd(rec_set_t* recSet, int argc, char* argv[]) {
    for (const auto* rec : recSet) {
        const auto& dargv = rec->rec_di->d_launch.run_argv;
        // start printing args from argv[1]
        PrintMsgWithSetInfo(rec->rec_di->d_handle, rec->rec_group.get(), 
            "\"%s\"\n", dargv.asString(1).c_str());
    }
            
    return 1;
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
    AddCommand("launch", NULL, proc_ignore, 0, cmd_launch_cmd,
               cmd_launch_help, "Launch an application.");
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
    AddCommand("run", "r", proc_optional, 0, cmd_run_cmd,
               cmd_run_help, "Release a procset and re-launch it, transferring any procset-specific settings/breakpoints/etc.");

    // set/show commands
    AddSetCommand("args", proc_required, 0, set_subcmd_args_cmd,
               set_subcmd_args_help, "Set program arguments to be used when re-launching with the run command.");
    AddShowCommand("args", proc_required, 0, show_subcmd_args_cmd,
               show_subcmd_args_help, "Show program arguments to be used when re-launching with the run command.");
}

