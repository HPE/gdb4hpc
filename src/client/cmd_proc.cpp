/*********************************************************************************\
 * Process control commands.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2023 Hewlett Packard Enterprise Development LP.
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
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <unordered_map>
#include <optional>
#include <filesystem>
#include <regex>
#include <sstream>

#include "client.h"
#include "common_tools_fe.h"
#include "useful.h"
#include "gram.h"
#include "interpreter_mi.h"

extern dbgprocset_t *CreateProcSetForInvoke(const std::string& name);

static dbgprocset_t* DoLaunch(LaunchOpts&& opts);
static dbgprocset_t* DoRelaunchInvoke(dbgprocset_t* procset, const char* cloneName);
static int DoRelaunch(dbgprocset_t* procset, const char* cloneName);
static int DoAttach(dbgprocset_t *ps, const AttachOpts& opts);
static void ReleaseProc(dbgprocset_t *, DbgQuitType);

// Main async event handler
static int EventCB(DbgInvoke::EventSet& ev_set, const Bitset& reply_set);

static int cmd_info_application_cmd() {
    PrintOutMsg("%-10s %-10s %-5s %s\n", "Handle", "Ranks", "Focus",
                "Application");

    int num_ps = 0;
    for (auto&& ps : GetProcSets())  {
        if (ps->s_type != DBGSET_INVOKE) continue;

        ++num_ps;

        // Get the current process set.
        auto c_ps = CurrentProcSet();

        for (auto over_set : ps->s_over_sets) {
            // point at the di
            auto* di = over_set->s_di.lock();
            PrintOutMsg("$%-9s %-10d ", di->handle().c_str(), di->numRanks());

            // reset found, we only print n if the di is not in the over_set
            auto found = false;
            for (auto c_over_set : c_ps->s_over_sets) {
                if (di->handle() == c_over_set->s_di) {
                    PrintOutMsg("%-5s ", "y");
                    found = true;
                    break;
                }
            }
            if (!found) {
                PrintOutMsg("%-5s ", "n");
            }

            auto first = true;
            for (auto&& job_opt : di->launchOptions().job_opts) {
                if (first) {
                    first = false;
                } else {
                    PrintOutMsg(" : ");
                }
                PrintOutMsg("%s", job_opt.app_argv.asString().c_str());
            }
            PrintOutMsg("\n");
        }
    }

    if (num_ps == 0) {
        PrintOutError(
            "There are currently no launched/attached applications.\n");
        return 1;
    }

    return 1;
}

static dbgprocset_t* getNewProcSet(const std::string& procName) {

    auto lookName = procName;
    if (auto br = lookName.find('{'); br != std::string::npos) {
        lookName.erase(br, std::string::npos);
    }
               
    if (dbgprocset_t* existingProcSet = FindProcSet(lookName.c_str())) {
        // give the user a chance to release the existing launch and re-use the
        // launch id.   Release and reuse is assumed in non-interactive contexts.
        if (!Client.confirm ||
            cli_query("A process set with this name is in use, release?",
                      cli_query_default::NO, cli_query_default::YES)) {

            // Out with the old
            ReleaseProc(existingProcSet, DbgQuitType::Detach);
        } else {
            return nullptr;
        }
    }

    // In with the new
    return CreateProcSetForInvoke(procName);
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
static int cmd_launch_cmd(int argc, const char* const argv[]) {
    try {
        // throws if arguments are malformed
        auto args = LaunchOpts(argc, argv, Client);
        args.enforceValid();
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
static dbgprocset_t* DoLaunch(LaunchOpts&& args) {
    rec_set_t *rec_set = nullptr;
    auto ps = getNewProcSet(args.procset_name);

    const auto exitWithFailure = [&](){
        FreeRecSet(rec_set);
        FreeProcSet(ps);

        return nullptr;
    };

    if (!ps) return exitWithFailure();

    // if the user has set any environment variables via the set env command, add them
    for (const auto& [key, value] : Client.launch_env) {
        // args.env << key << "=" << value would be wrong,
        // ArgvStream expects a single string per << operation.
        args.env << (key + "=" + value);
    }

    // set below depending on launch strategy
    auto newHandle = DbgInvoke::Handle(nullptr);

    // Submit PBS script and attach
    if (!args.qsub_file_path.empty()) {

        try {

            // Check WLM for PBS PALS
            cti_pals_ops_t *ops = nullptr;
            if (cti_open_ops((void**)&ops) != CTI_WLM_PALS) {

                // Did not have PBS PALS
                auto wlm = cti_current_wlm();
                auto wlm_name = cti_wlm_type_toString(wlm);
                throw std::runtime_error("PBS batch launch requires PBS / PALS as "
                    "the current WLM " + (wlm_name != nullptr)
                        ? "(have " + std::string(wlm_name) + ")"
                        : "(failed to detect WLM)");
            }

            PrintOutMsg("Starting application, please wait...\n");

            // Submit PBS script and get PBS job ID
            if (args.job_opts.size() != 1) {
                throw std::runtime_error("PBS batch launch cannot start an MPMD job");
            }
            auto launcherArgs = args.job_opts[0].launcher_args.c_strs(0);
            auto envList = args.env.c_strs(0);
            auto pbs_job_id = ops->submitJobScript(args.qsub_file_path.c_str(),
                launcherArgs.data(), envList.data());
            if (pbs_job_id == nullptr) {
                throw std::runtime_error(cti_error_str());
            }

            auto attachOpts = AttachOpts{.proto = Client.proto,
                                         .app_ident = pbs_job_id,
                                         .backend = Client.backend,
                                         .gdb = args.gdb_path,
                                         .gdb_data_dir = args.gdb_data_dir,
                                         .vgdb_file = "",
                                         .gpu_flag = args.gpu_flag,
                                         .debug_flag = args.debug_flag };

            // Attach to submitted job
            if (newHandle = DbgAttach(ps->s_name.c_str(), attachOpts); !newHandle.lock()) {
                PrintOutError("Failed to attach to PBS job ID %s\n", pbs_job_id);
                free(pbs_job_id);
                return exitWithFailure();
            }

            // Continue job past barrier and into main
            if (cti_killApp(newHandle.lock()->appId(), SIGCONT)) {
                PrintOutError("Failed to continue ID %lu\n", newHandle.lock()->appId());
            }

            free(pbs_job_id);

        } catch (std::exception const& ex) {
            PrintOutError("Failed to submit PBS batch file %s: %s\n",
                args.qsub_file_path.c_str(), ex.what());
            return exitWithFailure();
        }

    // Submit Slurm batch script and attach
    } else if (!args.sbatch_file_path.empty()) {

        try {

            // Check WLM for Slurm
            cti_slurm_ops_t *ops = nullptr;
            if (cti_open_ops((void**)&ops) != CTI_WLM_SLURM) {

                // Did not have Slurm
                auto wlm = cti_current_wlm();
                auto wlm_name = cti_wlm_type_toString(wlm);
                throw std::runtime_error("Slurm batch launch requires Slurm as "
                    "the current WLM " + (wlm_name != nullptr)
                        ? "(have " + std::string(wlm_name) + ")"
                        : "(failed to detect WLM)");
            }

            PrintOutMsg("Starting application, please wait...\n");

            // Submit sbatch script and get Slurm job / step ID
            if (args.job_opts.size() != 1) {
                throw std::runtime_error("Slurm batch launch cannot start an MPMD job");
            }
            auto launcherArgs = args.job_opts[0].launcher_args.c_strs(0);
            auto envList = args.env.c_strs(0);
            auto slurmJobStep = std::string{};
            if (auto slurm_job_step_id = ops->submitBatchScript(args.sbatch_file_path.c_str(),
                                                                launcherArgs.data(),
                                                                envList.data())) {
                slurmJobStep =  std::to_string(slurm_job_step_id->jobid) + "."
                    + std::to_string(slurm_job_step_id->stepid);
                free(slurm_job_step_id);
                
            } else {
                throw std::runtime_error(cti_error_str());
            }

            auto attachOpts = AttachOpts{.proto = Client.proto,
                                         .app_ident = slurmJobStep,
                                         .backend = Client.backend,
                                         .gdb = args.gdb_path,
                                         .gdb_data_dir = args.gdb_data_dir,
                                         .vgdb_file = "",
                                         .gpu_flag = args.gpu_flag,
                                         .debug_flag = args.debug_flag };

            // Attach to submitted job
            if (newHandle = DbgAttach(ps->s_name.c_str(), attachOpts); !newHandle.lock()) {
                PrintOutError("Failed to attach to Slurm job ID %s\n", slurmJobStep.c_str());
                return exitWithFailure();
            }

        } catch (std::exception const& ex) {
            PrintOutError("Failed to submit Slurm batch file %s: %s\n",
                args.sbatch_file_path.c_str(), ex.what());
            return exitWithFailure();
        }

    // Launch normally
    } else {

        if (args.launcher_name != "") {
            setenv(CTI_LAUNCHER_NAME_ENV_VAR, args.launcher_name.c_str(), 1);
        }

        // Call the invoke in the dbg_client layer
        // moves args into the returned di as the d_launch member
        if ((newHandle = DbgLaunch(ps->s_name.c_str(), ps->s_nproc, std::move(args))); !newHandle.lock()) {
            // Do not print out an error message here since the lower layer already
            // printed it for us.
            return exitWithFailure();
        }
    }

    auto* di = newHandle.lock();

    // Finalize setup of the procset
    if (FinalizeProcSetForInvoke(ps, di)) {
        auto quit_rep = DbgQuit(*di, DbgQuitType::Kill);
        ProcessErrorInRepSet(quit_rep);
        return exitWithFailure();
    }

    // Set the client event callback handler
    DbgHandleEvent(di, EventCB);
    PrintOutMsg("Launch complete.\n");

    // Create a rec_set based on the di
    rec_set = NewRecSet();
    AddReceiverToSet(rec_set, NewGroupReceiver(*di, di->fullBitset()));

    // Issue the startup command
    auto rep_set = DbgStartup(rec_set);

    // process error
    if (ProcessErrorInRepSet(rep_set)) {
        PrintOutError("Could not startup application.\n");
        auto quit_rep = DbgQuit(*di, DbgQuitType::Kill);
        ProcessErrorInRepSet(quit_rep);
        return exitWithFailure();
    }

    // Print the startup frames based on this bitset
    if (Client.print_entry_frame) {
        for (auto&& reply : rep_set) {
            // print out the frame. we create a fake async message to let the
            // proper interpreter format the output. DbgStartup returns a
            // DBGEV_FRAME, but it's displayed like a breakpoint hit.

            // the async oberver needs a DbgInvoke::EventSet. luckily for us, an
            // EventSet is just a fancy version of a group_reply_t.

            auto* di = reply->rep_di.lock();
            if (!di || !reply->rep_group) continue;

            auto ev_set = DbgInvoke::EventSet(*di, *reply->rep_group, reply->rep_event);
            observers::async_event.notify(ev_set, *reply->rep_group);
        }
    }

    di->setAtInitialBreakpoint(true);

    // ensure the new process has the current print elements setting
    // 0 is the default value set up by the dbgsrv, so if the setting is already
    // 0, we don't have to do anything
    if (Client.print_elements != 0) {
        ClientSetPrintElements(Client.print_elements, rec_set, true);
    }

    // Free the created rec_set
    FreeRecSet(rec_set);

    return ps;
}

// relaunch procsets where s_type == DBGSET_INVOKE.
// returns newly launched procset.
static dbgprocset_t* DoRelaunchInvoke(dbgprocset_t* oldProcset, const char* cloneName) {
    if (!oldProcset) {
        PrintOutError("Error: null procset\n");
        return nullptr;
    }

    // get convenience pointer to old procset's internal di
    // DBGSET_INVOKE procsets should only have one di
    assert(oldProcset->s_type == DBGSET_INVOKE);
    assert(oldProcset->s_over_sets.size() == 1);
    auto* oldDi = oldProcset->s_over_sets[0]->s_di.lock();

    // validate that this di can be relaunched
    if (!oldDi) {
        PrintOutError("Error: null di in procset\n");
        return nullptr;
    }

    if (oldDi->processType() == DbgInvoke::ProcessType::Attached) {
        PrintOutError("Can't re-launch attached programs.\n");
        return nullptr;
    }

    auto newArgs = LaunchOpts(oldDi->launchOptions());
    newArgs.procset_name = (cloneName ? cloneName : "$tmp_clone");
    newArgs.procset_name += "{" + std::to_string(oldProcset->s_nproc) + "}";

    for (size_t i = 0; i < oldDi->launchOptions().job_opts.size(); i++) {
        newArgs.job_opts[i].app_argv = oldDi->launchOptions().job_opts[i].run_argv;
    }

    try {
        newArgs.enforceValid();
    } catch (const std::runtime_error &e) {
        PrintOutError("Launch failed: %s\n", e.what());
        return nullptr;
    }

    dbgprocset_t* newProcset = nullptr;

    // launch new app
    if (!(newProcset = DoLaunch(std::move(newArgs)))) {
        PrintOutError("Launch failed.\n");
        return nullptr;
    }

    bool printedHeader = false;
    for (const auto& oldBp : oldDi->breakpoints()) {
        if (!printedHeader) {
            PrintOutMsg("Re-setting breakpoints...\n");
            printedHeader = true;
        }

        auto* recSet = CreateRecSetForProcset(newProcset);
        auto* di = getFirstDbgInvoke(*newProcset).lock();

        // mask recSet to only the ranks that the breakpoint was on
        auto tmpGroup = group_recv{
            .rec_di = *di,
            .rec_group = oldBp.ranksSet()->clone(),
            ._cmd_sent = nullptr
        };

        recSet->intersectReceiver(&tmpGroup);

        auto repSet = rep_set_t();
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

        if (repSet.isEmpty() || ProcessErrorInRepSet(repSet)) {
            PrintOutError("Warning: failed to set breakpoint for $%s.\n",
                          newProcset->s_name.c_str());
            continue;
        }

        auto theNewBpId = int{0};;

        for (const auto* reply : repSet) {
            auto newDbgBp = EVENT_BP(reply->rep_event);
            auto* di = reply->rep_di.lock();
            auto newBp = di->breakpoints().findByDbgBreakpoint(*newDbgBp);
            if (!newBp) {
                PrintOutError("Warning: Failed to set breakpoint for $%s.\n", reply->rep_di.c_str());
                continue;
            }

            // with the back-end mapping; the new BPs should all have the same id
            if (!theNewBpId) theNewBpId = newBp->id();  
            PrintOutMsg("Re-set breakpoint $%s::%d as new breakpoint $%s::%d.\n",
                        oldDi->handle().c_str(), oldBp.id(), newProcset->s_name.c_str(),
                        newBp->id());
        }

        if (theNewBpId && !oldBp.condition().empty()) {
            // set the condition
            auto repSet = DbgConditionBreak(recSet, theNewBpId, oldBp.condition());
            ProcessErrorInRepSet(repSet);
        }
    }

    if (!cloneName) {
        // replace old procset with new one by making the new procset look
        // identical to the old
        // TODO: do this in background thead?

        // claim old name and release old procset
        auto oldName = oldProcset->s_name;
        ReleaseProc(oldProcset, DbgQuitType::Default); // frees oldProcset
        oldProcset = nullptr;

        // Set names for the newly created procset/di. There are four names we need to update:
        // 1. The name of the newly created di (di.handle().name())
        // 2. The name of the newly created procset (procset.s_name)
        // 3. The stored handle in the newly created procset, used to look up the di
        //    (items in procset.s_over_set_list)

        // update newly created di's internal name (1)
        auto newHandle = newProcset->s_over_sets[0]->s_di;
        newHandle = DbgInvoke::rename(newHandle, oldName.c_str());
        if (!newHandle.lock()) {
            PrintOutError("Error: failed to rename newly created procset.\n");
            return nullptr;
        }

        // update newly created procset's name (2)
        newProcset->s_name = oldName;

        // update handle stored in newly created procset. (3)
        // The new procset only has one overlay set.
        newProcset->s_over_sets[0]->s_di = newHandle;
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
            // we can't restart only part of a di, so a procset
            // is invalid if it only includes a subset of a di.
            if (auto partials = GetPartialOverlaySets(*oldProcset); !partials.empty()) {
                PrintOutError("Error: procset $%s contains these subsets:\n", oldProcset->s_name.c_str());
                for (const auto* os : partials) {
                    std::string full_set_text;
                    if (auto* di = os->s_di.lock()) {
                        full_set_text = di->fullBitset()->toSetString();
                    } else {
                        full_set_text = " (expired)";
                    }
                    PrintOutError("  Ranks %s of $%s%s\n",
                        os->s_di_bs->toSetString().c_str(), os->s_di.c_str(), full_set_text.c_str()
                    );
                }
                PrintOutError("Only complete sets can be restarted.\n");
                return 0;
            }

            // create a list of procsets to relaunch.  Do in two passes because
            // DoRelaunchInvoke modifies the s_over_sets!
            std::vector<dbgprocset_t*> toRelaunch;
            for (auto&& os : oldProcset->s_over_sets) {
                if (auto* diProcset = FindProcSet(os->s_di.c_str())) {
                    toRelaunch.push_back(diProcset);
                } else {
                    PrintOutError("Error: failed to find procset for $%s!\n", os->s_di.c_str());
                    return 0;
                }
            }

            for (auto* diProcset : toRelaunch) {
                auto* newProcset = DoRelaunchInvoke(diProcset, clone);
                if (!newProcset) {
                    PrintOutError("Error: relaunching $%s failed!\n", diProcset->s_name.c_str());
                    return 0;
                }
            }

            return 1;
        }
    }

    // should never get here
    return 0;
}

/*
** attach $proc apid [--gpu] [--debug] [--vgdb]
*/
static int cmd_attach_cmd(Interpreter& caller, int argc, const char* const argv[]) {
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
    std::optional<std::string> debug_dir;

    const struct option attach_long_opts[] = {
        {"gpu", no_argument, &gpu_flag, 1},
        {"debug", optional_argument, 0, 'D'},
        {"gdb", required_argument, 0, 'b'},
        {"gdb-data-dir", required_argument, 0, 'd'},
        {"vgdb", required_argument, 0, 'v'},
        {0, 0, 0, 0}
    };

    if (!Client.proto) {
        PrintOutError("Must specify a protocol.\n");
        return 0;
    }

    // getopt can change argv so we work with a local copy
    struct CStrVector {
        // ensure memory is cleaned up if something throws
        ~CStrVector() {
            for (auto& str : v) free(str);
        }
        std::vector<char*> v;
    };
    CStrVector mut_argv;
    std::transform(argv, argv + argc, std::back_inserter(mut_argv.v), [](const auto& strp) {
        return strdup(strp);
    });

    auto opts = AttachOpts{ .proto = Client.proto,
                            .app_ident = "",
                            .backend = Client.backend,
                            .gdb = "",
                            .gdb_data_dir = "",
                            .vgdb_file = "",
                            .gpu_flag = false,
                            .debug_flag = false
    };
    
    while ((opt = getopt_long(argc, mut_argv.v.data(), "", attach_long_opts, &opt_ind)) !=
           -1) {
        switch (opt) {
            case 0:
                // if this is a flag, do nothing
                break;
            case 'b':
                if (auto custom_gdb = pathFind(optarg,nullptr)) {
                    opts.gdb = custom_gdb;
                    free(custom_gdb);
                } else {
                    PrintOutError("Could not find %s in $PATH.\n", optarg);
                    return 0;
                }
                break;

            case 'd':
                if (std::filesystem::is_directory(optarg)) {
                    opts.gdb_data_dir = optarg;
                } else {
                    PrintOutError("gdb-data-dir %s is not a directory.\n", optarg);
                    return 0;
                }
                break;

            case 'D':
                debug_flag = 1;
                if (optarg && *optarg) debug_dir = optarg;
                break;

            case 'v':
                opts.vgdb_file = optarg;
                break;

            default:
                PrintOutError(
                    "Invalid attach argument provided. See \"help attach\" for "
                    "usage.\n");
                return 0;
        }
    }

    opts.gpu_flag = gpu_flag;
    opts.debug_flag = debug_flag;
    if (debug_dir && !DbgSetLogDir(*debug_dir)) {
        PrintOutError("Argument for --debug must be a user writable directory\n");
        return Command::Failure;
    }

    // XXX: find a better way
    if (dynamic_cast<MiInterpreter*>(&caller)){
        if (argc - optind != 1) {
            PrintOutError("Invalid number of non-option arguments. See \"help attach\" for "
                          "usage.\nExpected 1, found %d.\n", argc - optind);
            return Command::Failure;
        }
    }else{
        // Ensure the user provided two non-option arguments
        if (argc - optind != 2) {
            PrintOutError(
                "Invalid number of non-option arguments. See \"help attach\" for "
                "usage.\n");
            PrintOutError("Expected 2, found %d.\n", argc - optind);
            return Command::Failure;
        }
    }

    // XXX: find a better way
    auto is_mi = dynamic_cast<MiInterpreter*>(&caller) != nullptr;

    // parse the non-option arguments
    const auto* psName = (is_mi ? "$a" : mut_argv.v[optind++]);
    dbgprocset_t *ps = getNewProcSet(psName);
    if (!ps) {
        return 0;
    }

    opts.app_ident = mut_argv.v[optind++];

    if (!DoAttach(ps, opts)) {
        return 0;
    }

    return 1;
}

