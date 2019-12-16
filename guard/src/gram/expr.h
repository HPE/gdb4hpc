/*********************************************************************************\
 * Expression definitions.
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
 *********************************************************************************/

#ifndef _EXPR_H
#define _EXPR_H

#include	"list.h"
#include	"hash.h"

#define EXPRTAB_SIZE    100

#define TOP(e)		&((e)->e_expr[(e)->e_eval])
#define POP(e)		&((e)->e_expr[(e)->e_eval--])
#define PUSH(e)	(e)->e_eval++
#define RESET(e)	(e)->e_eval = (e)->e_lval - 1
#define EMPTY(e)	((e)->e_eval < 0)
#define LAST(e)	&((e)->e_expr[(e)->e_lval - 1])

#define MODE_EXPR		1	/* parsing an expression */
#define MODE_FUNC		2	/* parsing a function definition */
#define	 MODE_COMP		3	/* parsing a compare */
#define	 MODE_ASSERT	4	/* parsing an assert */
#define	 MODE_IMM		5	/* immediate mode */
#define	 MODE_DECL		6	/* declarative mode */

enum expr_op
{
	EXPR_OPADD,			/* addition */
	EXPR_OPSUB,			/* subtraction */
	EXPR_OPMUL,			/* multiplication */
	EXPR_OPDIV,			/* division */
	EXPR_OPREM,			/* remainder */	// TODO: Broken to fix derived fortran types
	EXPR_OPPOS,			/* unary addition */
	EXPR_OPNEG,			/* unary negation */
	EXPR_OPAND,			/* logical and */
	EXPR_OPOR,			/* logical or */
	EXPR_OPEQUAL,		/* equal */
	EXPR_OPNOTEQUAL,	/* not equal */
	EXPR_OPLEQ,			/* less than or equal */
	EXPR_OPGEQ,			/* greater than or equal */
	EXPR_OPLT,			/* less than */
	EXPR_OPGT,			/* greater than */
	EXPR_OPCOND,		/* conditional */
	EXPR_OPSUBSCR,		/* array subscript */
	EXPR_OPCOMMA,		/* comma */
	EXPR_OPDREF,		/* dereference operation */
	EXPR_OPADDROF,		/* address-of operation */
	EXPR_OPELEMENTREF,	/* element selection by reference */
	EXPR_OPELEMENTPTR,	/* element selection through pointer */
	EXPR_OPELEMENTACC,  /* element access for derived type */
	EXPR_OPCAST,		/* Type cast */
	EXPR_TYPE,			/* Type information */
	EXPR_TYPE_SPEC,		/* Type specifier */
	EXPR_TYPE_IDENT,	/* TYpe identifier */
	EXPR_TYPE_PTR,		/* Type pointer */
	EXPR_LOC,			/* program location */
	EXPR_SLICE,			/* array slice */
	EXPR_PROC,			/* process specifier */
	EXPR_VAR,			/* program variable */
	EXPR_GDBVAR,		/* language-specific variable */
	EXPR_INT,			/* integer */
	EXPR_FLOAT,			/* floating point */
	EXPR_DSYM,			/* debugger variable */
	EXPR_SYM,			/* symbol */
	EXPR_STRING,		/* string */
	EXPR_STAR,			/* '*' use in blockmaps */
	EXPR_FUNC,			/* function call */
	EXPR_LIST_START,	/* expression list start */
	EXPR_LIST,			/* expression list element */
	EXPR_BLOCKMAP,		/* blockmap call */ // Minh
	EXPR_ASSERT,		/* assertion definition*/
	EXPR_HASH_BOB,		/* Bob Jenkins hashing call */
	EXPR_REDUCE_NONE,	/* no hash function use call */
	EXPR_SUM,			/* sum function call */
	EXPR_PRODUCT,		/* product function call */
	EXPR_AVERAGE,		/* average function call */
	EXPR_MIN,			/* min function call */
	EXPR_MAX,			/* max function call */
	EXPR_STDEV,			/* standard deviation function call */
	EXPR_COUNTELEM,		/* element counting function call */
	EXPR_P2P,			/* p2p comp mode */
	EXPR_HISTOGRAM,		/* histogram function call */
	EXPR_UNKNOWN		/* unknown error */
};
typedef enum expr_op	expr_op;

// use the following to stringize the above enumeration
extern const char *	getExprOpStr(expr_op);

union expr_val
{
	char *		e_v_string;
	int			e_v_int;
	double		e_v_float;
	hentry_t *	e_v_sym;
	char *		e_v_sym_str;
};
typedef union expr_val		expr_val;

struct expr_elt
{
	expr_op		e_op;
	expr_val	e_val;
#define e_string	e_val.e_v_string
#define e_int		e_val.e_v_int
#define e_float	e_val.e_v_float
#define e_sym		e_val.e_v_sym
#define e_sym_str	e_val.e_v_sym_str
};
typedef struct expr_elt	expr_elt;

struct expr_t
{
	int			e_len;
	int			e_lval;
	int			e_eval;
	expr_elt	e_expr[1];
};
typedef struct expr_t		expr_t;

extern char **	ExprStr;
#endif /* !_EXPR_H */

