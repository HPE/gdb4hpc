/******************************************************************************\
 * rd_help_dict.c - RD command help string defintion dictionary.
 *
 * © 2011-2013 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 ******************************************************************************/

/**********************************************
 * Regular rd command help definitions
 **********************************************/

const char *cmd_blockmap_help[] = {
	"Summary: Decomposition creation command.",
	"Usage: decomposition $<name>",
	"Example command: decomposition $myName",
	"",
	"Script mode commands: dimension, distribute, proc_grid, dim_order, end, help",
	"Arguments: name (debugger handle)",
	"",
	"Description:",
	"Begin the definition of a new decomposition scheme. Decomposition sub-commands",
	"are read until \"end\" is encountered. Decomposition is used to specify the",
	"reconstruction of distributed variables into the global representation of",
	"the data.",
	"",
	"Decompositions can be used in an assertion to compare data between instances of",
	"applications. When a decomposition is used in an assertion, it is supplied with a",
	"process handle and a local variable to operate on. It will take each ranks variable",
	"and recreate the global array based on the definitions provided to the decomposition.",
	"",
	"Typically a decomposition is used inside of an assertion script in order to compare",
	"the global representation of the data space between two instances of applications",
	"that may vary in terms of scale but the global data size is the same.",
	"",
	"To create a decomposition, the user must provide definitions for dimension,",
	"distribute, proc_grid, and dim_order.",
	"",
	"See usage dimension/distribute/proc_grid/dim_order for more information.",
	0};

const char *cmd_build_help[] = {
	"Summary: Assertion script creation command.",
	"Usage: build <name>",
	"Example command: build $my_script",
	"",
	"Script mode commands: assert, assign, end, help, set, show",
	"Arguments: name (debugger variable)",
	"",
	"Description:",
	"Begin creation of a new assertion script. Build sub-commands are read",
	"until \"end\" is encountered. See usage assert for more information.",
	0};

const char *cmd_start_help[] = {
	"Summary: Assertion script execution command.",
	"Usage: start <name>",
	"Example command: start $my_script",
	"",
	"Arguments: name (built assertion script name)",
	"",
	"Description:",
	"Begin execution of a built assertion script. Assertion script execution",
	"cycles until encountering assertion failures or reaching the end of program",
	"execution. Behavior can be changed via the \"set stop\" command.",
	0};

const char *cmd_stop_help[] = {
	"Summary: Assertion script halt command.",
	"Usage: stop",
	"Example command: stop",
	"",
	"Arguments: None.",
	"",
	"Description:",
	"Halt the current assertion script execution immediately. This command is used",
	"to abort assertion script execution.",
	0};

/**********************************************
 * Alt rd command help definitions
 **********************************************/

const char *cmd_blockmap_alt_dimension_help[] = {
	"Summary: Decomposition dimension command.",
	"Usage: dimension <dimensional definition>",
	"Example command: dimension 32,64,1024",
	"",
	"Arguments: dimensional definition (rank/size information)",
	"",
	"Description:",
	"The dimension definition is used to describe the dimensionality (or rank) and size",
	"of the global decomposition.", 
	"",
	"The dimension command accepts a comma-separated numerical list to define the",
	"decompositions's global data dimension and size in each dimension.",
	"For example, a dimension definition of 32,64,1024 would define a three dimensional",
	"global array of size 32x64x1024. The first entry in the list represents the first",
	"dimension of the global reconstruction, the second entry represents the second",
	"dimension, and so on.",
	"",
	"The dimensionality of each provided decomposition characteristic must match.",
	"If a dimension definition of 32,64,1024 is supplied, then three dimensional",
	"distribute, proc_grid, and dim_order definitions must also be provided to",
	"describe the characteristics of each of the three dimensions.",
	0};
	
