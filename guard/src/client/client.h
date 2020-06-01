/******************************************************************************\
 * client.h - Client interface definitions.
 *
 * Copyright 2011-2013,2020 Cray Inc.
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

#ifndef _CLIENT_H
#define _CLIENT_H

#include <stdarg.h>

/*
** These includes define a common interface that many of the functions which
** interface with the client need to access. We place them here to ensure that
** the function that is interfacing with the client has the definitions it needs
** through including a single header.
*/
#include "cli_funcs.h"
#include "client_settings.h"
#include "cmd.h"
#include "dbg_client.h"
#include "defs.h"
#include "gram.h"
#include "useful.h"
#include "xml_tag.h"

/*
** Functions defined in main.c
*/
extern int EventCB(event_set_t *, bitset *);
extern void ForceQuit(void);

/*
** Functions defined in cmd_env.c
*/
// Note that normally we don't want to expose command functions directly, but
// for the quit command we want access to it from the passthru mode. I could
// call the FindCommand function, but that requires that the env init be called
// before the passthru init. I don't like this requirement, so I am exposing the
// cmd_quit_cmd directly.
extern int cmd_quit_cmd(rec_set_t *, int, char **);

/*
** Functions defined in cmd_frame.c
*/
extern void PrintFrameSet(char *, bitset *, dbgframe_t *);

/*
** Functions defined in cmd_print.c
*/
extern int PrintVal(val_t *, rec_set_t *);

/*
** Functions defined in cmd_proc.c
*/
extern void PrintHaltedLocationSet(char *, bitset *, dbgloc_t *);
extern void EndAll(void);
extern void SetLaunchNonStopMode(bool isNonStop);
extern bool GetLaunchNonStopMode();

#endif /* _CLIENT_H */
