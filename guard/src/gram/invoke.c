/*
** Process invocation support.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<string.h>
#include	<stdarg.h>

#include	"defs.h"
#include	"useful.h"
#include	"aif.h"
#include	"fds.h"
#include	"gram.h"
#include	"dbg_set_sync.h"

static list_t *	InvokeList = (list_t *)NULL;

invoke_t *
NewInvoke(char *cmd, list_t *igl, char *arch, void *exec)
{
	invoke_t *		i;
	invoke_group *	ig;

	i = (invoke_t *)Alloc(sizeof(invoke_t));

	i->i_ref = 0;
	i->i_cmd = (cmd != NULL ) ? strdup(cmd) : NULL;
	i->i_arch = (arch != NULL ) ? strdup(arch) : NULL;
	i->i_iexec = exec;
	i->i_nproc = 0;
	i->i_groups = igl;
	i->i_tree = NULL;
	i->i_set_bs = NULL; 
	i->i_attached = PROC_CREATED;

	SetList(igl);

	while ( (ig = (invoke_group *)GetListElement(igl)) != (invoke_group *)NULL )
	{
		i->i_nproc += ig->ig_nproc;
		ig->ig_invoke = i;
	}

	return i;
}

/*
** Val should be a scalar or a slice with no data allocated. (i.e. rval_none)
** We use the size and shape information to define a new value that
** is used to store the invoke_proc_t values.
*/
invoke_group *
NewInvokeGroup(hentry_t *sym, val_t *val, int offset)
{
	invoke_group *	ig;
	int				procset_size=0;
	int				i;
	slice_t *		proc_slice;

	ig = (invoke_group *)Alloc(sizeof(invoke_group));
	
	ig->ig_sym = sym;
	ig->ig_invoke = (invoke_t *)NULL;
	
	if (RVAL_TYPE(val) == rval_ref)
	{
		// Note that we only expect the user to be providing us with SLICE_POS
		// type of slices, that is $a{n} where n is the number of ranks. This
		// syntax is unique to launch only.
		proc_slice = RVAL_REF_SLICE(val);
		for (i=0; i < proc_slice->s_rank; ++i)
		{
			if (proc_slice->s_type[i] != SLICE_POS)
			{
				PrintOutError("Invalid proc handle syntax. Expected $handle{n} where n is the number of application ranks.\n");
				return NULL;
			}
			procset_size += proc_slice->s_high[i];
		}
		
		ig->ig_nproc = procset_size;
		ig->ig_rank = proc_slice->s_rank;
	} else
	{
		ig->ig_nproc = 1;
		ig->ig_rank = 1;
	}
		
	ig->ig_offset = offset;
	
	// We are going to turn this debugger variable into a procset rval. Destroy
	// the old value, including discarding any slicing info.
	DestroyRval(RVAL_TYPE(val), RVAL_VAL(val));
	// Now turn the rval into an rval_proc
	RVAL_TYPE(val) = rval_proc;
	// Set the proc to null, this is set later on
	RVAL_PROCSET(val) = (dbgprocset_t *)NULL;
	// Important! Set RVAL_DESTROY to false since it will be true here since
	// the original rval was a ref type
	RVAL_DESTROY(val) = 0;

	return ig;
}

/*
** Create a list of invoke groups from the expression e, which
** should be a list of gdb variables such as $a,$b,$c[4].
*/
int
CreateInvokeGroupList(expr_t *e, list_t *igl, rec_set_t *rec_set)
{
	int			n;
	int			np = 0;
	val_t *		v;
	list_t *	l;
	
	if ( EvalExprList(&l, e, rec_set) < 0 )
	{
		return -1;
	}

	SetList(l);

	while ( (v = (val_t *)GetListElement(l)) != (val_t *)NULL )
	{
		if ( (n = CreateInvokeGroup(igl, v, rec_set, np)) < 0 )
		{
			DestroyList(l, DestroyVal);
			return -1;
		}

		np += n;
	}

	// Don't destroy the val's in the list, these were inserted into the val
	// hash table.
	DestroyList(l, NULL);

	return np;
}

