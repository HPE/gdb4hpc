/*********************************************************************************\
 * path.h - Header file for the path interface.
 *
 * © 2011 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL: http://svn/svn/debugger-support/tools_interface/trunk/useful/path.h $
 * $Date: 2011-02-24 17:03:50 -0600 (Thu, 24 Feb 2011) $
 * $Rev: 797 $
 * $Author: andrewg $
 *
 *********************************************************************************/

#ifndef _PATH_H
#define _PATH_H

#define EXTRA_LIBRARY_PATH  "/lib64:/usr/lib64:/lib:/usr/lib"

char *pathFind(const char *, const char *);
char *libFind(const char *, const char *);
int adjustPaths(char *);
char *pathToName(char *);

#endif /* _PATH_H */
