/****************************************************************************** \
 * help_dict.c - Command help string definition dictionary.
 *
 * Copyright 2011-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#include "help_dict.h"

/**********************************************
 * Regular command help definitions
 **********************************************/

const char* const cmd_attach_help[] = {
    "Summary: Attach to an application.",
    "Usage: attach <app_handle> <app_ident> [--gpu]",
    "Example command: attach $a 9544773",
    "",
    "Arguments: app_handle (debugger variable syntax), app_ident (wlm specific",
    "           identifier of the running application)",
    "",
    "Description:",
    "Attach to an application that was previously launched outside of the "
    "debugger.",
    "",
    "The app_handle argument is a process set handle used to refer to this",
    "application in future commands. The handle must not be array syntax. The",
    "size of the array is automatically set to the number of ranks in the",
    "application.",
    "",
    "For example, app_handle should only be a scalar debugger variable such as "
    "$a.",
    "",
    "The apid can be obtained by using the apstat utility. For slurm, the "
    "job_id",
    "can be optained from squeue and the step_id can be obtained from sstat. "
    "The",
    "slurm app_ident should be provided as <job_id>.<step_id> on Cray systems.",
    "",
    "For Cray clusters, app_ident should be the pid of the launcher process "
    "that",
    "started the application. This can be obtained using ps.",
    "",
    "If you wish to enable GPU debugging, add --gpu to your attach command.",
    "This will enable the use of a GPU supported version of gdb and set "
    "several",
    "environment variables for debugging.",
    0};

const char* const cmd_assign_help[] = {
    "Summary: Assignment command.",
    "Usage: assign <target> <expr> [-force]",
    "Example command: assign j 20",
    "",
    "Arguments: target (expression syntax), expr (expression syntax), -force",
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
    "",
    "The target can include a list of application ranks such as $a{1..3,5,10}.",
    "The assignment will occur as long as the expr evaluates into a single "
    "scalar,",
    "or the number of ranks in the expr result match the number of ranks in "
    "the",
    "target.",
    "",
    "For example, if the value 10 is to be assigned myVar in ranks 1,3,4,5,10",
    "of app_handle $a, the following assign command would be issued:",
    "",
    "assign $a{1,3..5,10}::myVar 10",
    0};

