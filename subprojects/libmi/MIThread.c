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

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "MIList.h"
#include "MIValue.h"
#include "MIResult.h"
#include "MIArg.h"
#include "MIThread.h"
#include "MIOOBRecord.h"

static void
initMIThread(MIThread* thread) {
	thread->id = -1;
	thread->target_id = NULL;
	thread->details = NULL;
	thread->name = NULL;
	thread->state = THREADSTATE_UNSPECIFIED;
	thread->frame = NULL;
	thread->core = -1;
}

MIThread*
MIThreadNew() {
	MIThread* thread;
	thread = (MIThread *)malloc(sizeof(MIThread));
	initMIThread(thread);
	return thread;
}

void MIThreadFree(MIThread* thread) {
	free(thread->target_id);
	free(thread->details);
	free(thread->name);
	if (thread->frame) {
		MIFrameFree(thread->frame);
	}
	initMIThread(thread);
	free(thread);
}

static void
initMIThreadInfoResult(MIThreadInfoResult* thread_info) {
	thread_info->threads = NULL;
	thread_info->current_thread = 0;
}

MIThreadInfoResult*
MIThreadInfoResultNew() {
	MIThreadInfoResult* thread_info;
	thread_info = (MIThreadInfoResult *)malloc(sizeof(MIThreadInfoResult));
	initMIThreadInfoResult(thread_info);
	thread_info->threads = MIListNew();
	return thread_info;
}

void MIThreadInfoResultFree(MIThreadInfoResult* thread_info) {
	MIListFree(thread_info->threads, MIThreadFree);
	initMIThreadInfoResult(thread_info);
	free(thread_info);
}

// `thread_tuple` must have MIValueTypeTuple, a non-NULL `results` field
// and data should be of the general form:
//
// {id="2",target-id="Thread 0xb7e14b90 (LWP 21257)",
//    frame={level="0",addr="0xffffe410",func="__kernel_vsyscall",
//            args=[]},state="running"},
//
// or
//
// {id="1",target-id="Thread 0xb7e156b0 (LWP 21254)",
//    frame={level="0",addr="0x0804891f",func="foo",
//            args=[{name="i",value="10"}],
//            file="/tmp/a.c",fullname="/tmp/a.c",line="158",arch="i386:x86_64"},
//            state="running"}
//
// not all supported fields are shown in the above examples. if present,
// `parseThread` will capture the following fields:
// - id
// - target-id
// - details
// - name
// - state
// - frame
// - core
static
MIThread*
parseThread(MIValue* thread_tuple) {
	if (thread_tuple == NULL || thread_tuple->type != MIValueTypeTuple || thread_tuple->results == NULL) return NULL;

	MIThread* parsed_thread = MIThreadNew();

	for (MIResult* iter; (iter = (MIResult*)MIListGet(thread_tuple->results)) != NULL; ){
		if (iter->variable == NULL) continue;

		if (strcmp(iter->variable, "id") == 0) {
			parsed_thread->id = strtoul(iter->value->cstring, NULL, 10);
		} else if (strcmp(iter->variable, "target-id") == 0) {
			parsed_thread->target_id = strdup(iter->value->cstring);
		} else if (strcmp(iter->variable, "details") == 0) {
			parsed_thread->details = strdup(iter->value->cstring);
		} else if (strcmp(iter->variable, "name") == 0) {
			parsed_thread->name = strdup(iter->value->cstring);
		} else if (strcmp(iter->variable, "state") == 0) {
			if (strcmp(iter->value->cstring, "stopped") == 0) {
				parsed_thread->state = THREADSTATE_STOPPED;
			} else if (strcmp(iter->value->cstring, "running") == 0) {
				parsed_thread->state = THREADSTATE_RUNNING;
			} else {
				parsed_thread->state = THREADSTATE_UNSPECIFIED;
			}
		} else if (strcmp(iter->variable, "frame") == 0) {
			parsed_thread->frame = MIFrameParse(iter->value);
		} else if (strcmp(iter->variable, "core") == 0) {
			parsed_thread->core = strtoul(iter->value->cstring, NULL, 10);
		}
	}

	return parsed_thread;
}

MIThreadInfoResult*
MIGetThreadInfoResult(MICommand* cmd) {
	if (!cmd || !cmd->completed || cmd->output == NULL || cmd->output->rr == NULL) {
		return NULL;
	}

	MIResultRecord* rr = cmd->output->rr;
	MIThreadInfoResult* result = MIThreadInfoResultNew();

	MIListSet(rr->results);
	for (MIResult* rr_iter; (rr_iter = (MIResult *)MIListGet(rr->results)) != NULL; ) {
		if (strcmp(rr_iter->variable, "threads") == 0) {
			if (rr_iter->value == NULL || rr_iter->value->values == NULL) continue;
			MIList* thread_tuples = rr_iter->value->values;

			MIListSet(thread_tuples);
			for (MIValue* thread_iter; (thread_iter = (MIValue*)MIListGet(thread_tuples)) != NULL; ) {
				if (thread_iter->results == NULL) continue;
				MIListSet(thread_iter->results);
				MIThread* parsed_thread = parseThread(thread_iter);
				if (parsed_thread == NULL) continue;
				MIListAdd(result->threads, parsed_thread);
			}
		} else if (strcmp(rr_iter->variable, "current-thread-id") == 0) {
			result->current_thread = strtoul(rr_iter->value->cstring, NULL, 10);
		}
	}

	return result;
}

MIThreadSelectInfo *
MIThreadSelectInfoNew(void) 
{
	MIThreadSelectInfo * info;
	info = (MIThreadSelectInfo *)malloc(sizeof(MIThreadSelectInfo));
	info->current_thread_id = 0;
	info->frame = NULL;
	return info;
}

MIThreadSelectInfo *
MISetThreadSelectInfo(MICommand *cmd) 
{
	MIThreadSelectInfo * info = MIThreadSelectInfoNew();
	MIValue * val;
	MIResultRecord * rr;
	MIResult * result;

	if (!cmd->completed || cmd->output == NULL || cmd->output->rr == NULL)
		return NULL;

	rr = cmd->output->rr;
	for (MIListSet(rr->results); (result = (MIResult *)MIListGet(rr->results)) != NULL; ) {
		if (strcmp(result->variable, "new-thread-id") == 0) {
			val = result->value;
			info->current_thread_id = (int)strtol(val->cstring, NULL, 10);
		} else if (strcmp(result->variable, "frame") == 0) {
			val = result->value;
			if (val->type == MIValueTypeTuple) {
				info->frame = MIFrameParse(val);
			}
		}
	}	
	return info;	
}
