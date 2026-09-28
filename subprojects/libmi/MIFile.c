/******************************************************************************\
 * MIFile.c - Extended libmi support for -file-* commands.
 *
 * Copyright 2013-2014 Cray Inc. All Rights Reserved.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MIResult.h"
#include "MIString.h"
#include "MIList.h"
#include "MIValue.h"
#include "MIFile.h"

MISource *
MISourceNew(void)
{
	MISource *	source;

	source = (MISource *)malloc(sizeof(MISource));
	source->line = 0;
	source->file = NULL;
	source->fullname = NULL;
	source->macro_info = 0;
	
	return source;
}

void
MISourceFree(MISource *source)
{
	if (source->file != NULL)
		free(source->file);
	if (source->fullname != NULL)
		free(source->fullname);
	free(source);
}

MISources *
MISourcesNew(void)
{
	MISources *	sources;

	sources = (MISources *)malloc(sizeof(MISources));
	sources->file = NULL;
	sources->fullname = NULL;
	
	return sources;
}

void
MISourcesFree(MISources *sources)
{
	if (sources->file != NULL)
		free(sources->file);
	if (sources->fullname != NULL)
		free(sources->fullname);
	free(sources);
}

MISharedLibrary *
MISharedLibraryNew(void)
{
	MISharedLibrary *	shared_library;

	shared_library = (MISharedLibrary *)malloc(sizeof(MISharedLibrary));
	shared_library->id = NULL;
	shared_library->target_name = NULL;
	shared_library->host_name = NULL;
	shared_library->symbols_loaded = 0;
	shared_library->thread_group = NULL;
	shared_library->range_from = NULL;
	shared_library->range_to = NULL;

	return shared_library;
}

void
MISharedLibraryFree(MISharedLibrary *shared_library)
{
	if (shared_library->id != NULL)
		free(shared_library->id);
	if (shared_library->target_name != NULL)
		free(shared_library->target_name);
	if (shared_library->host_name != NULL)
		free(shared_library->host_name);
	if (shared_library->thread_group != NULL)
		free(shared_library->thread_group);
	if (shared_library->range_from != NULL)
		free(shared_library->range_from);
	if (shared_library->range_to != NULL)
		free(shared_library->range_to);
	free(shared_library);
}

MISource *
MIGetFileListExecSourceFileInfo(MICommand *cmd)
{
	MIResultRecord *	rr;
	MIResult *			result;
	MISource *			source = MISourceNew();

	if (!cmd->completed || cmd->output == NULL || cmd->output->rr == NULL)
		return NULL;

	rr = cmd->output->rr;
	
	for (MIListSet(rr->results); (result = (MIResult *)MIListGet(rr->results)) != NULL; ) 
	{
		if (result->value == NULL || result->value->type != MIValueTypeConst)
			continue;
		
		if (strcmp(result->variable, "line") == 0)
		{
			source->line = (int)strtol(result->value->cstring, NULL, 10);
		} else if (strcmp(result->variable, "file") == 0)
		{
			source->file = strdup(result->value->cstring);
		} else if (strcmp(result->variable, "fullname") == 0)
		{
			source->fullname = strdup(result->value->cstring);
		} else if (strcmp(result->variable, "macro-info") == 0)
		{
			source->macro_info = (int)strtol(result->value->cstring, NULL, 10);
		}
	}
	
	return source;
}

static MISources *
MISourcesParse(MIValue *tuple)
{
	MIList *	results = tuple->results;
	MIResult *	result;
	MISources *	sources = MISourcesNew();
	
	for (MIListSet(results); (result = (MIResult *)MIListGet(results)) != NULL; )
	{
		if (result->value == NULL || result->value->type != MIValueTypeConst)
			continue;
			
		if (strcmp(result->variable, "file") == 0)
		{
			sources->file = strdup(result->value->cstring);
		} else if (strcmp(result->variable, "fullname") == 0)
		{
			sources->fullname = strdup(result->value->cstring);
		}
	}
	
	return sources;
}

MIList *
MIGetFileListExecSourceFilesInfo(MICommand *cmd)
{
	MIResultRecord *	rr;
	MIResult *			result;
	MIValue *			res_val;
	MIValue *			val;
	MIList *			sources = MIListNew();

	if (!cmd->completed || cmd->output == NULL || cmd->output->rr == NULL)
		return NULL;
		
	rr = cmd->output->rr;
	
	for (MIListSet(rr->results); (result = (MIResult *)MIListGet(rr->results)) != NULL; ) 
	{
		if (strcmp(result->variable, "files") == 0)
		{
			val = result->value;
			
			// only expect to have a list of tuple values here
			if (val->type == MIValueTypeList)
			{
				for (MIListSet(val->values); (res_val = (MIValue *)MIListGet(val->values)) != NULL; )
				{	
					MIListAdd(sources, MISourcesParse(res_val));
				}
			}
		}
	}
	
	return sources;
}

MIList *
MIGetFileListSharedLibrariesInfo(MICommand *cmd)
{
	MIResultRecord *	command_rr = NULL;
	MIResult *			command_result = NULL;

	MIList *shared_libraries_values = NULL;
	MIValue *shared_library_value = NULL;
	MIList *shared_library_results = NULL;
	MIResult *shared_library_result = NULL;
	MISharedLibrary *shared_library = NULL;
	MIValue *ranges_value = NULL;

	MIList *			shared_libraries = MIListNew();

	if (!cmd->completed || cmd->output == NULL || cmd->output->rr == NULL)
		return NULL;

	command_rr = cmd->output->rr;

	// Find the 'shared-libraries' values in results dictionary
	for (MIListSet(command_rr->results); (command_result = (MIResult *)MIListGet(command_rr->results)) != NULL; )
	{
		if (strcmp(command_result->variable, "shared-libraries") == 0)
		{
			shared_libraries_values = command_result->value->values;
			break;
		}
	}
	if (shared_libraries_values == NULL)
	{
		return NULL;
	}

	for (MIListSet(shared_libraries_values); (shared_library_value = (MIValue *)MIListGet(shared_libraries_values)) != NULL; )
	{
		shared_library_results = shared_library_value->results;
		shared_library = MISharedLibraryNew();

		for (MIListSet(shared_library_results); (shared_library_result = (MIResult *)MIListGet(shared_library_results)) != NULL; )
		{
			if (strcmp(shared_library_result->variable, "id") == 0)
			{
				shared_library->id = strdup(shared_library_result->value->cstring);
			} else if (strcmp(shared_library_result->variable, "target-name") == 0)
			{
				shared_library->target_name = strdup(shared_library_result->value->cstring);
			} else if (strcmp(shared_library_result->variable, "host-name") == 0)
			{
				shared_library->host_name = strdup(shared_library_result->value->cstring);
			} else if (strcmp(shared_library_result->variable, "symbols-loaded") == 0)
			{
				shared_library->symbols_loaded = (strcmp(shared_library_result->variable, "1") == 0);
			} else if (strcmp(shared_library_result->variable, "thread-group") == 0)
			{
				shared_library->thread_group = strdup(shared_library_result->value->cstring);
			} else if (strcmp(shared_library_result->variable, "ranges") == 0)
			{
				MIListSet(shared_library_result->value->values);
				ranges_value = (MIValue *)MIListGet(shared_library_result->value->values);
				if (ranges_value->type == MIValueTypeTuple)
				{
					MIList *range_dict = ranges_value->results;

					MIResult *range_kv = NULL;
					char const *key = NULL;
					MIValue *value_container;
					char const *value = NULL;
					for (MIListSet(range_dict); (range_kv = (MIResult *)MIListGet(range_dict)) != NULL; )
					{
						key = range_kv->variable;
						value_container = range_kv->value;
						if (value_container != NULL || value_container->type == MIValueTypeConst)
						{
							value = value_container->cstring;
						}

						if (strcmp(key, "from") == 0 && value != NULL)
						{
							shared_library->range_from = strdup(value);
						} else if (strcmp(key, "to") == 0 && value != NULL)
						{
							shared_library->range_to = strdup(value);
						}
					}
				}
			}
		}

		if (shared_libraries == NULL)
			shared_libraries = MIListNew();
		MIListAdd(shared_libraries, (void *)shared_library);
	}

	return shared_libraries;
}
