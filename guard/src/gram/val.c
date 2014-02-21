/*********************************************************************************\
 * Value routines
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

/*
** TODO: The following is old. The entire interface was changed.
**
** How vals work.
**
** Value Reference:
** 
** Data read from a program being debugged is stored in a val with
** type lval_gdb. Literal values are stored in a val of type lval_none.
**
** Indexing or slicing a val (and possibly derefrencing) creates
** a new val whose type depends on the original type of the val.
** For lval_gdb and lval_none, the new type is lval_none.
** For lval_internal and lval_ref, the new type is lval_ref.
** An lval_ref type contains a pointer to the original val.
**
** Value Assignment:
**
** The lval of an assignment is only allowed to be lval_gdb, lval_internal
** or lval_ref.
**
** When a new value is assigned to an internal variable using (1) or (2)
** below, the type of the new value is changed to lval_internal. This 
** means that an lval_ref also becomes an lval_internal again. Assignment 
** using (3) does not change the type, since the data is just copied to
** an existing val.
**
** 1) Assigning to a non-existent variable works like this. Referencing
** a non-existent variable causes a new value to be created of type 
** lval_internal and whose value is empty (either v0 or an array of v0).
** This value is then discarded and replaced with the val being assigned.
**
** 2) Assigning to an existing variable just results in the current val
** being discarded and replaced with the new val.
**
** 3) Assigning to an array location or slice is more complicated. First
** the location or slice is evaluated, resulting in a new val of
** type lval_ref (as described above). The assignment detects the 
** lval_ref and uses the pointer to the original variable along with
** the size and shape of the lval_ref to determine where to store
** the data.
**
** Destroying Vals
**
** The only special type of val is lval_internal which is sticky. That
** is, a call to DestroyVal() doesn't do anything.
*/

#ifdef HAVE_CONFIG_H
#include        <config.h>
#endif /* HAVE_CONFIG_H */

#include	<string.h>
#include	<signal.h>
#include	<stdio.h>

#include	"defs.h"
#include	"useful.h"
#include	"dbg_client.h"
#include	"aif.h"
#include	"fds.h"
#include	"gram.h"

// Static functions
static void	Destroyrval_ref(rval_ref_val *);
static rval_ref_val *	NewRvalRef(void);

#define DBG_VAL_TABLE_SIZE	7     /* 2^7 = 128 */

htable_t *	_val_table = (htable_t *)NULL;
val_t		_lval = { lval_internal, {NULL}, rval_aif, {NULL}, 0};

void
InitVal(void)
{
	if ( _val_table == (htable_t *)NULL )
		_val_table = HashCreate(DBG_VAL_TABLE_SIZE);
}

void
_destroy_val(char *s, void *d)
{
	DestroyInternalVal((val_t *)d);
	DestroyVal((val_t *)d);
}

void
FinishVal(void)
{
	HashDestroy(_val_table, _destroy_val);
	_val_table = (htable_t *)NULL;
}

/*
** This function was very confusing to me for a long time.
** Its primary function is to create a temporary val for a sym when it is going
** to be used with the AssignVal function. This function creates a NewVal from
** the lval and rval val's that are passed in.
*/
val_t *
MakeTransitoryLval(hentry_t *sym)
{
	_lval.lval_u.lval_sym = sym;
	return &_lval;
}

/*
** This function is used to return the actual rval, i.e. dereferencing references
** This is not actual dereference, but we want to know the type a reference is
** pointing at.
*/
rval_type
GetActualRvalType(val_t *val)
{
	rval_ref_val *	ref;

	if (val == NULL)
		return rval_none;
	
	if (RVAL_TYPE(val) == rval_ref)
	{
		switch (RVAL_REF_TYPE(val))
		{
			case rval_ref:
				ref = REF_VAL(RVAL_REF_VAL(val));
				// need to loop through to determine the type
				while (ref->ref_type == rval_ref)
				{
					ref = REF_VAL(*(ref->ref_val));
				}
				return ref->ref_type;
				
			default:
				// for everything else the actual rval is known by the ref type
				return RVAL_REF_TYPE(val);
		}
	}
	
	// for everything other than rval_ref, the actual rval is known
	return RVAL_TYPE(val);
}

