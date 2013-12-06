/******************************************************************************\
 * MIFileCommand.c - Extended libmi support for -file-* commands.
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

#include <stdio.h>
#include <stdlib.h>

#include "MICommand.h" 

MICommand *
MIFileExecAndSymbols(char *file)
{
	MICommand *	cmd = MICommandNew("-file-exec-and-symbols", MIResultRecordDONE);
	
	MICommandAddOption(cmd, file, NULL);
	
	return cmd;
}

MICommand *
MIFileExecFile(char *file)
{
	MICommand *	cmd = MICommandNew("-file-exec-file", MIResultRecordDONE);
	
	MICommandAddOption(cmd, file, NULL);
	
	return cmd;
}

MICommand *
MIFileListExecSourceFile(void)
{
	MICommand *	cmd = MICommandNew("-file-list-exec-source-file", MIResultRecordDONE);
	
	return cmd;
}

MICommand *
MIFileListExecSourceFiles(void)
{
	MICommand *	cmd = MICommandNew("-file-list-exec-source-files", MIResultRecordDONE);
	
	return cmd;
}

MICommand *
MIFileSymbolFile(char *file)
{
	MICommand *	cmd = MICommandNew("-file-symbol-file", MIResultRecordDONE);
	
	MICommandAddOption(cmd, file, NULL);
	
	return cmd;
}

