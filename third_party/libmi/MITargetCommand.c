/******************************************************************************\
 * MITargetCommand.c - Extended libmi support for -target-* commands.
 *
 * © 2012 Cray Inc.  All Rights Reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MICommand.h" 

MICommand *
MITargetAttach(int pgid)
{
	MICommand *	cmd = MICommandNew("-target-attach",MIResultRecordDONE);
	
	MICommandAddOption(cmd, MIIntToCString(pgid), NULL);
	
	return cmd;
}

MICommand *
MITargetDetach(int pgid)
{
	MICommand *	cmd = MICommandNew("-target-detach",MIResultRecordDONE);
	
	MICommandAddOption(cmd, MIIntToCString(pgid), NULL);
	
	return cmd;
}

