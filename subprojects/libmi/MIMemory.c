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

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MIValue.h"
#include "MIResult.h"
#include "MIMemory.h"

MIMemory * 
MIMemoryNew(void)
{
	MIMemory *	memory;
	memory = (MIMemory *)malloc(sizeof(MIMemory));
	memory->addr = NULL;
	memory->ascii = NULL;
	memory->data = NULL;
	return memory;	
}

void 
MIMemoryFree(MIMemory *memory)
{
	if (memory->addr != NULL)
		free(memory->addr);
	if (memory->ascii != NULL)
		free(memory->ascii);
	if (memory->data != NULL)
		MIListFree(memory->data, free);
	free(memory);
}

MIMemory *
MIMemoryParse(MIValue *tuple)
{
	char *		var;
	MIValue *	value;
	MIResult *	result;
	MIList *	results = tuple->results;
	MIMemory *	memory = MIMemoryNew();
	
	for (MIListSet(results); (result = (MIResult *)MIListGet(results)) != NULL;) {
		var = result->variable;
		value = result->value;
	
		if (strcmp(var, "addr") == 0) {
			memory->addr = strdup(value->cstring);
		} else if (strcmp(var, "ascii") == 0) {
			memory->ascii = strdup(value->cstring);
		} else if (strcmp(var, "data") == 0) {
			if (value != NULL) {
				memory->data = MIMemoryDataParse(value);
			}
		}
	}
	return memory;
}

MIList *
MIMemoryDataParse(MIValue* miValue)
{
	MIList *	data = MIListNew();
	MIList *	values = miValue->values;
	MIValue * 	value;
	
	if (values != NULL) {
		for (MIListSet(values); (value = (MIValue *)MIListGet(values)) != NULL; ) {
			if (value->type == MIValueTypeConst) {
				MIListAdd(data, (void *) strdup(value->cstring));
			}
		}
	}
	return data;
}

MIDataReadMemoryInfo *
MIDataReadMemoryInfoNew(void)
{
	MIDataReadMemoryInfo *	memoryInfo;
	memoryInfo = (MIDataReadMemoryInfo *)malloc(sizeof(MIDataReadMemoryInfo));
	memoryInfo->addr = NULL;
	memoryInfo->memories = NULL;
	return memoryInfo;	
}

void
MIDataReadMemoryInfoFree(MIDataReadMemoryInfo *memoryInfo)
{
	if (memoryInfo->addr != NULL)
		free(memoryInfo->addr);
	if (memoryInfo->memories != NULL)
		MIListFree(memoryInfo->memories, MIMemoryFree);
	free(memoryInfo);
}

MIDataReadMemoryInfo *
MIGetDataReadMemoryInfo(MICommand *cmd)
{
	char * var;
	MIValue * value;
	MIResultRecord * rr;
	MIResult * result;
	MIDataReadMemoryInfo * info = MIDataReadMemoryInfoNew();

	if (!cmd->completed || cmd->output == NULL || cmd->output->rr == NULL) {
		return NULL;
	}
	
	rr = cmd->output->rr;
	for (MIListSet(rr->results); (result = (MIResult *)MIListGet(rr->results)) != NULL; ) {
		var = result->variable;
		value = result->value;

		if (strcmp(var, "addr") == 0) {
			info->addr = strdup(value->cstring);
		} else if (strcmp(var, "nr-bytes") == 0) {
			info->numBytes = strtol(value->cstring, NULL, 10);
		} else if (strcmp(var, "total-bytes") == 0) {
			info->totalBytes = strtol(value->cstring, NULL, 10);
		} else if (strcmp(var, "next-row") == 0) {
			info->nextRow = strtol(value->cstring, NULL, 10);
		} else if (strcmp(var, "prev-row") == 0) {
			info->prevRow = strtol(value->cstring, NULL, 10);
		} else if (strcmp(var, "next-page") == 0) {
			info->nextPage = strtol(value->cstring, NULL, 10);
		} else if (strcmp(var, "prev-page") == 0) {
			info->prevPage = strtol(value->cstring, NULL, 10);
		} else if (strcmp(var, "memory") == 0) {
			if (value->type == MIValueTypeList) {
				info->memories = MIGetMemoryList(value);
			}
		}
	}	
	return info;
}

