// Copyright 2024 Hewlett Packard Enterprise Development LP.

#include <stdlib.h>
#include <string.h>

#include "MIValue.h"
#include "MISymbol.h"
#include "MIResult.h"

MISymbol* MISymbolNew() {
	MISymbol* result = malloc(sizeof(MISymbol));
	result->name = NULL;
	result->type = NULL;
	result->description = NULL;
	result->line = -1;
	result->address = 0;
	return result;
}

void MISymbolFree(MISymbol* sym) {
	free(sym->name);
	free(sym->type);
	free(sym->description);

	free(sym);
}

MIFileSymbols* MIFileSymbolsNew() {
	MIFileSymbols* result = malloc(sizeof(MIFileSymbols));
	result->filename = NULL;
	result->fullname = NULL;
	result->symbols = NULL;
	return result;
}

void MIFileSymbolsFree(MIFileSymbols* fsyms) {
	free(fsyms->filename);
	free(fsyms->fullname);
	MIListFree(fsyms->symbols, MISymbolFree);

	free(fsyms);
}

MISymbolInfo* MISymbolInfoNew() {
	MISymbolInfo* result = malloc(sizeof(MISymbolInfo));
	result->debug = NULL;
	result->nondebug = NULL;
	return result;
}

void MISymbolInfoFree(MISymbolInfo* syms) {
	MIListFree(syms->debug, MIFileSymbolsFree);
	MIListFree(syms->nondebug, MISymbolFree);

	free(syms);
}

// parses
//
// [{line="25",name="global_f1",type="float",
//   description="static float global_f1;"},
//  {line="24",name="global_i1",type="int",
//   description="static int global_i1;"}]
//
// or
//
//  [{address="0x00000000004005d0",name="_IO_stdin_used"},
//   {address="0x00000000004005d8",name="__dso_handle"}
//    ...
//  ]
static MIList* parse_symbol_list(const MIValue* in_value) {
	MIList* out = MIListNew(); // of MISymbol
	
	MIListSet(in_value->values);
	for (MIValue* sym_val; (sym_val = (MIValue*)MIListGet(in_value->values)); ) {
		MISymbol* this_sym = MISymbolNew();

		// note that it is not expected that every field will be present

		MIListSet(sym_val->results);
		for (MIResult* res; (res = (MIResult*)MIListGet(sym_val->results)); ) {
			if (strcmp(res->variable, "address") == 0 && res->value->type == MIValueTypeConst) {
				this_sym->address = strtoul(res->value->cstring, NULL, 16);
			} else if (strcmp(res->variable, "name") == 0 && res->value->type == MIValueTypeConst) {
				this_sym->name = strdup(res->value->cstring);
			} else if (strcmp(res->variable, "type") == 0 && res->value->type == MIValueTypeConst) {
				this_sym->type = strdup(res->value->cstring);
			} else if (strcmp(res->variable, "description") == 0 && res->value->type == MIValueTypeConst) {
				this_sym->description = strdup(res->value->cstring);
			} else if (strcmp(res->variable, "line") == 0 && res->value->type == MIValueTypeConst) {
				this_sym->line = strtol(res->value->cstring, NULL, 10);
			}
		}

		MIListAdd(out, this_sym);
	}

	return out;
}

// parses
//
// debug=
//   [{filename="mi-sym-info-1.c",
//     fullname="/project/gdb/testsuite/gdb.mi/mi-sym-info-1.c",
//     symbols=[{line="25",name="global_f1",type="float",
//               description="static float global_f1;"},
//              {line="24",name="global_i1",type="int",
//               description="static int global_i1;"}]},
//    {filename="mi-sym-info-2.c",
//     fullname="/project/gdb/testsuite/gdb.mi/mi-sym-info-2.c",
//     symbols=[{line="21",name="global_f2",type="int",
//               description="int global_f2;"},
//              {line="20",name="global_i2",type="int",
//               description="int global_i2;"},
//              {line="19",name="global_f1",type="float",
//               description="static float global_f1;"},
//              {line="18",name="global_i1",type="int",
//               description="static int global_i1;"}]}]
static MIList* parse_symbol_info_debug(const MIValue* in_value) {
	// assuming `in_value` is the data with the key "debug" from
	// -symbol-info-functions or -symbol-info-variables

	if (!in_value || in_value->type != MIValueTypeList) {
		return NULL;
	}

	MIList* out = MIListNew(); // of MIFileSymbols
	
	MIListSet(in_value->values);
	for (MIValue* file_val; (file_val = (MIValue*)MIListGet(in_value->values)); ) {
		MIFileSymbols* this_file = MIFileSymbolsNew();

		MIListSet(file_val->results);
		for (MIResult* res; (res = (MIResult*)MIListGet(file_val->results)); ) {
			if (strcmp(res->variable, "filename") == 0 && res->value->type == MIValueTypeConst) {
				this_file->filename = strdup(res->value->cstring);
			} else if (strcmp(res->variable, "fullname") == 0 && res->value->type == MIValueTypeConst) {
				this_file->fullname = strdup(res->value->cstring);
			} else if (strcmp(res->variable, "symbols") == 0 && res->value->type == MIValueTypeList) {
				this_file->symbols = parse_symbol_list(res->value);
			}
		}

		MIListAdd(out, this_file);
	}

	return out;
}

// parses
//
// nondebug=
//   [{address="0x00000000004005d0",name="_IO_stdin_used"},
//    {address="0x00000000004005d8",name="__dso_handle"}
//     ...
//   ]
static MIList* parse_symbol_info_nondebug(const MIValue* in_value) {
	// assuming `in_value` is the data with the key "nondebug" from
	// -symbol-info-functions or -symbol-info-variables.

	if (!in_value || in_value->type != MIValueTypeList) {
		return NULL;
	}

	return parse_symbol_list(in_value);
}

// this works for -symbol-info-functions and -symbol-info-variables.
// this MIGHT work for -symbol-info-types, but hasn't been tested.
MISymbolInfo* MIGetSymbolInfo(const MICommand* cmd) {
	if (!cmd->completed
		|| cmd->output == NULL || cmd->output->rr == NULL || cmd->output->rr->results == NULL)
	{
		return NULL;
	}

	// for convenience. list holds MIResult*. See MISynbol.h for expected structure.
	MIList* input_result_records = cmd->output->rr->results;

	MISymbolInfo* out = MISymbolInfoNew();

	// we only expect one item in the list called "symbols"
	MIListSet(input_result_records);
	for (MIResult* res; (res = (MIResult*)MIListGet(input_result_records)); ) {
		if (strcmp(res->variable, "symbols") == 0 && res->value->type == MIValueTypeTuple) {
			// iterate over the {debug=..., nondebug=...} tuple
			MIListSet(res->value->results);
			for (MIResult* symbol_result; (symbol_result = (MIResult*)MIListGet(res->value->results)); ) {
				if (strcmp(symbol_result->variable, "debug") == 0 && symbol_result->value->type == MIValueTypeList) {
					out->debug = parse_symbol_info_debug(symbol_result->value);
				} else if (strcmp(symbol_result->variable, "nondebug") == 0 && symbol_result->value->type == MIValueTypeList) {
					out->nondebug = parse_symbol_info_nondebug(symbol_result->value);
				}
			}
			break; // we only expect one "symbols", so we're done
		} else {
			// malformed.
			return NULL;
		}
	}

	return out;
}
