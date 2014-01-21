/******************************************************************************\
 * rd_cmd.c - Relative debugger command processing routines.
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
 
#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	"rd_cmd.h"

extern void	InitCmd_blockmap(void);
extern void	InitCmd_graph(void);
extern void	FinishCmd_blockmap(void);
extern void	FinishCmd_graph(void);

void
InitCmd_rd(void)
{
	// setup the regular rd commands
	InitCmd_graph();
	InitCmd_blockmap();
}

void
FinishCmd_rd(void)
{
	FinishCmd_graph();
	FinishCmd_blockmap();
}
