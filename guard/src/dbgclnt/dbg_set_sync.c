/*********************************************************************************\
 * dbg_set_sync.c
 *
 * © 2011-2013 Cray Inc.  All Rights Reserved.
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



#include	<stdio.h>
#include	<stdlib.h>

#include 	"defs.h"
#include	"dbgevent.h"
#include	"dbg_client.h"
#include	"dbgformat.h"
#include	"dbg_set_sync.h"
#include	"useful.h"

// FIXME: To do the f*cked up nature of how the PrintMsg* functions were implemented,
// we must expose the err_stream since other convienience libraries are depending on
// it being defined (see dbg_set_sync.c). This has been recorded in BUG 804428
#include	"client_settings.h"

// static prototypes
static void				FreeGroupReply(group_reply_t *);
static group_reply_t *	CopyGroupReply(group_reply_t *);
static void				SetErrHeadInSet(rep_set_t *);
static group_reply_t *	GetNextErrInSet(rep_set_t *);

rec_set_t*
NewRecSetByDbgInvoke(void* di, void* tree, bitset* full)
{
	rec_set_t* rec = NULL;

	if(di == NULL || tree == NULL || full == NULL)
		return NULL;
	
	rec = (rec_set_t*)Alloc(sizeof(rec_set_t));
	rec->s_setname = NULL;
	rec->s_invoke_handler = NULL;
	rec->s_dbginvoke_handler = di;
	rec->s_proto_handler = tree;
	rec->s_receiver_bs = full;
	
	return rec;
}

rec_set_t*
NewRecSetByOverSet(dbg_over_set_t* over_set, bitset* receivers)
{
	rec_set_t* rec = NULL;

	if(over_set == NULL)
		return NULL;

	rec = (rec_set_t*)Alloc(sizeof(rec_set_t));	

	rec->s_setname = over_set->s_setname;
	rec->s_invoke_handler = over_set->s_invoke;
	rec->s_dbginvoke_handler = over_set->s_invoke->i_iexec;
	rec->s_proto_handler = over_set->s_invoke->i_tree;

	rec->s_receiver_bs = bitset_dup(over_set->s_set_bs);
	if(receivers != NULL)
		rec->s_receiver_bs = receivers;

	rec->s_over_set = over_set;
	DPRINT("[INFO]: NewRecSetByOverSet over_set[%p], offset[%d], rec->s_receiver_bs=%s\n", over_set, over_set->s_offset, bitset_to_set(rec->s_receiver_bs));

	return rec;
}

rec_set_t*
NewRecSet(char* setname, void* invoke, void* dbginvoke, void* proto_handler, bitset* receiver)
{
	rec_set_t* rec = (rec_set_t*)Alloc(sizeof(rec_set_t));	

	rec->s_setname = setname;

	rec->s_invoke_handler = invoke;
	rec->s_dbginvoke_handler = dbginvoke;

	rec->s_proto_handler = proto_handler;
	rec->s_receiver_bs = receiver;

	rec->s_over_set = NULL;
	
	return rec;
}

void
FreeRecSet(rec_set_t* rec)
{
	if(rec == NULL)
		return;

	rec->s_setname = NULL;
	rec->s_proto_handler = NULL;
	rec->s_receiver_bs = NULL;

	Free(rec);
}

/*
* data structures for group replies from dbgsrv 
*/

group_reply_t *
NewGroupReply(void *di, bitset *src, dbgevent_t *reply)
{
	group_reply_t *	rep = (group_reply_t *)Alloc(sizeof(group_reply_t));
	
	rep->rep_di = di;
	rep->rep_event = reply;
	rep->rep_group = src;

	return rep;
}

static void
FreeGroupReply(group_reply_t* rep)
{
	FreeEvent(rep->rep_event);
	bitset_free(rep->rep_group);
	Free(rep);
}

static group_reply_t *
CopyGroupReply(group_reply_t *rep)
{
	group_reply_t *	newrep;
	
	if (rep == NULL)
		return NULL;
		
	newrep = (group_reply_t *)Alloc(sizeof(group_reply_t));
	
	newrep->rep_event = DupEvent(rep->rep_event);
	newrep->rep_group = bitset_dup(rep->rep_group);
	
	return newrep;
}

/*
* data structures for sets of group replies from dbgsrv 
*/

