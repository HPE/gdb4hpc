/*********************************************************************************\
 * Process Set Routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2014 Cray Inc.  All Rights Reserved.
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

// static prototypes
static dbgprocset_t *		NewProcSet(char *, dbgset_type);
static dbg_over_set_t *		NewOverlaySet(dbginvoke_t *, bitset *, int);
static void					FreeOverlaySet(dbg_over_set_t *);
static void					AddOverlaySetToAllProcSet(dbg_over_set_t *);
static void					_free_procset(dbgprocset_t *);
static void					_print_with_set_info(char *, bitset *, int, int, int, int, char *, va_list);

// Internal variables
static list_t *			ProcSets = NULL;
static dbgprocset_t *	CurrProcSet = NULL;
static dbgprocset_t *	AllProcSet = NULL;

void
InitProcSet(void)
{
	expr_t *	ex;
	val_t *		val;
	
	ProcSets = NewList();
	
	// force all procset to be created
	if ( (ex = ParseExpr("$all")) == NULL )
	{
		PrintError();
		return;
	}
	
	if( EvalExpr(&val, ex, NULL) < 0 )
	{
		PrintError();
		FreeExpr(ex);
		return;
	}
	
	FreeExpr(ex);
	
	AllProcSet = NewProcSet("all", DBGSET_INTERNAL);
	SetCurrentProcSet(AllProcSet);
	
	// Fixup the val
	RVAL_TYPE(val) = rval_proc;
	// Assign the procset directly
	RVAL_PROCSET(val) = AllProcSet;
	// Important! Set RVAL_DESTROY to false
	RVAL_DESTROY(val) = 0;
}

void
FinishProcSet(void)
{
	DestroyList(ProcSets, _free_procset);
}

/*
** Create a process set from a procset list.
*/
int
CreateProcSet(expr_t *e_name, expr_t *e_set)
{
	val_t *				v_name = NULL;
	dbgprocset_t *		new_procset;
	list_t *			l;
	val_t *				v;
	dbgprocset_t *		procset = NULL;
	bitset *			di_mask = NULL;
	dbg_over_set_t *	over_set = NULL;
	dbg_over_set_t *	o_ptr = NULL;
	int 				index;
	int					useSlice = 0;
	int					slice_min = -1, slice_max = -1;
	int					ps_offset = 0;
	
	if (e_name == NULL || e_set == NULL)
	{
		PrintOutError("Error: Create Procset had null args.\n");
		return 0;
	}
	
	// Eval the name and make sure it is acceptable.
	// After calling EvalExpr on the new procset, we need to call RemoveVal
	// to ensure proper cleanup if something goes wrong and this creates a new
	// dbg variable for us. We only do this if EvalExpr returns a rval_none val
	// meaning that there was not a name collision.
	if( EvalExpr(&v_name, e_name, NULL) < 0 )
	{
		PrintError();
		return 0;
	}
	
	if (LVAL_TYPE(v_name) != lval_internal)
	{
		PrintOutError("Must specify debugger variable for process handle.\n");
		return 0;
	}
	
	switch (RVAL_ACTUAL_TYPE(v_name))
	{
		case rval_ref:
			// This shouldn't be handled technically since we are using RVAL_ACTUAL_TYPE
		case rval_none:
			// This is what we expect the val to be
			break;
			
		case rval_aif:
			PrintOutError("A debugger variable named %s is already defined.\n", SymLookup(LVAL_SYM(v_name)));
			return 0;
		
		case rval_proc:
			PrintOutError("A procset named %s is already defined.\n", SymLookup(LVAL_SYM(v_name)));
			return 0;
			
		case rval_blockmap:
			PrintOutError("A decomposition named %s is already defined.\n", SymLookup(LVAL_SYM(v_name)));
			return 0;
			
		case rval_graph:
			PrintOutError("An assertion script named %s is already defined.\n", SymLookup(LVAL_SYM(v_name)));
			return 0;
			
		case rval_reply:
			PrintOutError("An application variable symbol named %s already exists.\n", SymLookup(LVAL_SYM(v_name)));
			return 0;
	}
	
	// create the new procset
	new_procset = NewProcSet(SymLookup(LVAL_SYM(v_name)), DBGSET_DEFSET);
	
	// Now eval the provided lists of procsets
	if ( EvalExprList(&l, e_set, NULL) < 0 )
	{
		PrintOutError("Error: Procset eval expr failed.\n");
		FreeProcSet(new_procset);
		RemoveVal(LVAL_SYM(v_name));
		DestroyVal(v_name);
		return 0;
	}

	SetList(l);
	while ((v = (val_t *)GetListElement(l)) != NULL)
	{
		if ( RVAL_ACTUAL_TYPE(v) != rval_proc )
		{
			PrintOutError("Invalid process set argument.\n");
			DestroyList(l, NULL);
			FreeProcSet(new_procset);
			RemoveVal(LVAL_SYM(v_name));
			DestroyVal(v_name);
			return 0;
		}
		
		// assign procset directly
		procset = RVAL_ACTUAL_PROCSET(v);
		
		if ( RVAL_TYPE(v) == rval_ref )
		{
			// FIXME: This only handles single rank arrays right now, in the future
			// this might be changed.
			slice_min = RVAL_REF_SLICE(v)->s_low[0];
			slice_max = RVAL_REF_SLICE(v)->s_high[0];
			useSlice = 1;
		}
		
		if (procset != NULL)
		{
			// We want to force this to only operate on invoke sets since those will only contain a single
			// overlay set. It gets tricky to compute otherwise.
			// TODO: If invoke sets ever allow an offset, this function will need to be fixed.
			if( procset->s_type != DBGSET_INVOKE)
			{
				PrintOutError("ERROR: ParseProcset does not support DBGSET_DEFSET procset!\n");
				DestroyList(l, NULL);
				FreeProcSet(new_procset);
				RemoveVal(LVAL_SYM(v_name));
				DestroyVal(v_name);
				return 0;
			}
			
			DPRINT("ParseProcSet name:%s, slice:[%d..%d]\n", procset->s_name, slice_min, slice_max);
			
			if ((over_set = GetOverlaySetForProcSet(procset)) == NULL)
			{
				PrintOutError("ERROR: ParseProcset cannot get OverlaySet for procset %s.\n", procset->s_name);
				DestroyList(l, NULL);
				FreeProcSet(new_procset);
				RemoveVal(LVAL_SYM(v_name));
				DestroyVal(v_name);
				return 0;
			}
			
			// grab the procset for this invoke and clear it, we will recompute
			di_mask = bitset_dup(over_set->s_di_bs);
			
			if (useSlice)
			{
				// The user provided a sliced definition for this rank, so we need to compute
				bitset_clear(di_mask);
				for (index = slice_min; index <= slice_max; ++index)
				{
					bitset_set(di_mask, index);
				}
				
			}
			
			// If the user didn't provide a slice, then we want to use the entire bitset "as is".
			
			// Check to see if an overlay set already exists for this di in the new_procset
			SetOverlaySetForProcSet(new_procset);
			while ((o_ptr = GetNextOverlaySetForProcSet(new_procset)) != NULL)
			{
				if (o_ptr->s_di == over_set->s_di)
				{
					// found an existing overlay set for this di
					break;
				}
			}
			
			// Update the existing overlay set if found
			if (o_ptr != NULL)
			{
				bitset_oreq(o_ptr->s_di_bs, di_mask);
				bitset_free(di_mask);
			} else
			{
				// Create a new overlay set for this invoke
				// Note that the offset gets updated on the second pass
				o_ptr = NewOverlaySet(over_set->s_di, di_mask, 0);
				AddToList(new_procset->s_over_set_list, over_set);
			}
		} else
		{
			PrintOutError("ERROR: ParseProcset cannot get procset\n");
			DestroyList(l, NULL);
			FreeProcSet(new_procset);
			RemoveVal(LVAL_SYM(v_name));
			DestroyVal(v_name);
			return 0;
		}
	}
	
	// Free the list from the eval
	DestroyList(l, NULL);
	
	// Second pass - need to update the offsets for each overlay set
	ps_offset = 0;
	SetOverlaySetForProcSet(new_procset);
	while ((o_ptr = GetNextOverlaySetForProcSet(new_procset)) != NULL)
	{
		o_ptr->s_ps_offset = ps_offset;
		ps_offset += bitset_numset(o_ptr->s_di_bs);
	}
	
	// Update the nproc in the procset
	new_procset->s_nproc = ps_offset;
	
	// Fixup the v_name val
	RVAL_TYPE(v_name) = rval_proc;
	// Assign the procset directly
	RVAL_PROCSET(v_name) = new_procset;
	// Important! Set RVAL_DESTROY to false
	RVAL_DESTROY(v_name) = 0;

	return 1;
}

