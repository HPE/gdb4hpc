/******************************************************************************\
 * Frontend debugger interface definition
 * TODO: This does not support asynchronous commands.
 *
 * © 2013 Cray Inc.	All Rights Reserved.
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
 ******************************************************************************/

#ifndef _DBGCLNT_DEBUGFE_H
#define _DBGCLNT_DEBUGFE_H

#include	"bitset.h"
#include	"dbgevent.h"
#include	"dbg_set_sync.h"
#include	"list.h"

typedef void *	dbgfrontend_sess;

struct set_location
{
	bitset *	set;
	char *		loc_str;
	int			count;
};
typedef struct set_location set_location;

struct dbgfrontend_sw
{
	char *				dbg_name;
	dbgfrontend_sess	dbg_session;
	dbgstat				dbg_status;
	int					dbg_list_isPending;
	list_t *			dbg_list_locs;

	dbgevent_t *		(*dbg_init)(struct dbgfrontend_sw *);
	dbgevent_t *		(*dbg_quit)(struct dbgfrontend_sw *);
	dbgevent_t *		(*dbg_set_file)(struct dbgfrontend_sw *, char *);
	dbgevent_t *		(*dbg_list)(struct dbgfrontend_sw *, char *);
	dbgevent_t *		(*dbg_getsources)(struct dbgfrontend_sw *);
	dbgevent_t *		(*dbg_setsourcepath)(struct dbgfrontend_sw *, char *);
	dbgevent_t *		(*dbg_getsourcepath)(struct dbgfrontend_sw *);
	dbgevent_t *		(*dbg_getscope)(struct dbgfrontend_sw *, char *);
};
typedef struct dbgfrontend_sw	dbgfrontend_sw;

// Used by debug frontend definitions to set their instantiation functions
struct dbgfrontend_sw_create
{
	void	(*dbg_instantiate)(dbgfrontend_sw *);
};
typedef struct dbgfrontend_sw_create	dbgfrontend_sw_create;

// Functions used for managing state information for DbgList commands
extern int				AddListStateLocation(dbgfrontend_sw *, bitset *, char *);
extern set_location *	GetListStateLocation(dbgfrontend_sw *);
extern void				SetListStateLocation(dbgfrontend_sw *);
extern void				ClearListState(dbgfrontend_sw *);

// Interface functions
extern dbgfrontend_sw *	DbgInstantiateFE(char *);
extern dbgevent_t *		DbgDestroyFE(dbgfrontend_sw *);
extern dbgevent_t *		DbgFESetFile(dbgfrontend_sw *, char *);
extern dbgevent_t *		DbgFEList(dbgfrontend_sw *, char *);
extern dbgevent_t *		DbgFEGetSources(dbgfrontend_sw *);
extern dbgevent_t *		DbgFESetSourcePath(dbgfrontend_sw *, char *);
extern dbgevent_t *		DbgFEGetSourcePath(dbgfrontend_sw *);
extern dbgevent_t *		DbgFEGetScope(dbgfrontend_sw *, char *);

#endif /* !_DBGCLNT_DEBUGFE_H */