/*
** Invoke helper routine. Invoke one or more processes.
** Note that the attach process always operates synchronously.
*/
static int DoAttach(dbgprocset_t *ps, const AttachOpts& opts) {
    dbgframe_t *fp;
    rec_set_t *rec_set;

    // Note that we are reusing the invoke callback.
    auto newHandle = DbgAttach(ps->s_name.c_str(), opts);

    if (!newHandle.lock()) {
        PrintOutError("Could not attach to application: %s\n",
            (cti_error_str()) ? cti_error_str() : "(no error provided)");
        return 0;
    }

    auto* di = newHandle.lock();

    // Finalize setup of the procset
    if (FinalizeProcSetForInvoke(ps, di)) {
        auto quit_rep = DbgQuit(*di, DbgQuitType::Kill);
        ProcessErrorInRepSet(quit_rep);
        return 0;
    }

    // Set the client event callback handler
    DbgHandleEvent(di, EventCB);

    // Create the rec_set based on the di
    rec_set = NewRecSet();
    AddReceiverToSet(rec_set, NewGroupReceiver(*di, di->fullBitset()));

    // grab all the frames
    auto rep_set = DbgShowFrame(rec_set, 0);

    // cuda-gdb doesn't report the threads as stopped at attachment time.
    // The debugger should not quit since since this does not inicate "Could not
    // communicate with application".
    bool isRunning = false;
    if (rep_set.hasError()) {
        isRunning = true;
        for (auto&& err : rep_set.getErrs()) {
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
        auto quit_rep = DbgQuit(*di, DbgQuitType::Detach);
        ProcessErrorInRepSet(quit_rep);
        FreeRecSet(rec_set);
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
                PrintFrameSet(reply->rep_di.c_str(), reply->rep_group.get(), f);
            }
        }
    }

    // Free the created rec_set
    FreeRecSet(rec_set);

    return 1;
}

