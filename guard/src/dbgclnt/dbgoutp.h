/******************************************************************************\
 * dbgoutp.h - Application output redirect interface
 *             Used by protocols to pass application stdout to the debugger 
 *             client.
 *
 * © 2013 Cray Inc.  All Rights Reserved.
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

#ifndef _DBGOUTP_H
#define _DBGOUTP_H

#include <stdio.h>

struct g_progoutp
{
	FILE *	p_o_stream;
	int		pipefd[2];
#define p_o_rfd	pipefd[0]
#define p_o_wfd	pipefd[1]
};
typedef struct g_progoutp g_progoutp_t;

extern g_progoutp_t *	DbgCreateGenericProgOutp(void);
extern void				DbgFreeGenericProgOutp(g_progoutp_t *po);

#endif /* !_DBGOUTP_H */
