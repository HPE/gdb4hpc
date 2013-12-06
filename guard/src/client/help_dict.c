/******************************************************************************\
 * help_dict.c - Command help string defintion dictionary.
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
 * Regular command help definitions
 **********************************************/

const char *cmd_attach_help[] = {
        "Summary: Attach to an application.",
        "Usage: attach <app_handle> <apid> [--gpu]",
        "Example command: attach $a 9544773",
        "",
        "Arguments: app_handle (debugger variable syntax), apid (apid from apstat",
        "           of the running application)",
        "",
        "Description:",
        "Attach to an application that was previously launched via aprun.",
        "",
        "The app_handle argument is a process set handle used to refer to this",
        "application in future commands. The handle must not be array syntax. The",
        "size of the array is automatically set to the number of ranks in the aprun",
        "session.",
        "",
        "For example, app_handle should only be a scalar debugger variable such as $a.",
        "",
        "The apid can be obtained by using the apstat utility.",
        "",
        "If you wish to enable OpenACC debugging on a supported Nvidia GPU, add --gpu",
        "to your attach command. This will enable the use of an OpenACC supported",
        "version of gdb and set several environment variables for debugging.",
        0};

const char *cmd_assign_help[] = {
        "Summary: Assignment command.",
        "Usage: assign <target> <expr> [-force]",
        "Example command: assign j 20",
        "",
        "Arguments: target (target variable), expr (expression syntax), -force",
        "           (force assignment)",
        "",
        "Description:",
        "Assign the value of an expression defined by expr to target. The target",
        "must evaluate to an l-value that is either a debugger variable (such as",
        "$a) or an application variable. If the target is the name of a debugger",
        "variable that does not exist, a new debugger variable with that name is",
        "automatically created. If the optional force option is specified, then",
        "the variable assignment is forced. This is used when the type of l-value",
        "defined by expr does not match the type of the l-value of target.",
        0};

const char *cmd_backtrace_help[] = {
        "Summary: Print stack frame backtrace command.",
        "Usage: backtrace [<num>]",
        "Example command: backtrace",
        "Shortcut: bt",
        "",
        "Arguments: num (pos/neg num frames)",
        "",
        "Description:",
        "Display a backtrace of the stack frames for every application rank in the",
        "current process set. If num is supplied, the innermost num frames are",
        "displayed. If num is negative, the outermost num frames are displayed.",
        0};
        
const char *cmd_break_help[] = {
        "Summary: Set breakpoint command.",
        "Usage: break <location>",
        "Example command: break main.c:20",
        "Shortcut: b",
        "",
        "Arguments: location (line number, function name, or address)",
        "",
        "Description:",
        "Set a breakpoint in every application rank in the current process set.",
        "The location of the breakpoint can be defined as a line number, a function",
        "name, or \"*\" and an address. If an address is specified, it must begin",
        "with an asterix (*). For example, break *0x0000000000400693 would break",
        "at address 0x400693.",
        0};
        
const char *cmd_compare_help[] = {
        "Summary: Compare command.",
        "Usage: compare <expr1> = <expr2>",
        "Example command: compare $a{0}::i = $b{0}::i",
        "",
        "Arguments: expr1 (variable expression), expr2 (variable expression)",
        "",
        "Description:",
        "Compare (subtract) the value of expression expr1 with the value of",
        "expression expr2. The values must be of the same size and shape, or a",
        "translation function (such as a decomposition) must be used. The output",
        "of this command depends on the settings of display, output, and error",
        "variables. For more complex expressions, the variable part of the",
        "expression can be enclosed with quotes.",
        "",
        "For example, the sizeof operator can be used by enclosing the syntax",
        "with quotes:",
        "compare $a{0}::\"sizeof(i)\" = $b{0}::\"sizeof(i)\"",
        0};

const char *cmd_continue_help[] = {
        "Summary: Continue debugging application.",
        "Usage: continue",
        "Example command: continue",
        "Shortcut: c",
        "",
        "Arguments: None.",
        "",
        "Description:",
        "Continue execution of every application rank in the current process set",
        "from its current location.",
        0};

