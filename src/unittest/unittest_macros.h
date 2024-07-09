/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "config.h"

#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include "doctest.h"

// Test writers should include this file to get access to doctest macros like TEST_CASE
// etc. DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL tells doctest to look for the test
// registry/runner and functions/symbols in a shared library that will be linked later.
// That shared library is created by unittest_impl.cpp and is called libgdb4_unittest.so.
// At some point in the build chain, users of this header need to -lgdb4_unittest. It is
// harmless for multiple translation units to -lgdb4_unittest.