/*
** This function is used to return the actual AIF val, ignoring refs.
*/
AIF *
GetActualAIF(val_t *val)
{
	rval_ref_val *	ref;

	if (val == NULL)
		return NULL;
		
	switch (RVAL_TYPE(val))
	{
		case rval_ref:
			// switch on the ref rval type
			switch (RVAL_REF_TYPE(val))
			{
				case rval_ref:
					ref = REF_VAL(RVAL_REF_VAL(val));
					// need to loop through to get at the base
					while (ref->ref_type == rval_ref)
					{
						ref = REF_VAL(*(ref->ref_val));
					}
					
					// ref is now pointing at the base, make sure its an AIF type
					if (ref->ref_type != rval_aif)
					{
						// The base is something other than AIF!
						SetError("Internal error: Called GetActualAIF with rval_ref other than AIF.\n");
						return NULL;
					}
					
					// return the base
					return AIF_VAL(*(ref->ref_val));
				
				case rval_aif:
					// Can return the AIF type directly
					return AIF_VAL(RVAL_REF_VAL(val));
				
				// Everything else is an error!
				default:
					SetError("Internal error: Called GetActualAIF with rval_ref other than AIF.\n");
					return NULL;
			}
			// shouldn't get here
			break;
		
		case rval_aif:
			// Can return the AIF type directly
			return RVAL_AIF(val);
		
		// Everything else is an error!
		default:
			SetError("Internal error: Called GetActualAIF without AIF rval.\n");
			return NULL;
	}
	
	// Shouldn't get here
	SetError("Internal error: GetActualAIF escaped control statement!!\n");
	return NULL;
}

/*
** This function is used to return the actual reply set val, ignoring refs.
*/
rep_set_t *
GetActualReply(val_t *val)
{
	rval_ref_val *	ref;

	if (val == NULL)
		return NULL;
		
	switch (RVAL_TYPE(val))
	{
		case rval_ref:
			// switch on the ref rval type
			switch (RVAL_REF_TYPE(val))
			{
				case rval_ref:
					ref = REF_VAL(RVAL_REF_VAL(val));
					// need to loop through to get at the base
					while (ref->ref_type == rval_ref)
					{
						ref = REF_VAL(*(ref->ref_val));
					}
					
					// ref is now pointing at the base, make sure its a reply type
					if (ref->ref_type != rval_reply)
					{
						// The base is something other than reply!
						SetError("Internal error: Called GetActualReply with rval_ref other than reply.\n");
						return NULL;
					}
					
					// return the base
					return REPLY_VAL(*(ref->ref_val));
				
				case rval_reply:
					// Can return the reply type directly
					return REPLY_VAL(RVAL_REF_VAL(val));
				
				// Everything else is an error!
				default:
					SetError("Internal error: Called GetActualReply with rval_ref other than reply.\n");
					return NULL;
			}
			// shouldn't get here
			break;
		
		case rval_reply:
			// Can return the reply type directly
			return RVAL_REPLY(val);
		
		// Everything else is an error!
		default:
			SetError("Internal error: Called GetActualReply without reply rval.\n");
			return NULL;
	}
	
	// Shouldn't get here
	SetError("Internal error: GetActualReply escaped control statement!!\n");
	return NULL;
}

/*
** This function is used to return the actual procset val, ignoring refs.
*/
dbgprocset_t *
GetActualProcset(val_t *val)
{
	rval_ref_val *	ref;

	if (val == NULL)
		return NULL;
		
	switch (RVAL_TYPE(val))
	{
		case rval_ref:
			// switch on the ref rval type
			switch (RVAL_REF_TYPE(val))
			{
				case rval_ref:
					ref = REF_VAL(RVAL_REF_VAL(val));
					// need to loop through to get at the base
					while (ref->ref_type == rval_ref)
					{
						ref = REF_VAL(*(ref->ref_val));
					}
					
					// ref is now pointing at the base, make sure its a procset type
					if (ref->ref_type != rval_proc)
					{
						// The base is something other than procset!
						SetError("Internal error: Called GetActualProcset with rval_ref other than process set.\n");
						return NULL;
					}
					
					// return the base
					return PROCSET_VAL(*(ref->ref_val));
				
				case rval_proc:
					// Can return the process set type directly
					return PROCSET_VAL(RVAL_REF_VAL(val));
				
				// Everything else is an error!
				default:
					SetError("Internal error: Called GetActualProcset with rval_ref other than process set.\n");
					return NULL;
			}
			// shouldn't get here
			break;
		
		case rval_proc:
			// Can return the process set type directly
			return RVAL_PROCSET(val);
		
		// Everything else is an error!
		default:
			SetError("Internal error: Called GetActualProcset without process set rval.\n");
			return NULL;
	}
	
	// Shouldn't get here
	SetError("Internal error: GetActualProcset escaped control statement!!\n");
	return NULL;
}

