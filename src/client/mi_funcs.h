// mi_funcs.h - Command line interface routines.
// Copyright 2022 Hewlett Packard Enterprise Development LP.

#pragma once

#include "dbgevent.h"
#include "list.h"

extern std::string convertMiToCli(const std::string& cmd);

extern void mi_logv(const char* format, va_list arg_list);
extern void mi_logv2(const char* tag, const char* format, va_list arg_list);
extern void mi_log(const char* format, ...) __attribute__((format (printf,1,2)));