const char *cmd_run_help[] = {
        "Summary: start debugging application.",
        "Usage: run",
        "Example command: continue",
        "",
        "Arguments: None.",
        "",
        "Description:",
        "Start execution of every application rank in the current process set from",
        "its begining location.",
        0};

const char *cmd_defset_help[] = {
        "Summary: Define process set command.",
        "Usage: defset <name> <process set expression>",
        "Example command: defset $new_set $a{0},$a{1}",
        "",
        "Arguments: name (new process set name), process set expression (process",
        "           set expression list)",
        "",
        "Description:",
        "Create a new process set of application ranks that can be focused upon.",
        "The values of the process set expression must be valid launched/attached",
        "application names and valid ranks if a particular rank is targeted. A",
        "process set other than \"$all\" cannot contain ranks from two or more",
        "distinct app_handle's of attached/launched applications.",
        "",
        "The process set expression should consist of a comma-separated list of",
        "debugger variable array's specifying which rank's of the process set should",
        "be included in the new process set. A \"..\" can be used to denote a range",
        "of ranks.",
        "",
        "For example, if the app_handle $a{1024} was provided to launch an application,",
        "$a{0..32},$a{64} would create a process set containing ranks zero thru",
        "thirty-two and rank sixty-four of application $a.",
        0};

const char *cmd_delete_help[] = {
        "Summary: Delete breakpoint command.",
        "Usage: delete [<breakpoints>]",
        "Example command: delete 1",
        "Shortcut: d",
        "",
        "Arguments: breakpoints (list of breakpoint numbers)",
        "",
        "Description:",
        "Delete the list of breakpoints specified by the breakpoints argument in",
        "every application rank in the current process set. If no argument is",
        "supplied, all breakpoints in the current process set are deleted.",
        0};
        
const char *cmd_directmode_help[] = {
        "Summary: Direct gdb mode command.",
        "Usage: gdbmode",
        "Example command: gdbmode",
        "",
        "Arguments: None.",
        "",
        "Description:",
        "This command will put the debugger into a gdb direct passthru mode. This",
        "mode has the look and feel of an actual gdb session across multiple parallel",
        "processes. Note that command results will be aggregated, and results that",
        "are different will be denoted with rank information. If multiple process",
        "sets are in the current focus, gdbmode will only operate on the first process",
        "set in focus. The other process sets are ignored. This is the case if the",
        "process set $all is currently in focus.",
        "",
        "To exit gdbmode, type \"end\". The \"quit\" command will cause the debugger",
        "to exit and behave in the same way it does outside of gdbmode.",
        0};

const char *cmd_disable_help[] = {
        "Summary: Disable breakpoint command.",
        "Usage: disable [<breakpoints>]",
        "Example command: disable 1",
        "",
        "Arguments: breakpoints (list of breakpoint numbers)",
        "",
        "Description:",
        "Disable the list of breakpoints specified by the breakpoints argument in",
        "every application rank in the current process set. If no argument is",
        "supplied, all breakpoints in the current process set are disabled.",
        0};

const char *cmd_down_help[] = {
        "Summary: Move stack frame down command.",
        "Usage: down [<num>]",
        "Example command: down",
        "",
        "Arguments: num (number of frames to go down)",
        "",
        "Description:",
        "Move the call stack down by num frames. If no argument is supplied, then",
        "move down by one frame.",
        0};

const char *cmd_enable_help[] = {
        "Summary: Enable breakpoint command.",
        "Usage: enable [-delete|-once] [<breakpoints>]",
        "Example command: enable 1",
        "",
        "Arguments: breakpoints (list of breakpoint numbers)",
        "",
        "Description:",
        "Enable the list of breakpoints specified by the breakpoints argument in",
        "every application rank of the current process set. If no argument is",
        "supplied, all breakpoints in the current process set are enabled.",
        "This command is used to cancel the effect of the \"disable\" command.",
        "Valid option arguments to this command are -delete and -once. Only one",
        "argument can be provided. The -delete argument will cause the breakpoint",
        "to be deleted when it is hit. The -once argument will cause the breakpoint",
        "to be disabled when it is hit.",
        0};
        
const char *cmd_finish_help[] = {
        "Summary: Finish function execution command.",
        "Usage: finish",
        "Example command: finish",
        "",
        "Arguments: None.",
        "",
        "Description:",
        "Execute every rank in the current process set from its current location",
        "until the selected stack frame returns.",
        0};

