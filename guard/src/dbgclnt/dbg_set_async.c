/*********************************************************************************\
 * dbg_set_async.c
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



#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include 	"alloc.h"
#include 	"defs.h"
#include 	"dbg_set_async.h"

// FIXME: These lists should go inside the dbg_invoke struct...

static	list_t*	async_info_list = NULL;
static	int		global_bpset_id = -1;

// static prototypes
static async_info_t*	FindAsyncInfo(void* tree);
static async_info_t*	NewAsyncInfo(void* tree);
static void 			FreeAsyncInfo(async_info_t* async);
static bp_set_t *		NewBPSet(bitset* group, dbgbp_t* bp);
static void				FreeBPSet(bp_set_t* bp_set);
static void				AddBPToBPSet(bitset* group, dbgbp_t* bp, bp_set_t* bpset);
static bp_id_set_t *	NewBPIDSet(bitset* group, int bp_id);
static void				FreeBPIDSet(bp_id_set_t* id_set);
static bp_disp_set_t *	NewBPDispSet(bitset *group, dbgbp_disp bp_disp);
static void				FreeBPDispSet(bp_disp_set_t *disp_set);
static int				CompareBPSetWithBP(bp_set_t* set, dbgbp_t* bp);
static event_set_t *	NewEventSet(dbgevent_t* event, bitset* group);
static void				FreeEventSet(event_set_t* eventset);
static void				DestroyEventSet(async_info_t *, dbgev);
static void				AddGroupToEventSet(bitset *, event_set_t *);

static async_info_t*
FindAsyncInfo(void* tree)
{
	async_info_t* async = NULL;

	if(async_info_list == NULL)
		return NULL;

	SetList(async_info_list);
	while((async = GetListElement(async_info_list)) != NULL)
	{
		if(async->t_tree == tree)
			break;
	}
	
	return async;
}

static async_info_t*
NewAsyncInfo(void* tree)
{
	async_info_t* async = (async_info_t*)Alloc(sizeof(async_info_t));
	async->t_tree = tree;
	async->t_eventset_list = NewList();
	async->t_bpset_list = NewList();

	return async;
}

static void 
FreeAsyncInfo(async_info_t* async)
{
	if(async == NULL)
		return;

	RemoveAllBPSet(async->t_tree);
	RemoveAllEventSet(async->t_tree);

	DestroyList(async->t_eventset_list, NULL);
	DestroyList(async->t_bpset_list, NULL);

	Free(async);
}

int
CreateAsyncInfoForTree(void* tree)
{
	if(async_info_list == NULL)
		async_info_list = NewList();

	async_info_t* async = FindAsyncInfo(tree);
	if( async == NULL)
	{ 
		async = NewAsyncInfo(tree);
		AddToList(async_info_list, async);

		return 0;
	}

	return -1;
}

void
DestroyAsyncInfoForTree(void* tree)
{
	async_info_t* async = FindAsyncInfo(tree);
	RemoveFromList(async_info_list, async);
	FreeAsyncInfo(async);	
}

void
DestroyAllAsyncInfo()
{
	DestroyList(async_info_list, FreeAsyncInfo);
	async_info_list = NULL;
}

static bp_id_set_t*
NewBPIDSet(bitset* group, int bp_id)
{
	bp_id_set_t* id_set = (bp_id_set_t*)Alloc(sizeof(bp_id_set_t));

	id_set->b_group = bitset_dup(group);
	id_set->b_bp_id = bp_id;
	
	return id_set;
}

static void
FreeBPIDSet(bp_id_set_t* id_set)
{
	if(id_set == (bp_id_set_t*)NULL)
		return;

	bitset_free(id_set->b_group);
	id_set->b_group = NULL;
	id_set->b_bp_id = -1;

	Free(id_set);
}

static bp_disp_set_t *
NewBPDispSet(bitset *group, dbgbp_disp bp_disp)
{
	bp_disp_set_t * disp_set = (bp_disp_set_t *)Alloc(sizeof(bp_disp_set_t));
	
	disp_set->b_group = bitset_dup(group);
	disp_set->b_bp_disp = bp_disp;
	
	return disp_set;
}

static void
FreeBPDispSet(bp_disp_set_t *disp_set)
{
	if (disp_set == NULL)
		return;
		
	bitset_free(disp_set->b_group);
	
	Free(disp_set);
}

static bp_set_t*
NewBPSet(bitset* group, dbgbp_t* bp)
{
	bp_id_set_t * 	id_set;
	bp_disp_set_t *	disp_set;

	bp_set_t* bp_set = (bp_set_t*)Alloc(sizeof(bp_set_t));
	memset(bp_set, 0, sizeof(bp_set_t));

	bp_set->b_group = bitset_dup(group);
	
	// Add the group to the enabled set if the bp is enabled
	if (bp->bp_enabled)
	{
		bp_set->b_bp_enabled = bitset_dup(group);
	}
	
	// Add the group and disposition to the disp list
	bp_set->b_bp_disp = NewList();
	disp_set = NewBPDispSet(group, bp->bp_disp);
	AddToList(bp_set->b_bp_disp, (void *)disp_set);
	
	// Set the type, this cannot vary by rank
	bp_set->b_bp_type = bp->bp_type;

	// Set the location
	if(bp->bp_loc.loc_addr != NULL)
		bp_set->b_loc_addr = strdup(bp->bp_loc.loc_addr);
		
	bp_set->b_loc_line = bp->bp_loc.loc_line;
	
	if(bp_set->b_loc_line >= 0)
	{
		bp_set->b_loc_file = strdup(bp->bp_loc.loc_file);
		bp_set->b_loc_func = strdup(bp->bp_loc.loc_func);
	} else {
		bp_set->b_loc_file = NULL;
		bp_set->b_loc_func = NULL;
	}
	
	// Set the expression for watchpoints
	bp_set->b_bp_exp = strdup(bp->bp_exp);

	// Set the id unique to the rank
	bp_set->b_bp_id_list = NewList();
	id_set = NewBPIDSet(group, bp->bp_id);
	AddToList(bp_set->b_bp_id_list, id_set);

	return bp_set;
}

static void
FreeBPSet(bp_set_t* bp_set)
{
	if(bp_set == NULL)
		return;

	if(bp_set->b_group != NULL)
		bitset_free(bp_set->b_group);
		
	if(bp_set->b_bp_id_list != NULL)
		DestroyList(bp_set->b_bp_id_list, FreeBPIDSet);
		
	if (bp_set->b_bp_enabled != NULL)
		bitset_free(bp_set->b_bp_enabled);
		
	if (bp_set->b_bp_disp != NULL)
		DestroyList(bp_set->b_bp_disp, FreeBPDispSet);

	if(bp_set->b_loc_addr != NULL)
		Free(bp_set->b_loc_addr);

	if(bp_set->b_loc_file != NULL)
		Free(bp_set->b_loc_file);

	if(bp_set->b_loc_func != NULL)
		Free(bp_set->b_loc_func);
		
	if(bp_set->b_bp_exp != NULL)
		Free(bp_set->b_bp_exp);
	
	Free(bp_set);
}

void
DisableBPForBPSet(bp_set_t *bp_set, bitset *group)
{
	if (bp_set == NULL || group == NULL)
		return;
		
	bitset_andeqnot(bp_set->b_bp_enabled, group);
}

void
EnableBPForBPSet(bp_set_t *bp_set, bitset *group)
{
	if (bp_set == NULL || group == NULL)
		return;
		
	bitset_oreq(bp_set->b_bp_enabled, group);
}

void
SetBPDispositionForBPSet(dbgbp_disp bp_disp, bp_set_t *bp_set, bitset *group)
{
	list_t *		bpdisp_list;
	bp_disp_set_t *	disp_set;
	int				found = 0;

	if (bp_set == NULL || group == NULL)
		return;
		
	bpdisp_list = bp_set->b_bp_disp;
	
	SetList(bpdisp_list);
	while((disp_set = (bp_disp_set_t *)GetListElement(bpdisp_list)) != NULL)
	{
		if (disp_set->b_bp_disp == bp_disp)
		{
			// This matches the requested disposition
			++found;
			bitset_oreq(disp_set->b_group, group);
		} else
		{
			// This doesn't match the requested disposition, so remove the group
			bitset_andeqnot(disp_set->b_group, group);
			
			// Make sure the disp_set isn't empty, otherwise remove it
			if(bitset_isempty(disp_set->b_group))
			{
				RemoveFromList(bpdisp_list, (void *)disp_set);
				FreeBPDispSet(disp_set);
			}
		}
	}
	
	// If we didn't find the disp_set, we need to create a new entry and add it
	// to the list
	if (!found)
	{
		disp_set = NewBPDispSet(group, bp_disp);
		AddToList(bp_set->b_bp_disp, (void *)disp_set);
	}
}

/*
** Returns 1 if the bp_set is no longer valid.
*/
int
RemoveBPSet(void* tree, bp_set_t* bp_set, int bp_id, bitset* group)
{
	list_t *		bpset_list;
	list_t *		idset_list;
	list_t *		bpdisp_list;
	bp_id_set_t *	idset;
	bp_disp_set_t *	disp_set;
	async_info_t *	async;
	

	if(tree == NULL || bp_set == NULL)
		return 1;

	if ((async = FindAsyncInfo(tree)) == NULL)
		return 1;

	idset_list = bp_set->b_bp_id_list;
	SetList(idset_list);
	while((idset = (bp_id_set_t*)GetListElement(idset_list)) != (bp_id_set_t*)NULL)
	{
		if(idset->b_bp_id == bp_id)
		{
			bitset_andeqnot(idset->b_group, group);
			bitset_andeqnot(bp_set->b_group, group);
			if(bitset_isempty(idset->b_group))
			{
				RemoveFromList(idset_list, (void*)idset);

				FreeBPIDSet(idset);
			}

			break;
		}
	}
	
	// Remove the group from the enabled list
	bitset_andeqnot(bp_set->b_bp_enabled, group);
	
	// Remove the group from the disp list
	bpdisp_list = bp_set->b_bp_disp;
	SetList(bpdisp_list);
	while((disp_set = (bp_disp_set_t *)GetListElement(bpdisp_list)) != NULL)
	{
		// Try to remove the group from this set
		bitset_andeqnot(disp_set->b_group, group);
		
		// Make sure the disp_set isn't empty, otherwise remove it
		if(bitset_isempty(disp_set->b_group))
		{
			RemoveFromList(bpdisp_list, (void *)disp_set);
			FreeBPDispSet(disp_set);
		}
	}
	
	// Remove the breakpoint set if its now empty
	if(SizeOfList(idset_list) == 0)
	{
		bpset_list = async->t_bpset_list;
		RemoveFromList(bpset_list, (void*)bp_set);
		FreeBPSet(bp_set);
		return 1;
	}
	
	return 0;
}

