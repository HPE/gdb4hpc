/*******************************************************************************
 * Compare Utilities
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include "val.h"

enum class CompareResult {
    Pass, Warn, Fail, Unknown
};

void printCompareResult(val_t* result, val_t* difference, rec_set_t& recSet);
// get a simple answer to the question: "did this assertion pass?"
CompareResult simpleCompareResult(const val_t& result);