const char *cmd_focus_help[] = {
        "Summary: Focus process set command.",
        "Usage: focus <proc_set>",
        "Example command: focus $all",
        "",
        "Arguments: proc_set (named process set)",
        "",
        "Description:",
        "Change the focus of the current process set to the named process set",
        "specified by the argument proc_set. This can be the app_handle of an",
        "attached/launched application or a process set created with the defset",
        "command.",
        0};
        
const char *cmd_frame_help[] = {
        "Summary: Print selected stack frame command.",
        "Usage: frame",
        "Example command: frame",
        "",
        "Arguments: None.",
        "",
        "Description:",
        "Display the selected stack frame from every application rank in the current",
        "process set.",
        0};

const char *cmd_halt_help[] = {
        "Summary: Halt all executing processes.",
        "Usage: halt",
        "Example command: halt",
        "",
        "Arguments: None.",
        "",
        "Description:",
        "Halt all executing ranks in the current process set. This is equivalent",
        "to sending a SIGINT to the ranks.",
        0};
        
const char *cmd_help_help[] = {
        "Summary: Help command.",
        "Usage: help [<command>]",
        "Example command: help",
        "Shortcut: h",
        "",
        "Arguments: command (command specific message)",
        "",
        "Description:",
        "Display help information about commands. If no argument is given,",
        "display a list of all valid commands.",
        0};

const char *cmd_invoke_help[] = {
        "Summary: Launch an application.",
        "Usage: launch <app_handle> <application>",
        "              [--args=\"<args>\" OR -a \"<args>\"]",
        "              [--aprun-args=\"<aprun_args>\" OR -g \"<aprun_args>\"]",
        "              [--aprun-input=\"<path_to_file>\" OR -i \"<path_to_file>\"]",
        "              [--workdir=\"<path>\" OR -d\"<path>\"]",
        "              [--env=\"<name=value>\"]",
        "              [--qsub=\"<batch_template>\" OR -q\"<batch_template>]\"",
        "              [--sbatch=\"<batch_template>\" OR -s\"<batch_template>]\"",
        "              [--gpu]",
        "",
        "Example command: launch $a ./helloworld",
        "",
        "Arguments: app_handle (debugger variable syntax), application",
        "           (name of application), args (application arguments), aprun_args",
        "           (aprun arguments), path_to_file (aprun stdin file),",
        "           path (cwd path), name=value (environment string),",
        "           batch_template (job script)",
        "",
        "Description:",
        "Start an application with process set handle of app_handle and optional",
        "application arguments args, aprun arguments aprun_args, aprun input file",
        "path_to_file, and aprun environment string name=value. Integration with",
        "qsub and sbatch is also supported. See below for more information on use.",
        "", 
        "For sequential programs, app_handle is a single scalar debugger variable.",
        "For parallel programs, app_handle must be a list of one or more debugger", 
        "variable arrays or scalar debugger variables. Application ranks are assigned", 
        "to each debugger variable in the app_handle list sequentially, starting from",
        "the left.",
        "",
        "The summation of all debugger variables, including the rank of any array,",
        "is used to determine the number of application ranks that will be launched.",
        "",
        "For example:",
        "An application with a app_handle of $a{9} will launch the applicaiton via",
        "aprun with 9 application ranks (-n9). An application with a app_handle of",
        "$a,$b{8} will launch the application via aprun with 9 application ranks (-n9)",
        "that are separated into two process sets. Process set $a will contain one",
        "application rank, and process set $b will contain eight application ranks.",
        "",
        "If you wish to enable OpenACC debugging on a supported Nvidia GPU, add --gpu",
        "to your launch command. This will enable the use of an OpenACC supported",
        "version of gdb and set several environment variables for debugging.",
        "",
        "The optional args, aprun_args, path_to_file, path, and name=value should be",
        "enclosed with quotes. The args argument is used to pass arguments directly to",
        "the application. The aprun_args argument is used to pass additional arguments",
        "to aprun when launching the application. The path_to_file argument is used",
        "to redirect the stdin of aprun to a file. This is useful when your",
        "application requires input from stdin in order to set itself up. The path",
        "argument is used to change the current working directory to a different",
        "location relative to its present setting where the debugger was started.",
        "This is useful when the application writes unique files to the CWD.",
        "If path is omitted to --workdir, then the current working directory will",
        "be changed to the location where the application a.out is located. The",
        "name=value argument is used to set environment variables specific to the",
        "aprun session instance. Note that the --env argument can be used more than",
        "once to set multiple environment variables.",
        "",
        "For example:",
        "To provide the aprun arguments -N1 -L134 you would use",
        "--aprun-args=\"-N1 -L134\"",
        "Alternatively you can use the short option of ",
        "-g \"-N1 -L134\"",
        "",
        "Note that if using the short option -d, the argument is optional and a space",
        "should not be placed between the option and the argument (i.e. use -d\"<path>\").",
        "",
        "Workload manager support:",
        "",
        "To launch inside of a job reservation, the --qsub or --sbatch arguments",
        "can be used. Modify the existing job script for the application by placing",
        "the comments #cray_debug_start and #cray_debug_end around the aprun command",
        "to be debugged.",
        "",
        "For example, we would modify an existing job script named sample.pbs:",
        "#cray_debug_start",
        "aprun -n128 -N32 a.out",
        "#cray_debug_end",
        "",
        "Everything between the comment lines will be ignored. The user is still required",
        "to recreate the aprun command using the existing launch command arguments.",
        "",
        "Using the above, the user would launch with the following arguments:",
        "launch $a{128} --aprun-args=\"-N32\" --qsub=sample.pbs a.out",
        "",
        "If the launch command is interrupted before completion, the user must manually",
        "delete the job reservation from the queueing system.",
        0};