/*
** Retrieve the contents of a val_t from the backend
*/
int
RetrieveContents(val_t *val, rec_set_t *rec_set)
{
	AIF *				a = NULL;
	char *				target_expr = NULL;
	dbgprocset_t *		target_procset = NULL;
	rec_set_t *			target_recset = NULL;
	rep_set_t *			rep_set = NULL;
	dbg_over_set_t *	over_set = NULL;
	
	DPRINT("In RetrieveContents\n");
	
	if (val == NULL)
		return 0;

	// We need to evaluate the value of the variable on the debug backends
			
	// Set the target_procset/target_expr based on the lval type

	switch (LVAL_TYPE(val))
	{
		case lval_gdb:
			DPRINT("RetrieveContents: lval is lval_gdb\n");
			// the rval of a gdb type should always be rval_reply, but
			// lets just check to ensure this is the case.
			if (RVAL_ACTUAL_TYPE(val) != rval_reply)
				return 0;
			
			// if the rval is not null, return and use the cached value
			if (RVAL_ACTUAL_REPLY(val) != NULL)
				return 0;
				
			target_expr = LVAL_GDB_EXPR(val);
			target_procset = LVAL_GDB_PROCSET(val);
			break;
			
		case lval_blockmap:
			DPRINT("RetrieveContents: lval is lval_blockmap\n");
			// the rval of a blockmap type should always be rval_aif, but
			// lets just check to ensure this is the case.
			if (RVAL_ACTUAL_TYPE(val) != rval_aif)
				return 0;
						
			// if the rval is not null, return and use the cached value
			if (RVAL_ACTUAL_AIF(val) != NULL)
				return 0;
				
			target_expr = LVAL_BM_GDB_EXPR(val);
			target_procset = LVAL_BM_GDB_PROCSET(val);
			break;
			
		default:
			DPRINT("RetrieveContents: lval is a const, returning...\n");
			// Don't need to retrieve anything for the other types
			return 0;
	}
			
	// if target_procset is not null, then the user provided a process targeted command.
	if (target_procset != NULL)
	{
		DPRINT("RetrieveContents: target_procset was specified. Going to use process targeted method.\n");
		
		// This gets free'ed later
		target_recset = NewRecSet();
		
		SetOverlaySetForProcSet(target_procset);
		while((over_set = GetNextOverlaySetForProcSet(target_procset)) != NULL)
		{
			// create a group_recv_t for the over_set and add it to the res
			AddReceiverToSet(target_recset, NewGroupReceiver(over_set->s_di, over_set->s_di_bs));
		}
		
		rec_set = target_recset;
	}
			
	// ensure rec_set is not null, if so we have an error!
	if (rec_set == NULL)
	{
		SetError("RetrieveContents called with null rec_set!\n");
		return -1;
	}
		
	// Evaluate the expression	
	rep_set = DbgEvalExpr(rec_set, target_expr);

	// Process errors - If there were no replies in the set we want to make sure
	// the rval gets NULL'ed. I don't think this is ever possible without an 
	// error being generated.
	if ((ProcessErrorInRepSet(rep_set) && GetNumberOfReply(rep_set) == 0) || (GetNumberOfReply(rep_set) == 0))
	{
		// Free the rep_set, we are done with it
		FreeRepSet(rep_set);
		
		// Ensure the RVAL is nulled
		switch (LVAL_TYPE(val))
		{
			case lval_gdb:
				RVAL_REPLY(val) = NULL;
				SetError("Could not retrieve contents of val.");
				if (target_recset != NULL)
					FreeRecSet(target_recset);
				return -1;
				
			case lval_blockmap:
				RVAL_AIF(val) = NULL;
				SetError("Could not retrieve contents of val.");
				if (target_recset != NULL)
					FreeRecSet(target_recset);
				return -1;
			
			default:
				// do nothing - shouldn't get here
				SetError("Could not retrieve contents of val.");
				if (target_recset != NULL)
					FreeRecSet(target_recset);
				return -1;
		}
	}

	// Set the RVAL based on its type.
	// Conduct special post processing for blockmap types
	// Note that the rval data gets free'ed when the val is destroyed
	switch (LVAL_TYPE(val))
	{
		case lval_gdb:
			// assign the reply to the val
			RVAL_REPLY(val) = rep_set;
			break;
	
		case lval_blockmap:
			// construct the blockmap based on the replies - This does error checking for us.
			a = construct_blockmap_data(LVAL_BM_BLOCKMAP(val), LVAL_BM_GDB_PROCSET(val), LVAL_BM_ARRAY_SIZE(val), rep_set);
			// Free the rep_set, we are done with it
			FreeRepSet(rep_set);
			// assign the AIF result to the val
			RVAL_AIF(val) = a;
			break;
					
		default:
			// do nothing
			break;
	}
	
	// cleanup
	if (target_recset != NULL)
		FreeRecSet(target_recset);
	
	return 0;
}