/*
** Create an invoke procset given an internal variable with rval_none. This internal
** variable can contain slicing information. This is the first step for launching
** or attaching. Use this to determine the handle name and number of ranks.
**
** This should be extended to support multi-dimensional procsets with the slice.
** XXX: This only works for a single handle (i.e. $a), not a list of handles.
*/
dbgprocset_t *
CreateProcSetForInvoke(expr_t *e)
{
	val_t *			val;
	dbgprocset_t *	rtn;
	slice_t *		proc_slice;
	int				i, nproc = 0;

	if ( EvalExpr(&val, e, NULL) < 0 )
	{
		PrintError();
		return NULL;
	}
	
	if (LVAL_TYPE(val) != lval_internal)
	{
		PrintOutError("Must specify debugger variable for process handle.\n");
		return NULL;
	}
	
	switch ( RVAL_ACTUAL_TYPE(val) )
	{
		case rval_graph:
			PrintOutError("An assertion script has already been defined using this name.\n");
			return NULL;
			
		case rval_proc:
			PrintOutError("A process set has already been defined using this name.\n");
			return NULL;
			
		case rval_blockmap:
			PrintOutError("A decomposition has already been defined using this name.\n");
			return NULL;
			
		case rval_aif:
			PrintOutError("A debugger variable has already been defined using this name.\n");
			return NULL;
			
		case rval_reply:
			PrintOutError("A reply has already been defined using this name.\n");
			return NULL;
			
		case rval_ref:	// This shouldn't happen since we called actual type
		case rval_none:
			// we expect the type to be rval_none
			break;
	}
	
	// create the new procset
	rtn = NewProcSet(SymLookup(LVAL_SYM(val)), DBGSET_INVOKE);
	
	// Calculate the number of ranks in the provided list
	// TODO: Add in the ability to have multi-dimensional handles for threading
	if (RVAL_TYPE(val) == rval_ref)
	{
		// Note that we only expect the user to be providing us with SLICE_POS
		// type of slices, that is $a{n} where n is the number of ranks. This
		// syntax is unique to launch only.
		proc_slice = RVAL_REF_SLICE(val);
		
		// TODO: Remove the following when we allow multi-dimensional handles
		if (proc_slice->s_rank > 1)
		{
			PrintOutError("Invalid proc handle syntax. Expected $handle{n} where n is the number of application ranks.\n");
			FreeProcSet(rtn);
			RemoveVal(LVAL_SYM(val));
			DestroyVal(val);
			return NULL;
		}
		
		for (i=0; i < proc_slice->s_rank; ++i)
		{
			if (proc_slice->s_type[i] != SLICE_POS)
			{
				PrintOutError("Invalid proc handle syntax. Expected $handle{n} where n is the number of application ranks.\n");
				FreeProcSet(rtn);
				RemoveVal(LVAL_SYM(val));
				DestroyVal(val);
				return NULL;
			}
			nproc += proc_slice->s_high[i];
		}
		
		rtn->s_nproc = nproc;
	} else
	{
		// Always default to one rank
		rtn->s_nproc = 1;
	}
	
	// We are going to turn this debugger variable into a procset rval. Destroy
	// the old value, including discarding any slicing info.
	DestroyRval(RVAL_TYPE(val), RVAL_VAL(val));
	// Now turn the rval into an rval_proc
	RVAL_TYPE(val) = rval_proc;
	// Set the procset in the val
	RVAL_PROCSET(val) = rtn;
	// Important! Set RVAL_DESTROY to false since it will be true here since
	// the original rval was a ref type
	RVAL_DESTROY(val) = 0;
	
	rtn->s_init = 0;

	return rtn;
}