const char *cmd_kill_help[] = {
        "Summary: Kill attached/launched application.",
        "Usage: kill <app_handle>",
        "Example command: kill %a",
        "",
        "Arguments: app_handle (process set handle)",
        "",
        "Description:",
        "Kill previously attached/launched application specified by app_handle.",
        0};

const char *cmd_list_help[] = {
        "Summary: List specified function or line command.",
        "Usage: list [-] [<expr>]",
        "Example command: list 20",
        "Shortcut: l",
        "",
        "Arguments: - (backward direction), expr (list expression syntax)",
        "",
        "List expression syntax",
        "",
        "The list expression can contain an argument of the following forms:",
        "",
        "        1. linenum - display source lines around specified line number.",
        "        2. file:linenum - display source lines around specified line number",
        "                          in specified source file.",
        "        3. function - display source lines around specified function.",
        "        4. file:function - display source lines around specified function in",
        "                           specified source file.",
        "        5. *addr - display source lines around the line corresponding to",
        "                   address addr.",
        "",
        "Additionally, a comma separated list of two arguments can be defined in",
        "expr to display all source lines between arg1 and arg2.",
        "",
        "Description:",
        "Display source code of the ranks in the current process set. With no",
        "argument, ten lines of code are displayed from the previous listing.",
        "If - is specified, ten previous lines of code are displayed.",
        0};

const char *cmd_next_help[] = {
        "Summary: Next command.",
        "Usage: next [<num>]",
        "Example command: next",
        "Shortcut: n",
        "",
        "Arguments: num (number of lines to advance)",
        "",
        "Description:",
        "Step program, but do not follow into subroutine calls. If the argument",
        "num is supplied, advance num lines.",
        0};

const char *cmd_print_help[] = {
        "Summary: Print the value of an expression.",
        "Usage: print <expr>",
        "Example command: print prog_variable",
        "Shortcut: p",
        "",
        "Arguments: expr (expression to print)",
        "",
        "Description:",
        "Print the value of expr for each application rank in the current process",
        "set. The expr can contain any combination of debugger convenience or",
        "application variables. Valid application variables are those of the",
        "lexical environment of each rank's selected stack frame, plus all those",
        "whose scope is global or over an entire file.",
        0};

const char *cmd_quit_help[] = {
        "Summary: Quit command.",
        "Usage: quit",
        "Example command: quit",
        "Shortcut: q",
        "",
        "Arguments: None.",
        "",
        "Description:",
        "Quit the debugger.",
        0};

