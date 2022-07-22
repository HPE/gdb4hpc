// mi_funcs.h - Command line interface routines.
// Copyright 2022 Hewlett Packard Enterprise Development LP.

#pragma once

#include "dbgevent.h"
#include "list.h"

extern char * convertMiToCli(char *);
extern char * mi_response(const char *, const char *, const char *);
