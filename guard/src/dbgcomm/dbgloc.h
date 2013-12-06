/*
** Program location.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _DBGLOC_H
#define _DBGLOC_H

struct dbgloc
{
	char *	loc_file;
	char *	loc_func;
	char *	loc_addr;
	int		loc_line;
};
typedef struct dbgloc	dbgloc_t;

#endif /* !_DBGLOC_H */
