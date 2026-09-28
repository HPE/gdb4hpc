// Copyright 2024 Hewlett Packard Enterprise Development LP.

#include <stdlib.h>

#include "MICommand.h"

static MICommand* make_symbol_info_cmd(const char* command, int include_nondebug, const char* type_regexp, const char* name_regexp, int max_results) {
	MICommand* cmd = MICommandNew(command, MIResultRecordDONE);

	if (include_nondebug != 0) {
		MICommandAddOption(cmd, "--include-nondebug", NULL);
	}

	if (type_regexp != NULL) {
		MICommandAddOption(cmd, "--type", type_regexp);
	}

	if (name_regexp != NULL) {
		MICommandAddOption(cmd, "--name", name_regexp);
	}

	if (max_results > 0) {
		MICommandAddOption(cmd, "--max-results", MIIntToCString(max_results));
	}

	return cmd;
}

MICommand* MISymbolInfoFunctions(int include_nondebug, const char* type_regexp, const char* name_regexp, int max_results) {
	return make_symbol_info_cmd("-symbol-info-functions", include_nondebug, type_regexp, name_regexp, max_results);
}

MICommand* MISymbolInfoVariables(int include_nondebug, const char* type_regexp, const char* name_regexp, int max_results) {
	return make_symbol_info_cmd("-symbol-info-variables", include_nondebug, type_regexp, name_regexp, max_results);
}
