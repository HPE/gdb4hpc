/******************************************************************************\
 * MIFile.h - Extended libmi support for -file-* commands.
 *
 * Copyright 2013-2014 Cray Inc. All Rights Reserved.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

struct MISharedLibrary {
	char * id;
	char * target_name;
	char * host_name;
	int symbols_loaded;
	char * thread_group;
	char * range_from;
	char * range_to;
};
typedef struct MISharedLibrary	MISharedLibrary;

extern MISource *	MISourceNew(void);
extern void			MISourceFree(MISource *);
extern MISources *	MISourcesNew(void);
extern void			MISourcesFree(MISources *);
extern MISharedLibrary *	MISharedLibraryNew(void);
extern void     			MISharedLibraryFree(MISharedLibrary *);
extern MISource *	MIGetFileListExecSourceFileInfo(MICommand *);
extern MIList *		MIGetFileListExecSourceFilesInfo(MICommand *);
extern MIList * 	MIGetFileListSharedLibrariesInfo(MICommand *);

#endif /* _MIFILE_H_ */
