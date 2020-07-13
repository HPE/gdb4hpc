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
#include <sstream>

// TODO: Make this static, everything should be an event or a command
extern char *AIFToSocProto(AIF *);
// Event to network proto string functions
extern char *EventToSocProto(dbgevent_t *);

/*
** The following define functions for converting from network protocol
*/
// TODO: Make this static, everything should be an event or a command
extern AIF *SocProtoToAIF(char *);
// Network proto string to event functions.
extern dbgevent_t *SocProtoToEvent(char *);

// String assembler class for CommandToSocProto -- it just needs to define an
// overrider for every type used as a command argument
class OSocStream {
private:
    dbgcmd m_cmd;
    std::ostringstream m_str;

public:
    OSocStream(dbgcmd id);

    OSocStream& operator<<(int i);
    OSocStream& operator<<(unsigned int i);
    OSocStream& operator<<(long i);
    OSocStream& operator<<(size_t i);
    OSocStream& operator<<(const char*s);
    OSocStream& operator<<(const std::string& s);
    OSocStream& operator<<(const std::pair<int,char**>& args);
    OSocStream& operator<<(AIF* val);

    // retrieve the result string
    std::string str();
};

// Create the protocol string for a command, \a args is the set of
// parameters to be encoded into the string.
template<typename ...Targs>
std::string CommandToSocProto(dbgcmd cmd, Targs&&... args) {
    OSocStream str(cmd);
    (void) (str << ... << std::forward<Targs>(args));
    return str.str();
}


#endif /* ! _DBGFORMAT_H */
