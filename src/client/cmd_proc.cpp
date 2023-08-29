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

#include "client.h"
#include "common_tools_fe.h"
#include "useful.h"
#include "gram.h"
extern dbgprocset_t *CreateProcSetForInvoke(const std::string& name);

static dbgprocset_t* DoLaunch(LaunchOpts&& opts);
static dbgprocset_t* DoRelaunchInvoke(dbgprocset_t* procset, const char* cloneName);
static int DoRelaunch(dbgprocset_t* procset, const char* cloneName);
static int DoAttach(dbgprocset_t *ps, const AttachOpts& opts);
static void ReleaseProc(dbgprocset_t *, DbgQuitType);

static void printEventLocation(const DbgInvoke::EventSet *ev_set, const Bitset *group);
// Main async event handler
static int EventCB(DbgInvoke::EventSet& ev_set, const Bitset& reply_set);

enum class StoppedType { END_RANGE, EXIT, BKPT_HIT};


void PrintMIStoppedMsg(StoppedType type,const std::string& file, const std::string& func,
                       int line, const std::string& bkpt_id, const std::string& proc_name,
                       const Bitset * proc_group, const std::string& fullpath){
    std::ostringstream ss;
    std::string group = proc_group->toSetString();
    switch(type){
        case StoppedType::BKPT_HIT:{
            ss << "reason=\"breakpoint-hit\",bkptno=\"" <<bkpt_id
               << "\",proc_set=\"" << proc_name
               << "\",group=\"" << group.substr(1,group.length()-2)
               << "\",frame={func=\"" << func
               << "\",args=[],file=\"" << file
               << "\",fullname=\""<< fullpath
               << "\",line=\"" << line << "\"}";
            break;
        }
        case StoppedType::END_RANGE:{
            ss << "reason=\"end-stepping-range\",proc_set=\""<<proc_name
               << "\",group=\"" << group.substr(1,group.length()-2)
               << "\",frame={func=\"" << func
               << "\",args=[],file=\""<< file
               << "\",fullname=\""<< fullpath
               << "\",line=\""<<line << "\"}";
            break;
        }
        case StoppedType::EXIT:{
            ss << "reason=\"exited-normally\", proc_set=\""<<proc_name
                << "\",group=\"" << group.substr(1,group.length()-2) << "\"";
            break;
        }
    }
    PrintOutMIResult(MIResult::STOPPED, "%s\n", ss.str().c_str() );
    
}