const char* const cmd_backtrace_help[] = {
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

const char* const cmd_break_help[] = {
    "Summary: Set breakpoint command.",
    "Usage: break <location>",
    "Usage: break <location> thread <thread-id>",
    "Example command: break main.c:20",
    "Example command: break main.c:20 thread 2",
    "Shortcut: b",
    "",
    "Arguments: location (line number, function name, or address)",
    "",
    "Description:",
    "Set a breakpoint in every application rank in the current process set.",
    "The location of the breakpoint can be defined as a line number, a ",
    "function name, a source file:line-no, or \"*\" and an address. If an address is",
    "specified, it must begin with an asterix (*)."
    "For example, break *0x0000000000400693 would break at address 0x400693.",
    "Adding thread thread-id creates a thread specific breakpoint which will",
    "apply only to the given thread.",
    0};

const char* const cmd_compare_help[] = {
    "Summary: Compare command.",
    "Usage: compare <expr1> = <expr2>",
    "Example command: compare $a{0,3..5,10}::i == $b{0,3..5,10}::i",
    "",
    "Arguments: expr1 (variable expression), expr2 (variable expression)",
    "",
    "Description:",
    "Compare (subtract) the value of expression expr1 with the value of",
    "expression expr2. The values must be of the same size and shape, or a",
    "translation function (such as a decomposition) must be used. For example,",
    "if the command compare $a::myVar == $b::myVar is issued and application "
    "$a",
    "has 32 ranks and application $b has 64 ranks, the comparison is only "
    "valid",
    "if myVar has the same value in every rank of both $a and $b. If myVar has",
    "unique values in different ranks, the size and shape of the results do "
    "not",
    "match and the comparison will fail.",
    "",
    "The output of this command depends on the settings of display, output, "
    "and",
    "error variables. For more complex expressions, the application variable "
    "part",
    "of the expression can be enclosed with quotes.",
    "",
    "For example, the sizeof operator can be used by enclosing the syntax",
    "with quotes:",
    "",
    "compare $a{0}::\"sizeof(i)\" = $b{0}::\"sizeof(i)\"",
    "",
    "The app_handle can be subsets of ranks using the process set notation. "
    "When",
    "a subset of ranks is targeted, individual ranks are separated with a "
    "comma",
    "and a range of ranks is denoted with \"..\". For example, if we wish to",
    "compare ranks 1,3,4,5,10 in $a, we would provide an app_handle of",
    "$a{1,3..5,10}.",
    0};

const char* const cmd_continue_help[] = {
    "Summary: Continue debugging application.",
    "Usage: continue [-a]",
    "Example command: continue",
    "Shortcut: c",
    "",
    "Arguments: optional -a for \"all\".",
    "",
    "Description:",
    "Continue execution of every application rank in the current process set",
    "from its current location.",
    "In non-stop mode, continue only applies to the current thread, use",
    "continue -a to continue execution on all threads.",
    0};

const char* const cmd_run_help[] = {
    "Summary: start debugging application.",
    "Usage: run",
    "Example command: continue",
    "",
    "Arguments: None.",
    "",
    "Description:",
    "Start execution of every application rank in the current process set from",
    "its beginning location.",
    0};

const char* const cmd_defset_help[] = {
    "Summary: Define process set command.",
    "Usage: defset <name> <process set expression>",
    "Example command: defset $new_set $a{0,1}",
    "",
    "Arguments: name (new process set name), process set expression (process",
    "           set expression list)",
    "",
    "Description:",
    "Create a new process set of application ranks that can be focused upon.",
    "The values of the process set expression must be valid launched/attached",
    "application names and valid ranks if a particular rank is targeted. A",
    "defined process set can contain ranks from two or more applications.",
    "",
    "The process set expression should consist of a comma-separated list of",
    "debugger variable arrays specifying which ranks of the process set should",
    "be included in the new process set. A \"..\" can be used to denote a "
    "range",
    "of ranks.",
    "",
    "For example, if the app_handle $a{1024} was provided to launch an "
    "application,",
    "$a{0..32,64} would create a process set containing ranks zero thru "
    "thirty-two",
    "and rank sixty-four of application $a. If the app_handle $b{2048} was "
    "provided",
    "to launch an application, $a{32},$b{32} would create a process set "
    "containing",
    "rank 32 of both applications.",
    0};

const char* const cmd_delete_help[] = {
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

const char* const cmd_directmode_help[] = {
    "Summary: Direct gdb mode command.",
    "Usage: gdbmode",
    "Example command: gdbmode",
    "",
    "Arguments: None.",
    "",
    "Description:",
    "This command puts the debugger into a gdb direct passthru mode. This mode "
    "has",
    "the look and feel of an actual gdb session across multiple parallel "
    "processes.",
    "Command results will be aggregated, and results that are different will "
    "be",
    "denoted with rank information.",
    "",
    "To exit gdbmode, type \"end\". The \"quit\" command will cause the "
    "debugger",
    "to exit and behave in the same way it does outside of gdbmode.",
    0};

const char* const cmd_disable_help[] = {
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

const char* const cmd_down_help[] = {
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

const char* const cmd_enable_help[] = {
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
    "to be deleted when it is hit. The -once argument will cause the "
    "breakpoint",
    "to be disabled when it is hit.",
    0};

const char* const cmd_finish_help[] = {
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

const char* const cmd_focus_help[] = {
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
    "command. A temporary proc_set will be created if the user focuses "
    "explicitly",
    "on a subset of ranks using the process set notation. In that case, the",
    "temporary proc_set will be removed when the focus is changed a second "
    "time.",
    "",
    "For example, if the app_handle $a{1024} was provided to launch an "
    "application,",
    "focus $a{0..32,64} would create a temporary process set named a_temp "
    "containing",
    "ranks zero thru thirty-two and rank sixty-four of application $a. The "
    "temporary",
    "process set name cannot be subsequently used in process targeted commands "
    "in",
    "the same way a defined process set can.",
    0};

const char* const cmd_frame_help[] = {
    "Summary: Print selected stack frame command.",
    "Usage: frame",
    "Example command: frame",
    "",
    "Arguments: None.",
    "",
    "Description:",
    "Display the selected stack frame from every application rank in the "
    "current",
    "process set.",
    0};

const char* const cmd_halt_help[] = {
    "Summary: Halt all executing processes.",
    "Usage: halt [-a]",
    "Example command: halt",
    "",
    "Arguments: None.",
    "",
    "Description:",
    "Halt all executing ranks in the current process set. This is equivalent",
    "to sending a SIGINT to the ranks.",
    "In non-stop mode only the current thread is halted; use halt -a to stop",
    "all threads.",
    0};

const char* const cmd_help_help[] = {
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

const char* const cmd_invoke_help[] = {
    "Summary: Launch an application.",
    "Usage: launch <app_handle> <application>",
    "              [--args=\"<args>\" OR -a \"<args>\"]",
    "              [--launcher=\"<launcher_name>\" OR -l \"<launcher_name>\"]",
    "              [--launcher-args=\"<launcher_args>\" OR -g "
    "\"<launcher_args>\"]",
    "              [--launcher-input=<path_to_file> OR -i <path_to_file>]",
    "              [--workdir=<path> OR -d<path>]",
    "              [--env=\"<name=value>\"]",
    "              [--qsub=<batch_template> OR -q <batch_template>]",
    "              [--sbatch=<batch_template> OR -s <batch_template>]",
    "              [--gpu]",
    "",
    "Example command: launch $a{20000} ./helloworld",
    "",
    "Arguments: app_handle (debugger variable syntax),",
    "           application (name of application), args (application "
    "arguments),",
    "           launcher_args (application launcher arguments),",
    "           path_to_file (application launcher stdin file), path (cwd "
    "path),",
    "           name=value (environment string), batch_template (job script)",
    "",
    "Description:",
    "Start an application with process set handle of app_handle and optional",
    "application arguments args, application launcher arguments launcher_args,",
    "application launcher input file path_to_file, and application environment",
    "string name=value via the system application launcher. Integration with",
    "qsub and sbatch is supported. See below for more information on use.",
    "",
    "The app_handle argument must be a debugger variable array specifying the",
    "number of ranks in the application. For example, an application with an",
    "app_handle of $a{9} will launch the appication with 9 application ranks.",
    "The application launcher is system dependent and is automatically "
    "determined.",
    "The aprun and srun application launchers are supported.",
    "",
    "If you wish to enable GPU debugging, add --gpu to your launch command.",
    "This will enable the use of a GPU supported version of gdb and set "
    "several",
    "environment variables for debugging.",
    "",
    "The optional args, launcher_args, and name=value arguments should be "
    "enclosed",
    "with quotes. The args argument is used to pass arguments directly to the",
    "application. The launcher_args argument is used to pass additional "
    "arguments",
    "specific to the application launcher (either aprun or srun). The "
    "path_to_file",
    "argument is used to redirect the stdin of the launcher process to a file. "
    "The path",
    "argument is used to change the current working directory of the "
    "application",
    "launcher. The path can be a full path or relative path. If path is "
    "omitted,",
    "then the current working directory of the application launcher will be "
    "changed",
    "to the location where the application a.out is located. The name=value "
    "argument",
    "is used to set environment variables for the application launcher. The "
    "--env",
    "argument can be used more than once to set multiple environment "
    "variables.",
    "",
    "For example:",
    "To provide the aprun arguments -N1 -L134 you would use",
    "--launcher-args=\"-N1 -L134\"",
    "Alternatively you can use the short option of ",
    "-g \"-N1 -L134\"",
    "",
    "Note that if using the short option -d, the argument is optional and a "
    "space",
    "should not be placed between the option and the argument (i.e. use "
    "-d<path>).",
    "",
    "Workload manager support:",
    "",
    "Launching on systems whose default workload manager is not SLURM or ALPS "
    "requires",
    "passwordless (public key) ssh to the compute nodes of active jobs. "
    "Contact a system administrator",
    "or the system usage guide for details on whether this is supported and "
    "how to set this up.",
    "",
    "To launch an application using a non default launcher, the --launcher "
    "argument can be used.",
    "This can be the name of an executable reachable in PATH or an absolute "
    "path.",
    "Setting this option only affects the name of the launcher executable "
    "itself",
    "and does not change the launch system. To launch with a non default "
    "launcher that is not related",
    "to the default running workload manager (such as mpiexec.hydra on a slurm "
    "system), this variable should be",
    "used in conjunction with setting CRAY_CTI_WLM to \"generic\" to ensure "
    "the launch mechanisms work with ",
    "the specified launcher. Note that setting CRAY_CTI_WLM to \"generic\" "
    "requires",
    "passwordless SSH to the compute nodes.",
    "Specify the name of the launcher to use as the argument to the launcher "
    "option.",
    "",
    "For example, launch $a{10} --launcher=\"myrun\"",
    "",
    "To launch inside of a job reservation, the --qsub or --sbatch arguments",
    "can be used. Modify the existing job script for the application by "
    "placing",
    "the comments #cray_debug_start and #cray_debug_end around the aprun/srun",
    "command to be debugged.",
    "",
    "For example, we would modify an existing job script named sample.pbs:",
    "",
    "#cray_debug_start",
    "aprun -n128 -N32 a.out",
    "#cray_debug_end",
    "",
    "Everything between the comment lines will be ignored. The user is still",
    "required to recreate the aprun/srun command using the existing launch",
    "command arguments.",
    "",
    "Using the above, the user would launch with the following arguments:",
    "launch $a{128} --launcher-args=\"-N32\" --qsub=sample.pbs a.out",
    0};

const char* const cmd_kill_help[] = {
    "Summary: Kill attached/launched application.",
    "Usage: kill <app_handle>",
    "Example command: kill $a",
    "",
    "Arguments: app_handle (process set handle)",
    "",
    "Description:",
    "Kill previously attached/launched application specified by app_handle.",
    "",
    "The entire application will be killed with SIGKILL, even if individual",
    "ranks are specified in the app_handle.",
    0};

const char* const cmd_list_help[] = {
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
    "        2. file:linenum - display source lines around specified line "
    "number",
    "                          in specified source file.",
    "        3. function - display source lines around specified function.",
    "        4. file:function - display source lines around specified function "
    "in",
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

const char* const cmd_next_help[] = {
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

const char* const cmd_print_help[] = {
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
    "lexical environment of each ranks selected stack frame, plus all those",
    "whose scope is global or over an entire file.",
    "",
    "Application ranks can be targeted using the process set notation for the",
    "desired app_handle. For example, to print variable myVar in ranks "
    "1,3,4,5,10",
    "of application $a, issue: print $a{1,3..5,10}::myVar.",
    0};

const char* const cmd_quit_help[] = {"Summary: Quit command.",
                                     "Usage: quit",
                                     "Example command: quit",
                                     "Shortcut: q",
                                     "",
                                     "Arguments: None.",
                                     "",
                                     "Description:",
                                     "Quit the debugger.",
                                     0};

const char* const cmd_release_help[] = {
    "Summary: Release attached/launched application from debugger control.",
    "Usage: release <app_handle>",
    "Example command: release $a",
    "",
    "Arguments: app_handle (process set handle)",
    "",
    "Description:",
    "Release previously attached/launched application specified by app_handle.",
    "",
    "Every rank of the app_handle will be released, even if individual ranks "
    "are",
    "specified.",
    0};

const char* const cmd_session_help[] = {
    "Summary: Create a new WLM session.",
    "Usage: session [--qsub=<batch_template> OR -q <batch_template>]",
    "               [--sbatch=<batch_template> OR -s <batch_template>]",
    "               [--end OR -e]",
    "",
    "Example command: session --qsub=job_script.pbs",
    "",
    "Arguments: batch_template (job script)",
    "",
    "Description:",
    "Create a new WLM session to associate all future launch/attach commands",
    "to. This allows multiple launch commands to be used inside of the same",
    "WLM batch session. The qsub and sbatch arguments are the same as what is",
    "used for the launch command. If multiple launches are used with the same",
    "session, the user must ensure enough resources are requested to cover all",
    "application launcher commands (aprun/srun) issued by launch.",
    "",
    "Modify the existing job script for the application by placing the "
    "comments",
    "#cray_debug_start and #cray_debug_end around the launcher command to be",
    "debugged.",
    "",
    "For example, we would modify an existing job script named sample.pbs:",
    "#cray_debug_start",
    "aprun -n128 -N32 a.out",
    "#cray_debug_end",
    "",
    "Everything between the comment lines will be ignored. The user is still",
    "required to recreate the launcher command (aprun/srun) using the existing",
    "launch command arguments.",
    "",
    "If the session command is interrupted before completion, the user must "
    "manually",
    "delete the job reservation from the queueing system.",
    0};

const char* const cmd_source_help[] = {
    "Summary: Read and execute debugger commands.",
    "Usage: source <filename>",
    "Example command: source myfile",
    "",
    "Arguments: filename (path to named command file)",
    "",
    "Description:",
    "Read debugger commands from a file to automate debugging tasks.",
    0};

const char* const cmd_step_help[] = {
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

const char* const cmd_tbreak_help[] = {
    "Summary: Set temporary breakpoint command.",
    "Usage: tbreak <location>",
    "Usage: tbreak <location> thread <thread-id>",
    "Example command: tbreak main.c:20",
    "Example command: tbreak myFunction thread 2",
    "",
    "Arguments: location (line number, function name, source-file:line or address)",
    "",
    "Description:",
    "Set a temporary breakpoint in every application rank in the current "
    "process set.",
    "The breakpoint will be deleted after being hit. This is equivalent to "
    "issuing a",
    "\"break\" followed by \"enable -delete\" with the breakpoint number.",
    "",
    "The location of the breakpoint can be defined as a line number, a "
    "function",
    "name, or \"*\" and an address. If an address is specified, it must begin",
    "with an asterisk (*). For example, break *0x0000000000400693 would break",
    "at address 0x400693.",
    "Adding thread thread-id create a thread specific temporary breakpoint that",
    "will only stop when that thread hits the break point.",
    0};

const char* const cmd_thread_help[] = {
    "Summary: Switch between threads.",
    "Usage: thread <thread-id>",
    "Example command: thread 5",
    "",
    "Arguments: thread-id",
    "",
    "Description:",
    "Change the thread context in every application rank in the current process set.",
    "Subsequent commands like \"print\", \"bt\", \"next\" and \"step\" will be",
    "directed to the thread-id",
    "",
    "Summary: apply a command to a set of threads.",
    "Usage: thread apply <thread-list> | all [-ascending] [-s] [-q] [-c] [-v] <command>",
    "Example command: thread apply all -s p i"
    "",
    "Description:",
    "Applies command to every thread in the selection list with the following flags",
    "to control the output:",
    "-q (quiet) disables printing the thread information for each result",
    "-s (silent) disables output from threads where the command generates an error"
    "     (e.g. variable not defined).",
    "-v (verbose) suppresses stripping out LWP and pointer values from the output.",
    "     Especially when not combined with -q this will tend to produce a full result for",
    "     each member of the procset.",
    "-c and -ascending (continue after error) and list in ascending order are the default behaviors",
    "     and are included for compatibility with gdb commands.",
    "",
    "Summary: apply a command silently to all threads.",
    "Usage: taas [-q] [-c] [-v] <command>",
    "",
    "Description:",
    "taas is a shortcut for thread apply all -s."
    "",
    "Summary: Name the current thread.",
    "Usage: thread name [<name>]",
    "Example command: thread name main",
    "",
    "Description:",
    "Renames the current thread to name.   If no name argument is given, remove any previous name assigned",
    "via thread name and revert to the program's name for the thread.",
    "",
    "Summary: Find threads using a regular expression",
    "Usage: thread find <regex>",
    "Example: thread find collector",
    "",
    "Description:",
    "Prints threads whose full id contains a regular expression.   The match is against the full",
    "thread identifier shown by \"info threads -v\" which may include not just the number and name",
    "but also the thread's address and LWP id",
    0};

const char* const cmd_up_help[] = {
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

const char* const cmd_viewset_help[] = {
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

const char* const cmd_watch_help[] = {
    "Summary: Set watchpoint command.",
    "Usage: watch [-a|-w|-r] <expression> [thread <thread-id>]",
    "Example command: watch -a foo(1,1)",
    "Example command: watch i thread 4",
    "",
    "Arguments: expression (variable expression)",
    "",
    "Description:",
    "Set a watchpoint in every application rank in the current process set. "
    "The",
    "expression of the watchpoint can be defined as a variable, an address,",
    "a variable operation (such as a cast), and other arbitrarily complex",
    "expressions using any operators valid in the program's native language.",
    "",
    "Valid option arguments to this command are -a and -w. The -a option",
    "specifies that an access watchpoint should be created. Access watchpoints",
    "will cause the program to break when the address defined by the "
    "expression",
    "is read or written to. To set a watchpoint on an address, ",
    "use an expression of the form *0xXXXXXX with the -a option.",
    "The -w option specifies that a write watchpoint",
    "should be created. Write watchpoints will cause the program to break",
    "when the address defined by the expression is written to.",
    "The -r option specifies that a read watchpoint",
    "should be created. Read watchpoints will cause the program to break",
    "when the address defined by the expression is read from.",
    "If no argument is provided the watchpoint will default to a write watchpoint.",
    "",
    "If the expression corresponds to a single address, a hardware watchpoint",
    "will be created. If the expression corresponds to a range of addresses a",
    "software watchpoint will be created. Software watchpoints cause "
    "application",
    "performance to slowdown by orders of magnitude and should be avoided, if",
    "possible.",
    "",
    "If the variable defined by the expression goes out of scope, the "
    "watchpoint",
    "will cause the application to break and the watchpoint will be removed.",
    "",
    "Adding thread thread-id creates the watchpoint that only applies to the",
    "given thread.",
    0};

const char* const cmd_whatis_help[] = {
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

const char* const class_info_help[] = {
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

const char* const class_maint_help[] = {
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

const char* const class_maint_set_help[] = {
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

const char* const class_maint_show_help[] = {
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

const char* const class_set_help[] = {
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

const char* const class_set_print_help[] = {
    "Summary: Set print command.",
    "Usage: set print [<subcommand>]",
    "Example command: set print",
    "",
    "Arguments: subcommand (set print subcommand)",
    "",
    "Description:",
    "Settings relating to the way things are printed. If no argument",
    "is given, display a list of all valid set print subcommands.",
    0};

const char* const class_set_break_help[] = {
    "Summary: Set break command.",
    "Usage: set break [<subcommand>]",
    "Example command: set break",
    "",
    "Arguments: subcommand (set break subcommand)",
    "",
    "Description:",
    "Settings relating to breakpoints. If no argument is given,",
    "display a list of all valid set break subcommands.",
    0};

const char* const class_show_help[] = {
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

const char* const class_show_print_help[] = {
    "Summary: Show print command.",
    "Usage: show print [<subcommand>]",
    "Example command: show print",
    "",
    "Arguments: subcommand (show print subcommand)",
    "",
    "Description:",
    "Show information relating to the way things are printed. If no argument",
    "is given, display a list of all valid show print subcommands.",
    0};

const char* const class_show_break_help[] = {
    "Summary: Show break command.",
    "Usage: show break [<subcommand>]",
    "Example command: show break",
    "",
    "Arguments: subcommand (show break subcommand)",
    "",
    "Description:",
    "Settings relating to breakpoints. If no argument is given,",
    "display a list of all valid show break subcommands.",
    0};

const char* const class_unset_help[] = {
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

const char* const class_usage_help[] = {
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

const char* const class_info_subcmd_breakpoints_help[] = {
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

const char* const class_info_subcmd_locals_help[] = {
    "Summary: Local symbol information.",
    "",
    "Description:",
    "Display a list of the local symbols in the current stack frame for each",
    "application rank in the current process set.",
    0};

const char* const class_info_subcmd_application_help[] = {
    "Summary: Application information.",
    "",
    "Description:",
    "Display information related to currently controlled applications. The "
    "handle",
    "column specifies the process set handle used to identify the "
    "launched/attached",
    "application. The ranks column indicates the number of application ranks.",
    "The focus column indicates if this application's process set is currently",
    "focused upon. The command column identifies the command that was passed",
    "to the application launcher in order to launch the application.",
    0};

const char* const class_info_subcmd_scope_help[] = {
    "Summary: Local symbol information relative to a scope.",
    "Usage: info scope <location>",
    "Example command: info scope source.f:20",
    "",
    "Arguments: location (function, line, or *addr)",
    "",
    "Description:",
    "Display a list of the local symbols relative to a scope for the "
    "application.",
    "The provided argument can either be a function name, a line number in a "
    "source",
    "file, or an address prefixed with *.",
    0};

const char* const class_info_subcmd_sources_help[] = {
    "Summary: Source file information.",
    "",
    "Description:",
    "Display the source files in the application. This command only displays "
    "the",
    "source files that have been located and are available to be used in "
    "conjunction",
    "with the list command. Try \"set directory\" to improve the results.",
    0};

const char* const class_info_subcmd_status_help[] = {
    "Summary: Status command.", "",
    "Description:", "Display the status of lauched/attached applications.", 0};

const char* const class_info_subcmd_threads_help[] = {
    "Summary: List threads.", "",
    "Usage: info threads",
    "Usage: info threads 2",
    "Usage: info threads 1-4 6",
    "Usage: info threads 2,4..8",
    "Usage: info threads \"worker[0-9]\"",
    "Arguments: [-v] [-l] optional list of thread ids", "",
    "Description:",
    "List the known threads in the current process set.",
    "-v option will suppress the removal of the  address and LWP for each thead",
    "(which will usually create a full thread set output for each process).",
    "-l prints every thread on a separate line instead of collecting",
    "threads at the same location.",
    "With no thread ids, this command displays all known threads.",
    "A subset of threads to display can be specified with a space or comma",
    "separated list of thread ids with ranges, e.g.: 2 4 7-10",
    "or 2,4,7..10    The list may also contain regular expressions within",
    "quotes, e.g \"thead.*1\" or 'thread.*1' which will match the set of threads",
    "returned by thread find.",
    nullptr};

/*
 * Class maint subcommands
 */

const char* const class_maint_subcmd_echo_help[] = {
    "Summary: Echo command.",
    "Usage: maint echo <string>",
    "Example command: maint echo print this string",
    "",
    "Arguments: string (string to echo)",
    "",
    "Description:",
    "Echo a string as output.",
    0};

const char* const class_maint_subcmd_sleep_help[] = {
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

const char* const class_maint_subcmd_direct_help[] = {
    "Summary: Passthru command.",
    "Usage: maint passthru <command>",
    "Example command: maint passthru show version",
    "",
    "Arguments: command (gdb command)",
    "",
    "Description:",
    "Pass a command directly to the debug backends of every application rank "
    "in",
    "the current process set. This can be used to talk directly to gdb. "
    "Console",
    "output will be aggregated and returned.",
    0};

const char* const class_maint_subcmd_system_help[] = {
    "Summary: System command.",
    "Usage: maint system <command>",
    "Example command: maint system ls -la",
    "",
    "Arguments: command (shell command)",
    "",
    "Description:",
    "Run a command in the shell environment of the client. This can be used",
    "to enumerate information about the system the client is running on. The",
    "stderr of the command will be redirected to stdout and printed in the "
    "result.",
    0};

/*
 * Class set subcommands
 */

const char* const class_set_subcmd_directory_help[] = {
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

const char* const class_set_subcmd_error_help[] = {
    "Summary: Warn and error epsilon for floating point comparison.",
    "Usage: set error <warn limit> <error limit>",
    "Example command: set error 1e-16 2e-15",
    "",
    "Default setting: ulp ulp",
    "                 Where ulp is the smallest non-zero double precision "
    "floating",
    "                 point value of the current machine.",
    "Arguments: warn limit (float), error limit (float)",
    "",
    "Description:",
    "Set the tolerance level for compare/assert commands difference deltas "
    "when",
    "comparing floating point values. Differences with deltas below the "
    "warning",
    "epsilon are ignored. Differences with deltas between the warning and "
    "error",
    "epsilon will be displayed using the current display mode setting and "
    "reported",
    "as a warning. Any differences with deltas above the error epsilon will be",
    "displayed using the current display mode setting and reported as an "
    "error.",
    "Error differences will halt the assertion script execution if stop mode "
    "is true.",
    0};

const char* const class_set_subcmd_environment_help[] = {
    "Summary: Set environment variable value.",
    "Usage: set environment <name> <value>",
    "Example command: set environment var val",
    "",
    "Arguments: name (environment variable name), value (value to assign to",
    "           environment variable)",
    "",
    "Description:",
    "Set the environment variable named name to the value provided by the "
    "value",
    "argument. Arguments can optionally be enclosed with quotes, the actual "
    "quotes",
    "will be stripped out for the final assignment. This will not affect any",
    "previously launched applications and will only take effect upon the next",
    "launch command. All environment variables passed this way are put into "
    "the",
    "forked environment of the application launcher process (aprun/srun).",
    0};

const char* const class_set_subcmd_function_help[] = {
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

const char* const class_set_subcmd_scheduler_locking_help[] = {
    "Summary: Set the behavior of the thread schedule locking during execution",
    "Usage set scheduler-locking  off|on|step|replay",
    "Example command: set scheduler-locking off",
    "",
    "Description:",
    "Set the mode for the locking scheduler:",
    "off    : no locking, threads may preemt any time",
    "on     : full locking, no thread except the current thread may run",
    "step   : scheduler is locked during stepping commands (step,next,stepi,nexti)",
    "replay : scheduler is locked during replay mode and unlocked during  normal execution.",
    0};

const char* const class_set_subcmd_non_stop_help[] = {
    "Summary: Set the behavior of the threads during a break",
    "Usage set non-stop off|on",
    "Example command: set non-stop on",
    "",
    "Description:",
    "Set the non-stop mode",
    "off    : stopping one thread in a process stops all threads",
    "on     : other threads continue running when one is stopped",
    0};

/*
 * Class set print subcommands
 */
const char* const class_set_print_subcmd_entryframe_help[] = {
    "Summary: Set printing of frame on entry mode.",
    "Usage: set print entry-frame [true|false]",
    "Example command: set print entry-frame false",
    "",
    "Default setting: true",
    "Arguments: true or false",
    "",
    "Description:",
    "Determines if the frames of the application ranks are printed on "
    "launch/attach.",
    0};

/*
 * Class set breakpoint subcommands
 */
const char* const class_set_break_subcmd_pending_help[] = {
    "Summary: Set behavior of setting pending breakpoints.",
    "",
    "Description:",
    "Set if pending breakpoints are enabled. If enabled an unrecognized breakpoint location",
    "will create a pending breakpoint. If disabled an unrecognized breakpoint location results",
    "in an error.",
    0};

/*
 * Class maint set subcommands
 */

const char* const class_maint_set_subcmd_earlyentry_help[] = {
    "Summary: Set early entry mode.",
    "Usage: maint set earlyentry [on|off]",
    "Example command: maint set earlyentry on",
    "",
    "Description:",
    "Set debugger to bypass continuing to application code on launch/attach. "
    "The",
    "application will be left in the PMI_Init() function upon startup.",
    0};

const char* const class_maint_set_subcmd_unsafe_help[] = {
    "Summary: Set unsafe execution mode.",
    "Usage: maint set unsafe [on|off]",
    "Example command: maint set unsafe on",
    "",
    "Description:",
    "Set debugger to bypass certain internal checks. This mode should only",
    "be modified by maintainers.",
    0};

const char* const class_maint_set_subcmd_sync_help[] = {
    "Summary: Set synchronous event mode.",
    "Usage: maint set sync [on|off]",
    "Example command: maint set sync on",
    "",
    "Description:",
    "Set debugger to treat asynchronous events as synchronous. This will cause",
    "commands such as continue to block input until all ranks affected by the",
    "command to report an asynchronous event. Use with caution! If a rank is",
    "not expected to reach a breakpoint, a deadlock can occur.",
    0};

/*
 * Class show subcommands
 */

const char* const class_show_subcmd_directories_help[] = {
    "Summary: Display source search path.",
    "",
    "Description:",
    "Display the current search path for locating source files. A $cwd in the",
    "path means the current working directory. A $cdir in the path means the",
    "compilation directory of the source file.",
    0};

const char* const class_show_subcmd_error_help[] = {
    "Summary: Warn and error epsilon for floating point comparison.",
    "",
    "Description:",
    "Display the current tolerance level setting for compare/assert command",
    "difference deltas when comparing floating point values.",
    0};

const char* const class_show_subcmd_function_help[] = {
    "Summary: Function definition information.", "",
    "Description:", "Display a list of the currently defined function macros.",
    0};

const char* const class_show_subcmd_environment_help[] = {
    "Summary: Environment variable information.",
    "",
    "Description:",
    "Display the environment variables that were set by calling the set "
    "environment",
    "command.",
    0};

const char* const class_show_subcmd_version_help[] = {
    "Summary: Version information.", "",
    "Description:", "Display the debugger version number.", 0};

const char* const class_show_subcmd_scheduler_locking_help[] = {
    "Summary: Show the behavior of the thread schedule locking during execution",
    "Usage: show scheduler-locking",
    "Example command: show scheduler-locking",
    "",
    "Description:",
    "Show the mode for the locking scheduler:",
    "off    : no locking, threads may preemt any time",
    "on     : full locking, no thread except the current thread may run",
    "step   : scheduler is locked during stepping commands (step,next,stepi,nexti)",
    "replay : scheduler is locked during replay mode and unlocked during  normal execution.",
    0};

const char* const class_show_subcmd_non_stop_help[] = {
    "Summary: Show the behavior of the thread schedule locking during execution",
    "Usage: show non-stop",
    "Example command: show non-stop",
    "",
    "Description:",
    "Show the non-stop mode for thread stopping:",
    "off    : stopping one thread in a process stops all threads",
    "on     : other threads continue running when one is stopped",
    0};

/*
 * Class show print subcommands
 */
const char* const class_show_print_subcmd_entryframe_help[] = {
    "Summary: Printing of frame on entry mode information.", "", "Description:",
    "Display if the frames of the application ranks are printed on "
    "launch/attach.",
    0};

/*
 * Class show breakpoint subcommands
 */
const char* const class_show_break_subcmd_pending_help[] = {
    "Summary: Show behavior of setting pending breakpoints.",
    "",
    "Description:",
    "Display if pending breakpoints are enabled. If enabled an unrecognized breakpoint location",
    "will create a pending breakpoint. If disabled an unrecognized breakpoint location results",
    "in an error.",
    0};

/*
 * Class maint show subcommands
 */

const char* const class_maint_show_subcmd_earlyentry_help[] = {
    "Summary: Early entry mode information.", "",
    "Description:", "Display the current setting of early entry mode.", 0};

const char* const class_maint_show_subcmd_unsafe_help[] = {
    "Summary: Unsafe mode information.", "",
    "Description:", "Display the current setting of unsafe mode.", 0};

const char* const class_maint_show_subcmd_sync_help[] = {
    "Summary: Synchronous event mode information.", "",
    "Description:", "Display the current setting of synchronous event mode.",
    0};

/*
 * Class unset subcommands
 */

const char* const class_unset_subcmd_environment_help[] = {
    "Summary: Unset environment variable value.",
    "Usage: unset environment <name>",
    "Example command: unset environment var",
    "",
    "Arguments: name (environment variable name)",
    "",
    "Description:",
    "Unset the environment variable named name from the environment. This can "
    "only",
    "be used to unset environment variables that were set using the set "
    "environment",
    "command. This will not affect any previously launched applications and "
    "will",
    "only take effect upon the next launch command.",
    0};

/*
 * Class usage subcommands
 */

// Dummy help
const char* const dummy_help[] = {0};