/*
** Finish setting up the procset after the application has finished
** launching/attaching in the dbg client layer.
*/
// TODO: Fix this when multidimensional handles are added for threading
int
FinalizeProcSetForInvoke(dbgprocset_t * ps, dbginvoke_t *di)
{
	dbg_over_set_t *	over_set = NULL;
	
	if (ps == NULL)
	{
		PrintOutError("Invalid procset_t in SetupInvoke()\n");
		return 1;
	}
	
	if (ps->s_init)
	{
		PrintOutError("Procset already initialized.\n");
		return 1;
	}
	
	// reconcile the nprocs with the di, it is our master
	ps->s_nproc = di->d_nproc;
	
	over_set = NewOverlaySet(di, bitset_dup(di->d_bitset_full), 0);
	AddToList(ps->s_over_set_list, over_set);
	
	AddOverlaySetToAllProcSet(over_set);
	
	ps->s_init = 1;
	
	return 0;
}

static dbgprocset_t *
NewProcSet(char *name, dbgset_type type)
{
	dbgprocset_t *	s;

	s = (dbgprocset_t *)Alloc(sizeof(dbgprocset_t));
	s->s_name = strdup(name);
	s->s_type = type;
	s->s_nproc = 0;
	s->s_over_set_list = NewList();
	s->s_indiv_proc = -1;	// TODO: Remove

	AddToList(ProcSets, (void *)s);

	return s;
}

static dbg_over_set_t*
NewOverlaySet(dbginvoke_t* di, bitset* bs, int offset)
{
	dbg_over_set_t* set = (dbg_over_set_t*)Alloc(sizeof(dbg_over_set_t));
	memset(set, 0, sizeof(dbg_over_set_t));

	set->s_di = di;
	set->s_di_bs = bs;
	set->s_ps_offset = offset;
	
	return set;
}

