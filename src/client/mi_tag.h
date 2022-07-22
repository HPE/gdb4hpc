// mi_tag.h - Command line interface routines.
// Copyright 2022 Hewlett Packard Enterprise Development LP.

#pragma once

#define MI_RESULT "^%s,%s=%s\n"

#define MI_SIMPLE_RESULT "^%s\n"

#define MI_STATUS_STOPPED "^stopped"
#define MI_STATUS_DONE "^done"
#define MI_STATUS_EXIT "^exit"
#define MI_STATUS_ERROR "^error"

#define MI_BKPT_TABLE "BreakpointTable={nr_rows=\"2\",nr_cols=\"6\", \
hdr=[{width=\"3\",alignment=\"-1\",col_name=\"number\",colhdr=\"Num\"}, \
{width=\"14\",alignment=\"-1\",col_name=\"type\",colhdr=\"Type\"}, \
{width=\"4\",alignment=\"-1\",col_name=\"disp\",colhdr=\"Disp\"}, \
{width=\"3\",alignment=\"-1\",col_name=\"enabled\",colhdr=\"Enb\"}, \
{width=\"10\",alignment=\"-1\",col_name=\"addr\",colhdr=\"Address\"}, \
{width=\"40\",alignment=\"2\",col_name=\"what\",colhdr=\"What\"}], \
body"
