/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "config.h"

#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

// By defining both DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL and DOCTEST_CONFIG_IMPLEMENT in
// this file, doctest will generate the test registration/runner functions/symbols in this
// translation unit. Test writers can then link this library (-lgdb4_unittest) and have
// all their needed symbols defined
