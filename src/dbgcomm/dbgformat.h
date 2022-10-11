/*********************************************************************************\
 * network format routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2012-2013,2020,2022 Hewlett Packard Enterprise Development LP.
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
 *********************************************************************************/

#ifndef _DBGFORMAT_H
#define _DBGFORMAT_H

#include "dbgbp.h"
#include "dbgcmd.h"
#include "dbgevent.h"
#include "dbgloc.h"
#include "socstream.h"

// Event to network proto string functions. Frees passed event.
extern char *EventToSocProto(dbgevent_t *);

// Create the protocol string for a command, \a args is the set of
// parameters to be encoded into the string.
template<typename ...Targs>
std::string CommandToSocProto(dbgcmd cmd, Targs&&... args) {
    try {
        soc::OSocStream str(cmd);
        (void) (str << ... << std::forward<Targs>(args));
        return str.str();
    } catch (std::exception& err) {
        fprintf(stderr, "CommandToSocProto failed: %s\n", err.what());
        return "";
    }
}

/*
** The following define functions for converting from network protocol
*/
// Network proto string to event functions.
extern dbgevent_t *SocProtoToEvent(const char *);

#endif /* ! _DBGFORMAT_H */
