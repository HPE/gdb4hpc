/******************************************************************************\
 * dbg_client_proxy.c - Debugger proxy dbg_client routines
 *
 * © 2013 Cray Inc.  All Rights Reserved.
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
 
// TODO: These routines are copy-pasta from dbg_client.c. The proxy daemon 
//       requires these functions to be linked in, and until I design a better
//       interface it works for now. It's a bad idea though since both versions
//       need to be maintained.

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "dbg_client.h"
#include "useful.h"

#include "proxy.h"

/*
** Global variables
*/
list_t *		DbgInvokeList = (list_t *)NULL;

/*
** Create a dbginvoke_t structure.
*/
dbginvoke_t *
NewDbgInvoke(int nproc, dbgclnt_proto *proto)
{
	dbginvoke_t *		di;

	di = (dbginvoke_t *)Alloc(sizeof(dbginvoke_t));
	memset(di, 0, sizeof(dbginvoke_t));
	
	// Set non-zero members here
	di->d_nproc = nproc;
	di->d_proto = proto;

	if ( DbgInvokeList == (list_t *)NULL )
		DbgInvokeList = NewList();

	/*
	** Add to DbgInvokeList
	*/
	AddToList(DbgInvokeList, (void *)di);

	return di;
}

/*
** Free a dbginvoke_t structure.
*/
void
FreeDbgInvoke(dbginvoke_t *di)
{
	int *	ev_fd;

	RemoveFromList(DbgInvokeList, (void *)di);
	
	// Call the protocol destructor if there is one
	if ( DBG_PROTO_SW(di)->sw_destroyclnt != NULL )
	{
		DBG_PROTO_SW(di)->sw_destroyclnt(di);
	}

	if( di->d_bitset_full != NULL )
		bitset_free(di->d_bitset_full);

	if( di->d_bitset_launched != NULL )
		bitset_free(di->d_bitset_launched);
		
	// remove any event fds from the proto if there are any for this dbginvoke.
	if (di->d_evfds != NULL)
	{
		SetList(di->d_evfds);
		while((ev_fd = (int *)GetListElement(di->d_evfds)) != NULL)
		{
			Free(ev_fd);
		}
	}
	DestroyList(di->d_evfds, NULL);

	Free(di);
}