const char *cmd_release_help[] = {
        "Summary: Release attached/launched application from debugger control.",
        "Usage: release <app_handle>",
        "Example command: release $a",
        "",
        "Arguments: app_handle (process set handle)",
        "",
        "Description:",
        "Release previously attached/launched application specified by app_handle.",
        0};

const char *cmd_session_help[] = {
        "Summary: Create a new WLM session.",
        "Usage: session [--qsub=\"<batch_template>\" OR -q\"<batch_template>\"]",
        "               [--sbatch=\"<batch_template>\" OR -s\"<batch_template>\"]",
        "               [--end OR -e]",
        "",
        "Example command: session --qsub=\"job_script.pbs\"",
        "",
        "Arguments: batch_template (job script)",
        "",
        "Description:",
        "Create a new WLM session to associate all future launch/attach commands",
        "to. This allows multiple launch commands to be used inside of the same",
        "WLM batch session. The qsub and sbatch arguments are the same as what is",
        "used for the launch command. If multiple launches are used with the same",
        "session, the user must ensure enough resources are requested to cover all",
        "aprun commands issued by launch.",
        "",
        "Modify the existing job script for the application by placing the comments",
        "#cray_debug_start and #cray_debug_end around the aprun command to be debugged.",
        "",
        "For example, we would modify an existing job script named sample.pbs:",
        "#cray_debug_start",
        "aprun -n128 -N32 a.out",
        "#cray_debug_end",
        "",
        "Everything between the comment lines will be ignored. The user is still required",
        "to recreate the aprun command using the existing launch command arguments.",
        "",
        "If the session command is interrupted before completion, the user must manually",
        "delete the job reservation from the queueing system.",
        0};

const char *cmd_source_help[] = {
        "Summary: Read and execute debugger commands.",
        "Usage: source <filename>",
        "Example command: source myfile",
        "",
        "Arguments: filename (path to named command file)",
        "",
        "Description:",
        "Read debugger commands from a file to automate debugging tasks.",
        0};

const char *cmd_step_help[] = {
        "Summary: Step command.",
        "Usage: step [<num>]",
        "Example command: step",
        "Shortcut: s",
        "",
        "Arguments: num (number of lines to step)",
        "",
        "Description:",
        "Advance the program to the next source line, following into subroutine",
        "calls. If the argument num is supplied, then advance num lines.",
        0};
        
const char *cmd_tbreak_help[] = {
        "Summary: Set temporary breakpoint command.",
        "Usage: tbreak <location>",
        "Example command: tbreak main.c:20",
        "",
        "Arguments: location (line number, function name, or address)",
        "",
        "Description:",
        "Set a temporary breakpoint in every application rank in the current process set.",
        "The breakpoint will be deleted after being hit. This is equivalent to issuing a",
        "\"break\" followed by \"enable -delete\" with the breakpoint number.",
        "",
        "The location of the breakpoint can be defined as a line number, a function",
        "name, or \"*\" and an address. If an address is specified, it must begin",
        "with an asterisk (*). For example, break *0x0000000000400693 would break",
        "at address 0x400693.",
        0};

const char *cmd_up_help[] = {
        "Summary: Move stack frame up command.",
        "Usage: up [<num>]",
        "Example command: up",
        "",
        "Arguments: num (number of frames to move up)",
        "",
        "Description:",
        "Move the call stack up by num frames. If no argument is supplied, then",
        "move up by one frame.",
        0};

const char *cmd_viewset_help[] = {
        "Summary: View process set command.",
        "Usage: viewset [<proc_set>]",
        "Example command: viewset",
        "",
        "Arguments: proc_set (named process set)",
        "",
        "Description:",
        "Display information about defined process sets. With no argument",
        "provided, information about all defined process sets is displayed.",
        0};
        