void
FreeInvokeGroupList(list_t *igl)
{
	invoke_group *	ig;

	SetList(igl);

	while ( (ig = (invoke_group *)GetListElement(igl)) != (invoke_group *)NULL )
		FreeInvokeGroup(ig);

	DestroyList(igl, (void (*)())NULL);
}

/*
** Create an invoke group given an internal variable with rval_none. This
** internal variable can contain slicing information.
*/
int
CreateInvokeGroup(list_t *igl, val_t *val, rec_set_t *rec_set, int offset)
{
	invoke_group *	ig;
	
	if (LVAL_TYPE(val) != lval_internal)
	{
		PrintOutError("Must specify debugger variable for process handle.");
		return -1;
	}
	
	switch ( RVAL_ACTUAL_TYPE(val) )
	{
		case rval_graph:
			PrintOutError("An assertion script has already been defined using this name.\n");
			return -1;
			
		case rval_proc:
			PrintOutError("A process set has already been defined using this name.\n");
			return -1;
			
		case rval_blockmap:
			PrintOutError("A decomposition has already been defined using this name.\n");
			return -1;
			
		case rval_aif:
			PrintOutError("A debugger variable has already been defined using this name.\n");
			return -1;
			
		default:
			// we expect the type to be rval_none
			break;
	}

	if ((ig = NewInvokeGroup(LVAL_SYM(val), val, offset)) == NULL)
	{
		return -1;
	}

	AddToList(igl, (void *)ig);

	return ig->ig_nproc;
}

int
SetProcsetForInvokeGroup(invoke_group *ig, dbgprocset_t *set)
{
	val_t *	val;

	if (ig == NULL)
		return 0;

	val = GetVal(ig->ig_sym);
	
	if (val == NULL)
	{
		PrintOutError("Internal error: Process handle not found.");
		return 1;
	}

	if (LVAL_TYPE(val) != lval_internal)
	{
		PrintOutError("Internal error: Process handle sym is not an internal var!");
		return 1;
	}
	
	switch ( RVAL_ACTUAL_TYPE(val) )
	{
		case rval_graph:
			PrintOutError("Internal error: Attempted to assign procset to graph val.\n");
			return 1;
			
		case rval_proc:
			// this is what we expect
			break;
			
		case rval_blockmap:
			PrintOutError("Internal error: Attempted to assign procset to blockmap val.\n");
			return 1;
			
		case rval_aif:
			PrintOutError("Internal error: Attempted to assign procset to AIF val.\n");
			return 1;
			
		case rval_none:
			PrintOutError("Internal error: Attempted to assign procset to none val.\n");
			return 1;
			
		case rval_ref:
			PrintOutError("Internal error: Attempted to assign procset to ref val.\n");
			return 1;
			
		case rval_reply:
			PrintOutError("Internal error: Attempted to assign procset to application val.\n");
			return 1;
	}
	
	// assign the procset directly
	RVAL_PROCSET(val) = set;
	
	return 0;
}

invoke_t *
AddInvoke(char *proc, list_t *igl, char *arch, void *exec, void* tree, bitset* full_set, int is_attached)
{
	invoke_t *	i;
	invoke_group *	ig;

	SetList(igl);

	while ( (ig = (invoke_group *)GetListElement(igl)) != (invoke_group *)NULL )
	{
		if ( ig->ig_invoke != NULL) 
			return (invoke_t *)NULL;
	}

	if ( InvokeList == (list_t *)NULL )
		InvokeList = NewList();

	i = NewInvoke(proc, igl, arch, exec);
	i->i_tree = tree;
	i->i_set_bs = bitset_dup(full_set);
	i->i_attached = is_attached;

	AddToList(InvokeList, (void *)i);

	return i;
}

invoke_t *
FindInvoke(hentry_t *sym)
{
	invoke_group *	ig;

	if ( (ig = FindInvokeGroup(sym)) == (invoke_group *)NULL )
		return (invoke_t *)NULL;

	return ig->ig_invoke;
}

