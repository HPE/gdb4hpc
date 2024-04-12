/******************************************************************************\
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP.
 * 
 * Helper functions for shipping files common to all protocols via CTI
 ******************************************************************************/

#pragma once

#include <optional>
#include <string>

#include "common_tools_fe.h"

// ship gdb-guard, or custom gdb-guard if *gdb is not nullptr
// returns name of binary to give to dbgsrv as --gdb option, or std::nullopt on error
std::optional<std::string> shipGdb(cti_manifest_id_t manifest,
                                   const std::string& gdb, const std::string& gdbDataDir);
// ship gpu specific gdbs
bool shipGpuGdbs(cti_manifest_id_t manifest);
// ship ompd related files
bool shipOmpd(cti_manifest_id_t manifest);
// ship extension for debugging the cpython interpreter
bool shipPythonGdb(cti_manifest_id_t manifest);
// ship files common to all protocols
bool shipCommonFiles(cti_manifest_id_t manifest);
