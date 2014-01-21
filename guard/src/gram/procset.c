/*********************************************************************************\
 * Process Set Routines.
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
 *********************************************************************************/



#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"defs.h"
#include	"useful.h"
#include	"gram.h"
#include	"procset.h"
#include	"dbgevent.h"
#include 	"dbg_client.h"

list_t *	ProcSets = (list_t *)NULL;
dbgprocset_t *	CurrProcSet = (dbgprocset_t *)NULL;
dbgprocset_t *	AllProcSet = (dbgprocset_t *)NULL;
list_t		*OriginalSetInfoList = (list_t*)NULL;

void	_free_procset(dbgprocset_t *);
void	FreeOriginalSetInfo(dbg_ori_set_info_t* info);
void	RemoveOverlaySetFromOriginalSet(dbg_over_set_t* over_set);

void
InitProcSet(void)
{
	ProcSets = NewList();
	AllProcSet = NewProcSet("all");
	SetCurrentProcSet(AllProcSet);
	OriginalSetInfoList = NewList();
}

void
FinishProcSet(void)
{
	DestroyList(ProcSets, _free_procset);
	DestroyList(OriginalSetInfoList, FreeOriginalSetInfo);
}

/*
** Parse a process set.
*/
int
ParseProcSet(dbgprocset_t *s, char *str, rec_set_t *rec_set)
{
	list_t *	l;
	val_t *		v;
	expr_t *	e;
	dbgprocset_t *	procset = NULL;
	dbg_over_set_t*	over_set = NULL;
	invoke_t *invoke = NULL;
	invoke_group *ig = NULL;
	invoke_group *one_ig = NULL;
	bitset* group = (bitset *)NULL;
	int index;
	int	slice_min = -1, slice_max = -1;
	int	min_index = -1,  max_index = -1;

	if ( (e = ParseExpr(str)) == (expr_t *)NULL )
	{
		PrintOutError("Error: Procset found null expr.\n");
		return 0;
	}

	if ( EvalExprList(&l, e, (rec_set_t *)NULL) < 0 )
	{
		PrintOutError("Error: Procset eval expr failed.\n");
		FreeExpr(e);
		return 0;
	}

	FreeExpr(e);

	SetList(l);

	while ( (v = (val_t *)GetListElement(l)) != (val_t *)NULL )
	{
		if ( RVAL_ACTUAL_TYPE(v) != rval_proc )
		{
			PrintOutError("Invalid process set argument.\n");
			DestroyList(l, DestroyVal);
			return 0;
		}
		
		// null out procset
		procset = NULL;
		slice_min = -1;
		slice_max = -1;
		min_index = -1;
		max_index = -1;
		
		// assign procset directly
		procset = RVAL_ACTUAL_PROCSET(v);
		
		if ( RVAL_TYPE(v) == rval_ref )
		{
			// FIXME: This only handles single rank arrays right now, in the future
			// this might be changed.
			slice_min = RVAL_REF_SLICE(v)->s_low[0];
			slice_max = RVAL_REF_SLICE(v)->s_high[0];
		}
		
		
		if (procset != NULL)
		{
			DPRINT("ParseProcSet name:%s, slice:[%d..%d]\n", procset->s_name, slice_min, slice_max);
			over_set = GetOverlaySetForProcSet(procset);
			if(over_set == NULL)
			{
				PrintOutError("ERROR: ParseProcset cannot get OverlaySet for procset[%s]:[%p]\n", procset->s_name, procset);
				DestroyList(l, DestroyVal);
				return 0;
			}

			if( over_set->s_type != DBGSET_INVOKE)
			{
				PrintOutError("ERROR: ParseProcset does not support DBGSET_DEFSET OverlaySet!\n");
				DestroyList(l, DestroyVal);
				return 0;
			}

			if(group == (bitset*)NULL)
			{
				group = bitset_dup(over_set->s_set_bs);
				bitset_clear(group);

				min_index = slice_min + over_set->s_offset;
				max_index = slice_max + over_set->s_offset;
			}
			else
			{
				if(max_index < (slice_max + over_set->s_offset))
					max_index = slice_max + over_set->s_offset;

				if((slice_min + over_set->s_offset) < min_index)
					min_index = slice_min + over_set->s_offset;
			}

			invoke = over_set->s_invoke;
			ig = over_set->s_ig;

			if(one_ig == NULL)
				one_ig = ig;
			else if(ig != one_ig)
			{
				PrintOutError("defset only supports porcesses created by the same invoke group!");

				DestroyList(l, DestroyVal);
				return 0;
			}

			for(index = slice_min; index <= slice_max; index++) 
	           	bitset_set(group, index + over_set->s_offset);
		} else
		{
			PrintOutError("ERROR: ParseProcset cannot get procset\n");
			DestroyList(l, DestroyVal);
			return 0;
		}
	}

	AddOverlaySetToProcSet(s, invoke, ig, group, min_index, (max_index - min_index + 1), bitset_count(group), DBGSET_DEFSET); 
	bitset_free(group);

	DestroyList(l, (void (*)())NULL);

	return 1;
}

