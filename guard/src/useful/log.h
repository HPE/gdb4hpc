/*********************************************************************************\
 * log.h - Header file for the log interface.
 *
 * © 2011 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL: http://svn/svn/debugger-support/tools_interface/trunk/useful/log.h $
 * $Date: 2011-02-24 17:03:50 -0600 (Thu, 24 Feb 2011) $
 * $Rev: 797 $
 * $Author: andrewg $
 *
 *********************************************************************************/

#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>

#define DBG_LOG_ENV_VAR "CRAY_DBG_LOG_DIR"

FILE *  create_log(int, char *);
FILE *  open_log(int, char *);
int     write_log(FILE *, char *);
int     write_log2(FILE *, char *);
int     close_log(FILE *);
int     hook_stdoe(FILE *);

#endif /* _LOG_H */
