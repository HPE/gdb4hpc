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
 * Copyright 2011-2014 Cray Inc. All Rights Reserved.
 *
 ******************************************************************************/
#ifndef _MISESSION_H_
#define _MISESSION_H_

#include <sys/select.h>

#include "MIList.h"
#include "MIOutput.h"
#include "MICommand.h"
#include "MIEvent.h"

struct MISession {
	int				in_fd; /* GDB input */
	int				out_fd; /* GDB output */
	int				pty_fd; /* App output */
	int				pid;
	int				exited;
	int				exit_status;
	MICommand *		command;
	MIList *		send_queue;
	char *			gdb_path;
	char *			data_directory;

	// if non zero, the spawned gdb will read the .gdbinit and .gdbearlyinit files (default 0)
	int gdbinit;

	char **         	gdb_args;
	struct timeval		select_timeout;
	struct timeval		response_timeout;
	void			(*event_callback)(void*, MIEvent *);
	void*			event_callback_ctx;
	void			(*cmd_callback)(void*, MIResultRecord *);
	void*			cmd_callback_ctx;
	void			(*exec_callback)(void*, char *, MIList *);
	void*			exec_callback_ctx;
	void			(*status_callback)(void*, char *, MIList *);
	void*			status_callback_ctx;
	void			(*notify_callback)(void*, char *, MIList *);
	void*			notify_callback_ctx;
	void			(*console_callback)(void*, char *);
	void*			console_callback_ctx;
	void			(*log_callback)(void*, char *);
	void*			log_callback_ctx;
	void			(*target_callback)(void*, char *);
	void*			target_callback_ctx;
	int 			(*thread_stopped_callback)(void*, int);
	void*			thread_stopped_callback_ctx;
};
typedef struct MISession	MISession;

extern MISession *MISessionNew(void);
extern void MISessionFree(MISession *sess);
extern void MISessionSetTimeout(MISession *sess, long sec, long usec);
extern void MISessionSetResponseTimeout(MISession *sess, long sec, long usec);
extern void MISessionSetDebug(int debug);
extern int MISessionStartLocal(MISession *sess);
extern void MISessionRegisterEventCallback(MISession *sess, void (*callback)(void*, MIEvent *), void* ctx);
extern void MISessionRegisterCommandCallback(MISession *sess, void (*callback)(void*, MIResultRecord *), void* ctx);
extern void MISessionRegisterExecCallback(MISession *sess, void (*callback)(void*, char *, MIList *), void* ctx);
extern void MISessionRegisterStatusCallback(MISession *sess, void (*callback)(void*, char *, MIList *), void* ctx);
extern void MISessionRegisterNotifyCallback(MISession *sess, void (*callback)(void*, char *, MIList *), void* ctx);
extern void MISessionRegisterConsoleCallback(MISession *sess, void (*callback)(void*, char *), void* ctx);
extern void MISessionRegisterLogCallback(MISession *sess, void (*callback)(void*, char *), void* ctx);
extern void MISessionRegisterTargetCallback(MISession *sess, void (*callback)(void*, char *), void* ctx);

// The thread-stopped callback is called when gdb reports a stopped thread in non-stop mode, while
// awaiting a response.   If the call-back is non-null and returns a non-zero value, it indicates
// the response is done.   i.e. typically, every thread expected to stop has stopped.
extern void MISessionRegisterThreadStoppedCallback(MISession *sess, int (*callback)(void*, int), void* ctx);
extern void MISessionSetGDBPath(MISession *sess, const char *path);
extern void MISessionSetGDBDataDirectory(MISession *sess, const char *path);
extern void MISessionAddGDBArg(MISession* sess, const char* arg);
extern int MISessionSendCommand(MISession *sess, MICommand *cmd);
extern void MISessionProcessCommandsAndResponses(MISession *sess, fd_set *rfds, fd_set *wfds);
extern void MISessionGetFds(MISession *sess, int *nfds, fd_set *rfds, fd_set *wfds, fd_set *efds);
extern int MISessionProgress(MISession *sess);
extern void ProcessCLIResultRecord(MIResultRecord *, void *);
#endif /* _MISESSION_H_ */