const char *cmd_blockmap_alt_distribute_help[] = {
	"Summary: Decomposition distribute command.",
	"Usage: distribute <dimensional definition>",
	"Example command: distribute *,block,block",
	"",
	"Arguments: dimensional definition (distribution type information)",
	"",
	"Description:",
	"The distribute definition is used to describe the distribution scheme for each",
	"dimension of the global decomposition.", 
	"",
	"The distribute definition accepts a comma-separated list of distribution types",
	"to define each dimension of the global decomposition. Valid distribution types",
	"are block, cyclic, a numerical value, or an asterisk '*'.",
	"",
	"The block keyword is used when equal sized contiguous chunks of data are assigned to",
	"each rank. The size of the chunk is determined by dividing the dimension by the proc_grid",
	"size for the dimension.",
	"", 
	"The cyclic keyword is used when elements in the dimension are dealt out in round robin", 
	"fashion. The number of ranks that are dealt over is determined by the proc_grid size for",
	"that dimension.",
	"",
	"A numerical value is used to describe a block-cyclic distribution scheme. The numerical",
	"value represents the blocking factor used to partition the global decomposition's",
	"dimension into contiguous blocks onto which the process grid is mapped.",
	"",
	"The '*' value indicates that the distribution scheme for that dimension is ignored.",
	"Each rank in the global decomposition will contain all of the data in that dimension.",
	"This can be thought of as columns (or rows) of data for that particular dimension.",
	"",
	"For '*' dimensions, the '*' keyword should also be provided to the proc_grid and",
	"dim_order definitions for that dimension because there is no distribution present.",
	"No ranks are assigned to that particular dimension (every rank contains that",
	"dimension's data).",
	"",
	"The dimensionality of each provided decomposition characteristic must match.",
	"If a distribute definition of *,block,block is supplied, then three dimensional",
	"dimension, proc_grid, and dim_order definitions must also be provided to describe",
	"the characteristics of each of the three dimensions.",
	0};
	
const char *cmd_blockmap_alt_proc_grid_help[] = {
	"Summary: Decomposition process grid command.",
	"Usage: proc_grid <dimensional definition>",
	"Example command: proc_grid *,4,64",
	"",
	"Arguments: dimensional definition (process rank information)",
	"",
	"Description:",
	"The process grid definition is used to describe the process grid (or number of",
	"ranks contained in each dimension) for the decomposition.",
	"",
	"The process grid definition accepts a comma-separated list of numerical values",
	"or asterisks '*' to define the number of ranks distributed in each dimension",
	"of the global decomposition.", 
	"",
	"For example, a process grid definition of *,4,64 would define a process grid of",
	"256 ranks (1*4*64=215) with no ranks distributed in the first dimension, four ranks",
	"distributed in the second dimension, and sixty four ranks distributed in the third",
	"dimension. As with the dimension definition, the first entry in the list represents",
	"the first dimension in the process grid of the global decomposition and so on.",
	"",
	"The '*' value represents that the distribution scheme for that dimension is ignored.",
	"Each rank in the global decomposition will contain all of the data in that dimension.",
	"This can be thought of as columns (or rows) of data for that particular dimension.",
	"",
	"For '*' dimensions, the '*' keyword should also be provided to the distribute and",
	"dim_order definitions for that dimension because there is no distribution present.",
	"No ranks are assigned to that particular dimension (every rank contains that",
	"dimension's data).",
	"",
	"The dimensionality of each provided decomposition characteristic must match.",
	"If a proc_grid definition of *,4,64 is supplied, then three dimensional dimension,",
	"distribute, and dim_order definitions must also be provided to describe the",
	"characteristics of each of the three dimensions.",
	0};

