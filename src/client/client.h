/******************************************************************************\
 * client.h - Client interface definitions.
 *
 * Copyright 2011-2013,2020-2022 Hewlett Packard Enterprise Development LP.
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

#ifndef _CLIENT_H
#define _CLIENT_H

#include <stdarg.h>
#include <string.h>

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
#include "useful.h"

#include "interpreter.h"
#include "interpreter_console.h"
#include "interpreter_mi.h"

/*
** Functions defined in main.c
*/
extern void ForceQuit(void);

/*
** Functions defined in cmd_frame.c
*/
extern void PrintFrameSet(const char *, Bitset *, const dbgframe_t *);

/*
** Functions defined in cmd_proc.c
*/
extern void PrintHaltedLocationSet(const char *, const Bitset *, dbgloc_t *);
extern void EndAll();

#endif /* _CLIENT_H */