static int cmd_halt_cmd(rec_set_t *rec_set, int argc, const char* const argv[]) {
    dbgloc_t *halt_loc;

    bool all = false;
    if (argc == 2 && strcmp(argv[1],"-a") == 0) {
        all = true;
    } else if (argc > 1) {
        PrintOutError("unrecognized command arguments.\n");
        PrintOutError("expected halt or halt -a.\n");
        return 0;
    }

    auto rep_set = DbgInterrupt(rec_set, all);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process location
    for (auto&& reply : rep_set) {
        // point at location data
        halt_loc = EVENT_LOC(reply->rep_event);
        PrintHaltedLocationSet(reply->rep_di.c_str(), reply->rep_group.get(),
                               halt_loc);
    }

    return 1;
}

// external exposure
void PrintHaltedLocationSet(const char *setname, const Bitset *group, dbgloc_t *loc) {
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

static int cmd_kill_cmd(int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: kill $proc\n");
        return 0;
    }

    auto ps = FindProcSet(argv[1]);
    if (!ps) {
        PrintOutError("Invalid process set argument.\n");
        return 0;
    }

    // We always want to kill, not detach, the application when the user
    // issues the kill command.
    if ((argc == 2)
     || (::strncmp(argv[2], "SIGKILL", 6) == 0)
     || (::strncmp(argv[2], "KILL", 4) == 0)) {
        ReleaseProc(ps, DbgQuitType::Kill);

    // Send signal to application
    } else {

        // Get recset from procset
        auto ps_rec_set = CreateRecSetForProcset(ps);

        // Send request
        auto rep_set = DbgSignal(ps_rec_set, argv[2]);
        ProcessErrorInRepSet(rep_set);

        // Print response
        for (auto&& reply : rep_set){
            auto ev = reply->rep_event->ev_event;
            if (ev != DBGEV_STRING){
                PrintOutError("unexpected result type.\n");
                continue;
            }
            auto is = std::istringstream{reply->rep_event->ev_u.ev_u_string};
            auto line = std::string{};
            while (std::getline(is, line)){
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(), "%s\n", line.c_str());
            }
        }

        FreeRecSet(ps_rec_set);
    }

    return 1;
}