const char *cmd_blockmap_alt_dim_order_help[] = {
	"Summary: Decomposition dimension order command.",
	"Usage: dim_order <dimensional definition>",
	"Example command: dim_order *,1,2",
	"",
	"Arguments: dimensional definition (rank assignment information)",
	"",
	"Description:",
	"The dimension order definition is used to describe the fastest varying dimensions",
	"in the global decomposition for rank assignment (i.e., which dimension contains",
	"sequential numbered ranks; and the order in which the other dimensions are",
	"incremented).",
	"",
	"The dimension order definition accepts a comma-separated list of numerical",
	"values or asterisks '*' to define the order in which application ranks are",
	"assigned in each dimension of the process grid for the global decomposition.",
	"",
	"Each local \"chunk\" of data obtained from each rank needs to be placed into the",
	"global decomposition. In order to do this, each rank needs to be assigned a",
	"logical position in the process grid for its \"chunk\" of data. When dealing",
	"with n-dimensional distributions, any of the n dimensions could be assigned",
	"sequential numbered ranks and any of the other higher order dimensions could be",
	"incremented after the dimension containing contiguous ranks is filled.",
	"",
	"To define this order, each of the distributed dimensions is assigned a sequential",
	"number, starting from 1 up to n, to describe the fastest-to-slowest varying dimensions.",
	"",
	"The fastest varying dimension is the dimension that is assigned contiguous ranks",
	"up to its corresponding process grid size. The second fastest varying dimension is", 
	"incremented after the fastest varying dimension is completely filled and ranks",
	"are again assigned to the fastest varying dimension. This process continues",
	"until all ranks have been assigned to all of the n-dimensions.", 
	"",
	"Any dimension that is not distributed should receive a '*' for its corresponding",
	"dimension order dimension because all of the data in that dimension is present in",
	"every rank. For '*' dimensions, the '*' keyword should also be provided to the",
	"distribute and proc_grid definitions for that dimension because there is no",
	"distribution present.",
	"",
	"If there is only one distributed dimension, then the dim_order definition is optional",
	"because there is only a single dimension and assignment order is constant.",
	"",
	"The dimensionality of each provided decomposition characteristic must match.",
	"If a dim_order definition of *,1,2 is supplied, then three dimensional dimension,",
	"distribute, and proc_grid definitions must also be provided to describe the",
	"characteristics of each of the three dimensions.",
	0};

const char *cmd_blockmap_alt_end_help[] = {
	"Summary: Decomposition end command.",
	"Usage: end",
	"Example command: end",
	"",
	"Arguments: None.",
	"",
	"Description:",
	"This command is used to end the current decomposition definition. After",
	"issuing end, the debugger will exit script mode and attempt to create",
	"a decomposition function for future use. The user will be warned if there",
	"were any errors that prevented the decomposition function from being created.",
	0};

const char *cmd_graph_alt_assert_help[] = {
	"Summary: Assertion script assert command.",
	"Usage: assert <expr1> = <expr2>",
	"Example command:",
	"assert $app1::var1@\"app1.c\":10 = $app2::var2@\"app2.c\":20",
	"",
	"Arguments: expr1 (assert expr syntax), expr2 (assert expr syntax)",
	"",
	"Assert expression syntax",
	"",
	"Expr Usage: <procset>::<var>@\"<source file>\":<line>",
	"Expr arguments: procset (defined process set name), var (application",
	"                variable), source file (quoted source file name), line (line",
	"                in source file to reference var)",
	"Expr example: $my_app::my_var@\"my_source.c\":10",
	"",
	"Description:",
	"Assert that the variable var defined in expr1 is equivalent to the",
	"variable var defined in expr2. Variables are specified using the assert",
	"expression syntax, which declares both spatial and temporal references",
	"for a variable.",
	"",
	"Define the spatial locality with $procset::var to specify the particular",
	"variable of interest within a set of application ranks. This is followed",
	"with the temporal reference of \"source file\":line to define the point",
	"within application execution where to examine the value of the variable.",
	"",
	"The result of the assertion command depends on the current settings of",
	"error, display, output, and logging.",
	0};
	
const char *cmd_graph_alt_assign_help[] = {
	"Summary: Assertion script variable assignment command.",
	"Usage: assign <convenience var> <application var> [-force]",
	"Example command: assign $saved_var $procset::var",
	"",
	"Arguments: convenience var (debugger convenience variable), application",
	"           var (application variable), -force (optional force mode)",
	"",
	"Description:",
	"Assign the value of the application variable application var to internal",
	"debugger convenience variable convenience var. The saved application",
	"variable will then be available for use in later expressions during",
	"application execution.",
	0};

const char *cmd_graph_alt_end_help[] = {
	"Summary: Assertion script end command.",
	"Usage: end",
	"Example command: end",
	"",
	"Arguments: None.",
	"",
	"Description:",
	"This command is used to end construction of the current assertion script.",
	"After issuing end, the debugger will exit script mode and attempt to compile",
	"the assertion script. The user will be warned if there were any errors that",
	"prevented the assertion script from being compiled. Upon successful compilation,",
	"the assertion script can be run by issuing the start command.",
	0};

