/******************************************************************************\
 * type_cmd.h - Types for command routines.
 *
 * © 2011-2013 Cray Inc.  All Rights Reserved.
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

#ifndef _TYPE_CMD_H
#define _TYPE_CMD_H

#include	"list.h"
#include	"invoke.h"
#include	"dbg_set_sync.h"

enum cmdres
{
	cmd_ok,
	cmd_notfound,
	cmd_ambig,
	cmd_ambig_set,
	cmd_ambig_info
};
typedef enum cmdres		cmdres_t;

enum cmdptype
{
	proc_required,
	proc_optional,
	proc_ignore
};
typedef enum cmdptype		cmdptype_t;

struct cmdtype
{
	char *		c_name;
	char *		c_alias;
	cmdptype_t	c_ptype;
	int			c_hassub;
	int			c_multi;
	char *		c_desc;
	char **		c_help;
	int			(*c_cmd)(rec_set_t *, int, char **);
};
typedef struct cmdtype		cmdtype_t;

struct cmdlist
{
	list_t *			cmd_list;
	void				(*cmd_notfound)(char *);
	void				(*cmd_ambig)(char *);
	void				(*cmd_failed)(void);
	struct cmdtype *	cmd_default;
};
typedef struct cmdlist		cmdlist_t;

struct cmdhelp
{
	char *		c_name;
	cmdtype_t *	(*c_find)(char *);
};
typedef struct cmdhelp		cmdhelp_t;

#endif /* _TYPE_CMD_H */
