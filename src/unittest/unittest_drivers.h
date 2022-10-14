/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <cstdio>

#include "unittest_macros.h"

// integration with doctest on the command line

// return 0 if unit tests are present in the binary, or -1 if they aren't
int hasUnitTests() {
#ifndef DOCTEST_CONFIG_DISABLE
    return 0;
#else
    return -1;
#endif
}

// run unit tests and return their generated exit code.
// if unit tests are not preset, print a message to stderr and return 0
int runUnitTests(int argc, char* argv[]) {
#ifndef DOCTEST_CONFIG_DISABLE
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
#else
    fprintf(stderr, "Compiled without tests.\n");
    return 0;
#endif
}
