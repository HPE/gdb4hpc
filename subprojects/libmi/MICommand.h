/******************************************************************************
 * Copyright (c) 2005 The Regents of the University of California.
 * This material was produced under U.S. Government contract W-7405-ENG-36
 * for Los Alamos National Laboratory, which is operated by the University
 * of California for the U.S. Department of Energy. The U.S. Government has
 * rights to use, reproduce, and distribute this software. NEITHER THE
 * GOVERNMENT NOR THE UNIVERSITY MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR
 * ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE. If software is modified
 * to produce derivative works, such modified software should be clearly
 * marked, so as not to confuse it with the version available from LANL.
 *
 * Additionally, this program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * LA-CC 04-115
 *
 * Copyright 2010-2024 Cray Inc. All Rights Reserved.
 *
 ******************************************************************************/

#ifndef _MICOMMAND_H_
#define _MICOMMAND_H_

#include "MIResultRecord.h"
#include "MIOutput.h"

struct MISession;

struct MICommand {
	char *			command;								/* command to execute */
	char **			options;								/* command options */
	int				num_options;							/* number of options for command */
	int				opt_size;								/* allocated size of options */
	int				completed;								/* command has been completed */
	int				expected_class;
	MIOutput *		output;									/* result from completed command */
	void			(*callback)(MIResultRecord *, void *);	/* command completed callback */
	void *			cb_data;								/* callback data */
	long			timeout;								/* timeout to wait for response (ms) */
};
typedef struct MICommand	MICommand;

/*
 * enum for commonly used print-values option passed to commands.
 */
enum MIPrintValues
{
        NO_VALUES,
        ALL_VALUES,
        SIMPLE_VALUES
};
typedef enum MIPrintValues MIPrintValues;

extern MICommand *		MICommandNew(const char *, int);
extern void				MICommandSetTimeout(MICommand *cmd, int timeout);
extern void				MICommandFree(MICommand *cmd);
extern void				MICommandAddOption(MICommand *cmd, const char *opt, const char *arg);
extern int				MICommandCompleted(MICommand *cmd);
extern int				MICommandResultOK(MICommand *cmd);
extern MIResultRecord *	MICommandResult(MICommand *cmd);
extern int				MICommandResultClass(MICommand *cmd);
extern char *			MICommandResultErrorMessage(MICommand *cmd);
extern char *			MICommandConsoleOOBMessages(MICommand *cmd);
extern void				MICommandRegisterCallback(MICommand *cmd, void (*callback)(MIResultRecord *, void *), void *data);
extern char *			MICommandToString(MICommand *cmd);

/*
 * -gdb-* commands
 */
extern MICommand *MIGDBSet(const char *, const char *);
extern MICommand *MIGDBExit(void);
extern MICommand *MIGDBShowEndian(void);
extern MICommand *MIGDBVersion(void);

/*
 * -environment-* commands
 */
extern MICommand *MIEnvironmentCd(char *pathdir);
extern MICommand *MIEnvironmentDirectory(int reset, char **pathdirs);
extern MICommand *MIEnvironmentPath(int reset, char **pathdirs);
extern MICommand *MIEnvironmentPwd(void);

/*
 * -exec-* commands
 */
extern MICommand *MIExecArguments(char **);
extern MICommand *MIExecContinue(void);
extern MICommand *MIExecRun(void);
extern MICommand *MIExecStep(int);
extern MICommand *MIExecNext(int);
extern MICommand *MIExecFinish(void);
extern MICommand *MIExecFinishThread(unsigned int tid);
extern MICommand *MIExecFinishThreadFrame(unsigned int tid, unsigned int frame);
extern MICommand *MIExecInterrupt(void);
extern MICommand *MIExecUtil(char*);
extern MICommand *MIExecStepInstruction(void);
extern MICommand *MIExecContinueAll(void);
extern MICommand *MIExecContinueThread(unsigned int tid);
extern MICommand *MIExecInterruptAll(void);
extern MICommand *MIExecInterruptThread(unsigned int tid);

/*
 * -file-* commands
 */
extern MICommand *MIFileExecAndSymbols(const char *);
extern MICommand *MIFileExecFile(const char *);
extern MICommand *MIFileListExecSourceFile(void);
extern MICommand *MIFileListExecSourceFiles(void);
extern MICommand *MIFileListSharedLibraries(void);
extern MICommand *MIFileSymbolFile(char *);

/*
 * -break-* commands
 */
extern MICommand *MIBreakInsert(int isTemporary, int isHardware, int allowPending, const char *condition, int ignoreCount, const char *line, int tid);
extern MICommand *MIBreakDelete(int nbps, int *bpids);
extern MICommand *MIBreakDisable(int nbps, int *bpids);
extern MICommand *MIBreakEnable(int nbps, int *bpids);
extern MICommand *MIBreakCondition(int nbps, int *bpids, char *expr);
extern MICommand *MIBreakWatch(const char *expr, int access, int read);
extern MICommand *MIBreakAfter(int nbps, int *bpids, int ignoreCount);
extern MICommand *MIBreakInfo(int bpid);