static rval_ref_val *
NewRvalRef(void)
{
	rval_ref_val *rtn;
	
	rtn = (rval_ref_val *)Alloc(sizeof(rval_ref_val));
	rtn->ref_val = (rval_val *)Alloc(sizeof(rval_val));
	
	return rtn;
}

val_t *
NewVal(lval_type ltype, ...)
{
	val_t *	v;
	va_list	args;

	v = (val_t *)Alloc(sizeof(val_t));

	RVAL_DESTROY(v) = 0;

	va_start(args, ltype);

	// set the lval
	switch ( LVAL_TYPE(v) = ltype )
	{
		case lval_none:
			// ensure it gets nulled
			memset(&LVAL_VAL(v), 0, sizeof(lval_val));
			break;

		case lval_internal:
			LVAL_SYM(v) = va_arg(args, hentry_t *);
			break;

		case lval_gdb:
			// This case should return, we don't allow the caller to specify an
			// rval type. It must be a RVAL_REPLY.
			LVAL_GDB_PROCSET(v) = va_arg(args, dbgprocset_t *);
			LVAL_GDB_EXPR(v) = strdup(va_arg(args, char *));
			// The rval is assumed.
			RVAL_TYPE(v) = rval_reply;
			RVAL_REPLY(v) = (rep_set_t *)NULL;
			va_end(args);
			return v;
			
		case lval_blockmap:
			// This case should return, we don't allow the caller to specify an
			// rval type. It must be a RVAL_REPLY.
			LVAL_BM_GDB_PROCSET(v) = va_arg(args, dbgprocset_t *);
			LVAL_BM_GDB_EXPR(v) = strdup(va_arg(args, char *));
			LVAL_BM_BLOCKMAP(v) = va_arg(args, g_block_map_t *);
			LVAL_BM_ARRAY_SIZE(v) = va_arg(args, int);
			// The rval is assumed.
			RVAL_TYPE(v) = rval_aif;
			RVAL_AIF(v) = (AIF *)NULL;
			va_end(args);
			return v;
	}

	// set the rval
	switch ( RVAL_TYPE(v) = va_arg(args, rval_type) )
	{
		case rval_none:
			// ensure it gets nulled
			memset(&RVAL_VAL(v), 0, sizeof(rval_val));
			break;
		
		case rval_aif:
			RVAL_AIF(v) = va_arg(args, AIF *);
			break;
	
		case rval_proc:
			RVAL_PROCSET(v) = va_arg(args, dbgprocset_t *);
			break;

		case rval_blockmap:
			RVAL_BLOCKMAP(v) = va_arg(args, g_block_map_t *);
			break;
			
		case rval_ref:
			RVAL_TYPE(v) = rval_ref;
			RVAL_REF(v) = NewRvalRef();
			
			RVAL_REF_SLICE(v) = va_arg(args, slice_t *);
			RVAL_REF_TYPE(v) = va_arg(args, rval_type);
			RVAL_REF_VAL(v) = va_arg(args, rval_val);
			// we want to mark the ref rval for destruction
			RVAL_DESTROY(v) = 1;
			break;

		case rval_reply:
			RVAL_REPLY(v) = va_arg(args, rep_set_t *);
			break;

		case rval_graph:
#ifdef RD
			RVAL_GRAPH(v) = va_arg(args, graph_t *);
#endif
			break;
	}

	va_end(args);

	return v;
}