static int cmd_release_cmd(int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: release $proc\n");
        return 0;
    }

    auto ps = FindProcSet(argv[1]);
    if (!ps) {
        PrintOutError("No procset %s.\n", argv[1]);
        return 0;
    }

    // We always want to detach, not kill, the application when the user
    // issues the release command.
    ReleaseProc(ps, DbgQuitType::Detach);

    return 1;
}

static void ReleaseProc(dbgprocset_t *p, DbgQuitType quit_type) {
    if (!p) return;

    // calling DbgQuit below will delete the associated di. if we called DbgQuit on a
    // procset that only contained half of the ranks in the original di, the di would get
    // deleted out from underneath the other half. in short, if a procset contains a
    // partial set of a di, we can't release it.
    //
    // if the given procset *only* contains expired procsets, the list of partial sets
    // will be empty and it will fall though. this is fine, since that will allow it to
    // get to FreeProcSet after no-oping through the DbgQuit loop.
    if (auto partials = GetPartialOverlaySets(*p); !partials.empty()) {
        PrintOutError("Error: procset $%s contains these subsets:\n", p->s_name.c_str());
        for (const auto* os : partials) {
            std::string full_set_text;
            if (auto* di = os->s_di.lock()) {
                full_set_text = di->fullBitset()->toSetString();
            } else {
                full_set_text = " (expired)";
            }
            PrintOutError("  Ranks %s of $%s%s\n",
                os->s_di_bs->toSetString().c_str(), os->s_di.c_str(), full_set_text.c_str()
            );
        }
        PrintOutError("Only complete sets can be released.\n");
        return;
    }

    for (auto over_set : p->s_over_sets) {
        // point at the dbginvoke for this set
        auto di = over_set->s_di.lock();
        if (!di) {
            PrintOutError("Warning: reply with unknown di %s. Ignoring.\n", over_set->s_di.c_str());
            continue;
        }

        switch (quit_type) {
            case DbgQuitType::Kill:
                PrintOutMsg(
                    "Shutting down debugger and killing application for "
                    "\'%s\'.\n",
                    di->handle().c_str());
                break;
            case DbgQuitType::Detach:
                PrintOutMsg(
                    "Shutting down debugger and resuming application for "
                    "\'%s\'.\n",
                    di->handle().c_str());
                break;
            case DbgQuitType::Default:
                // use di instead
                switch (di->processType()) {
                    case DbgInvoke::ProcessType::Attached:
                        PrintOutMsg(
                            "Shutting down debugger and resuming application "
                            "for \'%s\'.\n",
                            di->handle().c_str());
                        break;
                    case DbgInvoke::ProcessType::Launched:
                        PrintOutMsg(
                            "Shutting down debugger and killing application "
                            "for \'%s\'.\n",
                            di->handle().c_str());
                        break;
                }
                break;
        }

        auto rep_set = DbgQuit(*di, quit_type);

        ProcessErrorInRepSet(rep_set);
    }

    if (p == CurrentProcSet()) SetCurrentProcSet(NULL);

    FreeProcSet(p);
}

