/*********************************************************************************\
 * interface of synchronous group operation
 *
 * © 2011-2014 Cray Inc.  All Rights Reserved.
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


#ifndef _DBG_SET_SYNC_H
#define _DBG_SET_SYNC_H

#include "aif.h"
#include "list.h"
#include "bitset.h"
#include "procset.h"
#include "dbgevent.h"

/*
** An ReceiverSet(rec_set_t) carries the information of a set of BEs that are supposed to receive a cmd. 
** Normally, an ReceiverSet corresponds an OverlaySet
** FIXME: Only use void pointers if the type can be multiple things. Don't use void ptr out of laziness.........
**	 Include the header file if needed.
*/
struct rec_set_t
{
	char *				s_setname;				//the name of its OverlaySet
	void *				s_invoke_handler;		//a pointer to invoke_t for switching protocol
	void *				s_dbginvoke_handler;	//a pointer to dbginvoke_t for switching protocol
	void *				s_proto_handler;		//the handler of communication protocol
	bitset *			s_receiver_bs;			//the bitset representing the receivers
	dbg_over_set_t *	s_over_set;
};
typedef struct rec_set_t rec_set_t;

extern rec_set_t *	NewRecSetByDbgInvoke(void* di, void* tree, bitset* full);
extern rec_set_t *	NewRecSetByOverSet(dbg_over_set_t* over_set, bitset* receivers);
extern rec_set_t *	NewRecSet(char* setname, void* invoke, void* dbginvoke, void* proto_handler, bitset* receiver);
extern void			FreeRecSet(rec_set_t* act);

/* 
** A ReplySet(rep_set_t) keeps results replied by a number of BEs.
** These results are aggregated into several unquie groups. 	
** Each group maybe an error message (maintained by group_err_t),
** or a normal message (maintained by group_reply_t).
*/

// TODO: Add in a dbginvoke_t entry to each reply, these should be tied to that
struct group_reply_t
{
	void *			rep_di;	// TODO: Make this an actual dbginvoke_t *
	bitset *		rep_group;
	dbgevent_t *	rep_event;
};
typedef struct group_reply_t group_reply_t;

struct rep_set_t
{
	list_t *	rep_list;	// event replies of cmd
	list_t *	err_list;	// error events
};
typedef struct rep_set_t rep_set_t;

extern group_reply_t *		NewGroupReply(void *di, bitset *src, dbgevent_t *reply);
extern rep_set_t *			NewRepSet(void);
extern void					FreeRepSet(rep_set_t* set);
extern rep_set_t *			CopyRepSet(rep_set_t* set);
extern rep_set_t *			MergeRepSet(rep_set_t* set, rep_set_t *tmp_set);
extern void					AddReplyToSet(rep_set_t* set, group_reply_t * rep);
extern void					AddErrToSet(rep_set_t* set, group_reply_t * err_rep);
extern void					ResetSet(rep_set_t *set);
extern int					SetHasError(rep_set_t *set);
extern void					SetReplyHeadInSet(rep_set_t* set);
extern int					GetNumberOfReply(rep_set_t* set);
extern group_reply_t *		GetNextReplyInSet(rep_set_t* set);
extern int					ProcessErrorInRepSet(rec_set_t *, rep_set_t *);

/* This funciont is not rep_set-related*/
extern bitset*				SetIndividualProcess(bitset* bs, int offset, int procnum); 

#endif /* _DBG_SET_SYNC_H */