/*
** Destroy a value. Does not do anything to lval_internal, which
** can only be destroyed by calling AssignVal().
*/
void
DestroyVal(val_t *v)
{
	if ( v == (val_t *)NULL )
		return;

	// destroy the lval
	switch (LVAL_TYPE(v))
	{
		case lval_none:
			break;

		case lval_internal:
			if ( !RVAL_DESTROY(v) )
				return;
			break;

		case lval_blockmap:
			// Check to see if the procset is DBGSET_TEMP, if so we destroy it
			if (LVAL_BM_GDB_PROCSET(v) != NULL)
			{
				switch (LVAL_BM_GDB_PROCSET(v)->s_type)
				{
					case DBGSET_TEMP:
						FreeProcSet(LVAL_GDB_PROCSET(v));
						break;
					
					default:
						// do nothing
						break;
				}
			}
			Free(LVAL_BM_GDB_EXPR(v));
			break;
		
		case lval_gdb:
			// Check to see if the procset is DBGSET_TEMP, if so we destroy it
			if (LVAL_GDB_PROCSET(v) != NULL)
			{
				switch (LVAL_GDB_PROCSET(v)->s_type)
				{
					case DBGSET_TEMP:
						FreeProcSet(LVAL_GDB_PROCSET(v));
						break;
					
					default:
						// do nothing
						break;
				}
			}
			Free(LVAL_GDB_EXPR(v));
			break;
	}
	
	// destroy the rval
	DestroyRval(RVAL_TYPE(v), RVAL_VAL(v));

	Free(v);
}

/*
** Destroy the rvalue of a val. 
*/
void
DestroyRval(rval_type rtype, rval_val rval)
{
	switch (rtype)
	{
		case rval_none:
			return;
		
		case rval_aif:
			AIFFree(AIF_VAL(rval));
			return;
		
		case rval_blockmap:
			// FIXME: Make use of this
			//DestroyBlockMap(RVAL_BLOCKMAP(v));
			return;
			
		case rval_graph:
#ifdef RD
			// FIXME: Make use of this
			//DestroyGraph(RVAL_GRAPH(v));
#endif
			return;
			
		case rval_reply:
			FreeRepSet(REPLY_VAL(rval));
			return;
			
		case rval_proc:
			// FIXME: Make use of this
			//FreeProcSet(RVAL_PROCSET(v));
			return;
			
		case rval_ref:
			Destroyrval_ref(REF_VAL(rval));
			return;
	}
}

/*
** Used to destroy a rval_ref type, these can be infinite depth, so we need
** a function to be recursive.
*/
static void
Destroyrval_ref(rval_ref_val * ref)
{
	if (ref == NULL)
		return;

	// destroy the ref slice
	DestroySlice(ref->ref_slice);
	
	// destroy the ref val
	if (ref->ref_val != NULL)
	{
		DestroyRval(ref->ref_type, *(ref->ref_val));
	}
	
	// destroy the val pointer and the ref
	Free(ref->ref_val);
	Free(ref);
}

/*
** Special case to mark an lval_internal for destruction.
*/
void
DestroyInternalVal(val_t *v)
{
	if ( v == (val_t *)NULL )
		return;

	switch ( LVAL_TYPE(v) )
	{
	case lval_internal:
		break;

	default:
		return;
	}

	RVAL_DESTROY(v) = 1;
}

