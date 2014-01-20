/*********************************************************************************\
 * Assertion information for CompareData().
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
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
**********************************************************************************/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<string.h>

#include	"defs.h"
#include	"useful.h"
#include	"aif.h"
#include	"df.h"
#include	"ainfo.h"

ainfo_t **
NewAssertInfoTable(void)
{
	ainfo_t **      ai_tab;
	
	ai_tab = (ainfo_t **)Alloc(sizeof(ainfo_t *) * MAX_ASSERTS);
	memset((void *)ai_tab, 0, sizeof(ainfo_t *) * MAX_ASSERTS);
	
	return ai_tab;
}

ainfo_t *
NewAssertInfoEntry(ainfo_t **ai_tab, int id)
{
	ainfo_t *	a;
	
	if (ai_tab == (ainfo_t **)NULL)
		return (ainfo_t *)NULL;
	
	if ((id < 0) || (id >= MAX_ASSERTS))
		return (ainfo_t *)NULL;
		
	if (ai_tab[id] != (ainfo_t *)NULL)
		return ai_tab[id];

	a = (ainfo_t *)Alloc(sizeof(ainfo_t));
	
	a->a_display = NULL;
	a->a_file = NULL;
	a->a_aifupper = NULL;
	a->a_aiflower = NULL;
	a->a_stop = 1;

	ai_tab[id] = a;

	return a;
}

void
DestroyAssertInfo(ainfo_t **ai_tab, int ids)
{
	ainfo_t *	a;
	int i;
	
	if (ai_tab == (ainfo_t **)NULL)
		return;
	
	if (ids > MAX_ASSERTS)
		ids = MAX_ASSERTS;
	
	for (i=0; i<ids; i++)
	{
		if ((a = ai_tab[i]) == (ainfo_t *)NULL)
			continue;
		
		if ( a->a_display != NULL )
			Free(a->a_display);
		if ( a->a_file != NULL )
			Free(a->a_file);
		if(a->a_aifupper != NULL)
			AIFFree(a->a_aifupper);
		if(a->a_aiflower != NULL)
			AIFFree(a->a_aiflower);
			
		Free(a);
	}
	
	Free(ai_tab);
}

ainfo_t *
FindAssertInfo(ainfo_t **ai_tab, int id)
{
	if (ai_tab == (ainfo_t **)NULL)
		return (ainfo_t *)NULL;

	if ( id < 0 || id >= MAX_ASSERTS )
		return (ainfo_t *)NULL;

	return ai_tab[id];
}