invoke_group *
FindInvokeGroup(hentry_t *sym)
{
	val_t *		v;
	dbgprocset_t* p;

	if
	(
		(v = GetVal(sym)) == (val_t *)NULL 
		||
		RVAL_TYPE(v) != rval_proc
	)
		return (invoke_group *)NULL;

	// assign the procset directly
	if ((p = RVAL_PROCSET(v)) == NULL)
	{
		PrintOutError("ERROR: cannot find procset value for %s\n", SymLookup(sym));
		return (invoke_group*) NULL;
	}

	return p->s_ig;
}

int
SetInvoke(void)
{
	if ( EmptyList(InvokeList) )
		return 0;

	SetList(InvokeList);

	return 1;
}

invoke_t *
GetInvoke(void)
{
	return (invoke_t *)GetListElement(InvokeList);
}

void
SetInvokeGroup(invoke_t *i)
{
	SetList(i->i_groups);
}

invoke_group *
GetInvokeGroup(invoke_t *i)
{
	return (invoke_group *)GetListElement(i->i_groups);
}

void
FreeInvoke(invoke_t *in)
{
	invoke_group *	ig;

	RemoveFromList(InvokeList, in);

	if ( EmptyList(InvokeList) )
	{
		DestroyList(InvokeList, (void (*)())NULL);
		InvokeList = (list_t *)NULL;
	}

	SetList(in->i_groups);

	while ( (ig = (invoke_group *)GetListElement(in->i_groups)) != (invoke_group *)NULL )
		FreeInvokeGroup(ig);

	DestroyList(in->i_groups, (void (*)())NULL);

	if ( in->i_cmd != NULL )
		Free(in->i_cmd);

	if ( in->i_arch != NULL )
		Free(in->i_arch);

	in->i_tree = NULL;
	if(in->i_set_bs != NULL)
		bitset_free(in->i_set_bs);

	Free(in);
}

void
FreeInvokeGroup(invoke_group *ig)
{
	(void)RemoveVal(ig->ig_sym);
	Free(ig);
}

void
PrintProcSetInfo(FILE *fp, char *setname, bitset *set)
{
	char *	setinfo = bitset_to_set(set);

	PrintToFd(fp, "%s", setname);

	if(setinfo != NULL)
	{
		PrintToFd(fp, "%s", setinfo);
		Free(setinfo);
	}
}

void
PrintProcSetInfoByOffset(FILE *fp, char *setname, bitset *set, int offset, int len)
{
	char*	setinfo = bitset_to_set_by_offset(set, offset, len);

	PrintToFd(fp, "%s", setname);

	if(setinfo != NULL)
	{
	PrintToFd(fp, "%s", setinfo);
		Free(setinfo);
	}
}

void
PrintMsgWithSetInfo(FILE *fp, char* setname, bitset* group, char *f, ...)
{
	va_list ap;
	
	char* set_info = NULL;
	
	set_info = bitset_to_set(group);
	
	if(setname != NULL && set_info != NULL)
	{
	PrintToFd(fp, "%s%s", setname, set_info);
		PrintToFd(fp, ": ");
	}

	va_start(ap, f);
	vPrintToFd(fp, f, ap);
	va_end(ap);

	Free(set_info);	
}

void
PrintMsgWithSetInfoByOffset(FILE *fp, char* setname, bitset* group, int offset, int len, char *f, ...)
{
	va_list ap;
	
	char* set_info = NULL;
	
	if(offset >= 0 && len > 0)
		set_info = bitset_to_set_by_offset(group, offset, len);
	else
		set_info = bitset_to_set(group);
	
	if(setname != NULL && set_info != NULL)
	{
		PrintToFd(fp, "%s%s", setname, set_info);
		PrintToFd(fp, ": ");
	}

	va_start(ap, f);
	vPrintToFd(fp, f, ap);
	va_end(ap);

	Free(set_info);	
}