static void
FreeOverlaySet(dbg_over_set_t* set)
{
	set->s_di = NULL;
	bitset_free(set->s_di_bs);
	Free(set);
}

static void
AddOverlaySetToAllProcSet(dbg_over_set_t* over_set)
{
	AddToList(AllProcSet->s_over_set_list, over_set);
	AllProcSet->s_nproc += bitset_numset(over_set->s_di_bs);
}

static void
_free_procset(dbgprocset_t *s)
{
	// FIXME: Destroy the val!!!
	DestroyList(s->s_over_set_list, FreeOverlaySet); 

	Free(s->s_name);
	Free(s);
}

// Change this to only be used in val, if the val gets destroyed so does this.
void
FreeProcSet(dbgprocset_t *s)
{
	dbg_over_set_t *over_set;
	
	if (s == NULL)
		return;
		
	switch(s->s_type)
	{
		case DBGSET_INVOKE:
			SetOverlaySetForProcSet(s);
			while((over_set = GetNextOverlaySetForProcSet(s)) != NULL)
			{
				RemoveFromList(AllProcSet->s_over_set_list, over_set);
				AllProcSet->s_nproc -= bitset_numset(over_set->s_di_bs);
			}
			break;
		case DBGSET_DEFSET:
		case DBGSET_INTERNAL:
			// do nothing
			break;
	}

	RemoveFromList(ProcSets, (void *)s);

	_free_procset(s);
}

// TODO: Remove
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
PrintProcSet(dbgprocset_t *s)
{
	int					n = 0;
	dbg_over_set_t *	over_set;
	char *				setinfo;

	if ( EmptyList(s->s_over_set_list) )
	{
		PrintOutMsg("none");
		return;
	}

	SetList(s->s_over_set_list);

	// Here we want to print the "actual" ranks in the procset, not the "offset" ranks.
	while ( (over_set = (dbg_over_set_t *)GetListElement(s->s_over_set_list)) != NULL )
	{
		setinfo = bitset_to_set(over_set->s_di_bs);
		
		PrintOutMsg("%s", over_set->s_di->d_handle);
		
		if(setinfo != NULL)
		{
			PrintOutMsg("%s", setinfo);
			Free(setinfo);
			setinfo = NULL;
		}
		
		n += bitset_numset(over_set->s_di_bs);
		if ( n < s->s_nproc )
			PrintOutMsg(",");
	}
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

// TODO: Get rid of this, instead use the set and getnext above.
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

static void
_print_with_set_info(char *setname, bitset *group, int isOffset, int offset, int len, int isErr, char *f, va_list ap)
{
	char *	set_info = NULL;
	
	if (isOffset)
	{
		if (offset >= 0 && len > 0)
		{
			set_info = bitset_to_set_by_offset(group, offset, len);
		}
	}
	
	if (set_info == NULL)
	{
		set_info = bitset_to_set(group);
	}
	
	if (setname != NULL && set_info != NULL)
	{
		if (isErr)
		{
			PrintOutError("%s%s: ", setname, set_info);
		} else
		{
			PrintOutMsg("%s%s: ", setname, set_info);
		}
	}
	
	if (isErr)
	{
		vPrintOutError(f, ap);
	} else
	{
		vPrintOutMsg(f, ap);
	}
	
	Free(set_info);	
}

void
PrintMsgWithSetInfo(char* setname, bitset* group, char *f, ...)
{
	va_list ap;
	
	// setup the va_list
	va_start(ap, f);
	
	_print_with_set_info(setname, group, 0, 0, 0, 0, f, ap);
	
	// end the va_list
	va_end(ap);
}

// TODO: Remove
void
PrintMsgWithSetInfoByOffset(char* setname, bitset* group, int offset, int len, char *f, ...)
{
	va_list ap;
	
	// setup the va_list
	va_start(ap, f);
	
	_print_with_set_info(setname, group, 1, offset, len, 0, f, ap);
	
	// end the va_list
	va_end(ap);
}

void
PrintErrorWithSetInfo(char* setname, bitset* group, char *f, ...)
{
	va_list ap;
	
	// setup the va_list
	va_start(ap, f);
	
	_print_with_set_info(setname, group, 0, 0, 0, 1, f, ap);
	
	// end the va_list
	va_end(ap);
}

// TODO: Remove
void
PrintErrorWithSetInfoByOffset(char* setname, bitset* group, int offset, int len, char *f, ...)
{
	va_list ap;
	
	// setup the va_list
	va_start(ap, f);
	
	_print_with_set_info(setname, group, 1, offset, len, 1, f, ap);
	
	// end the va_list
	va_end(ap);
}

