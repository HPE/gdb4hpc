/*********************************************************************************\
 * Encapsulated communication level information
 *
 * Copyright 2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *********************************************************************************/

#ifndef _DBGCOMM_INFO_H
#define _DBGCOMM_INFO_H

// dbgcomm_info is intended to allow communication specific information
// (e.g. MRNet stream info) to be passed into the debugger back-end,
// stored with breakpoints and the like, then passed back during the
// asynchronous call-backs (e.g. break point hit).    This will allow
// the communication to optimize the message passing.
class dbgcomm_info {
   public:
    dbgcomm_info() {}
    virtual ~dbgcomm_info() {}

    // The debugger back-end should clone instances to store and
    // manage the object lifetime.
    virtual dbgcomm_info* clone() const = 0;
};

#endif
