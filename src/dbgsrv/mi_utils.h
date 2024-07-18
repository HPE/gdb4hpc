// Copyright (c) 2024 Hewlett Packard Enterprise Development LP.

#pragma once

#include <memory>

#include "MI.h"

// synchronously send `cmd`, wait for the result, and store it in `cmd`.
void SendMICommand(MISession* mi, MICommand* cmd);

// make a unique pointer out of `cmd`
std::unique_ptr<MICommand, decltype(&MICommandFree)> make_MICmd(MICommand* cmd);
