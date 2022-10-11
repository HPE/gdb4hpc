/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

// integration with doctest on the command line

// return 0 if unit tests are present in the binary, or -1 if they aren't
int hasUnitTests();
// run unit tests and return their generated exit code.
// if unit tests are not preset, print a message to stderr and return 0
int runUnitTests(int argc, char* argv[]);