static int cmd_rerun_cmd(rec_set_t* recSet, int argc, const char* const argv[]) {
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
            return 1;
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
            ReleaseProc(cloneSet, DbgQuitType::Kill);
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


static int cmd_run_cmd(Interpreter& caller, rec_set_t* recSet, int argc, const char* const argv[]) {

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
        return cmd_launch_cmd(argc, argv);
    } else if (procs.size() == 1 && numNonProcs) {
        return cmd_attach_cmd(caller, argc, argv);
    } else if (!numNonProcs && procs.size() < 3) {
        return cmd_rerun_cmd(recSet, argc, argv);
    } else if (!numProcArrs && procs.empty()) {
        PrintOutError("A procset handle is required, see \"help run\".\n");
    } else {
        PrintOutError("Unrecognized run arguments, see \"help run\".\n");
    }
    
    return 1;
}

static int set_subcmd_args_cmd(rec_set_t* recSet, int argc, const char* const argv[]) {
    bool onlyReset = (argc == 1);

    // collect dis to modify
    auto dis = std::set<DbgInvoke*>{};
    for (const auto* rec : recSet) {
        dis.insert(&rec->rec_di);
    }

    // Check for MPMD
    for (auto di : dis) {
        if (di->launchOptions().job_opts.size() != 1) {
            PrintOutError("This command cannot be used with an MPMD job");
            return 0;
        }
    }


    // set args
    for (auto di : dis) {
        // remember argv[0]
        auto argv0 = di->launchOptions().job_opts[0].run_argv[0];
        di->launchOptions().job_opts[0].run_argv.clear();
        di->launchOptions().job_opts[0].run_argv << argv0;
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
                di->launchOptions().job_opts[0].run_argv << window;
            }
        }
    }

    if (onlyReset) {
        PrintOutMsg("Arguments reset.\n");
    }

    return 1;
}