const char *cmd_watch_help[] = {
        "Summary: Set watchpoint command.",
        "Usage: watch [-a|-w] <expression>",
        "Example command: watch -a foo(1,1)",
        "",
        "Arguments: expression (variable expression)",
        "",
        "Description:",
        "Set a watchpoint in every application rank in the current process set. The",
        "expression of the watchpoint can be defined as a variable, an address,",
        "a variable operation (such as a cast), and other arbitrarily complex",
        "expressions using any operators valid in the program's native language.",
        "",
        "Valid option arguments to this command are -a and -w. The -a option",
        "specifies that an access watchpoint should be created. Access watchpoints",
        "will cause the program to break when the address defined by the expression",
        "is read or written to. The -w option specifies that a write watchpoint",
        "should be created. Write watchpoints will cause the program to break",
        "when the address defined by the expression is written to. Currently there",
        "is no support for read watchpoints. If no argument is provided the",
        "watchpoint will default to a write watchpoint.",
        "",
        "If the expression corresponds to a single address, a hardware watchpoint",
        "will be created. If the expression corresponds to a range of addresses a",
        "software watchpoint will be created. Software watchpoints cause application",
        "performance to slowdown by orders of magnitude and should be avoided, if",
        "possible.",
        "",
        "If the variable defined by the expression goes out of scope, the watchpoint",
        "will cause the application to break and the watchpoint will be removed.",
        0};

const char *cmd_whatis_help[] = {
        "Summary: Whatis data type of expression.",
        "Usage: whatis [-type] <expr>",
        "Example command: whatis prog_var",
        "",
        "Arguments: expr (expression syntax)",
        "",
        "The expr argument can be the name of a program variable in a launched",
        "application or a type name defined by typedef, or \"struct STRUCT-TAG,\"",
        "or \"union UNION-TAG,\" or \"enum ENUM-TAG.\"",
        "",
        "Description:",
        "Display the data type of an expression for each application rank in the",
        "current process set. By default, only the base type is displayed. If the",
        "-type argument is provided, the expr is interpreted as being a defined",
        "type name, struct, union, or enum. A type is always displayed as a full",
        "type definition.",
        0};

/**********************************************
 * Command class help definitions
 **********************************************/

const char *class_info_help[] = {
        "Summary: Info command.",
        "Usage: info [<subcommand>]",
        "Example command: info",
        "",
        "Arguments: subcommand (info subcommand)",
        "",
        "Description:",
        "Display information relating to the application being debugged. If no",
        "argument is given, display a list of all valid info subcommands.",
        0};
        
const char *class_maint_help[] = {
        "Summary: Maint command.",
        "Usage: maint [<subcommand>]",
        "Example command: maint",
        "",
        "Arguments: subcommand (maint subcommand)",
        "",
        "Description:",
        "Commands that are useful only to maintainers or are experimental.",
        "If no argument is given, display a list of all valid maint subcommands.",
        0};
        
const char *class_maint_set_help[] = {
        "Summary: Maint set command.",
        "Usage: maint set [<subcommand>]",
        "Example command: maint set",
        "",
        "Arguments: subcommand (maint set subcommand)",
        "",
        "Description:",
        "Set information relating to environment settings and behavior that",
        "is useful only to maintainers or is experimental. If no argument is",
        "given, display a list of all valid maint set subcommands.",
        0};
        
const char *class_maint_show_help[] = {
        "Summary: Maint show command.",
        "Usage: maint show [<subcommand>]",
        "Example command: maint show",
        "",
        "Arguments: subcommand (maint show subcommand)",
        "",
        "Description:",
        "Show information relating to environment settings and behavior that",
        "is useful only to maintainers or is experimental. If no argument is", 
        "given, display a list of all valid maint show subcommands.",
        0};

const char *class_set_help[] = {
        "Summary: Set command.",
        "Usage: set [<subcommand>]",
        "Example command: set",
        "",
        "Arguments: subcommand (set subcommand)",
        "",
        "Description:",
        "Set information relating to environment settings and behavior. If",
        "no argument is given, display a list of all valid set subcommands.",
        0};

const char *class_show_help[] = {
        "Summary: Show command.",
        "Usage: show [<subcommand>]",
        "Example command: show",
        "",
        "Arguments: subcommand (show subcommand)",
        "",
        "Description:",
        "Show information relating to environment settings and behavior. If",
        "no argument is given, display a list of all valid show subcommands.",
        0};
        
const char *class_unset_help[] = {
        "Summary: Unset command.",
        "Usage: unset [<subcommand>]",
        "Example command: unset",
        "",
        "Arguments: subcommand (unset subcommand)",
        "",
        "Description:",
        "Unset information relating to environment settings and behavior that was",
        "previously set. If no argument is given, display a list of all valid set",
        "subcommands.",
        0};
        