rep_set_t* 		
NewRepSet(void)
{
	rep_set_t* set = (rep_set_t*)Alloc(sizeof(rep_set_t));

	set->rep_list = NewList();
	set->err_list = NewList();
	
	return set;
}

void
FreeRepSet(rep_set_t* set)
{
	if(set == NULL)
		return;

	DestroyList(set->rep_list, FreeGroupReply);

	DestroyList(set->err_list, FreeGroupReply);
	
	Free(set);
}

rep_set_t *
CopyRepSet(rep_set_t * set)
{
	rep_set_t *		newset;
	group_reply_t *	elem;

	if (set == NULL)
		return NULL;
		
	newset = NewRepSet();
	
	// copy the replies
	SetList(set->rep_list);
	while ((elem = GetListElement(set->rep_list)) != NULL)
	{
		AddReplyToSet(newset, CopyGroupReply(elem));
	}
	
	// copy the errors
	SetList(set->err_list);
	while ((elem = GetListElement(set->err_list)) != NULL)
	{
		AddErrToSet(newset, CopyGroupReply(elem));
	}
	
	return newset;
}

rep_set_t *
MergeRepSet(rep_set_t* set, rep_set_t *tmp_set)
{
	group_reply_t *	elem;

	if (set == NULL || tmp_set == NULL)
		return NULL;
		
	// merge the replies
	while ((elem = RemoveFirst(tmp_set->rep_list)) != NULL)
	{
		AddReplyToSet(set, elem);
	}
	
	// Merge the errors
	while ((elem = RemoveFirst(tmp_set->err_list)) != NULL)
	{
		AddErrToSet(set, elem);
	}
	
	FreeRepSet(tmp_set);
	
	return set;
}

int
SetHasError(rep_set_t *set)
{
	if (set == (rep_set_t *)NULL)
		return 0;
		
	if (set->err_list == (list_t*)NULL)	
		return 0;

	return SizeOfList(set->err_list);
}

void
AddReplyToSet(rep_set_t* set, group_reply_t * rep)
{
	if (rep == NULL || set == NULL)
		return;

	AddToList(set->rep_list, (void *)rep);
}

void
AddErrToSet(rep_set_t* set, group_reply_t * err_rep)
{
	if (err_rep == NULL || set == NULL)
		return;

	AddToList(set->err_list, (void *)err_rep);
}

void
ResetSet(rep_set_t *set)
{
	if (set == NULL)
		return;
		
	SetList(set->rep_list);
	SetList(set->err_list);
}

static void
SetErrHeadInSet(rep_set_t* set)
{
	if (set != NULL)
		SetList(set->err_list);
}

static group_reply_t *
GetNextErrInSet(rep_set_t* set)
{
	if(set != NULL)
		return (void*)GetListElement(set->err_list);
	else
		return NULL;
}

void
SetReplyHeadInSet(rep_set_t* set)
{
	if (set != NULL)
		SetList(set->rep_list);
}

group_reply_t *
GetNextReplyInSet(rep_set_t* set)
{
    if (set != NULL)
		return (group_reply_t *)GetListElement(set->rep_list);
	else
		return NULL;
}

int
GetNumberOfReply(rep_set_t* set)
{
    if(set == NULL)
        return 0;
    
    return SizeOfList(set->rep_list);
}

// Used to print out an error from a rep_set
// FIXME: This shouldn't be here. Need to fix the interface somehow.
extern void	PrintMsgWithSetInfo(FILE *, char *, bitset *, char *, ...);

int
ProcessErrorInRepSet(rec_set_t* rec_set, rep_set_t* rep_set)
{
	group_reply_t *	err;
	
	if(rep_set == NULL)
		return 0;

	if(SetHasError(rep_set))
	{
		SetErrHeadInSet(rep_set);
		while((err = GetNextErrInSet(rep_set)) != NULL)
		{
			// Print out the error
			PrintMsgWithSetInfo(Client.cli_err_stream, rec_set->s_setname, err->rep_group, "%s\n", DbgEventErrorStr(err->rep_event));
		}
		return 1;
	}

	return 0;
}

bitset*
SetIndividualProcess(bitset* bs, int offset, int procnum)
{
	bitset* res = NULL;

	if((procnum + offset) > bitset_size(bs))	
		return NULL;

	res = bitset_dup(bs);
	bitset_clear(res);
	bitset_set(res, procnum + offset);
	
	return res;
} 
