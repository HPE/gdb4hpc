/******************************************************************************\
 * MIEnvironment.h - Extended libmi support for -environment-* commands.
 *
 * © 2012 Cray Inc.  All Rights Reserved.
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
 
#ifndef _MIENVIRONMENT_H_
#define _MIENVIRONMENT_H_
 
#include "MICommand.h"
#include "MIString.h"

extern MIString *MIGetEnvironmentDirectoryInfo(MICommand *cmd);

#endif /* _MIENVIRONMENT_H_ */
