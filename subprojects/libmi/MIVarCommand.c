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
 * Copyright 2013-2014 Cray Inc. All Rights Reserved.
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

#include "MIList.h"
#include "MIString.h"
#include "MICommand.h"

MICommand *
MIVarCreate(const char *name, const char *frame, const char *expr)
{
	MICommand *	cmd;
	
	cmd = MICommandNew("-var-create", MIResultRecordDONE);
	MICommandAddOption(cmd, name, frame);
	MICommandAddOption(cmd, expr, NULL);
	return cmd;
}

MICommand *
MIVarDelete(const char *name)
{
	MICommand *	cmd;
	
	cmd = MICommandNew("-var-delete", MIResultRecordDONE);
	MICommandAddOption(cmd, name, NULL);
	return cmd;
}

MICommand *
MIVarAssign(const char *name, const char *expr)
{
	MICommand *	cmd;
	
	cmd = MICommandNew("-var-assign", MIResultRecordDONE);
	MICommandAddOption(cmd, name, expr);
	return cmd;
}

MICommand *
MIVarListChildren(const char *name)
{
	MICommand *	cmd;
	
	cmd = MICommandNew("-var-list-children", MIResultRecordDONE);
	MICommandAddOption(cmd, "--simple-values", NULL);
	MICommandAddOption(cmd, name, NULL);
	return cmd;
}

MICommand *
MIVarEvaluateExpression(const char *name)
{
	MICommand *	cmd;
	
	cmd = MICommandNew("-var-evaluate-expression", MIResultRecordDONE);
	MICommandAddOption(cmd, name, NULL);
	return cmd;
}

MICommand *
MIVarUpdate(const char *name)
{
	MICommand *	cmd;
	
	cmd = MICommandNew("-var-update", MIResultRecordDONE);
	MICommandAddOption(cmd, name, NULL);
	return cmd;
}

MICommand *
MIDataEvaluateExpression(const char *name)
{
	MICommand *	cmd;
	
	cmd = MICommandNew("-data-evaluate-expression", MIResultRecordDONE);
	MICommandAddOption(cmd, name, NULL);
	return cmd;
}

MICommand *
MIDataEvaluateExpressionThread(const char *expr, unsigned int tid)
{
	MICommand * cmd;
	char thread_num[64];
	snprintf(thread_num, sizeof(thread_num), "%u", tid);
	cmd = MICommandNew("-data-evaluate-expression", MIResultRecordDONE);
	MICommandAddOption(cmd, "--thread", thread_num);
	MICommandAddOption(cmd, expr, NULL);
	return cmd;
}

MICommand *
MIDataEvaluateExpressionThreadFrame(const char *expr, unsigned int tid, unsigned int frame)
{
	MICommand* cmd = MICommandNew("-data-evaluate-expression", MIResultRecordDONE);
	char num_buf[64];

	snprintf(num_buf, sizeof(num_buf), "%u", tid);
	MICommandAddOption(cmd, "--thread", num_buf);

	snprintf(num_buf, sizeof(num_buf), "%u", frame);
	MICommandAddOption(cmd, "--frame", num_buf);

	MICommandAddOption(cmd, expr, NULL);

	return cmd;
}

MICommand *
MIVarInfoType(const char *name)
{
	MICommand *	cmd;
	
	cmd = MICommandNew("-var-info-type", MIResultRecordDONE);
	MICommandAddOption(cmd, name, NULL);
	return cmd;
}

MICommand *
MIVarInfoNumChildren(const char *name)
{
	MICommand *	cmd;
	
	cmd = MICommandNew("-var-info-num-children", MIResultRecordDONE);
	MICommandAddOption(cmd, name, NULL);
	return cmd;
}

MICommand *
MIVarInfoPathExpression(const char *name)
{
	MICommand *	cmd;
	
	cmd = MICommandNew("-var-info-path-expression", MIResultRecordDONE);
	MICommandAddOption(cmd, name, NULL);
	return cmd;
}

