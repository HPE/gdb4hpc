/******************************************************************************\
 * dbg_client_proxy.c - Debugger proxy dbg_client routines
 *
 * Copyright 2013,2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "dbg_client.h"
#include "proxy.h"
#include "useful.h"

/*
** Global variables
*/
list_t *DbgInvokeList = (list_t *)NULL;

/*
** Create a dbginvoke_t structure.
*/
dbginvoke_t *NewDbgInvoke(int nproc, DebugClientProtocol * const proto) {
    dbginvoke_t *di = new dbginvoke_t(proto);

    // Set non-zero members here
    di->d_nproc = nproc;
    // di->d_proto = proto; // proto set in constructor
    // XXX: This interface doesn't make use of the d_eventset_list or
    // d_pbset_list All of that gets forwarded to the client side.

    if (DbgInvokeList == NULL) DbgInvokeList = NewList();

    /*
    ** Add to DbgInvokeList
    */
    AddToList(DbgInvokeList, (void *)di);

    return di;
}

/*
** Free a dbginvoke_t structure.
*/
void FreeDbgInvoke(dbginvoke_t *di) {
    RemoveFromList(DbgInvokeList, (void *)di);

    // Call the protocol destructor if there is one
    if (DBG_PROTO_SW(di)->hasDestroyClient() == true) {
        DBG_PROTO_SW(di)->destroyClient(di);
    }

    if (di->d_bitset_full != NULL) bitset_free(di->d_bitset_full);

    if (di->d_bitset_launched != NULL) bitset_free(di->d_bitset_launched);

    if (di->d_cmd != NULL) Free(di->d_cmd);

    // remove any event fds from the proto
    DestroyList(di->d_evfds, Free);

    // XXX: This interface doesn't make use of the d_eventset_list or
    // d_pbset_list All of that gets forwarded to the client side.

    Free(di);
}