const char *class_usage_help[] = {
        "Summary: Usage command.",
        "Usage: usage [<subcommand>]",
        "Example command: usage",
        "",
        "Arguments: subcommand (usage subcommand)",
        "",
        "Description:",
        "Display information relating to script mode commands and their use. If",
        "no argument is given, display a list of all valid usage subcommands.",
        0};

/**********************************************
 * Command class sub-command help definitions
 **********************************************/

/*
 * Class info subcommands
 */

const char *class_info_subcmd_breakpoints_help[] = {
        "Summary: Breakpoint information.",
        "",
        "Description:",
        "Display the status of user defined breakpoints.",
        "",
        "The type column indicates one of the following:",
        "",
        "        breakpoint - normal breakpoint",
        "        watchpoint - watchpoint",
        "",
        "The disp column contains keep, del, or dis to indicate the disposition",
        "of the breakpoint after it is hit. If disp is displayed as dis, this",
        "means that the breakpoint will be disabled. The address and what columns",
        "indicate the address and file/line number of the breakpoint",
        "respectively.",
        0};

const char *class_info_subcmd_locals_help[] = {
        "Summary: Local symbol information.",
        "",
        "Description:",
        "Display a list of the local symbols in the current stack frame for each",
        "application rank in the current process set.",
        0};

const char *class_info_subcmd_application_help[] = {
        "Summary: Application information.",
        "",
        "Description:",
        "Display information related to currently controlled applications. The handle",
        "column specifies the process set handle used to identify the launched/attached",
        "application. The ranks column indicates the number of application ranks.",
        "The focus column indicates if this application's process set is currently",
        "focused upon. The command column identifies the command that was passed",
        "to aprun in order to launch the application.",
        0};
        
const char *class_info_subcmd_scope_help[] = {
        "Summary: Local symbol information relative to a scope.",
        "Usage: info scope <location>",
        "Example command: info scope source.f:20",
        "",
        "Arguments: location (function, line, or *addr)",
        "",
        "Description:",
        "Display a list of the local symbols relative to a scope for the application.",
        "The provided argument can either be a function name, a line number in a source",
        "file, or an address prefixed with *.",
        0};
        
const char *class_info_subcmd_sources_help[] = {
        "Summary: Source file information.",
        "",
        "Description:",
        "Display the source files in the application. This command will only display",
        "the source files that have been located and are available to be used in conjunction",
        "with the list command. Try \"set directory\" to improve the results.",
        0};

const char *class_info_subcmd_status_help[] = {
        "Summary: Status command.",
        "",
        "Description:",
        "Display the status of lauched/attached applications.",
        0};

/*
 * Class maint subcommands
 */

const char *class_maint_subcmd_sleep_help[] = {
        "Summary: Sleep command.",
        "Usage: maint sleep [<secs>]",
        "Example command: maint sleep 5",
        "",
        "Arguments: secs (seconds to sleep)",
        "",
        "Description:",
        "Pause the debugger for secs seconds. If secs is omitted, the debugger",
        "will sleep for a default of five seconds.",
        0};

const char *class_maint_subcmd_direct_help[] = {
        "Summary: Passthru command.",
        "Usage: maint passthru <command>",
        "Example command: maint passthru show version",
        "",
        "Arguments: command (gdb command)",
        "",
        "Description:",
        "Pass a command directly to the debug backends of every application rank in",
        "the current process set. This can be used to talk directly to gdb. Console",
        "output will be aggregated and returned.",
        0};
        
const char *class_maint_subcmd_system_help[] = {
        "Summary: System command.",
        "Usage: maint system <command>",
        "Example command: maint system ls -la",
        "",
        "Arguments: command (shell command)",
        "",
        "Description:",
        "Run a command in the shell envrionment of the client. This can be used",
        "to enumerate information about the system the client is running on. The",
        "stderr of the command will be redirected to stdout and printed in the result.",
        0};
        
/*
 * Class set subcommands
 */

