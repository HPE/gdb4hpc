/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "unit_tests.h"
#include "doctest.h"
#include <cstdio>

int hasUnitTests() {
#ifndef DOCTEST_CONFIG_DISABLE
    return 0;
#else
    return -1;
#endif
}

int runUnitTests(int argc, char* argv[]) {
#ifndef DOCTEST_CONFIG_DISABLE
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
#else
    fprintf(stderr, "This build of gdb4hpc was compiled without tests.\n");
    return 0;
#endif
}
