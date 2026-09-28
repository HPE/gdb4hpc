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
 * Copyright 2012-2014,2021 Hewlett Packard Enterprise Development LP.
 *
 ******************************************************************************/

#ifndef _MIEVENT_H_
#define _MIEVENT_H_

#include "MIList.h"
#include "MIFrame.h"

enum MIEventClass {
	MIEventClassStopped	= 1,
	MIEventClassRunning
};

enum MIEventType {
	MIEventTypeBreakpointHit = 1,
	MIEventTypeWatchpointTrigger,
	MIEventTypeWatchpointScope,
	MIEventTypeSteppingRange,
	MIEventTypeSignal,
	MIEventTypeLocationReached,
	MIEventTypeFunctionFinished,
	MIEventTypeInferiorExit,
	MIEventTypeInferiorSignalExit,
	MIEventTypeCatchpointHit,
	// if gdb didn't give a reason for the stop event:
	MIEventTypeNoReason,
};

/**
 * Represents an asynchronous event.
 */
struct MIEvent {
	enum MIEventClass	classId;
	enum MIEventType	type;
	int 				threadId;
	MIFrame *			frame;
	int					bkptno;
	int					number;
	char *				sigName;
	char *				sigMeaning;
	char *				gdbResult;
	char *				returnValue;
	char *				returnType;
	int					code;
	char *				exp;
	char *				oldValue;
	char *				newValue;
};
typedef struct MIEvent	MIEvent;

extern MIEvent *MIEventNew(enum MIEventClass, enum MIEventType);
extern void MIEventFree(MIEvent *var);
extern MIEvent *MIEventCreateStoppedEvent(char *reason, MIList *results);
#endif /* _MIEVENT_H_ */