/**********************************************
 * Command class rd sub-command help definitions
 **********************************************/

/*
 * Class info subcommands
 */

const char *class_info_subcmd_blockmap_help[] = {
	"Summary: Display defined decompositions.",
	"",
	"Description:",
	"Display the names of defined decompositions.",
	0};

const char *class_info_subcmd_diff_help[] = {
	"Summary: Saved assertion script diff information.",
	"",
	"Description:",
	"Display the set of saved difference data from assertion script assertions",
	"collected during execution. A numeric argument can be provided to view the",
	"actual deltas of the data type comparison. Or an assertion script name (i.e. $script)",
	"along with an AssertID number (i.e. 1) can be provided to view all saved difference",
	"data corresponding to the requested scripts assertion id.",
	0};
	
const char *class_info_subcmd_build_help[] = {
	"Summary: Display built assertion scripts.",
	"",
	"Description:",
	"Display the names of built assertion scripts. A name argument can be",
	"provided to view the assertions defined in the assertion script.",
	0};

const char *class_info_subcmd_script_help[] = {
	"Summary: Assertion script status information.",
	"",
	"Description:",
	"Display summary information relating to actively executing assertion scripts.",
	0};
	
/*
 * Class maint subcommands
 */

const char *class_maint_subcmd_dump_help[] = {
	"Summary: Dataflow graph dump command.",
	"Usage: maint dump <name> [<filename>]",
	"Example command: maint dump $my_script",
	"",
	"Arguments: NAME(defined assertion script variable), FILENAME(optional quoted filename)",
	"",
	"Description:",
	"Dump a built assertion script in digraph format. An optional filename argument",
	"can be given to dump the assertion script to a file in digraph format.",
	0};

/*
 * Class set subcommands
 */

const char *class_set_subcmd_dfmode_help[] = {
	"Summary: Dataflow processing mode.",
	"Usage: set dfmode <default|sync|async>",
	"Example command: set dfmode default",
	"",
	"Scope: Assertion script",
	"Default setting: default",
	"Arguments: default or sync or async",
	"",
	"Description:",
	"Set the dataflow processing mode to synchronous or asynchronous. This",
	"changes the assertion script that was generated using the build command so",
	"that it operates in one of these three modes. Synchronous mode is",
	"default for assertions that refer to independent processes. Asynchronous",
	"mode is default for assertions that refer to the same process. Using",
	"this setting overrides the default behavior.",
	0};

const char *class_set_subcmd_display_help[] = {
	"Summary: Assertion script assert display mode.",
	"Usage: set display <text|aif|hdf|hdf5>",
	"Example command: set display text",
	"",
	"Scope: Assertion script",
	"Default setting: text",
	"Arguments: text or aif or hdf or hdf5",
	"",
	"Description:",
	"Set the display format for the result of assertion script assertions. In",
	"text mode, any differences are displayed as plaintext. In aif mode,",
	"differences are displayed in architecture independent format (AIF). In",
	"hdf and hdf5 modes, differences are displayed in the hierarchical data",
	"format (HDF) version 4 and version 5 respectively.",
	0};

const char *class_set_subcmd_force_help[] = {
	"Summary: Assertion script assert force mode.",
	"Usage: set force [on|off]",
	"Example command: set force off",
	"",
	"Scope: Assertion script",
	"Default setting: off",
	"Arguments: on or off",
	"",
	"Description:",
	"Determines whether subsequent assertion script assertions will copy the",
	"value of the left-variable (LHS) into the right-variable (RHS) after the",
	"assertion has finished executing.",
	0};

const char *class_set_subcmd_halt_help[] = {
	"Summary: Assertion script halting mode.",
	"Usage: set halt [true|false]",
	"Example command: set halt true",
	"",
	"Scope: Assertion script",
	"Default setting: true",
	"Arguments: true or false",
	"",
	"Description:",
	"Determines if application ranks are halted when the assertion script terminates.",
	0};

