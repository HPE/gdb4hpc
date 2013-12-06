/******************************************************************************\
 * MIEnvironment.c - Extended libmi support for -environment-* commands.
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

#include "MIResult.h"
#include "MIString.h"
#include "MIList.h"
#include "MIValue.h"
#include "MIEnvironment.h"

MIString *
MIGetEnvironmentDirectoryInfo(MICommand *cmd)
{
	MIList *	results;
	MIResult *	r;
	MIString *	source_path = NULL;

	if (!cmd->completed || cmd->output == NULL || cmd->output->rr == NULL)
		return NULL;

	results = cmd->output->rr->results;
	
	for (MIListSet(results); (r = (MIResult *)MIListGet(results)) != NULL;) {
		if (strcmp(r->variable, "source-path") == 0) {
			if (r->value != NULL) {
				source_path = MIValueToString(r->value);
			}
		}
	}
	
	return source_path;
}

