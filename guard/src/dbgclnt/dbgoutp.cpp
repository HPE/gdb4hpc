/******************************************************************************\
 * dbgoutp.c - Application output redirect interface
 *
 * © 2013-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "dbg_client.h"
#include "defs.h"
#include "useful.h"

/*
** Create a program output stream handler.
** This function returns the g_progoutp_t object on success or NULL on failure.
*/
g_progoutp_t *DbgCreateGenericProgOutp() {
    g_progoutp_t *po;

    po = (g_progoutp_t *)Alloc(sizeof(g_progoutp_t));

    // create the program output pipe
    if (pipe(po->pipefd)) {
        PrintOutError("pipe: %s\n", strerror(errno));
        Free(po);
        return NULL;
    }

    // change the read end to be non-blocking
    if (fcntl(po->p_o_rfd, F_SETFL, O_NONBLOCK) < 0) {
        PrintOutError("pipe: Could not fcntl to O_NONBLOCK.\n");
        Free(po);
        return NULL;
    }

    // create the stream
    if ((po->p_o_stream = fdopen(po->p_o_rfd, "r")) == (FILE *)NULL) {
        PrintOutError("pipe: %s\n", strerror(errno));
        Free(po);
        return NULL;
    }

    // return the object
    return po;
}

// Function to free g_progoutp_t objects
void DbgFreeGenericProgOutp(g_progoutp_t *po) {
    if (po == NULL) return;

    // close the file stream.
    fclose(po->p_o_stream);

    // close the pipe
    close(po->pipefd[0]);
    close(po->pipefd[1]);

    // Free the object
    Free(po);
}
