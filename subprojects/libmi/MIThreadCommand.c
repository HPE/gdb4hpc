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

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MICommand.h"

MICommand *
MIThreadInfo(const char* id_list) {
	MICommand *cmd;
	cmd = MICommandNew("-thread-info", MIResultRecordDONE);

	if (id_list != NULL) {
		// the input list needs to be quoted for mi to understand expressions like "1 2".
		// only double quotes work. we assume that there will be no
		// quotes in the input since no id list format supports them.
		char* quoted = NULL;
		asprintf(&quoted, "\"%s\"", id_list);

		MICommandAddOption(cmd, quoted, NULL);
	}

	return cmd;
}

MICommand *
MIThreadSelect(int threadNum)
{
	MICommand * cmd;
	cmd = MICommandNew("-thread-select", MIResultRecordDONE);
	MICommandAddOption(cmd, MIIntToCString(threadNum), NULL);
	return cmd;
}
