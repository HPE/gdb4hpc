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

// FIXME: Remove the s_ind_proc since it's not being used anymore.

rec_set_t*
NewRecSetByDbgInvoke(void* di, void* tree, bitset* full, int ind_proc)
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
	rec->s_ind_proc = ind_proc;
	
	return rec;
}

rec_set_t*
NewRecSetByOverSet(dbg_over_set_t* over_set, bitset* receivers, int ind_proc)
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

	rec->s_ind_proc = ind_proc;

	rec->s_over_set = over_set;
	DPRINT("[INFO]: NewRecSetByOverSet over_set[%p], offset[%d], rec->s_receiver_bs=%s\n", over_set, over_set->s_offset, bitset_to_set(rec->s_receiver_bs));

	return rec;
}

rec_set_t*
NewRecSet(char* setname, void* invoke, void* dbginvoke, void* proto_handler, bitset* receiver, int ind_proc)
{
	rec_set_t* rec = (rec_set_t*)Alloc(sizeof(rec_set_t));	

	rec->s_setname = setname;

	rec->s_invoke_handler = invoke;
	rec->s_dbginvoke_handler = dbginvoke;

	rec->s_proto_handler = proto_handler;
	rec->s_receiver_bs = receiver;
	rec->s_ind_proc = ind_proc;

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
	rec->s_ind_proc = -1;

	Free(rec);
}

/*
* data structures for group replies from dbgsrv 
*/

group_reply_t *
NewGroupReply(bitset *src, dbgevent_t *reply)
{
	group_reply_t *	rep = (group_reply_t *)Alloc(sizeof(group_reply_t));
	
	rep->rep_event = reply;
	rep->rep_group = src;

	return rep;
}

void
FreeGroupReply(group_reply_t* rep)
{
	FreeEvent(rep->rep_event);
	bitset_free(rep->rep_group);
	Free(rep);
}

group_reply_t *
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
NewRepSet(void* tree)
{
	rep_set_t* set = (rep_set_t*)Alloc(sizeof(rep_set_t));

	set->tree = tree;
	set->rep_list = NewList();
	set->err_list = NewList();
	
	return set;
}

void
FreeRepSet(rep_set_t* set)
{
	if(set == NULL)
		return;

	set->tree = NULL;

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
		
	newset = NewRepSet(set->tree);
	
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

void
SetErrHeadInSet(rep_set_t* set)
{
	if (set != NULL)
		SetList(set->err_list);
}

group_reply_t *
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

// FIXME: Remove
AIF*
ConvertRepSetToAIF(rep_set_t* set)
{
	AIF*			res = NULL;
	AIF*			value = NULL;
	int				index = 0;
	char			index_str[32];
	group_reply_t*	rep = NULL;
	int				num = 0;

	if(set == NULL)
		return NULL;

	num = GetNumberOfReply(set);
	
	if(num == 0)
		return NULL;

	if(num == 1) /* only one AIF */
	{
		SetReplyHeadInSet(set);
		rep = GetNextReplyInSet(set);
		// Mark the event as save data to prevent the AIF data from being free'ed
		// when the reply is cleaned up.
		EVENT_SAVE_DATA(rep->rep_event);
		// Point at the aif data
		res = EVENT_DATA(rep->rep_event);
	} else { /* an array of AIF. Impelementation is based on AIFStruct */
		SetReplyHeadInSet(set);
		while((rep = GetNextReplyInSet(set)) != NULL)
		{
			// point at the aif data
			value = EVENT_DATA(rep->rep_event);

			if(res == NULL)
			{
			  	res = EmptyStructToAIF("repset_struct");
				AIFAddFieldToStruct(res, "size", IntToAIF(num));
			}

			snprintf(index_str, 32, "%d", index++);
			// This will copy the AIF data, no need to save it.
	        AIFAddFieldToStruct(res, index_str, value);
		}
	}

	return res;
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
