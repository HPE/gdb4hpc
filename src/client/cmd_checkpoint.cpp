/******************************************************************************
 * Copyright 2022-2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <map>
#include <filesystem>

#include "client.h"
#include "useful.h"

using namespace std::string_literals;

static int cmd_checkpoint_cmd(rec_set_t *rec_set, int argc, char **argv) {
    auto repSet = std::unique_ptr<rep_set_t>(DbgSetCheckpoint(rec_set));

    if (repSet == nullptr) {
        PrintOutError("Failed to create checkpoint: No response from compute nodes.\n");
        return 0;
    }

    if (ProcessErrorInRepSet(repSet.get())) {
        return 0;
    }

    for (const auto *rec : rec_set) {
        if (auto newest = rec->rec_di.checkpoints().newest()) {
            PrintOutMsg(
                "Created checkpoint %u on $%s%s.\n", 
                newest->id(), rec->rec_di.handle().c_str(), 
                newest->ranksSet().toSetString().c_str()
            );
        }
    }

    return 1;
}

static int cmd_info_checkpoints_cmd(rec_set_t *rec_set, int argc, char **argv) {
    // combine all checkpoints from all dis into single ID-indexed map
    std::map<unsigned int, std::vector<std::pair<DbgInvoke::Handle, const MappedCheckpoint *>>> idMap;
    for (const auto *rec : rec_set) {
        for (const auto &check : rec->rec_di.checkpoints()) {
            idMap[check.id()].push_back({rec->rec_di.handle(), &check});
        }
    }

    if (idMap.size() == 0) {
        PrintOutMsg("No checkpoints.\n");
        return 1;
    }

    // table formating values
    auto cheHeader = "Checkpoint"s;
    auto proHeader = "Procset"s;
    auto proWidth = proHeader.size();

    // Print an entry in the checkpoint table with proper spacing
    auto printTableEntry = [&](
        const std::string &id,
        const std::string &resident,
        const std::string &cStar,
        const std::string &at
    ){
        std::ostringstream out;

        out << std::left
            << std::setw(cheHeader.size() + 1) << id
            << std::setw(proWidth + 1) << resident
            << std::setw(2) << cStar
            << std::setw(0) << at
        ;

        PrintOutMsg("%s\n", out.str().c_str());
    };

    // One pass to calibrate table entry widths
    for (const auto &[id, checkpointsV] : idMap) {
        for (const auto &[di, checkPtr] : checkpointsV) {
            if (!di.lock() || !checkPtr) {
                // shouldn't happen...
                continue;
            }

            for (const auto &loc : checkPtr->locations()) {
                proWidth = std::max(proWidth, ("$"s + di.c_str() + loc.group->toSetString()).size());
            }
        }
    }

    // Print header
    printTableEntry(cheHeader, proHeader, "", "Location");

    // Print data
    for (const auto &[id, checkpointsV] : idMap) {
        std::string idStr = std::to_string(id);
        for (const auto &[di, checkPtr] : checkpointsV) {
            if (!di.lock() || !checkPtr) {
                // shouldn't happen...
                PrintOutError("Warning: null checkpoint data\n");
                continue;
            }

            bool isCurrent = !checkPtr->currentSet().isEmpty();

            for (const auto &loc : checkPtr->locations()) {
                printTableEntry(
                      idStr 
                    , "$"s + di.c_str() + loc.group->toSetString()
                    , isCurrent ? "*" : ""
                    , isCurrent ? 
                        "$"s + di.c_str() + checkPtr->currentSet().toSetString()
                        :
                        std::filesystem::path(loc.file).filename().string()
                        + ":" + std::to_string(loc.line) + " in " + loc.func
                );
                idStr = ""; // only print ID once
            }
        }
    }

    return 1;
}

static int cmd_restart_cmd(rec_set_t *recSet, int argc, char **argv) {
    const auto usage = [](){
        PrintOutError("Usage: restart <checkpoint id>\n");
    };
    
    if (argc < 2 || !IsInteger(argv[1])) {
        usage();
        return 0;
    }

    errno = 0;
    auto id = strtoul(argv[1], nullptr, 10);
    if (errno) {
        usage();
        return 0;
    }

    if (auto repSet = std::unique_ptr<rep_set_t>(DbgRestoreCheckpoint(recSet, id))) {
        ProcessErrorInRepSet(repSet.get());
        return 0;
    }

    PrintOutMsg("Checkpoint %lu restored:\n", id);

    // show new locations
    auto frameRepSet = std::unique_ptr<rep_set_t>(DbgGetFrame(recSet, false));

    if (ProcessErrorInRepSet(frameRepSet.get())) {
        return 0;
    }

    for (const auto* reply : frameRepSet.get()) {
        auto fp = EVENT_FRAME(reply->rep_event);
        PrintFrameSet(reply->rep_di.c_str(), reply->rep_group.get(), fp);
    }

    return 1;
}

void InitCmd_checkpoint(void) {
    AddCommand("checkpoint", "ch", proc_required, cmd_checkpoint_cmd,
               cmd_checkpoint_help,
               "Fork program for restoration at a later point.");

    AddCommand("restart", "res", proc_required, cmd_restart_cmd,
               cmd_restart_help,
               "Restore program to state of previously created checkpoint.");

    // the delete checkpoint command lives in cmd_break.cpp, as a subcommand of
    // the delete command

    // info commands
    AddCommand(
        "info checkpoints", proc_required,
        cmd_info_checkpoints_cmd,
        cmd_info_checkpoints_help, "Show status of checkpoints.");
}