void
RemoveAllBPSet(void* tree)
{
	list_t* 	bpset_list;
	bp_set_t*	bpset;

	async_info_t* async = FindAsyncInfo(tree);
	if(async == NULL)
		return;

	bpset_list = async->t_bpset_list;
	SetList(bpset_list);
	while ( (bpset = (bp_set_t *)GetListElement(bpset_list)) != (bp_set_t *)NULL )
	{
		RemoveFromList(bpset_list, (void*)bpset);
		FreeBPSet(bpset);
	}
}

static void
AddBPToBPSet(bitset* group, dbgbp_t* bp, bp_set_t* bpset)
{
	bp_id_set_t* id_set = NULL;
	bitset_oreq(bpset->b_group, group);

	SetList(bpset->b_bp_id_list);	
	while((id_set = (bp_id_set_t*)GetListElement(bpset->b_bp_id_list)) != (bp_id_set_t*)NULL)
	{
		if(id_set->b_bp_id == bp->bp_id)
			break;
	}

	if(id_set == NULL)
	{
		id_set = NewBPIDSet(group, bp->bp_id);
		AddToList(bpset->b_bp_id_list, id_set);
	}
	else
	{
		bitset_oreq(id_set->b_group, group);
	}
	
	// Add this group to the enabled bitset if enabled
	if (bp->bp_enabled)
	{
		EnableBPForBPSet(bpset, group);
	}
	
	// Set the disposition for the group
	SetBPDispositionForBPSet(bp->bp_disp, bpset, group);
}

