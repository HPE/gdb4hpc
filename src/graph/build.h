/*********************************************************************************\
 * Graph builder definitions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 **********************************************************************************/

#ifndef _BUILD_H
#define _BUILD_H

#include "expr.h"
#include "graph.h"
#include "list.h"

extern void BuildUnused(graph_t *);
extern int BuildAssert(graph_t *, int, expr_t *, rec_set_t *, list_t *);
extern int BuildExpr(graph_t *, int *, int *, list_t *, expr_t *, rec_set_t *);

#endif /* _BUILD_H */
