/*********************************************************************************\
 * network format routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2012-2013,2020 Cray Inc.
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

#ifndef _DBGFORMAT_H
#define _DBGFORMAT_H

#include "aif.h"
#include "dbgbp.h"
#include "dbgcmd.h"
#include "dbgevent.h"
#include "dbgloc.h"
/*
** The following define function for converting to network protocol
*/
class EventToSocProtoStream {
public:
    // takes ownership of event
    EventToSocProtoStream(dbgevent_t* event);
    ~EventToSocProtoStream();

    // user is responsible for freeing the result
    char* getNextProtoString();

    // user is responsible for freeing the result
    char* getSingleProtoString();

private:
    char* m_protoString;
    AIFStream* m_aifStream;
};

// TODO: Make this static, everything should be an event or a command
extern char *AIFToSocProto(AIF *);
// Event to network proto string functions
extern char *EventToSocProto(dbgevent_t *);
// Command to network proto string function
extern char *CommandToSocProto(dbgcmd cmd, ...);

/*
** The following define functions for converting from network protocol
*/
// TODO: Make this static, everything should be an event or a command
extern AIF *SocProtoToAIF(char *);
// Network proto string to event functions.
extern dbgevent_t *SocProtoToEvent(char *);
// Network proto string to command functions.
extern dbgcommand_t *SocProtoToCommand(char *s);

#endif /* ! _DBGFORMAT_H */