static int
CompareBPSetWithBP(bp_set_t* set, dbgbp_t* bp)
{
	// This returns 1 if the breakpoints don't match, otherwise it returns 0 if they do.
	
	// Do not check the state of the enabled since this can be different for every
	// rank
		
	// Do not check the state of the disposition since this can be different for
	// every rank.
		
	// check if both are the same type
	if (set->b_bp_type != bp->bp_type)
		return 1;

	// Here we only want to check locale info if it is a breakpoint, otherwise
	// if its a watchpoint we want to check the expression.
	switch (bp->bp_type)
	{
		case DBGBP_BREAK_TYPE:
			// check if both have the same file name and line number
			if(bp->bp_loc.loc_file != NULL && bp->bp_loc.loc_line >= 0)
			{
				// First check if the file name matches.
				if(strcmp(set->b_loc_file, bp->bp_loc.loc_file))
					return 1;
				// The file name matches, so we can now safely check the line number.
				if(set->b_loc_line != bp->bp_loc.loc_line)
					return 1;
			}
	
			// check if both have the same address
			if(bp->bp_loc.loc_addr != NULL)
			{
				if(strcmp(set->b_loc_addr, bp->bp_loc.loc_addr))
					return 1;
			}
			
			break;
			
		case DBGBP_WATCH_TYPE:
		case DBGBP_AWATCH_TYPE:
		case DBGBP_RWATCH_TYPE:
			// check if both have the same bp expression
			if (bp->bp_exp != NULL)
			{
				if(strcmp(set->b_bp_exp, bp->bp_exp))
					return 1;
			}
			
			break;
			
		default:
			// FIXME: What to do here???
			break;
	}

	// If we reach this point, everything matches
	return 0;
}