dbgprocset_t *
NewProcSet(char *name)
{
	dbgprocset_t *	s;

	s = (dbgprocset_t *)Alloc(sizeof(dbgprocset_t));
	s->s_name = strdup(name);
	s->s_nproc = 0;
	s->s_over_set_list = NewList();
	s->s_ig = NULL;
	s->s_indiv_proc = -1;

	AddToList(ProcSets, (void *)s);

	return s;
}

dbg_over_set_t*
NewOverlaySet(char* name, invoke_t* invoke, invoke_group* ig, bitset* bs, int offset, int len, int nproc, dbgset_type type)
{
	dbg_over_set_t* set = (dbg_over_set_t*)Alloc(sizeof(dbg_over_set_t));
	memset(set, 0, sizeof(dbg_over_set_t));

	set->s_setname = strdup(name);
	set->s_type = type;
	if(nproc > 0)
		set->s_nproc = nproc;

	set->s_invoke = invoke;
	set->s_set_bs = bitset_dup(bs);

	if(type == DBGSET_INVOKE)
	{
		set->s_ig = ig;
		if(offset > 0)
			set->s_offset = offset;

		if(len > 0)
			set->s_len = len;
	}

	if(type == DBGSET_DEFSET)
	{
		if(offset > 0)
			set->s_offset = offset;

		if(len > 0)
			set->s_len = len;
	}

	return set;
}

void
FreeOverlaySet(dbg_over_set_t* set)
{
	if(set != NULL)
	{
		Free(set->s_setname);
		set->s_setname = NULL;
		set->s_invoke = NULL;
		if(set->s_set_bs != NULL)
			bitset_free(set->s_set_bs);

		RemoveOverlaySetFromOriginalSet(set);
	}
}

dbg_ori_set_info_t*
NewOriginalSetInfo(invoke_t* ori_set, invoke_group* group)
{
	dbg_ori_set_info_t* info = (dbg_ori_set_info_t*)Alloc(sizeof(dbg_ori_set_info_t));
	info->s_invoke = ori_set;
	info->s_ig = group;
	info->s_over_set_list = NewList();

	return info;
}

void
FreeOriginalSetInfo(dbg_ori_set_info_t* info)
{
	if(info != NULL)
	{
		DestroyList(info->s_over_set_list, FreeOverlaySet);
		info->s_invoke = NULL;
		info->s_over_set_list = NULL;
	}
}

void
AddOverlaySetToOrignalSet(dbg_over_set_t* over_set)
{
	dbg_ori_set_info_t *info = NULL;
	list_t		*overSetList = NULL;

	if(over_set == NULL || OriginalSetInfoList == NULL || over_set->s_type != DBGSET_INVOKE)
		return;

	SetList(OriginalSetInfoList);
    while((info = (dbg_ori_set_info_t*)GetListElement(OriginalSetInfoList)) != (dbg_ori_set_info_t*)NULL)
    {
        if(info->s_invoke == over_set->s_invoke)
            break;
    }

	if(info == NULL)
	{
		info = NewOriginalSetInfo(over_set->s_invoke, over_set->s_ig);
		AddToList(OriginalSetInfoList, info);
	}

	overSetList = info->s_over_set_list;
	AddToList(overSetList, over_set);
}