rval_val
CopyRval(rval_type rtype, rval_val rval)
{
	rval_val	new_rval;
	
	switch (rtype)
	{
		case rval_none:
			// ensure it gets nulled
			memset(&new_rval, 0, sizeof(rval_val));
			return new_rval;
			
		case rval_aif:
			AIF_VAL(new_rval) = CopyAIF(AIF_VAL(rval));
			return new_rval;
		
		case rval_blockmap:
			// A blockmap val is a pointer to a blockmap. These are impossible
			// to copy right now.
			BLOCKMAP_VAL(new_rval) = BLOCKMAP_VAL(rval);
			return new_rval;
			
		case rval_graph:
#ifdef RD
			// A graph val is a pointer to a graph. These are impossible
			// to copy right now.
			GRAPH_VAL(new_rval) = GRAPH_VAL(rval);
#else
			// ensure it gets nulled
			memset(&new_rval, 0, sizeof(rval_val));
#endif
			return new_rval;
			
		case rval_reply:
			// A reply val is a pointer to a reply set. These are impossible to
			// copy right now.
			REPLY_VAL(new_rval) = REPLY_VAL(rval);
			return new_rval;
		
		case rval_proc:
			// A procset val is a pointer to a procset. These are impossible
			// to copy right now.
			PROCSET_VAL(new_rval) = PROCSET_VAL(rval);
			return new_rval;
			
		case rval_ref:
		{
			// TODO: How to support this??? I cannot envision a case where this
			// will be used, but its here just in case someone decides its useful
			// to support.
			REF_VAL(new_rval) = NewRvalRef();
			
			REF_VAL(new_rval)->ref_slice = CopySlice(REF_VAL(rval)->ref_slice);
			REF_VAL(new_rval)->ref_type = REF_VAL(rval)->ref_type;
			*(REF_VAL(new_rval)->ref_val) = CopyRval(REF_VAL(rval)->ref_type, *(REF_VAL(rval)->ref_val));
			return new_rval;
		}
	}
	
	// Shouldn't get here.
	
	// ensure it gets nulled
	memset(&new_rval, 0, sizeof(rval_val));
	
	return new_rval;
}

val_t *
CopyVal(val_t *val)
{
	val_t *		v = NULL;

	if ( val == (val_t *)NULL )
		return (val_t *)NULL;

	// First switch on the lval type, then switch on the rval type. We call
	// the NewVal function with the proper arguments that way.
	switch (LVAL_TYPE(val))
	{
		case lval_none:
			switch (RVAL_TYPE(val))
			{
				case rval_none:
					v = NewVal(lval_none, rval_none);
					break;
					
				case rval_aif:
					v = NewVal(lval_none, rval_aif, CopyAIF(RVAL_AIF(val)));
					break;
					
				case rval_blockmap:
					v = NewVal(lval_none, rval_blockmap, RVAL_BLOCKMAP(val));
					break;
					
				case rval_graph:
#ifdef RD
					v = NewVal(lval_none, rval_graph, RVAL_GRAPH(val));
#endif
					break;
					
				case rval_proc:
					v = NewVal(lval_none, rval_proc, RVAL_PROCSET(val));
					break;
					
				case rval_reply:
					v = NewVal(lval_none, rval_reply, RVAL_REPLY(val));
					break;
					
				case rval_ref:
					v = NewVal(	lval_none, rval_ref, CopySlice(RVAL_REF_SLICE(val)),
								RVAL_REF_TYPE(val), CopyRval(RVAL_REF_TYPE(val), RVAL_REF_VAL(val)));
					break;
			}
			break;

		case lval_internal:
			switch (RVAL_TYPE(val))
			{
				case rval_none:
					v = NewVal(lval_internal, LVAL_SYM(val), rval_none);
					break;
					
				case rval_aif:
					v = NewVal(lval_internal, LVAL_SYM(val), rval_aif, CopyAIF(RVAL_AIF(val)));
					break;
					
				case rval_blockmap:
					v = NewVal(lval_internal, LVAL_SYM(val), rval_blockmap, RVAL_BLOCKMAP(val));
					break;
					
				case rval_graph:
#ifdef RD
					v = NewVal(lval_internal, LVAL_SYM(val), rval_graph, RVAL_GRAPH(val));
#endif
					break;
					
				case rval_proc:
					v = NewVal(lval_internal, LVAL_SYM(val), rval_proc, RVAL_PROCSET(val));
					break;
					
				case rval_reply:
					v = NewVal(lval_internal, LVAL_SYM(val), rval_reply, RVAL_REPLY(val));
					
				case rval_ref:
					v = NewVal(	lval_internal, LVAL_SYM(val), rval_ref, CopySlice(RVAL_REF_SLICE(val)),
								RVAL_REF_TYPE(val), CopyRval(RVAL_REF_TYPE(val), RVAL_REF_VAL(val)));
					break;
			}
			break;

	case lval_gdb:
		// GDB lval's should only have reply rvals!!!
		switch (RVAL_TYPE(val))
		{
			case rval_reply:
				v = NewVal(lval_gdb, LVAL_GDB_PROCSET(val), LVAL_GDB_EXPR(val));
				if ( RVAL_AIF(val) != (AIF *)NULL )
				{
					RVAL_AIF(v) = CopyAIF(RVAL_AIF(val));
				}
				break;
		
			case rval_aif:
			case rval_none:
			case rval_blockmap:
			case rval_graph:
			case rval_proc:
			case rval_ref:
				SetError("Internal error: lval_gdb must have rval_reply.\n");
				break;
		}
		break;

	case lval_blockmap:
		// Why is this? This should work with a CopyAIF call...
		SetError("Internal error: lval_blockmap cannot be copied!\n");
		break;
	}

	return v;
}