void 
AddBP(void* tree, dbgbp_t* bp, bitset* group)
{
	async_info_t *	async;
	list_t * 		bpset_list;
	bp_set_t *		bpset_i;
	bp_set_t *		bpset;

	async = FindAsyncInfo(tree);
	if(async == NULL)
		return;

	bpset_list = async->t_bpset_list;
	SetList(bpset_list);
	while ( (bpset_i = (bp_set_t *)GetListElement(bpset_list)) != (bp_set_t *)NULL )
	{
		if(!CompareBPSetWithBP(bpset_i, bp))
		{
			break;
		}
	}

	if(bpset_i != NULL) /* this breakpoint has been setted */
	{
		AddBPToBPSet(group, bp, bpset_i);
	}
	else
	{
		bpset = NewBPSet(group, bp);
		AddToList(bpset_list, bpset);
	}
}

bp_set_t*
FindBPSetByBP(void* tree, dbgbp_t* bp, int* id)
{
	async_info_t* async;
	list_t* 	bpset_list;
	bp_set_t*	bpset_i;
	int			bpset_id = 0;

	async = FindAsyncInfo(tree);
	if(async == NULL)
		return NULL;

	bpset_list = async->t_bpset_list;
	SetList(bpset_list);
	while ( (bpset_i = (bp_set_t *)GetListElement(bpset_list)) != (bp_set_t *)NULL )
	{
		bpset_id++;

		if(!CompareBPSetWithBP(bpset_i, bp))
		{
			break;
		}
	}

	*id = bpset_id;
	return bpset_i;
}

bp_set_t*
FindBPSetByID(void* tree, int id)
{
	async_info_t* async;
	list_t* 	bpset_list;
	bp_set_t*	bpset_i;
	int			bpset_id = 0;

	if(id < 1)
		return NULL;

	async = FindAsyncInfo(tree);
	if(async == NULL )
		return NULL;

	bpset_list = async->t_bpset_list;
	SetList(bpset_list);
	while ( (bpset_i = (bp_set_t *)GetListElement(bpset_list)) != (bp_set_t *)NULL )
	{
		bpset_id++;

		if(bpset_id == id)
		{
			break;
		}
	}

	return bpset_i;
}

void
SetBPSetHead(void* tree)
{
	async_info_t* async;
	list_t* 	bpset_list;

	async = FindAsyncInfo(tree);
	if(async == NULL )
		return;

	bpset_list = async->t_bpset_list;
	SetList(bpset_list);

	global_bpset_id = 0;
}

bp_set_t*
GetNextBPSet(void* tree, int* id)
{
	async_info_t* async;
	list_t* 	bpset_list;

	async = FindAsyncInfo(tree);
	if(async == NULL )
		return NULL;

	bpset_list = async->t_bpset_list;
	*id = ++global_bpset_id;
	return (bp_set_t*)GetListElement(bpset_list);
}