const char *class_set_subcmd_logfile_help[] = {
	"Summary: Assertion script event logfile name.",
	"Usage: set logfile <filename>",
	"Example command: set logfile \"my.file\"",
	"",
	"Scope: Assertion script",
	"Default setting: null",
	"Arguments: filename (quoted string)",
	"",
	"Description:",
	"Specifies the name of the file to send assertion script event logging entries,",
	"if assertion script event logging is turned on. Note that the provided filename",
	"must be enclosed by quotes.",
	0};

const char *class_set_subcmd_logging_help[] = {
	"Summary: Assertion script event logging mode.",
	"Usage: set logging [on|off]",
	"Example command: set logging on",
	"",
	"Scope: Assertion script",
	"Default setting: off",
	"Arguments: on or off",
	"",
	"Description:",
	"Turn assertion script event logging on or off.",
	0};

const char *class_set_subcmd_output_help[] = {
	"Summary: Assertion script assertion result logfile name.",
	"Usage: set output <filename>",
	"Example command: set output \"my.file\"",
	"",
	"Scope: Assertion script",
	"Default setting: null",
	"Arguments: filename (quoted string)",
	"",
	"Description:",
	"Specify a file to which the results from assertion script assertions will be",
	"sent. Note that only difference delta information from the execution of an",
	"assertion will be written. The provided filename must be enclosed by",
	"quotes.",
	0};

const char *class_set_subcmd_reduce_help[] = {
	"Summary: Assertion script reduce mode.",
	"Usage: set reduce [sum|product|mean|hash|none]",
	"       [<lower precision>] [<upper precision>]",
	"Example command: set reduce hash",
	"",
	"Scope: Assertion script",
	"Arguments: sum or product or mean or hash or none,",
	"           lower precision (float), upper precision (float)",
	"",
	"Description:",
	"Use a data reduction function to reduce the volume of data sent from",
	"compute nodes. There are both simple and complex functions currently",
	"supported for data reduction. The simple functions are sum, product, and",
	"mean which compute the sum, product, or mean of the data structure in",
	"question.",
	"",
	"The complex functions is hash. This will compute a hash for data reduction",
	"purposes. The hash function implemented is prone to collisions, the expected",
	"rate is one collision in 2^32. When hashing is used, the two original",
	"sets of data will no longer be directly compared. Instead, two sets of",
	"hashed signatures will be compared. If a decomposition is used to compare a",
	"serial data structure to an n-ary distributed data structure, then the",
	"serial program will calculate n hashes and compare it with its",
	"associated counterpart in the n-ary distributed data structure.",
	"",
	"If floating point values are being used with a hashing function, the",
	"user can specify the precision tolerance (lower and upper) for the hash",
	"function using the optional lower precision and upper precision",
	"arguments. For example, issuing the command set reduce hash 1 2 will",
	"set the reduce function to hash with floating point lower precision and",
	"upper precision of 10^-1 and 10^-2 respectively.",
	0};
	
const char *class_set_subcmd_stop_help[] = {
	"Summary: Assertion script stopping mode.",
	"Usage: set stop [true|false]",
	"Example command: set stop true",
	"",
	"Scope: Assertion script",
	"Default setting: true",
	"Arguments: true or false",
	"",
	"Description:",
	"Determines if the assertion script should halt due to encountering assertion",
	"failures.",
	0};

const char *class_set_subcmd_p2p_help[] = {
	"Summary: Assertion script p2p comparison mode.",
	"Usage: set p2p [true|false]",
	"Example command: set p2p true",
	"",
	"Scope: Assertion script",
	"Default setting: false",
	"Arguments: true or false",
	"",
	"Description:",
	"Use the direct P2P comparison technique to compare datasets when",
	"evaluating comparitive assertion",
	0};

/*
 * Class show subcommands
 */

const char *class_show_subcmd_dfmode_help[] = {
	"Summary: Assertion script processing setting.",
	"",
	"Description:",
	"Display the current assertion script processing mode setting. When set to",
	"default, synchronous mode is used for assertions that refer to",
	"independent processes. Asynchronous mode is default for assertions that",
	"refer to the same process. The value of this setting overrides the",
	"default behavior.",
	0};

