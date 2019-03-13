/*********************************************************************************\
 * Node definitions
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
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
 **********************************************************************************/

#ifndef _NODEDEF_H
#define _NODEDEF_H

#define MAXARGCHECK	5

struct nodedef_t
{
	char *		nd_name;		/* node name */
	int			nd_min;			/* minimum number of args */
	int			nd_max;			/* maximum number of args */
	node_type	nd_type;		/* behaviour of node */
	token_type	nd_otype;		/* node output type */

	token_t *	(*nd_func)(graph_t *, node_t *);
	/*
	int		(*nd_flow)(graph_t *, node_t *);
	int		(*nd_fire)(graph_t *, node_t *);
	void	(*nd_output)(graph_t *, node_t *, token_t *);
	void	(*nd_input)(graph_t *, node_t *, input_t *, token_t *);
	*/
	token_type	nd_args[MAXARGCHECK];
};
typedef struct nodedef_t	nodedef_t;

extern nodedef_t *	AddNodeDef(char *, token_t *(*)(graph_t *, node_t *), token_type, int, int, ...);
extern void			InitNodeDef(void);
extern void			FinishNodeDef(void);
extern nodedef_t *	FindNodeDef(char *);

#endif /* !_NODEDEF_H */
