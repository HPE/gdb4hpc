/******************************************************************************\
 * MIFile.h - Extended libmi support for -file-* commands.
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

#ifndef _MIFILE_H_
#define _MIFILE_H_

#include "MICommand.h"
#include "MIList.h"

struct MISource {
	int			line;
	char *		file;
	char *		fullname;
	int			macro_info;
};
typedef struct MISource	MISource;

struct MISources {
	char *		file;
	char *		fullname;
};
typedef struct MISources	MISources;

extern MISource *	MISourceNew(void);
extern void			MISourceFree(MISource *);
extern MISources *	MISourcesNew(void);
extern void			MISourcesFree(MISources *);
extern MISource *	MIGetFileListExecSourceFileInfo(MICommand *);
extern MIList *		MIGetFileListExecSourceFilesInfo(MICommand *);

#endif /* _MIFILE_H_ */
