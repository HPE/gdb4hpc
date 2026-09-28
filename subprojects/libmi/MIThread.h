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
 * Copyright 2012-2014 Cray Inc. All Rights Reserved.
 *
 ******************************************************************************/
 
 /*
  * Based on the QNX Java implementation of the MI interface
  */

#ifndef _MITHREAD_H_
#define _MITHREAD_H_

#include "MICommand.h"
#include "MIValue.h"
#include "MIFrame.h"

struct MIThreadSelectInfo {
	int current_thread_id;
	MIFrame * frame;
};
typedef struct MIThreadSelectInfo MIThreadSelectInfo;

extern MIThreadSelectInfo *MIThreadSelectInfoNew(void);
extern MIThreadSelectInfo *MISetThreadSelectInfo(MICommand *cmd);

enum ThreadState {
	THREADSTATE_UNSPECIFIED,
	THREADSTATE_STOPPED,
	THREADSTATE_RUNNING,
};

struct MIThread {
	// e.g. -thread-info
	// ^done,threads=[
	// {id="2",target-id="Thread 0xb7e14b90 (LWP 21257)",
	//    frame={level="0",addr="0xffffe410",func="__kernel_vsyscall",
	//            args=[]},state="running"},
	// {id="1",target-id="Thread 0xb7e156b0 (LWP 21254)",
	//    frame={level="0",addr="0x0804891f",func="foo",
	//            args=[{name="i",value="10"}],
	//            file="/tmp/a.c",fullname="/tmp/a.c",line="158",arch="i386:x86_64"},
	//            state="running"}],
	// current-thread-id="1"
	//
	// this struct is a single entry in the threads=[...] list.

	int id;
	// `target_id` might be null
	char* target_id;
	// `details` might be null
	char* details;
	// `name` might be null
	char* name;
	enum ThreadState state;
	// `frame` might be null
	MIFrame* frame;
	// `core` might be < 0
	int core;
};
typedef struct MIThread MIThread;

extern MIThread* MIThreadNew();
extern void MIThreadFree(MIThread* tinfo);

struct MIThreadInfoResult {
	// list of MIThreads
	MIList* threads;
	int current_thread;
};
typedef struct MIThreadInfoResult MIThreadInfoResult;

extern MIThreadInfoResult* MIThreadInfoResultNew();
extern void MIThreadInfoResultFree(MIThreadInfoResult* tinfo);

// extract the MIThreadInfoResult from the -thread-info command.
extern MIThreadInfoResult* MIGetThreadInfoResult(MICommand* cmd);

#endif /* _MITHREAD_H_ */

