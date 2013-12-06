/*
** String support defs
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _USEFUL_STR_H
#define _USEFUL_STR_H

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

#endif /* !_USEFUL_STR_H */
