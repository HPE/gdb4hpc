/* interface of asychronous group operation */
#ifndef _DBG_SET_ASYNC_H
#define _DBG_SET_ASYNC_H

#include "bitset.h"
#include "dbgevent.h"

// Async stuff
struct async_info
{
	void *			t_tree;
	list_t *		t_eventset_list;
	list_t *		t_bpset_list; 
};
typedef struct async_info async_info_t;

// bp stuff
struct bp_id_set
{
	bitset *		b_group;
	int				b_bp_id; //unique bkpt id assigned by each gdb
};
typedef struct bp_id_set bp_id_set_t;

struct bp_disp_set
{
	bitset *		b_group;
	dbgbp_disp		b_bp_disp;
};
typedef struct bp_disp_set bp_disp_set_t;

struct bp_set
{
	// group information
	bitset *		b_group;
	
	// breakpoint unique gdb id's
	list_t *		b_bp_id_list;	// a list of bp_id_set_t
	
	// breakpoint information
	bitset *		b_bp_enabled;	// set representing enabled breakpoints, this can vary by rank.
	list_t *		b_bp_disp;		// a list of bp_disp_set_t representing breakpoint disposition (i.e. keep, dis, del), this can vary by rank.
	dbgbp_type		b_bp_type;		// breakpoint type (i.e. breakpoint, watchpoint)
	
	// breakpoint locale
	char *			b_loc_addr;		// locale address
	char *			b_loc_file;		// locale file
	char *			b_loc_func;		// locale function
	int				b_loc_line;		// locale line
	
	char *			b_bp_exp;		// watchpoint expression
};
typedef struct bp_set bp_set_t;

// event stuff
struct event_set
{
	bitset *		e_group;
	dbgevent_t *	e_ev;
};
typedef struct event_set event_set_t;

// async funcs
extern int	 			CreateAsyncInfoForTree(void* tree); //called by DoInvoke()
extern void 			DestroyAsyncInfoForTree(void* tree); //callled by ReleaseInv()
extern void 			DestroyAllAsyncInfo(); //called by ReleaseAll()

// bp funcs
extern void				AddBP(void* tree, dbgbp_t* bp, bitset* group);
extern int				RemoveBPSet(void*tree, bp_set_t* bp_set, int bp_id, bitset* group);
extern void				RemoveAllBPSet(void* tree);
extern bp_set_t * 		FindBPSetByBP(void* tree, dbgbp_t* bp, int* id);
extern bp_set_t * 		FindBPSetByID(void* tree, int id);
extern void				SetBPSetHead(void* tree);
extern bp_set_t *		GetNextBPSet(void* tree, int* id);
extern int				EmptyBPSetList(void* tree);
extern void				DisableBPForBPSet(bp_set_t *bp_set, bitset *group);
extern void				EnableBPForBPSet(bp_set_t *bp_set, bitset *group);
extern void				SetBPDispositionForBPSet(dbgbp_disp bp_disp, bp_set_t *bp_set, bitset *group);

// event funcs
extern event_set_t *	AddEvent(void* tree, dbgevent_t* event, bitset* group);
extern void				CheckForFullEventSet(void *, bitset *, dbgev);
extern void				RemoveEventSet(void* tree, dbgev ev);
extern void				RemoveAllEventSet(void* tree);

#endif /* _DBG_SET_ASYNC_H */