void
RemoveOverlaySetFromOriginalSet(dbg_over_set_t* over_set)
{
	dbg_ori_set_info_t *info = NULL;
	list_t		*overSetList = NULL;

	if(over_set == NULL || OriginalSetInfoList == NULL)
		return;

	SetList(OriginalSetInfoList);
    while((info = (dbg_ori_set_info_t*)GetListElement(OriginalSetInfoList)) != (dbg_ori_set_info_t*)NULL)
    {
        if(info->s_ig == over_set->s_ig)
            break;
    }

	if(info != NULL)
	{
		overSetList = info->s_over_set_list;
		RemoveFromList(overSetList, over_set);
	}
}

list_t*
GetOverlaySetListByTree(void* tree)
{
	dbg_ori_set_info_t *info = NULL;

	SetList(OriginalSetInfoList);
    while((info = (dbg_ori_set_info_t*)GetListElement(OriginalSetInfoList)) != (dbg_ori_set_info_t*)NULL)
    {
        if(info->s_invoke->i_tree == tree)
            break;
    }

	if(info != NULL)
	{
		return info->s_over_set_list;
	}

	return NULL;
}

void
AddOverlaySetToAllProcSet(dbg_over_set_t* over_set)
{
	dbg_over_set_t *set = NULL;
//	dbgprocset_t *procset = NULL;

	if(over_set == NULL || AllProcSet == NULL || over_set->s_type != DBGSET_INVOKE)
		return;

	SetList(AllProcSet->s_over_set_list);
	while( (set = (dbg_over_set_t*)GetListElement(AllProcSet->s_over_set_list)) != (dbg_over_set_t*)NULL )
	{
		if(set->s_ig == over_set->s_ig)
			break;	
	}

	if(set == NULL)
	{
		AddToList(AllProcSet->s_over_set_list, over_set);
		AllProcSet->s_nproc += over_set->s_nproc;
	}
}

void
AddOverlaySetToProcSet(dbgprocset_t *s, invoke_t* invoke, invoke_group* ig, bitset *p, int offset, int len, int nproc, dbgset_type type)
{
	dbg_over_set_t *over_set = NULL;

	if(strcmp(s->s_name, "all") == 0)
	{
		PrintOutError("WARNING: \"all\" is a system procset!\n");
		return;
	}

	if(SizeOfList(s->s_over_set_list) == 0)
	{
		over_set = NewOverlaySet(s->s_name, invoke, ig, p, offset, len, nproc, type);
		AddToList(s->s_over_set_list, over_set);
		AddOverlaySetToOrignalSet(over_set);
		s->s_nproc += nproc;
		s->s_ig = ig;	//TODO: remove me

		AddOverlaySetToAllProcSet(over_set);
	} else {
		PrintOutError("WARNING: OverlaySet of %s has already been created!\n", s->s_name);
	}

	DPRINT("AddOverlaySetToProcSet: over_set[%p], [%s], invoke:[%p], offset[%d], procnum[%d], bs%s\n", over_set, s->s_name, invoke, offset, nproc, bitset_to_set(p));
}

void
_free_procset(dbgprocset_t *s)
{
	DestroyList(s->s_over_set_list, FreeOverlaySet); 
	s->s_over_set_list = NULL;
	s->s_ig = NULL;

	Free(s->s_name);
	Free(s);
}

