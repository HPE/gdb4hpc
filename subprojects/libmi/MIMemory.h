/*******************************************************************************
 * Copyright (c) 2005 The Regents of the University of California. 
 * This material was produced under U.S. Government contract W-7405-ENG-36 
 * for Los Alamos National Laboratory, which is operated by the University 
 * of California for the U.S. Department of Energy. The U.S. Government has 
 * rights to use, reproduce, and distribute this software. NEITHER THE 
 * GOVERNMENT NOR THE UNIVERSITY MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR 
 * ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE. If software is modified 
 * to produce derivative works, such modified software should be clearly marked, 
 * so as not to confuse it with the version available from LANL.
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
 *******************************************************************************/
 
 /**
 * @author Clement chu
 * 
 */
 
#ifndef _MIMEMORY_H_
#define _MIMEMORY_H_

#include "MIList.h"
#include "MICommand.h"
#include "MIValue.h"

struct MIMemory {
	char *		addr;
	char *		ascii;
	MIList *	data;
};
typedef struct MIMemory	MIMemory;

struct MIDataReadMemoryInfo {
	char *		addr;
	long 		nextRow;
	long		prevRow;
	long 		nextPage;
	long 		prevPage;
	long 		numBytes;
	long 		totalBytes;
	MIList *	memories;
};
typedef struct MIDataReadMemoryInfo	MIDataReadMemoryInfo;

extern MIMemory *MIMemoryNew(void);
extern MIDataReadMemoryInfo *MIDataReadMemoryInfoNew(void);

extern void MIMemoryFree(MIMemory *memory);
extern void MIDataReadMemoryInfoFree(MIDataReadMemoryInfo *memoryInfo);

extern MIMemory *MIMemoryParse(MIValue *tuple);
extern MIList *MIMemoryDataParse(MIValue *miValue);

extern MIDataReadMemoryInfo *MIGetDataReadMemoryInfo(MICommand *cmd);
extern MIList *MIGetMemoryList(MIValue *miValue);

struct MIMemoryBytes {
	// e.g. -data-read-memory-bytes &a 10
	// ^done,memory=[{begin="0xbffff154",offset="0x00000000",
	//               end="0xbffff15e", contents="01000000020000000300"}]

	// beginning of memory region
	unsigned long	begin;
	// end of memory region. range is exclusive i.e. [begin, end).
	unsigned long	end;
	// offset from original start addr passed to command
	long	offset;
	// result of memory read. MI will parse this into raw bytes,
	// it is not the human readable `contents` string shown above
	unsigned char *	contents;
	unsigned long contents_len;
};
typedef struct MIMemoryBytes	MIMemoryBytes;

extern MIMemoryBytes *MIMemoryBytesNew();
extern void MIMemoryBytesFree(MIMemoryBytes *memory);

extern MIList* MIGetMemoryBytesInfo(MICommand *cmd);

#endif /* _MIMEMORY_H_ */
