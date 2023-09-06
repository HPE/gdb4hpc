// mi_funcs.h - Command line interface routines.
// Copyright 2022-2023 Hewlett Packard Enterprise Development LP.

#pragma once

#include "dbgevent.h"

extern void mi_logv(const char* format, va_list arg_list);
extern void mi_logv2(const char* tag, const char* format, va_list arg_list);
extern void mi_log(const char* format, ...) __attribute__((format (printf,1,2)));

// ccdb use the frame address to match the stack trace to the current thread
extern const char* mi_dummyFrameAddr;