MIList *
MIGetMemoryList(MIValue *miValue)
{
	MIList *	memories = MIListNew();
	MIList *	values = miValue->values;
	MIValue *	value;
	
	if (values != NULL) {
		for (MIListSet(values); (value = (MIValue *)MIListGet(values)) != NULL;) {
			if (value->type == MIValueTypeTuple) {
				MIListAdd(memories, (void *)MIMemoryParse(value));
			}
		}
	}
	return memories;
}

MIMemoryBytes *
MIMemoryBytesNew()
{
	MIMemoryBytes* memory = (MIMemoryBytes *)malloc(sizeof(MIMemoryBytes));
	memory->begin = 0;
	memory->end = 0;
	memory->offset = 0;
	memory->contents = NULL;
	memory->contents_len = 0;
	return memory;	
}

void
MIMemoryBytesFree(MIMemoryBytes *memory)
{
	free(memory->contents);
	memory->contents = NULL;
	free(memory);
}

static unsigned char ascii_to_nibble(char c) {
	if ('0' <= c && c <= '9') {
		return c - '0';
	} else if ('a' <= c && c <= 'f') {
		return c - 'a' + 10;
	} else {
		return c - 'A' + 10;
	}
}

// `val` must have MIValueTypeTuple and a non-NULL `results` field
MIMemoryBytes* parse_memory_bytes(MIValue* val) {
	if (val == NULL || val->type != MIValueTypeTuple || val->results == NULL) return NULL;

	MIMemoryBytes* parsed_mem = MIMemoryBytesNew();

	// e.g. -data-read-memory-bytes &a 10
	// ^done,memory=[{begin="0xbffff154",offset="0x00000000",
	//               end="0xbffff15e", contents="01000000020000000300"}]
	for (MIResult* iter; (iter = (MIResult*)MIListGet(val->results)) != NULL; ){
		if (iter->variable == NULL) continue;

		if (strcmp(iter->variable, "begin") == 0) {
			parsed_mem->begin = strtoul(iter->value->cstring, NULL, 16);
		} else if (strcmp(iter->variable, "end") == 0) {
			parsed_mem->end = strtoul(iter->value->cstring, NULL, 16);
		} else if (strcmp(iter->variable, "offset") == 0) {
			parsed_mem->offset = strtol(iter->value->cstring, NULL, 16);
		} else if (strcmp(iter->variable, "contents") == 0) {
			char* contents_str = iter->value->cstring;
			size_t contents_str_len = strlen(contents_str);
			// each byte is printed as two hex digits in contents_str
			parsed_mem->contents_len = contents_str_len / 2;
			parsed_mem->contents = malloc(parsed_mem->contents_len);
			for (size_t i = 0; i < parsed_mem->contents_len; i++) {
				unsigned char hi_nibble = ascii_to_nibble(contents_str[i * 2]);
				unsigned char lo_nibble = ascii_to_nibble(contents_str[i * 2 + 1]);
				parsed_mem->contents[i] = (hi_nibble << 4) | lo_nibble;
			}
		}
	}

	return parsed_mem;
}

MIList*
MIGetMemoryBytesInfo(MICommand *cmd) {
	if (!cmd || !cmd->completed || cmd->output == NULL || cmd->output->rr == NULL) {
		return NULL;
	}

	MIResultRecord* rr = cmd->output->rr;
	MIList* result = MIListNew();

	MIListSet(rr->results);
	for (MIResult* rr_iter; (rr_iter = (MIResult *)MIListGet(rr->results)) != NULL; ) {
		// actually only expecting a single result with "memory" key
		if (strcmp(rr_iter->variable, "memory") != 0) continue;

		if (rr_iter->value == NULL || rr_iter->value->values == NULL) continue;
		MIList* mem_tuples = rr_iter->value->values;
		MIListSet(mem_tuples);

		for (MIValue* mem_iter; (mem_iter = (MIValue*)MIListGet(mem_tuples)) != NULL; ) {
			if (mem_iter->results == NULL) continue;
			MIListSet(mem_iter->results);

			MIMemoryBytes* parsed = parse_memory_bytes(mem_iter);
			if (parsed == NULL) continue;
			MIListAdd(result, parsed);
		}
	}

	return result;
}