static int show_subcmd_args_cmd(rec_set_t* recSet) {
    for (const auto* rec : recSet) {
        auto all_args = std::stringstream{};

        auto first = true;
        for (auto&& job_opt : rec->rec_di.launchOptions().job_opts) {
            if (first) { first = false; } else { all_args << " : "; }

            // start printing args from argv[1]
            const auto& dargv = job_opt.run_argv;
            all_args << dargv.asString(1);
        }

        PrintMsgWithSetInfo(rec->rec_di.handle().c_str(), rec->rec_group.get(), 
            "\"%s\"\n", all_args.str().c_str());
    }

    return 1;
}

// external exposure
void EndAll() {
    // make sure I'm iterating over a copy because the global list will be
    // chaning
    auto pss = std::vector<dbgprocset_t*>{};
    for (auto ps : GetProcSets()) pss.push_back(ps.get());

    for (auto ps : pss) {
        if (ps->s_type == DBGSET_INVOKE) ReleaseProc(ps, DbgQuitType::Default);
    }
}

// This is called each time an asynchronous reply is recieved. The entire event
// set which contains the aggregated event is passed in as ev_set and the
// individual reply that was just received is passed in as reply_set.
int EventCB(DbgInvoke::EventSet &ev_set, const Bitset &reply_set) {
    // need to do "special" things if we are currently reading from stdin
    // for readline support.
    cli_save_stream();

    DPRINT(
        "INFO[EventCB]: received event type:[%s], ev_bitset:%s, reply_set:%s "
        "from:%s\n",
        EventToStr(ev_set.event()), ev_set.group().toSetString().c_str(),
        reply_set.toSetString().c_str(), ev_set.di().handle().c_str());

    observers::async_event.notify(ev_set, reply_set);

    cli_restore_stream();

    return 1;
}

