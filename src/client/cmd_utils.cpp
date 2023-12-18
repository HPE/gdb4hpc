// Copyright 2023 Hewlett Packard Enterprise Development LP.
//
// Utility commands - data/state management commands that touch multiple components

#include <filesystem>
#include <fstream>

#include "interpreter_console.h"
#include "interpreter_mi.h"

static int save_all_cmd(rec_set_t* rec_set, int argc, const char* const argv[]) {
    extern int save_all_breakpoints(rec_set_t* rec_set, std::ostream& os); // cmd_break.cpp
    extern int save_all_blockmaps(std::ostream& os); // cmd_blockmap.cpp
    extern int save_all_scripts(std::ostream& os); // cmd_script.cpp

    if (argc < 2) {
        PrintOutError("Usage: save all [filename]\n");
        return Command::Failure;
    }

    auto filename = std::filesystem::path{argv[1]};
    auto outfile = std::ofstream{filename};
    if (!outfile) {
        PrintOutError("Failed to open %s for writing.\n", filename.c_str());
        return Command::Failure;
    }

    auto result = Command::Success;

    const auto try_and_sticky_fail = [](auto new_result, auto& running_result){
        running_result = running_result != Command::Failure ? new_result : running_result;
    };

    // these will all print out errors as they are encountered
    try_and_sticky_fail(save_all_breakpoints(rec_set, outfile), result);
    try_and_sticky_fail(save_all_blockmaps(outfile), result);
    try_and_sticky_fail(save_all_scripts(outfile), result);

    if (result == Command::Success && outfile) {
        PrintOutMsg("Saved debugger state to %s. Use the source command to load it.\n", argv[1]);
        return result;
    } else {
        PrintOutError("Failed to save state to %s.\n", argv[1]);
        return result;
    }
}

// TODO: PE-51617: move delete command here

void ConsoleCommandsFactory::insert_utils_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "save all",
        .ptype = proc_required, // required to save breakpoints
        .fn = save_all_cmd,
        .help = cmd_save_all_help,
        .desc = "Save all debugger state to a file.",
   });
}

void MiCommandsFactory::insert_utils_commands([[maybe_unused]] CommandNode& root) {
    // none yet
}