int
EmptyBPSetList(void* tree)
{
	async_info_t* async;

	async = FindAsyncInfo(tree);
	if(async == NULL )
		return 1;

	return EmptyList(async->t_bpset_list);
}

static event_set_t*
NewEventSet(dbgevent_t* event, bitset* group)
{
	event_set_t* eventset = (event_set_t*)Alloc(sizeof(event_set_t));

	eventset->e_ev = DupEvent(event);
	eventset->e_group = bitset_dup(group);	//duplicate group

	return eventset;
}

static void
FreeEventSet(event_set_t* eventset)
{
	if(eventset == NULL)
		return;

	if(eventset->e_ev != NULL)
		FreeEvent(eventset->e_ev);

	if(eventset->e_group != NULL)
		bitset_free(eventset->e_group);
}

static void
AddGroupToEventSet(bitset* group, event_set_t* eventset)
{
	bitset_oreq(eventset->e_group, group);
}

event_set_t*
AddEvent(void* tree, dbgevent_t* event, bitset* group)
{
	async_info_t * 	async;
	list_t * 		eventset_list = NULL;
	event_set_t *	eventset = NULL;

	async = FindAsyncInfo(tree);
	if(async == NULL )
		return (event_set_t *)NULL;

	eventset_list = async->t_eventset_list;

	SetList(eventset_list);
	
	while ( (eventset = (event_set_t *)GetListElement(eventset_list)) != (event_set_t *)NULL )
	{
		// Call the CompareEvents function and handle it if they are equal
		if (CompareEvents(eventset->e_ev, event) == EV_EQUAL)
		{
			AddGroupToEventSet(group, eventset);
			return eventset;
		}
	}
	
	// If we get here we need to make a new event set
	eventset = NewEventSet(event, group);
	AddToList(eventset_list, eventset);

	return eventset;
}

static void
DestroyEventSet(async_info_t* async, dbgev ev)
{
	list_t* 		eventset_list = NULL;
	event_set_t*	eventset_i = NULL;

	if(async == NULL)
		return;

	eventset_list = async->t_eventset_list;

	SetList(eventset_list);
	while ( (eventset_i = (event_set_t *)GetListElement(eventset_list)) != (event_set_t *)NULL )
	{
		if((ev < 0) || (eventset_i->e_ev->ev_event == ev))
		{
			RemoveFromList(eventset_list, (void *)eventset_i);
			FreeEventSet(eventset_i);
		}
	}
}

// This will remove the event set if an event of ev_event has been recieved for
// every rank in the set
void
CheckForFullEventSet(void *tree, bitset *fullset, dbgev ev)
{
	async_info_t *	async = FindAsyncInfo(tree);
	// A set that represents all ev that we have recieved so far
	bitset *		aggset;
	// list of all event sets for the tree
	list_t * 		eventset_list;
	// individual event set
	event_set_t *	eventset_i;
	
	// Return if async is null
	if (async == NULL)
		return;
	
	// dup the fullset and then clear it 
	aggset = bitset_dup(fullset);
	bitset_clear(aggset);	

	// Create a set that contains all the ev events by looping over
	// every event
	eventset_list = async->t_eventset_list;
	SetList(eventset_list);
	while ((eventset_i = (event_set_t *)GetListElement(eventset_list)) != NULL)
	{
		if (eventset_i->e_ev->ev_event == ev)
		{
			// Add it to the bitset
			bitset_oreq(aggset, eventset_i->e_group);
		}
	}
	
	// Check to see if the aggset is the same as the fullset
	if (bitset_eq(aggset, fullset) == 1)
	{
		// An event of ev has been recieved for every rank in fullset, so
		// we should remove this event
		DPRINT("CheckForFullEventSet: Recieved ev of %s for every rank. Removing event set.\n", DbgevToStr(ev));
		DestroyEventSet(async, ev);
	}
	
	// Destroy the aggset we dup'ed
	bitset_free(aggset);
}

void
RemoveEventSet(void* tree, dbgev ev)
{
	async_info_t* async = FindAsyncInfo(tree);

	if(async != NULL)
		DestroyEventSet(async, ev);
}

void
RemoveAllEventSet(void* tree)
{
	async_info_t* async = FindAsyncInfo(tree);

	if(async != NULL)
		DestroyEventSet(async, -1);
}