void
FreeProcSet(dbgprocset_t *s)
{
	if ( s == (dbgprocset_t *)NULL || AllProcSet == (dbgprocset_t *)NULL )
		return;

	dbg_over_set_t* over_set;
	if(strcmp(s->s_name, "all") != 0)
	{
		SetOverlaySetForProcSet(s);
		while((over_set = GetNextOverlaySetForProcSet(s)) != (dbg_over_set_t*)NULL)
		{
			if(over_set->s_type == DBGSET_INVOKE)
			{
				RemoveFromList(AllProcSet->s_over_set_list, over_set);
				AllProcSet->s_nproc -= s->s_nproc;
			}
		}
	}

	RemoveFromList(ProcSets, (void *)s);

	_free_procset(s);
}

dbgprocset_t *
FindProcSet(char *name)
{
	dbgprocset_t *	s;

	if ( ProcSets == (list_t *)NULL )
		return (dbgprocset_t *)NULL;

	SetList(ProcSets);

	while ( (s = (dbgprocset_t *)GetListElement(ProcSets)) != (dbgprocset_t *)NULL )
		if ( strcmp(s->s_name, name) == 0 )
			return s;

	return (dbgprocset_t *)NULL;
}

void
SetCurrentProcSet(dbgprocset_t *s)
{
	CurrProcSet = s;
}

dbgprocset_t *
CurrentProcSet(void)
{
	return CurrProcSet;
}

void
SetProcSet(void)
{
	SetList(ProcSets);
}

dbgprocset_t *
GetProcSet(void)
{
	return (dbgprocset_t *)GetListElement(ProcSets);
}

void
PrintProcSet(FILE *fp, dbgprocset_t *s)
{
	int		n = 0;
	int	print_flag;
	list_t* innerSetList = NULL;
	dbg_over_set_t *	over_set;
	dbg_over_set_t *	inner_set;

	if ( EmptyList(s->s_over_set_list) )
	{
		PrintOutMsg("none");
		return;
	}

	SetList(s->s_over_set_list);

	while ( (over_set = (dbg_over_set_t *)GetListElement(s->s_over_set_list)) != (dbg_over_set_t *)NULL )
	{
		if(over_set->s_type == DBGSET_INVOKE)
		{
			PrintProcSetInfoByOffset(fp, over_set->s_setname, over_set->s_set_bs, over_set->s_offset, over_set->s_nproc);
		}
		else if (over_set->s_type == DBGSET_DEFSET)
		{
			innerSetList = (list_t*)GetOverlaySetListByTree(over_set->s_invoke->i_tree);
			print_flag = 0;
			SetList(innerSetList);
			while((inner_set = (dbg_over_set_t*)GetListElement(innerSetList)) != (dbg_over_set_t*)NULL)
			{
				if(bitset_compare(inner_set->s_set_bs, over_set->s_set_bs) != 0)
				{
					if(print_flag)
						PrintToFd(fp, ",");
					else
						print_flag = 1;

					PrintProcSetInfoByOffset(fp, inner_set->s_setname, over_set->s_set_bs, inner_set->s_offset, inner_set->s_nproc);
				}
			}
		}
			
		n += bitset_count(over_set->s_set_bs);
		if ( n < s->s_nproc )
			PrintToFd(fp, ",");
	}
}

int
NumProcs(void)
{
	return AllProcSet->s_nproc;
}

void
SetOverlaySetForProcSet(dbgprocset_t *s)
{
	SetList(s->s_over_set_list);
}

dbg_over_set_t*
GetNextOverlaySetForProcSet(dbgprocset_t *s)
{
	dbg_over_set_t* set = GetListElement(s->s_over_set_list);
	return set;
}

dbg_over_set_t*
GetOverlaySetForProcSet(dbgprocset_t *s)
{
	dbg_over_set_t* set = NULL;

	if(s == NULL)
		return NULL;

 	if(strcmp(s->s_name, "all") == 0)
	{
		PrintOutError("WARNING: GetOverlaySetForProcSet is not supposed to invoke on \"all\"\n");
		return NULL;
	}

	SetList(s->s_over_set_list);
	set = GetListElement(s->s_over_set_list);

	return set;
}