const char *class_show_subcmd_display_help[] = {
	"Summary: Assertion script assert display setting.",
	"",
	"Description:",
	"Display the current display format for the result of assertion script",
	"assertions. In text mode, any differences are displayed as plaintext. In",
	"aif mode, differences are displayed in architecture independent format",
	"AIF). In hdf and hdf5 modes, differences are displayed in the",
	"hierarchical data format (HDF) version 4 and version 5, respectively.",
	0};

const char *class_show_subcmd_force_help[] = {
	"Summary: Assertion script force mode.",
	"",
	"Description:",
	"Display the current setting of the force mode for assertion script",
	"assertions. Determines whether subsequent assertions will copy the value",
	"of the left-variable (LHS) into the right-variable (RHS) after the assertion",
	"has finished executing.",
	0};

const char *class_show_subcmd_halt_help[] = {
	"Summary: Assertion script halting mode.",
	"",
	"Description:",
	"Display the current setting of the assertion script halting mode. Determines",
	"if application ranks are halted when the assertion script terminates.",
	0};

const char *class_show_subcmd_logfile_help[] = {
	"Summary: Assertion script event logfile name.",
	"",
	"Description:",
	"Display the current name of the file to send assertion script event logging",
	"entries if assertion script event logging is turned on.",
	0};

const char *class_show_subcmd_logging_help[] = {
	"Summary: Assertion script event logging mode.",
	"",
	"Description:",
	"Display the current mode of assertion script event logging.",
	0};

const char *class_show_subcmd_output_help[] = {
	"Summary: Assertion script assertion result logfile name.",
	"",
	"Description:",
	"Display the file to which the results from assertion script assertions will be",
	"sent. Only difference delta information from the execution of an assertion",
	"will be written to this file.",
	0};

const char *class_show_subcmd_reduce_help[] = {
	"Summary: Assertion script reduce mode.",
	"",
	"Description:",
	"Display the current setting of the assertion script reduce mode. There are",
	"both simple and complex functions currently supported for data reduction.",
	"The simple functions are sum, product, and mean which compute the sum,",
	"product, or mean of the data structure in question.",
	"",
	"The complex function is hash. This will compute a hash for data reduction",
	"purposes. The hash function implemented is prone to collisions, the expected",
	"rate is one collision in 2^32. When hashing is used, the two original",
	"sets of data will no longer be directly compared. Instead, two sets of",
	"hashed signatures will be compared. If a decomposition is used to compare a",
	"serial data structure to an n-ary distributed data structure, then the",
	"serial program will calculate n hashes and compare it with its",
	"associated counterpart in the n-ary distributed data structure.",
	"",
	"If floating point values are being used with the hash function, the",
	"precision tolerance (lower and upper) for the hash function is",
	"determined by the lower precision and upper precision settings.",
	0};
	
const char *class_show_subcmd_stop_help[] = {
	"Summary: Assertion script stopping mode.",
	"",
	"Description:",
	"Display the current setting of the assertion script stopping mode. Determines",
	"if the assertion script should halt due to encountering assertion failures.",
	0};

const char *class_show_subcmd_p2p_help[] = {
	"Summary: Dataflow p2p comparison mode.",
	"",
	"Description:",
	"Enable the direct P2P comparison mode when evaluating comparative",
	"assertions.",
	0};

/*
 * Class usage subcommands
 */

const char *class_usage_subcmd_assert_help[] = {
	"Summary: Assertion script assert command.",
	"",
	"Description:",
	"Display help information regarding the use of assertion script assert",
	"commands. The assert command is only valid while building an assertion script",
	"in script mode.",
	0};

const char *class_usage_subcmd_assign_help[] = {
	"Summary: Assertion script assign command.",
	"",
	"Description:",
	"Display help information regarding the use of assertion script assign",
	"commands. The assign command is only valid while building an assertion script",
	"in script mode.",
	0};

const char *class_usage_subcmd_decomposition_help[] = {
	"Summary: Decomposition commands.",
	"",
	"Description:",
	"Display help information regarding the use of decomposition commands.",
	"The decomposition commands are only valid while defining a decomposition in",
	"script mode.",
	0};