/*
** Read Value.
*/
val_t *
GetVal(hentry_t *sym)
{
	hentry_t *	h;

	if ( (h = HashSearch(_val_table, SymLookup(sym))) == (hentry_t *)NULL )
		return (val_t *)NULL;

	return (val_t *)h->h_data;
}

/*
** Assign value to debugger variable.
** Note that this is the only "approved" method of assigning the rval of an 
** internal variable. 
** This function creates a new val and inserts it into the internal val table
** that consists of the lval of the provided lval and the rval of the provided
** rval.
*/
int
AssignVal(val_t *lval, val_t *rval, rec_set_t *rec_set)
{
	char *		var;
	val_t *		val = NULL;
	hentry_t *	h;

	switch ( LVAL_TYPE(lval) )
	{
		case lval_internal:
			// lookup the string name of the symbol
			var = SymLookup(LVAL_SYM(lval));
		
			// try to look up the symbol in the hash table, otherwise add it to
			// the hash table
			if ((h = HashSearch(_val_table, var)) == (hentry_t *)NULL)
			{
				if ((h = HashInsert(_val_table, var, (void *)NULL)) == (hentry_t *)NULL)
				{
					SetError("Internal error: Cannot add lval symbol to the val hash table!\n");
					return -1;
				}
			}
			
			switch (RVAL_TYPE(rval))
			{
				case rval_none:
					val = NewVal(lval_internal, LVAL_SYM(lval), rval_none);
					break;
				
				case rval_aif:
					// Make sure to refresh the contents
					if (VAL_RETRIEVE(rval, rec_set))
					{
						return -1;
					}
					val = NewVal(lval_internal, LVAL_SYM(lval), rval_aif, CopyAIF(RVAL_AIF(rval)));
					break;
				
				case rval_proc:
					val = NewVal(lval_internal, LVAL_SYM(lval), rval_proc, RVAL_PROCSET(rval));
					break;
			
				case rval_blockmap:
					val = NewVal(lval_internal, LVAL_SYM(lval), rval_blockmap, RVAL_BLOCKMAP(rval));
					break;
				
				case rval_ref:
					val = NewVal(	lval_internal, LVAL_SYM(lval), rval_ref, CopySlice(RVAL_REF_SLICE(rval)),
									RVAL_REF_TYPE(rval), CopyRval(RVAL_REF_TYPE(rval), RVAL_REF_VAL(rval)));
					break;
					
				case rval_reply:
					// Make sure to refresh the contents
					if (VAL_RETRIEVE(rval, rec_set))
					{
						return -1;
					}
					val = NewVal(lval_internal, LVAL_SYM(lval), rval_reply, RVAL_REPLY(rval));
					break;
					
				case rval_graph:
#ifdef RD
					val = NewVal(lval_internal, LVAL_SYM(lval), rval_graph, RVAL_GRAPH(rval));
#endif
					break;
			}
			
			// Make sure that the hash entries data is not set, otherwise free it.
			if ( h->h_data != NULL )
			{
				_destroy_val(h->h_key, h->h_data);
			}

			h->h_data = (void *)val;
			return 0;

		case lval_blockmap:
			SetError("Internal error: Cannot assign rval to lval_blockmap!\n");
			return -1;

		case lval_gdb:
			SetError("Internal error: Cannot assign rval to lval_gdb!\n");
			return -1;

		case lval_none:
			SetError("Internal error: Cannot assign rval to lval_blockmap!\n");
			return -1;
	}

	// Shouldn't get here
	return 0;
}

/*
** Remove association between an internal variable and its symbol.
*/
void
RemoveVal(hentry_t *sym)
{
	val_t *		val;
	hentry_t *	hval = NULL;

	hval = HashSearch(_val_table, SymLookup(sym));

	if (hval != NULL)
	{
		val = (val_t *)hval->h_data;
		HashRemove(_val_table, hval);
	}
}
