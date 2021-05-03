/*********************************************************************************\
 * Argument definitions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _USEFUL_ARGS_H
#define _USEFUL_ARGS_H

#ifdef __cplusplus
#include <string>
extern std::string ArgsToString(int argc, char** argv);

extern "C" {
#endif

extern char **Str2Argv(const char *);
extern char **NewArgv(const char *, ...);
extern char **CopyArgv(char **);
extern char **AppendArgv(char **, char **);
extern char **AppendArgs(char **, ...);
extern char *ArgsToBuf(char **, int);
extern void FreeArgv(char **);

#ifdef __cplusplus
}
#endif

#endif /* !_USEFUL_ARGS_H */
