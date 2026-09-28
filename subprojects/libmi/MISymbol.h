// Copyright 2024 Hewlett Packard Enterprise Development LP.

#ifndef _MISYMBOL_H_
#define _MISYMBOL_H_

#include "MIList.h"
#include "MICommand.h"

struct MISymbolInfo {
	// holds the full result of a -symbol-info-* command.
	// field names are based on field names of an mi -symbol-info-* response.

	// list of MIFileSymbols, always present after MIGetSymbolInfo.
	// symbols in this list have debug info fields
	// (see other data structures below for what that means).
	// freed by MISymbolInfoFree.
	MIList* debug;

	// list of MISymbols, only present after MIGetSymbolInfo if the --include-nondebug
	// flag was used in the -symbol-info-* command.
	// symbols in this list do not have debug info fields
	// (see other data structures below for what that means).
	// freed by MISymbolInfoFree.
	MIList* nondebug;
};
typedef struct MISymbolInfo MISymbolInfo;

// given a completed -symbol-info-functions or -symbol-info-variables `cmd`,
// extract MISymbolInfo. all data from the `cmd` is copied - the caller is
// responsible for calling MISymbolInfoFree on the returned data.
//
// returns NULL on error.
MISymbolInfo* MIGetSymbolInfo(const MICommand* cmd);

void MISymbolInfoFree(MISymbolInfo* syms);

// -----------------------------------------------------------------------------
// the rest of this file is implementation details of MIGetSymbolInfo
// and are extra credit for the average libmi user.
//
// these data structures are all closely related to the
//   -symbol-info-functions
//   -symbol-info-types (currently unimplemented)
//   -symbol-info-variables (currently unimplemented)
// commands, which all generate similar output.
//
// -symbol-info-module* (fortran) commands are not implemented yet.
//
// these data structures are layered together to build the
// parse result of an mi response like:
//
// ^done,symbols=
//   {debug=
//     [{filename="mi-sym-info-1.c",
//       fullname="/project/gdb/testsuite/gdb.mi/mi-sym-info-1.c",
//       symbols=[{line="25",name="global_f1",type="float",
//                 description="static float global_f1;"},
//                {line="24",name="global_i1",type="int",
//                 description="static int global_i1;"}]},
//      {filename="mi-sym-info-2.c",
//       fullname="/project/gdb/testsuite/gdb.mi/mi-sym-info-2.c",
//       symbols=[{line="21",name="global_f2",type="int",
//                 description="int global_f2;"},
//                {line="20",name="global_i2",type="int",
//                 description="int global_i2;"},
//                {line="19",name="global_f1",type="float",
//                 description="static float global_f1;"},
//                {line="18",name="global_i1",type="int",
//                 description="static int global_i1;"}]}],
//    nondebug=
//     [{address="0x00000000004005d0",name="_IO_stdin_used"},
//      {address="0x00000000004005d8",name="__dso_handle"}
//       ...
//     ]}
//
// note that for most data structures some fields are left blank depending
// on the flags passed to -symbol-info-*. each field below is noted to
// be "always present" or not. note that the the presence of the data
// refers to the state after the GetSymbolInfo function, not the *New() functions.

// allocate and initialize a MISymbolInfo to NULL/0 state.
MISymbolInfo* MISymbolInfoNew();

struct MISymbol {
	// the basic symbol atom. contents of the inner symbol= tuples in example above.

	// name of symbol.
	// always present.
	char* name;

	// type of symbol.
	// only present if symbol has debug info, otherwise NULL.
	char* type;

	// "description" of symbol. gdb tends to combine the `type`
	// and `name` in this field. only present if symbol
	// has debug info, otherwise NULL.
	char* description;

	// declaration line of symbol. only present if symbol
	// has debug info, otherwise -1.
	int line;

	// address of symbol.
	// only present if symbol DOES NOT have debug info, otherwise 0.
	unsigned long address;
};
typedef struct MISymbol MISymbol;

// allocate and initialize a MISymbol to NULL/0 state.
MISymbol* MISymbolNew();
void MISymbolFree(MISymbol* sym);

struct MIFileSymbols {
	// a collection of symbols for a given file.
	// contents of the tuples with filename=, fullname=, symbols= above.

	char* filename; // always present.
	char* fullname; // always present.

	// list of MISymbol. always present.
	// freed by MIFileSymbolsFree.
	MIList* symbols;
};
typedef struct MIFileSymbols MIFileSymbols;

// allocate and initialize a MIFileSymbol to NULL/0 state.
// note that the resulting MIFileSymbol's `symbols` list is NULL too.
// populate it with MIGetSymbolInfo.
MIFileSymbols* MIFileSymbolsNew();
void MIFileSymbolsFree(MIFileSymbols* fsyms);

#endif /* _MISYMBOL_H_ */
