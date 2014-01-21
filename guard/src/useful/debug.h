/*********************************************************************************\
 * Debug handling definitions.
 *
 *Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
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
 *********************************************************************************/




#ifndef _USEFUL_DEBUG_H
#define _USEFUL_DEBUG_H

#include	<stdarg.h>

extern void	PrintDebugTime(char *, ...);
extern void	PrintDebugTimeLog(char *, ...);

#ifdef PERFLOG
struct guard_timer_t
{
	char* name;
	struct timeval tv_begin;
	struct timeval tv_end;
};
typedef struct guard_timer_t guard_timer_t;

extern double 			CalElapsedTime(struct timeval* tv_begin, struct timeval* tv_end);
extern int				CompareTime(struct timeval* tv_l, struct timeval* tv_r);
extern int				CreateTimerLog(char* log);
extern int				CloseTimerLog();
extern void				ReleaseTimers();
extern guard_timer_t* 	SetTimerStart(char* name);
extern guard_timer_t*	SetTimerStop(char* name);
extern void				SetTimerStopAndLog(char* name);
extern void				SetLogInfo(char* fmt, ...);
#endif /* PERFLOG */

#endif /* !_USEFUL_DEBUG_H */
