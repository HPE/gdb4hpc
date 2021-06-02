/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 * 
 * Helper functions common to all protocols
 ******************************************************************************/

#pragma once

#include <optional>
#include <string>

#include "common_tools_fe.h"

// ship gdb-guard, or custom gdb-guard if *gdb is not nullptr
// returns name of binary to give to dbgsrv as --gdb option, or std::nullopt on error
std::optional<std::string> shipGdb(cti_manifest_id_t manifest, const char *gdb);
// ship gpu specific gdbs
bool shipGpuGdbs(cti_manifest_id_t manifest);
// ship ompd related files
bool shipOmpd(cti_manifest_id_t manifest);
// ship files common to all protocols
bool shipCommonFiles(cti_manifest_id_t manifest, bool gpu_flag);
