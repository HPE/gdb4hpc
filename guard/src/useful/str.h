/*********************************************************************************\
 * String support defs
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2019 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/




#ifndef _USEFUL_STR_H
#define _USEFUL_STR_H

#ifdef __cplusplus
extern "C" {
#endif

extern char *	skipspace(char *);
extern char *	StripQuotes(char *);
extern char *	SkipTo(char *, char);
extern char *	SkipNum(char *);
extern int		StrToNum(char *);
extern char *	TrimStr(char *);
extern char *	TrimStrWithTarget(char *, char);
extern int		CountOccurence(char, char *);
extern char *	ReplaceStr(const char *, const char *, const char *);
extern int		IsInteger(char *);

#ifdef __cplusplus
}
#endif

#endif /* !_USEFUL_STR_H */
