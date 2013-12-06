/*
** File info defines.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _FILE_H
#define _FILE_H

#include	"list.h"

struct file_t
{
	char *		f_name;
	list_t *	f_lines;
	int			f_ref;
};
typedef struct file_t  file_t;

extern file_t *	AddFile(list_t **, char *);
extern void	DestroyFile(file_t *);
#endif /* !_FILE_H */