/*
 * -stack-* commands
 */
extern MICommand *MIStackSelectFrame(int level);
extern MICommand *MIStackListFrames(int low, int high);
extern MICommand *MIStackListAllFrames(void);
extern MICommand *MIStackListAllFramesThread(unsigned int tid);
extern MICommand *MIStackListLocals(MIPrintValues vals);
extern MICommand *MIStackListVariables(MIPrintValues vals);
extern MICommand *MIStackListArguments(MIPrintValues vals, int low, int high);
extern MICommand *MIStackListAllArguments(MIPrintValues vals);
extern MICommand *MIStackInfoFrame(void);
extern MICommand *MIStackInfoDepth(void);
extern MICommand *CLIFrame(void);

/*
 * -symbol-* commands
 */

// create a -symbol-info-function command.
//
// if `include_nondebug` is non-zero, the --include-nondebug will be added.
// if `type_regexp` is non-null, it will be passed with the --type flag.
// if `name_regexp` is non-null, it will be passed with the --name flag.
// if `max_results` is > 0, it will be passed with the --max-results flag.
//
// see the gdb/mi documentation for flag behavior.
extern MICommand* MISymbolInfoFunctions(int include_nondebug, const char* type_regexp, const char* name_regexp, int max_results);

// create a -symbol-info-variables command.
//
// arguments have the same meaning as in MISymbolInfoFunctions.
extern MICommand* MISymbolInfoVariables(int include_nondebug, const char* type_regexp, const char* name_regexp, int max_results);

/*
 * -var-* commands
 */
extern MICommand *MIVarCreate(const char *name, const char *frame, const char *expr);
extern MICommand *MIVarDelete(const char *name);
extern MICommand *MIVarAssign(const char *name, const char *expr);
extern MICommand *MIVarListChildren(const char *name);
extern MICommand *MIVarEvaluateExpression(const char *name);
extern MICommand *MIDataEvaluateExpression(const char *expr);
extern MICommand *MIDataEvaluateExpressionThread(const char *expr, unsigned int tid);
extern MICommand *MIDataEvaluateExpressionThreadFrame(const char *expr, unsigned int tid, unsigned int frame);
extern MICommand *MIVarUpdate(const char *name);
extern MICommand *MIVarInfoType(const char *name);
extern MICommand *MIVarInfoNumChildren(const char *name);
extern MICommand *MIVarInfoPathExpression(const char *name);

/*
 * -thread-* commands
 */
// `id_list` is the thread ID list format specified in the gdb docs.
// e.g. "1", "1-3", "1 2-4", "1.3", "1.*", "1.3-9".
// for info about all threads, pass nullptr. an empty string will also
// get info about all threads, but the current thread id will not be reported (gdbmi quirk)
extern MICommand *MIThreadInfo(const char* id_list);
extern MICommand *CLIInfoThreads(void);
extern MICommand *MIThreadSelect(int threadNum);

/*
 * -target-* commands
 */
extern MICommand *MITargetAttach(int pgid);
extern MICommand *MITargetDetach(int pgid);
extern MICommand *MITargetDetachAll(void);

/*
 * -data-* commands
 */
extern MICommand *MIDataReadMemory(long, char *, char *, int, int, int, char *);
extern MICommand *MIDataWriteMemory(long offset, char * address, char * format, int wordSize, char * value);
extern MICommand *MIDataReadMemoryBytes(unsigned long start_addr, long offset, unsigned long len);
extern MICommand *MIDataWriteMemoryBytes(char *address, char *contents);
extern MICommand *MIDataReadDisassemble(char* startAddr, char* endAddr, char* format);

#ifdef __APPLE__
extern MICommand *MIPidInfo(void);
#endif /* __APPLE__ */

/*
 * Non-MI CLI commands
 */

extern MICommand *CLIPtype(const char *name);
extern MICommand *CLIWhatis(const char *var);
extern MICommand *CLIInfoProc(void);
extern MICommand *CLIBypass(int cmd_argc, char *cmd_argv[]);
extern MICommand *CLIInfoScope(const char *location);

/*
 * CLI signal commands
 */
extern MICommand *CLIListSignals(const char *);
extern MICommand *CLISignalInfo(const char *);
extern MICommand *CLIHandle(const char *);

/*
 *  CLI List source lines command
 */
extern MICommand *CLIList(const char *);
extern MICommand *CLIListSingleLine(int);

/*
 *  Print command
 */
extern MICommand *CLIPrint(const char *arg);
extern MICommand *CLIPrintHex(const char *arg);

#endif /* _MICOMMAND_H_ */
