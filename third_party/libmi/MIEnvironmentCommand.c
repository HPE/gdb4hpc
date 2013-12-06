/******************************************************************************\
 * MIEnvironmentCommand.c - Extended libmi support for -environment-* commands.
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
MIEnvironmentCd(char *pathdir)
{
	MICommand *	cmd = MICommandNew("-environment-cd", MIResultRecordDONE);
	
	MICommandAddOption(cmd, pathdir, NULL);
	
	return cmd;
}

MICommand *
MIEnvironmentDirectory(int reset, char **pathdirs)
{
	int i;
	MICommand *	cmd = MICommandNew("-environment-directory", MIResultRecordDONE);
	
	if (reset) {
		MICommandAddOption(cmd, "-r", NULL);
	}
	
	for ( i=0; pathdirs[i] != NULL; i++) {
		MICommandAddOption(cmd, pathdirs[i], NULL);
	}
	
	return cmd;
}

MICommand *
MIEnvironmentPath(int reset, char **pathdirs)
{
	int i;
	MICommand *	cmd = MICommandNew("-environment-path", MIResultRecordDONE);
	
	if (reset) {
		MICommandAddOption(cmd, "-r", NULL);
	}
	
	for ( i=0; pathdirs[i] != NULL; i++) {
		MICommandAddOption(cmd, pathdirs[i], NULL);
	}
	
	return cmd;
}

MICommand *
MIEnvironmentPwd(void)
{
	MICommand *	cmd = MICommandNew("-environment-pwd", MIResultRecordDONE);
	
	return cmd;
}