const char *class_set_subcmd_directory_help[] = {
        "Summary: Directory search source path.",
        "Usage: set directory <path>",
        "Example command: set directory /path/to/source",
        "",
        "Arguments: path (directory containing source)",
        "",
        "Description:",
        "Add the directory defined by path to the beginning of the search path",
        "for the application source files that were used to generate symbol",
        "information for application. Typically this is used when an application",
        "was compiled on a different machine than run, but the source files used",
        "to compile the application are available at a new location. This will",
        "cause cached information about source file locations and line positions",
        "to be lost.",
        0};
        
const char *class_set_subcmd_error_help[] = {
        "Summary: Warn and error epsilon for floating point comparison.",
        "Usage: set error <warn limit> <error limit>",
        "Example command: set error 1e-16 2e-15",
        "",
        "Default setting: ulp ulp",
        "                 Where ulp is the smallest non-zero double precision floating",
        "                 point value of the current machine.",
        "Arguments: warn limit (float), error limit (float)",
        "",
        "Description:",
        "Set the tolerance level for compare/assert commands difference deltas when",
        "comparing floating point values. Differences with deltas below the warning",
        "epsilon are ignored. Differences with deltas between the warning and error",
        "epsilon will be displayed using the current display mode setting and reported",
        "as a warning. Any differences with deltas above the error epsilon will be",
        "displayed using the current display mode setting and reported as an error.",
        "Error differences will halt the assertion script execution if stop mode is true.",
        0};
        
const char *class_set_subcmd_environment_help[] = {
        "Summary: Set environment variable value.",
        "Usage: set environment <name> <value>",
        "Example command: set environment var val",
        "",
        "Arguments: name (environment variable name), value (value to assign to",
        "           environment variable)",
        "",
        "Description:",
        "Set the environment variable named name to the value provided by the value",
        "argument. Arguments can optionally be enclosed with quotes, the actual quotes",
        "will be stripped out for the final assignment. This will not affect any",
        "previously launched applications and will only take effect upon the next",
        "launch command. All environment variables passed this way are put into the",
        "forked environment of the aprun process.",
        0};

const char *class_set_subcmd_function_help[] = {
        "Summary: Set function definition.",
        "Usage: set function <function_name>(<function_args>) = <expr>",
        "Example command: set function myfunc(myvar) = 1",
        "",
        "Arguments: function_name (name of function), function_args (function",
        "           arguments), expr (function expression definition)",
        "",
        "Description:",
        "Define a function macro named function_name. The expression expr can",
        "contain format arguments that are substituted by actual arguments",
        "supplied to the function. Recursive function expression calls are not",
        "allowed.",
        0};

/*
 * Class show subcommands
 */

const char *class_show_subcmd_directories_help[] = {
        "Summary: Display source search path.",
        "",
        "Description:",
        "Display the current search path for locating source files. A $cwd in the",
        "path means the current working directory. A $cdir in the path means the",
        "compilation directory of the source file.",
        0};

const char *class_show_subcmd_error_help[] = {
		"Summary: Warn and error epsilon for floating point comparison."
        "",
        "Description:",
        "Display the current tolerance level setting for compare/assert command",
        "difference deltas when comparing floating point values.",
        0};

const char *class_show_subcmd_function_help[] = {
        "Summary: Function definition information.",
        "",
        "Description:",
        "Display a list of the currently defined function macros.",
        0};
        
const char *class_show_subcmd_environment_help[] = {
        "Summary: Environment variable information.",
        "",
        "Description:",
        "Display the environment variables that were set by calling the set environment",
        "command.",
        0};

const char *class_show_subcmd_version_help[] = {
        "Summary: Version information.",
        "",
        "Description:",
        "Display the debugger version number.",
        0};
        
/*
 * Class unset subcommands
 */
const char *class_unset_subcmd_environment_help[] = {
        "Summary: Unset environment variable value.",
        "Usage: unset environment <name>",
        "Example command: unset environment var",
        "",
        "Arguments: name (environment variable name)",
        "",
        "Description:",
        "Unset the environment variable named name from the environment. This can only",
        "be used to unset environment variables that were set using the set environment",
        "command. This will not affect any previously launched applications and will",
        "only take effect upon the next launch command.",
        0};

/*
 * Class usage subcommands
 */



