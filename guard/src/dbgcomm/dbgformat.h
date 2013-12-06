/*
** network format routines.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _DBGFORMAT_H
#define _DBGFORMAT_H

#include "aif.h"
#include "dbgbp.h"
#include "dbgevent.h"
#include "dbgloc.h"
#include "dbgcmd.h"

/*
** The following define function for converting to network protocol
*/
// TODO: Make this static, everything should be an event or a command
extern char *		AIFToSocProto(AIF *);
// Event to network proto string functions
extern char *		EventToSocProto(dbgevent_t *);
// Command to network proto string function
extern char *		CommandToSocProto(dbgcmd cmd, ...);


/*
** The following define functions for converting from network protocol
*/
// TODO: Make this static, everything should be an event or a command
extern AIF *			SocProtoToAIF(char *);
// Network proto string to event functions.
extern dbgevent_t *		SocProtoToEvent(char *);
// Network proto string to command functions.
extern dbgcommand_t *	SocProtoToCommand(char *s);

#endif /* ! _DBGFORMAT_H */