static int file_exec_and_sym_mi(MiInterpreter& mi, int argc, const char* const argv[]) {
    //check a procset was received
    if (mi.m_procset.empty()){
        // create a default if one wasn't specified
        mi.m_procset = "$App";
    }

    auto app=mi.m_procset;
    //add rank information if available
    if (!mi.m_num_ranks.empty()){
        app.append("{");
        app.append(mi.m_num_ranks);
        app.append("}");
    }

    auto launch_str = "launch " + app + mi.m_launcher_args + ArgsToString(&argv[1], argc - 1);
    auto launch_args = ArgvStream();
    launch_args.parseAndAppendArgs(launch_str);
    return cmd_launch_cmd(launch_args.argc(), launch_args.c_strs().data());
}

static void insert_setshow_commands(CommandNode& root, std::string set, std::string show) {
    root.addSubCmd(set + " args", nullptr, proc_required, set_subcmd_args_cmd,
               set_subcmd_args_help, "Set program arguments to be used when re-launching with the run command.");
    root.addSubCmd(show + " args", nullptr, proc_required, show_subcmd_args_cmd,
               show_subcmd_args_help, "Show program arguments to be used when re-launching with the run command.");
}

void ConsoleCommandsFactory::insert_proc_commands(CommandNode& root) {
    root.addSubCmd(
        "info application", nullptr, proc_ignore,
        cmd_info_application_cmd,
        cmd_info_application_help,
        "Show attached/launched applications under debugger control.");
    root.addSubCmd(
        "launch", nullptr, proc_ignore, cmd_launch_cmd,
        cmd_launch_help, "Launch an application.");
    root.addSubCmd(
        "attach", nullptr, proc_ignore, cmd_attach_cmd,
        cmd_attach_help, "Attach to an application.");
    root.addSubCmd(
        "halt", nullptr, proc_required, cmd_halt_cmd,
        cmd_halt_help, "Halt execution of attached/launched application.");
    root.addSubCmd(
        "release", nullptr, proc_ignore, cmd_release_cmd,
        cmd_release_help, "Release attached/launched application from debugger control.");
    root.addSubCmd(
        "kill", nullptr, proc_ignore, cmd_kill_cmd,
        cmd_kill_help, "Send signal to attached/launched application (default SIGKILL).");
    root.addSubCmd(
        "run", "r", proc_optional, cmd_run_cmd,
        cmd_run_help, "Release a procset and re-launch it, transferring any procset-specific settings/breakpoints/etc.");

    insert_setshow_commands(root, "set", "show");
}

void MiCommandsFactory::insert_proc_commands(CommandNode& root) {
    insert_setshow_commands(root, "-gdb-set", "-gdb-show");

    root.addSubCmd({ .name = "-exec-run", .ptype = proc_optional, .fn = cmd_run_cmd });
    root.addSubCmd({
        .name = "-file-exec-and-symbols", .ptype = proc_ignore, .fn = file_exec_and_sym_mi});
}
