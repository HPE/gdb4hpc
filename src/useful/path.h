/*********************************************************************************\
 * path.h - Header file for the path interface.
 *
 * Copyright 2011-2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _PATH_H
#define _PATH_H

#ifdef __cplusplus
extern "C" {
#endif

#define EXTRA_LIBRARY_PATH "/lib64:/usr/lib64:/lib:/usr/lib"

char *pathFind(const char *file, const char *path=nullptr);
bool pathHas(const char* file, const char *path=nullptr);
char *libFind(const char *, const char *);
int adjustPaths(const char *);
int adjustLibPath(const char *);
int adjustExecPath(const char *);
char *getinstallbase(void);
char *getexepath(void);

#ifdef __cplusplus
}
#endif

#endif /* _PATH_H */