static int cmd_info_application_cmd(Command::RunContext& ctx, rec_set_t *rec_set, int argc, const char* const argv[]) {
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

            PrintOutMsg("%s\n", di->launchOptions().app_argv.asString().c_str());
        }
    }

    if (num_ps == 0) {
        PrintOutError(
            "There are currently no lauched/attached applications.\n");
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
static int cmd_launch_cmd(Command::RunContext& ctx, rec_set_t *rec_set, int argc, const char* const argv[]) {
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
    
    //print launch sequence to console
    PrintOutMItoCli printMItoCli;

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
            auto launcherArgs = args.launcher_argv.c_strs(1);
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
            auto launcherArgs = args.launcher_argv.c_strs(1);
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

        if (args.launcher_argv[0] != "") {
            setenv(CTI_LAUNCHER_NAME_ENV_VAR, args.launcher_argv[0].c_str(), 1);
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
    if (Client.MI_mode){
        PrintOutOk();  // LOOKAT: better way to suppress when in -iex
    }

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
            // point at the frame
            auto f = EVENT_FRAME(reply->rep_event);
            const auto &loc = f->getLoc();

            // print out the message
            if (loc.loc_line > 0) {
                if (Client.MI_mode){
                    auto* di = reply->rep_di.lock();
                    PrintMIStoppedMsg(StoppedType::BKPT_HIT, loc.loc_file, loc.loc_func,
                                        loc.loc_line, "0", reply->rep_di.name(), reply->rep_group.get(),
                                        di->sourcePaths().getFullPath(loc.loc_file));
                }
                PrintMsgWithSetInfo(
                    reply->rep_di.c_str(), reply->rep_group.get(),
                    "Initial breakpoint, %s at %s:%d\n", loc.loc_func, loc.loc_file, loc.loc_line);
            } else {
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                    "Initial breakpoint, in %s\n",
                                    loc.loc_func);
            }
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

    newArgs.app_argv = oldDi->launchOptions().run_argv;

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
static int cmd_attach_cmd(Command::RunContext& ctx, rec_set_t *rec_set, int argc, const char* const argv[]) {
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
                            .backend = Client.backend };
    
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

    if (Client.MI_mode){
        if (argc - optind != 1) {
            PrintOutMsg("^error,msg=\"Invalid number of non-option arguments. See \"help attach\" for "
                        "usage.\nExpected 1, found %d.\"\n%s", argc - optind, GetMIEpilogue());
            return 0;
        }
    }else{
        // Ensure the user provided two non-option arguments
        if (argc - optind != 2) {
            PrintOutError(
                "Invalid number of non-option arguments. See \"help attach\" for "
                "usage.\n");
            PrintOutError("Expected 2, found %d.\n", argc - optind);
            return 0;
        }
    }

    // parse the non-option arguments
    const auto* psName = (Client.MI_mode ? "$a" : mut_argv.v[optind++]);
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

static int cmd_halt_cmd(Command::RunContext& ctx, rec_set_t *rec_set, int argc, const char* const argv[]) {
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

static int cmd_kill_cmd(Command::RunContext& ctx, rec_set_t *rec_set, int argc, const char* const argv[]) {
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
    ReleaseProc(ps, DbgQuitType::Kill);

    return 1;
}

static int cmd_release_cmd(Command::RunContext& ctx, rec_set_t *rec_set, int argc, const char* const argv[]) {
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

static int cmd_rerun_cmd(Command::RunContext& ctx, rec_set_t* recSet, int argc, const char* const argv[]) {
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


static int cmd_run_cmd(Command::RunContext& ctx, rec_set_t* recSet, int argc, const char* const argv[]) {

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
        return cmd_launch_cmd(ctx, recSet, argc, argv);
    } else if (procs.size() == 1 && numNonProcs) {
        return cmd_attach_cmd(ctx, recSet, argc, argv);
    } else if (!numNonProcs && procs.size() < 3) {
        return cmd_rerun_cmd(ctx, recSet, argc, argv);
    } else if (!numProcArrs && procs.empty()) {
        PrintOutError("A procset handle is required, see \"help run\".\n");
    } else {
        PrintOutError("Unrecognized run arguments, see \"help run\".\n");
    }
    
    return 1;
}

static int set_subcmd_args_cmd(Command::RunContext& ctx, rec_set_t* recSet, int argc, const char* const argv[]) {
    bool onlyReset = (argc == 1);

    // collect dis to modify
    auto dis = std::set<DbgInvoke*>{};
    for (const auto* rec : recSet) {
        dis.insert(&rec->rec_di);
    }

    // set args
    for (auto di : dis) {
        // remember argv[0]
        auto argv0 = di->launchOptions().run_argv[0];
        di->launchOptions().run_argv.clear();
        di->launchOptions().run_argv << argv0;
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
                di->launchOptions().run_argv << window;
            }
        }
    }

    if (onlyReset) {
        PrintOutMsg("Arguments reset.\n");
    }

    return 1;
}

static int show_subcmd_args_cmd(Command::RunContext& ctx, rec_set_t* recSet, int argc, const char* const argv[]) {
    for (const auto* rec : recSet) {
        const auto& dargv = rec->rec_di.launchOptions().run_argv;
        // start printing args from argv[1]
        PrintMsgWithSetInfo(rec->rec_di.handle().c_str(), rec->rec_group.get(), 
            "\"%s\"\n", dargv.asString(1).c_str());
    }

    return 1;
}

// external exposure
void EndAll(void) {
    // make sure I'm iterating over a copy because the global list will be
    // chaning
    auto pss = std::vector<dbgprocset_t*>{};
    for (auto ps : GetProcSets()) pss.push_back(ps.get());

    for (auto ps : pss) {
        if (ps->s_type == DBGSET_INVOKE) ReleaseProc(ps, DbgQuitType::Default);
    }
}

// Prints program location after a step event is received
static void printEventLocation(const DbgInvoke::EventSet *ev_set, const Bitset *group) {
    if (!ev_set || !ev_set->event()) {
        return;
    }

    const dbgframe_t *frame = ev_set->event()->ev_step.step_frame;
    bool frame_changed = ev_set->event()->ev_step.step_frame_changed;

    bool doPrintLocation = !Client.MI_mode;

    if (frame) {
        const auto &frameLoc = frame->getLoc();

        // Print location info
        if (frameLoc.loc_file && frameLoc.loc_func && frameLoc.loc_line > 0) {
            // See if we can get the source line
        
            auto lineArg = std::string{frameLoc.loc_file} + ":" + std::to_string(frameLoc.loc_line);
            lineArg += "," + lineArg; // filename:xx,filename:xx
            dbgevent_t *listResult = nullptr;

            if (auto fe = ev_set->di().frontendGdb()) {
                listResult = fe->list(nullptr, lineArg.c_str());
            }

            if (!doPrintLocation) {
                // no-op
            } else if (listResult && listResult->ev_event == DBGEV_LIST &&
                       listResult->ev_list->list_lno == frameLoc.loc_line) {
                // Print a frame if the last step event changed functions
                if (frame_changed) {
                    PrintMsgWithSetInfo(
                        ev_set->di().handle().c_str(), &ev_set->group(),
                        "%s at %s:%d\n", frameLoc.loc_func, frameLoc.loc_file, frameLoc.loc_line
                    );
                }

                auto line = std::string{listResult->ev_list->list_line};
                formatSourceLine(line);
                
                PrintMsgWithSetInfo(
                    ev_set->di().handle().c_str(), &ev_set->group(),
                    "%d\t%s\n", frameLoc.loc_line, line.c_str()
                );
            } else {
                // For some reason we didn't get the right line, just print the frame
                PrintMsgWithSetInfo(
                    ev_set->di().handle().c_str(), &ev_set->group(),
                    "%s at %s:%d\n", frameLoc.loc_func, frameLoc.loc_file, frameLoc.loc_line
                );
            }
            FreeEvent(listResult);

        } else if (frameLoc.loc_func) {
            // No file/line, just print function name
            PrintMsgWithSetInfo(
                ev_set->di().handle().c_str(), &ev_set->group(), "in %s\n", frameLoc.loc_func);
        } else {
            // Shouldn't ever happen
            PrintMsgWithSetInfo(ev_set->di().handle().c_str(), &ev_set->group(),
                "Step finished with no location information.\n"
            );
        }

        if (Client.MI_mode){
            // Print the source line
            auto file = frameLoc.loc_file, frame = frameLoc.loc_func;
            PrintMIStoppedMsg(StoppedType::END_RANGE, file ? file : "unknown", frame ? frame : "unknown",
                              frameLoc.loc_line, "", ev_set->di().handle().name(), &ev_set->group(), ev_set->di().sourcePaths().getFullPath(file));
        }

    } else {
        // Got a null frame? This should never happen
        PrintMsgWithSetInfo(ev_set->di().handle().c_str(), &ev_set->group(),
            "Step finished with no location information.\n"
        );
    }
}

// This is called each time an asynchronous reply is recieved. The entire event
// set which contains the aggregated event is passed in as ev_set and the
// individual reply that was just received is passed in as reply_set.
int EventCB(DbgInvoke::EventSet &ev_set, const Bitset &reply_set) {
    std::string bpThread;

    // need to do "special" things if we are currently reading from stdin
    // for readline support.
    cli_save_stream();

    DPRINT(
        "INFO[EventCB]: received event type:[%s], ev_bitset:%s, reply_set:%s "
        "from:%s\n",
        EventToStr(ev_set.event()), ev_set.group().toSetString().c_str(),
        reply_set.toSetString().c_str(), ev_set.di().handle().c_str());

    switch (ev_set.event()->ev_event) {
        case DBGEV_EXIT: {
            //always print exits to console
            PrintOutMItoCli printMItoCli;
            if (ev_set.event()->ev_exit == 0) {
                PrintMsgWithSetInfo(ev_set.di().handle().c_str(), &ev_set.group(),
                                    "Program exited normally.\n");
            } else {
                PrintMsgWithSetInfo(ev_set.di().handle().c_str(), &ev_set.group(),
                                    "Program exited with code 0%o.\n",
                                    ev_set.event()->ev_exit);
            }
            break;
        }

        case DBGEV_BPHIT: {
            // ensure the recv bp has a corresponding bp_set. If not, this is an
            // unknown breakpoint. If its unknown either its a breakpoint
            // encountered from ending the graph (graph breakpoints are hidden)
            // or an exit breakpoint that was set by the dbg servers.

            //always print breakpoint hits to console
            PrintOutMItoCli printMItoCli;
            auto bpset = ev_set.di().breakpoints().findByDbgBreakpoint(*ev_set.event()->ev_bp);

            if (!bpset) {
                if (ev_set.event()->ev_bp->bp_type == DBGBP_EXIT_TYPE) {
                    if (Client.MI_mode){
                        PrintMIStoppedMsg(StoppedType::EXIT, "", "", 0, "",ev_set.di().handle().c_str(),&ev_set.group(),"");
                    }
                    PrintMsgWithSetInfo(
                        ev_set.di().handle().c_str(), &ev_set.group(),
                        "The application has reached an exit breakpoint.\n");
                } else if (Client.print_bp_frames) {
                    if (ev_set.event()->ev_bp->bp_loc.loc_line > 0) {
                        PrintMsgWithSetInfo(
                            ev_set.di().handle().c_str(), &ev_set.group(),
                            "In %s at %s:%d\n",
                            ev_set.event()->ev_bp->bp_loc.loc_func,
                            ev_set.event()->ev_bp->bp_loc.loc_file,
                            ev_set.event()->ev_bp->bp_loc.loc_line);
                    } else {
                        PrintMsgWithSetInfo(
                            ev_set.di().handle().c_str(), &ev_set.group(), "In %s\n",
                            ev_set.event()->ev_bp->bp_loc.loc_func);
                    }
                }
                break;
            }

            // What we print out is based on if we are at a watchpoint or
            // breakpoint
            // TODO: What should this default to? Right now I am having it
            // default to the breakpoint message.
            if (!bpset->threadId().empty()) bpThread = " thread " + bpset->threadId();

            switch (bpset->type()) {
                case DBGBP_WATCH_TYPE:
                case DBGBP_AWATCH_TYPE:
                case DBGBP_RWATCH_TYPE:
                    // Print out the updated locale information for the watchpoint.
                    // This was updated in EventCB before calling this function.
                    for (const auto &watch_loc : bpset->watchLocaleSet()) {
                        PrintMsgWithSetInfo(
                            ev_set.di().handle().c_str(), watch_loc.group.get(),
                            "Watchpoint %d, variable expression: %s\n",
                            bpset->id(), bpset->watchExpression().c_str());
                        if (watch_loc.line > 0) {
                            PrintMsgWithSetInfo(
                                ev_set.di().handle().c_str(), watch_loc.group.get(),
                                "In %s at %s:%d\n", watch_loc.func.c_str(),
                                watch_loc.file.c_str(), watch_loc.line);
                        } else {
                            PrintMsgWithSetInfo(
                                ev_set.di().handle().c_str(), watch_loc.group.get(),
                                "In %s\n", watch_loc.func.c_str());
                        }
                    }
                    // clear out the watchpoint locale to avoid printing multiple times
                    bpset->clearWatchLocaleSet();
                    break;

                case DBGBP_BREAK_TYPE:
                    if (Client.print_bp_frames) {
                        if (bpset->line() > 0) {
                            if (Client.MI_mode){
                                PrintMIStoppedMsg(StoppedType::BKPT_HIT, bpset->file(), bpset->function(), 
                                                  bpset->line(), std::to_string(bpset->id()), ev_set.di().handle().name(),
                                                  &ev_set.group(), ev_set.di().sourcePaths().getFullPath(bpset->file()));
                            }
                            PrintMsgWithSetInfo(
                                ev_set.di().handle().c_str(), &ev_set.group(),
                                "Breakpoint %d, %s at %s:%d%s\n", bpset->id(),
                                bpset->function().c_str(), bpset->file().c_str(),
                                bpset->line(), bpThread.c_str());
                        } else {
                            PrintMsgWithSetInfo(ev_set.di().handle().c_str(),
                                                &ev_set.group(),
                                                "Breakpoint %d, in %s%s\n",
                                                bpset->id(), bpset->function().c_str(),
                                                bpThread.c_str());
                        }
                    }
                    break;

                case DBGBP_CATCH_TYPE:
                    if (!bpset->watchExpression().empty()) {
                        PrintMsgWithSetInfo(
                            ev_set.di().handle().c_str(), &ev_set.group(),
                            "Catchpoint %d (%s)\n",
                            bpset->id(), bpset->watchExpression().c_str());
                    } else {
                        PrintMsgWithSetInfo(ev_set.di().handle().c_str(),
                            &ev_set.group(),
                            "Catchpoint %d\n",
                            bpset->id());
                    }
                    break;

                default:
                    if (bpset->line() > 0) {
                        PrintMsgWithSetInfo(
                            ev_set.di().handle().c_str(), &ev_set.group(),
                            "Unknown break type %d, %s at %s:%d\n",
                            bpset->id(), bpset->function().c_str(),
                            bpset->file().c_str(), bpset->line());
                    } else {
                        PrintMsgWithSetInfo(ev_set.di().handle().c_str(),
                                            &ev_set.group(),
                                            "Unknown break type %d, in %s\n",
                                            bpset->id(), bpset->function().c_str());
                    }
                    break;
            }

            break;
        }

        case DBGEV_BPDEL: {
            // ensure the recv bp has a cooresponding bp_set. If not, this is an
            // unknown breakpoint. If its unknown either its a breakpoint
            // encountered from ending the graph (graph breakpoints are hidden)
            // or an exit breakpoint that was set by the dbg servers.
            auto bpset = ev_set.di().breakpoints().findByDbgBreakpoint(*ev_set.event()->ev_bp);

            if (!bpset) {
                PrintErrorWithSetInfo(
                    ev_set.di().handle().c_str(), &ev_set.group(),
                    "ERROR: cannot find breakpoint in BPSetList.\n");
                return 0;
            }

            // ranks removed from breakpoint bitset in
            // src/dbgclnt/dbg_client.cpp

            // What we print out is based on if we are at a watchpoint or
            // breakpoint
            switch (bpset->type()) {
                case DBGBP_WATCH_TYPE:
                case DBGBP_AWATCH_TYPE:
                case DBGBP_RWATCH_TYPE:
                    // Print out the updated locale information for the watchpoint.
                    // This was updated in EventCB before calling this function.
                    for (const auto &watch_loc : bpset->watchLocaleSet()) {
                        PrintMsgWithSetInfo(
                            ev_set.di().handle().c_str(), watch_loc.group.get(),
                            "Watchpoint %d deleted because the program has left "
                            "the block in which its expression is valid.\n",
                            bpset->id());
                        if (watch_loc.line > 0) {
                            PrintMsgWithSetInfo(
                                ev_set.di().handle().c_str(), watch_loc.group.get(),
                                "In %s at %s:%d\n", watch_loc.func.c_str(),
                                watch_loc.file.c_str(), watch_loc.line);
                        } else {
                            PrintMsgWithSetInfo(
                                ev_set.di().handle().c_str(), watch_loc.group.get(),
                                "In %s\n", watch_loc.func.c_str());
                        }
                    }
                    // clear out the watchpoint locale to avoid printing multiple times
                    bpset->clearWatchLocaleSet();
                    break;

                case DBGBP_BREAK_TYPE:
                    if (bpset->line() > 0) {
                        if (Client.MI_mode){
                            PrintMIStoppedMsg(StoppedType::BKPT_HIT, bpset->file(), bpset->function(), bpset->line(), 
                                            std::to_string(bpset->id()), ev_set.di().handle().name(),&ev_set.group(),
                                            ev_set.di().sourcePaths().getFullPath(bpset->file()));
                        }
                        PrintMsgWithSetInfo(
                            ev_set.di().handle().c_str(), &ev_set.group(),
                            "Breakpoint %d deleted, %s at %s:%d\n",
                            bpset->id(), bpset->function().c_str(),
                            bpset->file().c_str(), bpset->line());
                    } else {
                        PrintMsgWithSetInfo(ev_set.di().handle().c_str(),
                                            &ev_set.group(),
                                            "Breakpoint %d deleted, in %s\n",
                                            bpset->id(), bpset->function().c_str());
                    }
                    break;

                case DBGBP_CATCH_TYPE:
                    if (!bpset->watchExpression().empty()) {
                        PrintMsgWithSetInfo(
                            ev_set.di().handle().c_str(), &ev_set.group(),
                            "Catchpoint %d deleted (%s)\n",
                            bpset->id(), bpset->watchExpression().c_str());
                    } else {
                        PrintMsgWithSetInfo(ev_set.di().handle().c_str(),
                            &ev_set.group(),
                            "Catchpoint %d deleted.\n",
                            bpset->id());
                    }
                    break;

                default:
                    if (bpset->line() > 0) {
                        PrintMsgWithSetInfo(
                            ev_set.di().handle().c_str(), &ev_set.group(),
                            "Unknown break type %d deleted, %s at %s:%d\n",
                            bpset->id(), bpset->function().c_str(),
                            bpset->file().c_str(), bpset->line());
                    } else {
                        PrintMsgWithSetInfo(
                            ev_set.di().handle().c_str(), &ev_set.group(),
                            "Unknown break type %d deleted, in %s\n",
                            bpset->id(), bpset->function().c_str());
                    }
                    break;
            }

            break;
        }

        case DBGEV_SIGNAL: {
            //always print signals to console
            PrintOutMItoCli printMItoCli;
            PrintMsgWithSetInfo(ev_set.di().handle().c_str(), &ev_set.group(),
                                "Program received signal %s.\n",
                                ev_set.event()->ev_sig_type);

            if (ev_set.event()->ev_sig_frame != (dbgframe_t *)NULL) {
                const auto &frameLoc = ev_set.event()->ev_sig_frame->getLoc();
                if (frameLoc.loc_file != NULL) {
                    PrintMsgWithSetInfo(
                        ev_set.di().handle().c_str(), &ev_set.group(),
                        "In %s at %s:%d\n",
                        frameLoc.loc_func, frameLoc.loc_file, frameLoc.loc_line);
                } else {
                    PrintMsgWithSetInfo(
                        ev_set.di().handle().c_str(), &ev_set.group(), "In %s\n",
                        frameLoc.loc_func);
                }
            } else if (ev_set.event()->ev_sig_line != NULL) {
                PrintMsgWithSetInfo(ev_set.di().handle().c_str(), &ev_set.group(),
                                    "%s\n", ev_set.event()->ev_sig_line);
            }

            break;
        }

        case DBGEV_STEP:
            if (Client.print_source_on_step)
                printEventLocation(&ev_set, &reply_set);
            break;

        case DBGEV_ERROR:
            PrintErrorWithSetInfo(ev_set.di().handle().c_str(), &ev_set.group(),
                                  "%s\n", DbgEventErrorStr(ev_set.event()));
            break;

        case DBGEV_OK:
            break;

        default:
            PrintErrorWithSetInfo(ev_set.di().handle().c_str(), &ev_set.group(),
                                  "Can't print event %d\n",
                                  ev_set.event()->ev_event);
            break;
    }

    cli_restore_stream();

    return 1;
}

void ConsoleCommandsFactory::insert_proc_commands(Command& root) {
    root.addSubCmd(
        "info application", nullptr, proc_ignore,
        cmd_info_application_cmd,
        cmd_info_application_help,
        "Show attached/launched applications under debugger control.");
}

void InitCmd_proc(void) {
    // regular commands
    AddCommand("launch", NULL, proc_ignore, cmd_launch_cmd,
               cmd_launch_help, "Launch an application.");
    AddCommand("attach", NULL, proc_ignore, cmd_attach_cmd,
               cmd_attach_help, "Attach to an application.");
    AddCommand("halt", NULL, proc_required, cmd_halt_cmd,
               cmd_halt_help,
               "Halt execution of attached/launched application.");
    AddCommand("release", NULL, proc_ignore, cmd_release_cmd,
               cmd_release_help,
               "Release attached/launched application from debugger control.");
    AddCommand("kill", NULL, proc_ignore, cmd_kill_cmd,
               cmd_kill_help, "Kill attached/launched application.");
    AddCommand("run", "r", proc_optional, cmd_run_cmd,
               cmd_run_help, "Release a procset and re-launch it, transferring any procset-specific settings/breakpoints/etc.");

    // set/show commands
    AddCommand("set args", proc_required, set_subcmd_args_cmd,
               set_subcmd_args_help, "Set program arguments to be used when re-launching with the run command.");
    AddCommand("show args", proc_required, show_subcmd_args_cmd,
               show_subcmd_args_help, "Show program arguments to be used when re-launching with the run command.");
}
