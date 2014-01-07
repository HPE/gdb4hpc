/*
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
** See COPYRIGHT in top-level directory.
**
** Node definitions
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<time.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<stdlib.h>
#include	<string.h>
#include	<signal.h>
#include	<unistd.h>
#include	<math.h>

#include	"defs.h"
#include	"client.h"
#include	"useful.h"
#include	"df.h"

#define TSTORE_SIZE	100
#define HSTORE_SIZE	7	/* 2^7 = 128 */

enum arith_op
{
	op_add,
	op_sub,
	op_mul,
	op_div,
	op_rem,
	op_eq,
	op_neq,
	op_leq,
	op_geq,
	op_lt,
	op_gt,
	op_and,
	op_or,
	op_neg
};
typedef enum arith_op	arith_op;

token_t *	TokenStore[TSTORE_SIZE];
htable_t *	HashStore = (htable_t *)NULL;
htable_t *	InvokeStore = (htable_t *)NULL;

token_t *	binop_apply(graph_t *, node_t *, arith_op);
token_t *	unop_apply(graph_t *, node_t *, arith_op);
token_t *	binop_aif(graph_t *, node_t *, arith_op, char *, AIF *, char *, AIF *);

int socket_on=0;

/*
** Clean up on exit.
*/
void
Cleanup(int status)
{
	ExitDF();
}

/*************************************************************
**************************************************************
**															**
**		   Node Definition Routines							**
**															**
**************************************************************
*************************************************************/

/*
** Compare two data tokens. Call the graph compare routine to do the
** comparison. Returns 1 to continue or 0 to exit.
*/
token_t *
NodeCompare(graph_t *g, node_t *n)
{
	token_t *		t0;
	token_t *		t1;
	token_t *		t2;
	token_t *		t3;
	token_t *		t;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);
	t3 = GetTokenFromInput(n, 3);

	t = NewToken(g, TK_INT);

	// Note that the assertid loops. This MUST be modded by the number
	// of assertions.
	t->t_int = g->g_compare(g, (t0->t_int % (g->g_assertions)),
		t1->t_name, t1->t_data,
		t2->t_name, t2->t_data,
		t3->t_int);

	DPERFLOG(SetTimerStopAndLog("client time"));
	DPERFLOG(SetTimerStopAndLog("total time"));
	DPERFLOG(CloseTimerLog());

	return t;
}

/*
** Display the result of p2p comp operation and also determine if the graph needs to continue
** Returns 1 to continue or 0 to exit.
*/
token_t *
NodeDisplayP2P(graph_t *g, node_t *n)
{
	token_t *		t0;
	token_t *		t1;
	token_t *		t;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);

	t = NewToken(g, TK_INT);

	// Note that the assertid loops. This MUST be modded by the number
	// of assertions.
	t->t_int = g->g_display_p2p(g, (t0->t_int % (g->g_assertions)),
		t1->t_name, t1->t_data);

	DPERFLOG(SetTimerStopAndLog("client time"));
	DPERFLOG(SetTimerStopAndLog("total time"));
	DPERFLOG(CloseTimerLog());

	return t;
}

/*
** Display the contents of a token. Passes token on just in
** case you want to do something after display (like exit for example).
*/
token_t *
NodeDisplay(graph_t *g, node_t *n)
{
	token_t *		t0;
	token_t *		t1;
	token_t *		t2;
	token_t *		t3;
	token_t *		t4;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);
	t3 = GetTokenFromInput(n, 3);
	t4 = GetTokenFromInput(n, 4);

	if ( strcmp(t0->t_str, "none") == 0 )
		return DupToken(t2);

	if
	(
		t2->t_type != TK_DATA
		||
		t3->t_type != TK_DATA
		||
		t4->t_type != TK_DATA
	)
		DFError(n, "Display: Token must contain data\n");

	if ( !g->g_display(t0->t_str, t1->t_str, 
			t2->t_name, t2->t_data, 
			t3->t_name, t3->t_data, 
			t4->t_name, t4->t_data) )
	{
		DFError(n, "Display routine failed\n");
		return (token_t *)NULL;
	}

	return DupToken(t2);
}

int
IsEqual(token_t * t1, token_t *t2)
{
	switch ( t1->t_type )
	{
	case TK_BOOL:
		return t1->t_bool == t2->t_bool;

	case TK_INT:
		return t1->t_int == t2->t_int;

	case TK_INT_SET:
	{
		int t1_length = t1->t_set_length;
		int t2_length = t2->t_set_length;
		int i;

		if (t1_length != t2_length)
			return 0;

		for ( i = 0 ; i < t1_length ; i++ )
		{
			if ( !bitset_test(t1->t_set_bits, i)  &&
					!bitset_test(t2->t_set_bits, i) )
			{
				continue;
			}

			if ( t1->t_int_set[i] != t2->t_int_set[i] )
				return 0;
		}
		return 1;
	}

	case TK_FLOAT:
		return t1->t_float == t2->t_float;

	case TK_ADDR:
		return t1->t_addr == t2->t_addr;

	case TK_STR:
		return strcmp(t1->t_str, t2->t_str) == 0;

	case TK_INV:
		return t1->t_inv == t2->t_inv;

	case TK_PROC_SET:
		return t1->t_proc_set == t2->t_proc_set;
	}

	return 0;
}

int
IsGreater(token_t * t1, token_t *t2)
{
	switch ( t1->t_type )
	{
	case TK_INT:
		return t1->t_int > t2->t_int;

	case TK_FLOAT:
		return t1->t_float > t2->t_float;

	case TK_ADDR:
		return t1->t_addr > t2->t_addr;

	case TK_STR:
		return strcmp(t1->t_str, t2->t_str) > 0;
	}

	return 0;
}

int
IsLess(token_t * t1, token_t *t2)
{
	switch ( t1->t_type )
	{
	case TK_INT:
		return t1->t_int < t2->t_int;

	case TK_FLOAT:
		return t1->t_float < t2->t_float;

	case TK_ADDR:
		return t1->t_addr < t2->t_addr;

	case TK_STR:
		return strcmp(t1->t_str, t2->t_str) < 0;
	}

	return 0;
}

/*
** Pass token if input values are equal.
*/
token_t *
NodePassIfEqual(graph_t *g, node_t *n)
{
	token_t *	t0;
	token_t *	t1;
	token_t *	t2;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);

	if ( t0->t_type != t1->t_type )
	{
		if (t0->t_type == TK_INT_SET && t1->t_type == TK_INT)
		{
			// DONNY FIXME
			int set_length = t0->t_set_length;
			int i;

			for ( i = 0 ; i < set_length ; i++ )
			{
				if ( !bitset_test(t0->t_set_bits, i) )
				{
					continue;
				}

				if ( t0->t_int_set[i] != t1->t_int )
				{
//					DFError(n, "Values are not the same %d - %d\n", t0->t_int_set[i], t1->t_int);
//					DFError(n, "\t t0 id[%d], t1 id[%d], t2 id[%d]\n", t0->t_id, t1->t_id, t2->t_id);
					return (token_t *)NULL;
				}
			}

			return DupToken(t2);
		}

		DFError(n, "Types must be the same");
		return (token_t *)NULL;
	}

	if ( IsEqual(t0, t1) )
		return DupToken(t2);

	return (token_t *)NULL;
}

/*
** Pass token if input values are not equal.
*/
token_t *
NodePassIfNotEqual(graph_t *g, node_t *n)
{
	token_t *	t0;
	token_t *	t1;
	token_t *	t2;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);

	if ( t0->t_type != t1->t_type )
	{
		DFError(n, "Types must be the same");
		return (token_t *)NULL;
	}

	if ( !IsEqual(t0, t1) )
		return DupToken(t2);

	return (token_t *)NULL;
}

/*
** Pass token if value1 < value2.
*/
token_t *
NodePassIfLess(graph_t *g, node_t *n)
{
	token_t *	t0;
	token_t *	t1;
	token_t *	t2;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);

	if ( t0->t_type != t1->t_type )
	{
		DFError(n, "Types must be the same");
		return (token_t *)NULL;
	}

	if ( IsLess(t0, t1) )
		return DupToken(t2);

	return (token_t *)NULL;
}

/*
** Pass token if value1 <= value2.
*/
token_t *
NodePassIfLessOrEqual(graph_t *g, node_t *n)
{
	token_t *	t0;
	token_t *	t1;
	token_t *	t2;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);

	if ( t0->t_type != t1->t_type )
	{
		DFError(n, "Types must be the same");
		return (token_t *)NULL;
	}

	if ( !IsGreater(t0, t1) )
		return DupToken(t2);

	return (token_t *)NULL;
}

/*
** Pass token if value1 > value2.
*/
token_t *
NodePassIfGreater(graph_t *g, node_t *n)
{
	token_t *	t0;
	token_t *	t1;
	token_t *	t2;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);

	if ( t0->t_type != t1->t_type )
	{
		DFError(n, "Types must be the same");
		return (token_t *)NULL;
	}

	if ( IsGreater(t0, t1) )
		return DupToken(t2);

	return (token_t *)NULL;
}

/*
** Pass token if value1 >= value2.
*/
token_t *
NodePassIfGreaterOrEqual(graph_t *g, node_t *n)
{
	token_t *	t0;
	token_t *	t1;
	token_t *	t2;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);

	if ( t0->t_type != t1->t_type )
	{
		DFError(n, "Types must be the same");
		return (token_t *)NULL;
	}

	if ( !IsLess(t0, t1) )
		return DupToken(t2);

	return (token_t *)NULL;
}

/*
** Pass token if any token present on other input.
*/
token_t *
NodePassIfToken(graph_t *g, node_t *n)
{
	token_t *	t1;
	token_t *	t2;

	t1 = GetTokenFromInput(n, 0);
	t2 = GetTokenFromInput(n, 1);

	return DupToken(t2);
}

/*
** Add two tokens.
*/
token_t *
NodeAdd(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_add);
}

/*
** Subtact two tokens.
*/
token_t *
NodeSubtract(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_sub);
}

/*
** Multiply two tokens.
*/
token_t *
NodeMultiply(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_mul);
}

/*
** Divide two tokens.
*/
token_t *
NodeDivide(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_div);
}

token_t *
NodeRemainder(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_rem);
}

token_t *
NodeEqual(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_eq);
}

token_t *
NodeNotEqual(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_neq);
}

token_t *
NodeLessOrEqual(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_leq);
}

token_t *
NodeGreaterOrEqual(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_geq);
}

token_t *
NodeLess(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_lt);
}

token_t *
NodeGreater(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_gt);
}

token_t *
NodeAnd(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_and);
}

token_t *
NodeOr(graph_t *g, node_t *n)
{
	return binop_apply(g, n, op_or);
}


/*
** Negate the token.
*/
token_t *
NodeNegate(graph_t *g, node_t *n)
{
	return unop_apply(g, n, op_neg);
}

/*
** Apply operator when we know the type of the operands
*/
token_t *
binop_aif(graph_t *g, node_t *n, arith_op op, char *n1, AIF *d1, char *n2, AIF *d2)
{
	int		res;
	int		v=0;
	AIF *		d = NULL;
	token_t *	t;

	switch ( op )
	{
	case op_add:
		d = AIFAdd(d1, d2);
		break;

	case op_sub:
		DFMsg("Comparing %s and %s.", n1, n2);
		d = AIFSub(d1, d2);
		break;

	case op_mul:
		d = AIFMul(d1, d2);
		break;

	case op_div:
		d = AIFDiv(d1, d2);
		break;

	case op_rem:
		d = AIFRem(d1, d2);
		break;

	case op_eq:
	case op_neq:
	case op_geq:
	case op_leq:
	case op_gt:
	case op_lt:
		// FIXME: This doesn't work for all AIF types. Some AIF types like arrays
		// will not return negative values from the compare function. This needs
		// to be rewritten.
		if ( AIFCompare(d1, d2, &res, NULL, NULL, NULL) < 0 )
		{
			DFError(n, "%s", AIFErrorStr());
			return (token_t *)NULL;
		}

		switch ( op )
		{
		case op_eq:
			v = (res == 0);
			break;

		case op_neq:
			v = (res != 0);
			break;

		case op_geq:
			v = (res > 0 || res == 0);
			break;

		case op_leq:
			v = (res < 0 || res == 0);
			break;

		case op_gt:
			v = (res > 0);
			break;

		case op_lt:
			v = (res < 0);
			break;

		default:
			break;
		}

		d = IntToAIF(v);
		break;

	case op_and:
		d = AIFAnd(d1, d2);
		break;

	case op_or:
		d = AIFOr(d1, d2);
		break;

	case op_neg:
		break;
	}

	if ( d == (AIF *)NULL )
	{
		DFError(n, "%s", AIFErrorStr());
		return (token_t *)NULL;
	}

	t = NewToken(g, TK_DATA);
	t->t_data = d;

	if ( n1 != NULL && n2 != NULL )
	{
		t->t_name = (char *)Alloc(strlen(n1) + strlen(n2) + 2);

		strcpy(t->t_name, n1);

		switch ( op )
		{
		case op_add:
			strcat(t->t_name, "+");
			break;

		case op_sub:
			strcat(t->t_name, "-");
			break;

		case op_mul:
			strcat(t->t_name, "*");
			break;

		case op_div:
			strcat(t->t_name, "/");
			break;

		case op_rem:
			strcat(t->t_name, "%");
			break;

		case op_eq:
			strcat(t->t_name, "==");
			break;

		case op_neq:
			strcat(t->t_name, "!=");
			break;

		case op_geq:
			strcat(t->t_name, ">=");
			break;

		case op_leq:
			strcat(t->t_name, "<=");
			break;

		case op_gt:
			strcat(t->t_name, ">");
			break;

		case op_lt:
			strcat(t->t_name, "<");
			break;

		case op_and:
			strcat(t->t_name, "&");
			break;

		case op_or:
			strcat(t->t_name, "|");
			break;

		case op_neg:
			break;
		}

		strcat(t->t_name, n2);
	}

	return t;
}

/*
** Apply binary operator op to two tokens.
*/
token_t *
binop_apply(graph_t *g, node_t *n, arith_op op)
{
	int		v;
	double		f;
	AIF *		d;
	token_t *	t;
	token_t *	t1;
	token_t *	t2;

	t1 = GetTokenFromInput(n, 0);
	t2 = GetTokenFromInput(n, 1);

	switch ( t1->t_type )
	{
	case TK_DATA:
		switch ( t2->t_type )
		{
		case TK_DATA:
			d = t2->t_data;
			break;

		case TK_INT:
			if ( (d = IntToAIF(t2->t_int)) == (AIF *)NULL )
			{
				DFError(n, "%s", AIFErrorStr());
				return (token_t *)NULL;
			}
			break;

		case TK_FLOAT:
			if ( (d = DoubleToAIF(t2->t_float)) == (AIF *)NULL )
			{
				DFError(n, "%s", AIFErrorStr());
				return (token_t *)NULL;
			}
			break;

		default:
			DFError(n, "Incompatible types for binary operation");
			return (token_t *)NULL;
		}
		
		return binop_aif(g, n, op, t1->t_name, t1->t_data, t2->t_name, d);

	case TK_INT:
		switch ( t2->t_type )
		{
		case TK_DATA:
			if ( (d = IntToAIF(t1->t_int)) == (AIF *)NULL )
			{
				DFError(n, "%s", AIFErrorStr());
				return (token_t *)NULL;
			}

			return binop_aif(g, n, op, t1->t_name, d, t2->t_name, t2->t_data);

		case TK_INT:
			v = t2->t_int;
			break;

		case TK_FLOAT:
			v = (int)t2->t_float;
			break;

		default:
			DFError(n, "Incompatible types for binary operation");
			return (token_t *)NULL;
		}

		t = NewToken(g, TK_INT);

		switch ( op )
		{
		case op_add:
			t->t_int = t1->t_int + v;
			break;

		case op_sub:
			t->t_int = t1->t_int - v;
			break;

		case op_mul:
			t->t_int = t1->t_int * v;
			break;

		case op_div:
			t->t_int = t1->t_int / v;
			break;

		case op_rem:
			t->t_int = t1->t_int % v;
			break;

		case op_eq:
			t->t_int = t1->t_int == v;
			break;

		case op_neq:
			t->t_int = t1->t_int != v;
			break;

		case op_geq:
			t->t_int = t1->t_int >= v;
			break;

		case op_leq:
			t->t_int = t1->t_int <= v;
			break;

		case op_gt:
			t->t_int = t1->t_int > v;
			break;

		case op_lt:
			t->t_int = t1->t_int < v;
			break;

		case op_and:
			t->t_int = t1->t_int & v;
			break;

		case op_or:
			t->t_int = t1->t_int | v;
			break;

		case op_neg:
			break;
		}
		break;

	case TK_FLOAT:
		switch ( t2->t_type )
		{
		case TK_DATA:
			if ( (d = DoubleToAIF(t1->t_float)) == (AIF *)NULL )
			{
				DFError(n, "%s", AIFErrorStr());
				return (token_t *)NULL;
			}

			return binop_aif(g, n, op, t1->t_name, d, t2->t_name, t2->t_data);

		case TK_INT:
			f = (double)t2->t_int;
			break;

		case TK_FLOAT:
			f = t2->t_float;
			break;

		default:
			DFError(n, "Incompatible types for binary operation");
			return (token_t *)NULL;
		}

		t = NewToken(g, TK_FLOAT);

		switch ( op )
		{
		case op_add:
			t->t_float = t1->t_float + f;
			break;

		case op_sub:
			t->t_float = t1->t_float - f;
			break;

		case op_mul:
			t->t_float = t1->t_float * f;
			break;

		case op_div:
			t->t_float = t1->t_float / f;
			break;

		case op_rem:
			t->t_float = fmod(t1->t_float, f);
			break;

		case op_eq:
			t->t_float = t1->t_float == f;
			break;

		case op_neq:
			t->t_float = t1->t_float != f;
			break;

		case op_geq:
			t->t_float = t1->t_float >= f;
			break;

		case op_leq:
			t->t_float = t1->t_float <= f;
			break;

		case op_gt:
			t->t_float = t1->t_float > f;
			break;

		case op_lt:
			t->t_float = t1->t_float < f;
			break;

		case op_and:
		case op_or:
			DFError(n, "Type not supported");
			return (token_t *)NULL;

		case op_neg:
			break;
		}
		break;

	default:
		DFError(n, "Type not supported");
		return (token_t *)NULL;
	}

	return t;
}

/*
** Apply unary operator op to a token.
*/
token_t *
unop_apply(graph_t *g, node_t *n, arith_op op)
{
	AIF *		d = NULL;
	token_t *	t;
	token_t *	t1;

	t1 = GetTokenFromInput(n, 0);

	switch ( t1->t_type )
	{
	case TK_DATA:
		switch ( op )
		{
		case op_neg:
			d = AIFNeg(t1->t_data);
			break;

		default:
			break;
		}

		if ( d == (AIF *)NULL )
		{
			DFError(n, "%s", AIFErrorStr());
			return (token_t *)NULL;
		}

		t = NewToken(g, TK_DATA);
		t->t_data = d;

		if ( t1->t_name != NULL )
		{
			t->t_name = (char *)Alloc(strlen(t1->t_name)+ 2);

			switch ( op )
			{
			case op_neg:
				strcat(t->t_name, "-");
				break;

			default:
				break;
			}

			strcpy(t->t_name, t1->t_name);
		}
		break;

	case TK_INT:
		t = NewToken(g, TK_INT);

		switch ( op )
		{
		case op_neg:
			t->t_int = -t1->t_int;
			break;

		default:
			break;
		}
		break;

	case TK_FLOAT:
		t = NewToken(g, TK_FLOAT);

		switch ( op )
		{
		case op_neg:
			t->t_float = -t1->t_float;
			break;

		default:
			break;
		}
		break;

	default:
		DFError(n, "Type not supported");
		return (token_t *)NULL;
	}

	return t;
}

/*
** Retrieve token from node store. Used to implement
** graph constants.
*/
token_t *
NodeConst(graph_t *g, node_t *n)
{
	if ( n->n_store == NULL )
	{
		DFError(n, "No token to return");
		return (token_t *)NULL;
	}

	return n->n_store = DupToken(n->n_store);
}

/*
** Slice an array.
*/
token_t *
NodeSlice(graph_t *g, node_t *n)
{
	int		i;
	int		nd;
	int *		min;
	int *		max;
	token_t	*	t;
	token_t	*	t0;
	AIF *		a;

	t0 = GetTokenFromInput(n, 0);

	a = t0->t_data;
	
	if ( AIFType(a) != AIF_ARRAY )
	{
		DFError(n, "Expected array on input 0, got \"%s\"", AIF_FORMAT(a));
		return (token_t *)NULL;
	}

	if ( 2 * (nd = AIFArrayRank(a)) + 1 != n->n_ninp )
	{
		DFError(n, "Tried to slice array dim %d with %d inputs", nd, n->n_ninp);
		return (token_t *)NULL;
	}

	min = (int *)Alloc(nd * sizeof(int));
	max = (int *)Alloc(nd * sizeof(int));

	for ( i = 0 ; i < nd ; i++ )
	{
		t = GetTokenFromInput(n, 2 * i + 1);
		min[i] = t->t_int;

		t = GetTokenFromInput(n, 2 * i + 2);
		max[i] = t->t_int;

		if ( min[i] > max[i] )
		{
			Free(min);
			Free(max);

			DFError(n, "Invalid slice: %d, min > max (%d > %d)", i, min[i], max[i]);
			return (token_t *)NULL;
		}
	}

	if ( (a = AIFArraySlice(a, nd, min, max)) == NULL )
	{
		Free(min);
		Free(max);

		DFError(n, "AIFArraySlice: %s", AIFErrorStr());
		return (token_t *)NULL;
	}

	t = NewToken(g, TK_DATA);
	t->t_data = a;
	t->t_name = strdup(t0->t_name);

	Free(min);
	Free(max);

	return t;
}

/*
** Permute an array.
*/
token_t *
NodePermute(graph_t *g, node_t *n)
{
	int		i;
	int		nd;
	int *		new;
	token_t	*	t;
	token_t	*	t0;
	AIF *		a;
	AIF *		na;

	t0 = GetTokenFromInput(n, 0);

	a = t0->t_data;

	if ( AIFType(a) != AIF_ARRAY )
	{
		DFError(n, "Expected array on input 0");
		return (token_t *)NULL;
	}

	if ( (nd = AIFArrayRank(a)) >= n->n_ninp )
	{
		DFError(n, "Tried to permute array dim %d with %d inputs", nd, n->n_ninp);
		return (token_t *)NULL;
	}

	new = (int *)Alloc(nd * sizeof(int));

	for ( i = 0 ; i < nd ; i++ )
	{
		t = GetTokenFromInput(n, i + 1);
		new[i] = t->t_int;

		if ( new[i] >= nd )
		{
			Free(new);
			DFError(n, "Invalid permutation, %d -> %d", i, new[i]);
			return (token_t *)NULL;
		}
	}

	if ( (na = AIFArrayPerm(a, new)) == (AIF *)NULL )
	{
		Free(new);
		DFError(n, "AIFArraySlice: %s", AIFErrorStr());
		return (token_t *)NULL;
	}

	t = NewToken(g, TK_DATA);
	t->t_data = na;
	t->t_name = strdup(t0->t_name);

	Free(new);

	return t;
}

#if 0
/*
** Compute EPS.
*/
token_t *
NodeEPS(graph_t *g, node_t *n)
{
	token_t	*	t;
	token_t	*	t0;
	token_t	*	t1;
	token_t	*	t2;
	token_t	*	t3;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);
	t3 = GetTokenFromInput(n, 3);

	t = NewToken(g, TK_INT);

	if ( AIFEPS(t0->t_float, t1->t_float, (int)(t2->t_int != 0), t3->t_data, &t->t_int) < 0 )
	{
		DFError(n, "AIFEPS: %s", AIFErrorStr());
		return (token_t *)NULL;
	}

	return t;
}
#endif

/*
** Return type of variable.
*/
token_t *
NodeTypeSet(graph_t *g, node_t *n)
{
	token_t	*	t;
	token_t	*	t0;
	AIF *		a;

	t0 = GetTokenFromInput(n, 0);

	// DONNY TODO FIXME
	// We use (0) as the "base".
	a = t0->t_data_set[0];

	t = NewToken(g, TK_STR);
	t->t_str = strdup(AIF_FORMAT(a));

	return t;
}

/*
** Return type of variable.
*/
token_t *
NodeType(graph_t *g, node_t *n)
{
	token_t	*	t;
	token_t	*	t0;
	AIF *		a;

	t0 = GetTokenFromInput(n, 0);

	a = t0->t_data;

	t = NewToken(g, TK_STR);
	t->t_str = strdup(AIF_FORMAT(a));

	return t;
}

/*
** Pass token if tokens on all inputs. Arbitrarily passes
** token on input 0.
*/
token_t *
NodeSync(graph_t *g, node_t *n)
{
	token_t	*	t;

	t = GetTokenFromInput(n, 0);

	t = DupToken(t);

	return t;
}


/*
** Pass token if token on any inputs. Arbitrarily passes
** first token. This is used to implement a graph merge
** operation.
*/
token_t *
NodeMerge(graph_t *g, node_t *n)
{
	token_t	*	t;

	if ( (t = GetTokenFromInput(n, -1)) == (token_t *)NULL )
	{
		DFError(n, "no tokens found on input!");
		return (token_t *)NULL;
	}

	t = DupToken(t);

	return t;
}

/*
** Store a token in the global token store.
*/
token_t *
NodeStore(graph_t *g, node_t *n)
{
	token_t	*	t0;
	token_t	*	t1;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);

	DPRINT("[GRAPH DEBUG] NodeStore[%d], t0->t_int[%d], t1 id[%d]\n", n->n_id, t0->t_int, t1->t_id);

	TokenStore[t0->t_int] = DupToken(t1);

	return DupToken(t1);
}

/*
** Recall a token from the global token store.
*/
token_t *
NodeRecall(graph_t *g, node_t *n)
{
	token_t	*	t;

	t = GetTokenFromInput(n, 0);

	DPRINT("[GRAPH DEBUG] NodeRecall[%d], t->t_int[%d]i, t_id[%d]\n", n->n_id, t->t_int, TokenStore[t->t_int]->t_id);
	return TokenStore[t->t_int];
}

/*
** Store a token in the global hash store. Replaces existing token,
** if any.
*/
token_t *
NodeInsert(graph_t *g, node_t *n)
{
	token_t	*		t;
	token_t	*		t0;
	token_t	*		t1;
	hentry_t *		h;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);

	if 
	(
		HashStore == (htable_t *)NULL 
		&&
		(HashStore = HashCreate(HSTORE_SIZE)) == (htable_t *)NULL
	)
	{
		DFError(n, "Could not create hash store");
		return (token_t *)NULL;
	}

	if ( (h = HashSearch(HashStore, t0->t_str)) != (hentry_t *)NULL )
		DestroyToken(g, (token_t *)h->h_data);

	t = (void *)DupToken(t1);

	if ( HashInsert(HashStore, t0->t_str, (void *)t) == (hentry_t *)NULL )
	{
		DFError(n, "Could not insert \"%s\" into hash store.", t0->t_str);
		return (token_t *)NULL;
	}

	return DupToken(t1);
}

/*
** Fetch a token from the global hash store.
*/
token_t *
NodeSearch(graph_t *g, node_t *n)
{
	token_t	*	t0;
	hentry_t *	h;

	t0 = GetTokenFromInput(n, 0);

	if ( (h = HashSearch(HashStore, t0->t_str)) == (hentry_t *)NULL )
		return (token_t *)NULL;

	return (token_t *)h->h_data;
}

/*
** Fetch an invoke from the global hash store.
*/
token_t *
NodeLookupInvoke(graph_t *g, node_t *n)
{
	token_t	*	t;
	token_t	*	t0;
	hentry_t *	h;

	t0 = GetTokenFromInput(n, 0);

	if ( (h = HashSearch(InvokeStore, t0->t_str)) == (hentry_t *)NULL )
	{
		DFError(n, "No such process \"%s\"", t0->t_str);
		return (token_t *)NULL;
	}

	t = NewToken(g, TK_INV);
	t->t_inv = (invoke_t *)h->h_data;

	return t;
}

/*
** Check if a token is in the global hash store.
*/
token_t *
NodeHashEntry(graph_t *g, node_t *n)
{
	token_t	*	t0;
	token_t	*	t;

	t0 = GetTokenFromInput(n, 0);

	t = NewToken(g, TK_INT);
	t->t_int = (HashSearch(HashStore, t0->t_str) != (hentry_t *)NULL);

	return t;
}

/*
** Exit interpreter.
*/
token_t *
NodeExit(graph_t *g, node_t *n)
{
	/*DbgFinish();*/
	ExitDF();
	return (token_t *)NULL;
}

/*
** Log a message.
*/
token_t *
NodeLog(graph_t *g, node_t *n)
{
	struct tm *	tm;
#ifdef HAVE_CLOCK_REALTIME
	struct timespec tp;
#else /* HAVE_CLOCK_REALTIME */
	time_t		tp;
#endif /* HAVE_CLOCK_REALTIME */
	FILE *		fp;
	token_t *	t0;
	token_t *	t1;
	token_t *	t2;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);

	if
	(
#ifdef HAVE_CLOCK_REALTIME
		clock_gettime(CLOCK_REALTIME, &tp) < 0
#else /* HAVE_CLOCK_REALTIME */
		time(&tp) < 0
#endif /* HAVE_CLOCK_REALTIME */
		||
#ifdef HAVE_CLOCK_REALTIME
		(tm = localtime(&(tp.tv_sec))) == (struct tm *)NULL
#else /* HAVE_CLOCK_REALTIME */
		(tm = localtime(&tp)) == (struct tm *)NULL
#endif /* HAVE_CLOCK_REALTIME */
		||
		(fp = fopen(t2->t_str, "a")) == (FILE *)NULL
	)
		return (token_t *)NULL;

	PrintToFd(fp, "%d:%d:%d %s -> %s\n", 
		tm->tm_hour,
		tm->tm_min,
		tm->tm_sec,
		t0->t_name,
		t1->t_str);

	fclose(fp);

	return (token_t *)NULL;
}

token_t *
NodeBlockMapSet(graph_t *g, node_t *n)
{
	int		i;
	int		np;
	token_t	*	t0;
	token_t	*	t1;
	token_t	*	t2;
	token_t	*	t;
	g_block_map_t *	m;
	AIF *		a;
	AIF **		d;
	AIF **		da;
	char *		hashinfo= (char *)NULL;
	char *		tok = (char *)NULL;
	char *	  t0_str_tok = (char *)NULL;
	char *	  t0_str = (char *)NULL;

	t0 = GetTokenFromInput(n, 0);
	t0_str_tok = strdup(t0->t_str);
	t0_str = strdup(t0->t_str);     // needs to be at least t0->t_str in size

	// check if slcing info is attached
	if ((tok = strtok(t0_str_tok, "|")) != (char *)NULL)
	{
		// ignore it
		strcpy(t0_str, tok);
	}

	// check if var info is attached
	if ((tok = strtok(t0_str_tok, "(")) != (char *)NULL)
	{
		// ignore it
		strcpy(t0_str, tok);
	}
	
	// check now if hashinfo is attached
	// note here we need to ensure that a hash character is
	// present using strchr before calling strtok since we
	// are making assumptions about the string format.
	if (strchr(t0_str_tok, '#') != NULL)
	{
		tok = strtok(t0_str_tok, "#");
		hashinfo = strdup(tok);
		
		tok = strtok(NULL, "#");
		tok = strtok(NULL, "#");
		
		if (tok != (char *)NULL)
		{
			// ignore it
			strcpy(t0_str, tok);
		}else
		{
			// improper hash info, so ignore the hash info
			strcpy(t0_str, hashinfo);
			Free(hashinfo);
			hashinfo = (char *)NULL;
		}
	}

	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);

	if ( (m = FindBlockMap(t0_str)) == (g_block_map_t *)NULL )
	{
		DFError(n, "No such blockmap \"%s\"", t0_str);
		return (token_t *)NULL;
	}

	np = t2->t_int;

	d = da = (AIF **)Alloc(sizeof(AIF *) * np);

	t = GetTokenFromInput(n, 3);
	for ( i = 0 ; i < np ; i++ )
	{
		//t = GetTokenFromInput(n, i+3);
		//*d++ = t->t_data;
		*d++ = t->t_data_set[i];
	}
	if ( (a = ApplyBlockMap(m, t1->t_str, np, da, hashinfo)) == (AIF *)NULL )
	{
		DFError(n, GetError());
		ExitDF();
		return (token_t *)NULL;
	}
	Free(da);

	t = NewToken(g, TK_DATA);
	t->t_name = strdup(t0->t_str);
	t->t_data = a;

	if (hashinfo)		Free(hashinfo);
	if (t0_str)		Free(t0_str);
	if (t0_str_tok)	 Free(t0_str_tok);

	return t;
}

/*
AIF *
_deal_with_histograms(AIF ** data, int size)
{
	int i;
	double diff, p_val;
	AIF *tmp1, *tmp2;
	AIF ** array1;
	AIF ** array2;
	AIF * res;

	FILE * fd = fopen("histogram.jpg", "w");

	array1 = (AIF **)malloc(sizeof(AIF *)*size);
	array2 = (AIF **)malloc(sizeof(AIF *)*size);

	for (i=0; i<size; i++)
	{
		tmp1 = AIFGetStruct(data[i], "h1");
		array1[i] = tmp1;
		tmp2 = AIFGetStruct(data[i], "h2");
		array2[i] = tmp2;
	}
	tmp1 = MergeAIFHistogramArray(array1, size);
	tmp2 = MergeAIFHistogramArray(array2, size);

	DisplayAIFHistogram(fd, tmp1);
	res = DoubleToAIF(0.2);
	fclose(fd);

	// now compare histogram
	CompareAIFHistograms(tmp1, tmp2, &diff, &p_val);

	// store result
	res = DoubleToAIF(p_val);

	// free the temp histograms
	for (i=0; i<size; i++)
	{
		AIFFree(array1[i]);
		AIFFree(array2[i]);
	}
	free(array1);
	free(array2);

	AIFFree(tmp1);
	AIFFree(tmp2);

	return res;
}
*/

token_t *
NodeReduceSet(graph_t *g, node_t *n)
{
	int		np;
	int		reducefunc;
	token_t	*	t0;
	token_t	*	t1;
	token_t	*	t;
	AIF *		res;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	np = t1->t_int;

	GetGraphVar(GRAPH_REDUCE, (int *)&reducefunc);

	t = GetTokenFromInput(n, 2);

	switch (reducefunc) {
	case EXPR_SUM:
		res = AIFSumArray(t->t_data_set, np);
		break;
	case EXPR_PRODUCT:
		res = AIFProductArray(t->t_data_set, np);
		break;
	case EXPR_AVERAGE:
		res = AIFAverageArray(t->t_data_set, np);
		break;
	case EXPR_MIN:
		res = AIFMinArray(t->t_data_set, np);
		break;
	case EXPR_MAX:
		res = AIFMaxArray(t->t_data_set, np);
		break;
	case EXPR_STDEV:
		res = AIFStdevArray(t->t_data_set, np);
		break;
	case EXPR_COUNTELEM:
		res = AIFCountElemArray(t->t_data_set, np);
		break;
	/*
	case EXPR_HISTOGRAM:
		//res = MergeAIFHistogramArray(t->t_data_set, np);
				res = _deal_with_histograms(t->t_data_set, np);
				AIFPrint(Client.cli_out_stream, 0, res);
				printf("\n");
		break;
	*/
	default:
		DFError(n, "Undefined reduce function \"%d\"", reducefunc);
		return (token_t *)NULL;
	}
	t = NewToken(g, TK_DATA);
	t->t_name = strdup(t0->t_str);
	t->t_data = res;

	return t;
}

/* Minh add this function here to implement blockmap 15/05/2009
** Generate data from a block map function.
**
** data NodeBlockMap(char *blockmap, int np, data d0, ...)
**
** - blockmap is the block map function
** - np is the number of processes
** - d0, d1, ... are the variables from process 0, 1, ...
**
** Applies the blockmap function to each variable and returns
** data of the type specified in type.
*/
token_t *
NodeBlockMap(graph_t *g, node_t *n)
{
	int		i;
	int		np;
	token_t	*	t0;
	token_t	*	t1;
	token_t	*	t2;
	token_t	*	t;
	g_block_map_t *	m;
	AIF *		a;
	AIF **		d;
	AIF **		da;
	char *		hashinfo = (char *)NULL;
	char *		tok = (char *)NULL;
	char *		t0_str_tok = (char *)NULL;
	char *	  t0_str = (char *)NULL;

	t0 = GetTokenFromInput(n, 0);
	t0_str_tok = strdup(t0->t_str);
	t0_str = strdup(t0->t_str);     // at least t0->t_str in size
	
	// check if slcing info is attached
	if ((tok = strtok(t0_str_tok, "|")) != (char *)NULL)
	{
		// ignore it
		strcpy(t0_str, tok);
	}
	
	// check now if hashinfo is attached
	// note here we need to ensure that a hash character is
	// present using strchr before calling strtok since we
	// are making assumptions about the string format.
	if (strchr(t0_str_tok, '#') != NULL)
	{
		tok = strtok(t0_str_tok, "#");
		hashinfo = strdup(tok);
		
		tok = strtok(NULL, "#");
		tok = strtok(NULL, "#");
		
		if (tok != (char *)NULL)
		{
			// ignore it
			strcpy(t0_str, tok);
		}else
		{
			// improper hash info, so ignore the hash info
			strcpy(t0_str, hashinfo);
			Free(hashinfo);
			hashinfo = (char *)NULL;
		}
	}
	
	
	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);

	if ( (m = FindBlockMap(t0_str)) == (g_block_map_t *)NULL )
	{
		DFError(n, "No such blockmap \"%s\"", t0_str);
		return (token_t *)NULL;
	}

	np = t2->t_int;

	d = da = (AIF **)Alloc(sizeof(AIF *) * np);

	for ( i = 0 ; i < np ; i++ )
	{
		t = GetTokenFromInput(n, i+3);
		*d++ = t->t_data;
	}
	if ( (a = ApplyBlockMap(m, t1->t_str, np, da, hashinfo)) == (AIF *)NULL )
	{
		DFError(n, GetError());
		ExitDF();
		return (token_t *)NULL;
	}
	Free(da);

	t = NewToken(g, TK_DATA);
	t->t_name = strdup(t0->t_str);
	t->t_data = a;

	if (hashinfo)		Free(hashinfo);
	if (t0_str)	     Free(t0_str);
	if (t0_str_tok)	 Free(t0_str_tok);

	return t;
}

token_t *
NodeReduce(graph_t *g, node_t *n)
{
	int		i;
	int		np;
	token_t	*	t0;
	token_t	*	t1;
	token_t	*	t;
	AIF **		aif_array;
	AIF *		res;
	int		reducefunc;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	np = t1->t_int;

	GetGraphVar(GRAPH_REDUCE, (int *)&reducefunc);

	aif_array = (AIF**)malloc(sizeof(AIF*)*np);

	for ( i = 0 ; i < np ; i++ )
	{
		t = GetTokenFromInput(n, i+2);
		aif_array[i] = t->t_data;
	}

	switch (reducefunc) {
	case EXPR_SUM:
		res = AIFSumArray(aif_array, np);
		break;
	case EXPR_PRODUCT:
		res = AIFProductArray(aif_array, np);
		break;
	case EXPR_AVERAGE:
		res = AIFAverageArray(aif_array, np);
		break;
	case EXPR_MIN:
		res = AIFMinArray(aif_array, np);
		break;
	case EXPR_MAX:
		res = AIFMaxArray(aif_array, np);
		break;
	case EXPR_STDEV:
		res = AIFStdevArray(aif_array, np);
		break;
	case EXPR_COUNTELEM:
		res = AIFCountElemArray(aif_array, np);
		break;
	/*
	case EXPR_HISTOGRAM:
		res = MergeAIFHistogramArray(aif_array, np);
		break;
	*/
	default:
		DFError(n, "Undefined reduce function \"%d\"", reducefunc);
		return (token_t *)NULL;
	}

	free(aif_array);

	t = NewToken(g, TK_DATA);
	t->t_name = strdup(t0->t_str);
	t->t_data = res;

	return t;
}

/*
** Assign data to debugger variable described by str.
*/
token_t *
NodeAssign(graph_t *g, node_t *n)
{
	val_t *		v1;
	val_t *		v2;
	expr_t *	e;
	token_t	*	t0;
	token_t	*	t1;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);

	if
	(
		(e = ParseExpr(t0->t_str)) == (expr_t *)NULL
		||
		EvalExpr(&v1, e, (rec_set_t *)NULL) < 0
	)
	{
		DFError(n, GetError());
		return (token_t *)NULL;
	}

	v2 = NewVal(lval_none, rval_aif, CopyAIF(t1->t_data));

	if ( AssignVal(v1, v2, (rec_set_t *)NULL) < 0 )
	{
		DFError(n, GetError());
		DestroyVal(v2);
		return (token_t *)NULL;
	}

	DestroyVal(v2);

	return DupToken(t1);
}

/*
** Return a debugger variable described by str.
** FIXME: Is this even called? The ConvertRepSetToAIF is unable to be removed
** because of this function's use of VAL_RETRIEVE...
*/
token_t *
NodeGetVar(graph_t *g, node_t *n)
{
	void *		sym;
	val_t *		v;
	token_t	*	t0;
	token_t	*	t1;
	token_t	*	t;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);

	if
	(
		(sym = FindSym(t1->t_str)) == (void *)NULL
		||
		(v = GetVal(sym)) == (val_t *)NULL
	)
	{
		DFError(n, "No such variable: \"%s\"", t1->t_str);
		return (token_t *)NULL;
	}

	t = NewToken(g, TK_DATA);
	t->t_name = strdup(t1->t_str);
	
	// Refresh the val data
	// FIXME: How to deal with the rec_set_t properly in this macro call?
	if (VAL_RETRIEVE(v, (rec_set_t*)NULL))
	{
		DFError(n, GetError());
		DestroyVal(v);
		DestroyToken(g, t);
		return (token_t *)NULL;
	}
	
	// Set the data based on the val type
	// FIXME: This should not be calling ConvertRepSetToAIF, but I don't know 
	// how its going to be used.
	switch (LVAL_TYPE(v))
	{
		case lval_gdb:
			// Convert the reply to AIF
			t->t_data = ConvertRepSetToAIF(RVAL_REPLY(v));
			break;
			
		case lval_blockmap:
			// assign the AIF data directly
			t->t_data = CopyAIF(RVAL_AIF(v));
			break;
			
		default:
			t->t_data = NULL;
			DFError(n, "NodeGetVar called with lval other than gdb or blockmap!");
			break;
	}

	DestroyVal(v);

	return t;
}

/*********************************
** DEBUGGER INTERFACE ROUTINES **
*********************************/

/*
** Send request to read variable from program.
*/
token_t *
NodeDbgRequestVarSet(graph_t *g, node_t *n)
{
	token_t *	t;
	token_t *	t1;
	token_t *	t2;
	token_t *	t3;
	int		set_length;
	dbgprocset_t *  procset;
	dbg_over_set_t*  over_set;
	bitset*		receiver = NULL;
	bitset*		result = NULL;
	int	     index;
	char *	  procset_name;
	
#ifdef PERFLOG
	char *envDir;
	char logfile[PATH_MAX];

	if ((envDir = getenv("GUARD_LOG_DIR")) == (char *)NULL)
	{
		// user didn't set the environment variable, so lets
		// default to tmp
		envDir = "/tmp";
	}

	// create the fullpath string to the log file using PATH_MAX plus a null term
	snprintf(logfile, PATH_MAX+1, "%s/ccdb_perf_log", envDir);
#endif

	DPERFLOG(CreateTimerLog(logfile));
    DPERFLOG(SetTimerStart("total time"));
    DPERFLOG(SetTimerStart("server time"));

	t1 = GetTokenFromInput(n, 0);
	t2 = GetTokenFromInput(n, 1);
	
	//iAssertID
	t3 = GetTokenFromInput(n, 2);

	set_length = t1->t_set_length;

	procset_name = t1->t_set_name;
	procset = FindProcSet(procset_name);
	if(procset == NULL)
	{
		DFError(n, "NodeDbgRequestVarSet cannot find procset for %s\n", procset_name);
		return NULL;
	}

	over_set = GetOverlaySetForProcSet(procset);
	if(over_set == (dbg_over_set_t*)NULL)
	{
		DFError(n, "NodeDbgRequestVarSet cannot get Overlay Set for [%s]!\n", procset_name); 
		return (token_t *)NULL;
	}

	receiver = bitset_dup(over_set->s_set_bs);
	if(!bitset_isfull(t1->t_set_bits))
	{
		for(index = 0; index < t1->t_set_length; index++)
		{
			if(!bitset_test(t1->t_set_bits, index))
				bitset_unset(receiver, over_set->s_offset + index);
		}	
	}
		
//	rec_set_t* rec_set = NewRecSetByOverSet(over_set, t1->t_set_bits, -1);
	rec_set_t* rec_set = NewRecSetByOverSet(over_set, receiver);

	result = DbgSendExpr(rec_set, t2->t_str, t3->t_int);
	bitset_free(receiver);

	t = NewToken(g, TK_BIT_SET);
	t->t_set_bits = bitset_dup(result);


	return t;
}

/*
** Get variable values from program.
*/
token_t *
NodeDbgRecvVarSet(graph_t *g, node_t *n)
{
	token_t *		t;
	token_t *		t0;
	token_t *		t1;
	token_t *		t2;
	token_t *		t3;
	AIF **			ax;
	int				set_length;
	rep_set_t *		rep_set;
	group_reply_t *	reply;
	dbgprocset_t *  procset;
	dbg_over_set_t*	over_set;
	bitset*			receiver = NULL;
	int				i;
	int	     		index;
	char *			procset_name;

	t0 = GetTokenFromInput(n, 0);	// procset
	t1 = GetTokenFromInput(n, 1);	// iAssertID
	t2 = GetTokenFromInput(n, 2);	// epxr string
	t3 = GetTokenFromInput(n, 3);	// bitset
	
	set_length = t0->t_set_length;

	ax = (AIF **)Alloc(set_length * sizeof(AIF *));

	procset_name = t0->t_set_name;
	procset = FindProcSet(procset_name);
	if(procset == NULL)
	{
		DFError(n, "NodeDbgRecvVarSet cannot find procset for %s\n", procset_name);
		return NULL;
	}

	over_set = GetOverlaySetForProcSet(procset);
	if(over_set == (dbg_over_set_t*)NULL)
	{
		DFError(n, "NodeDbgRecvVarSet cannot get Overlay Set for [%s]!\n", procset_name); 
		return (token_t *)NULL;
	}

	receiver = bitset_dup(over_set->s_set_bs);
	if(!bitset_isfull(t0->t_set_bits))
	{
		for(index = 0; index < t0->t_set_length; index++)
		{
			if(!bitset_test(t0->t_set_bits, index))
				bitset_unset(receiver, over_set->s_offset + index);
		}	
	}
	
	rec_set_t* rec_set = NewRecSetByOverSet(over_set, receiver);

	rep_set = DbgGetExpr(rec_set, t3->t_set_bits, t1->t_int);
	
	ProcessErrorInRepSet(rec_set, rep_set);
	
	// Process the data replies
	// FIXME: Modify graph to understand rep_set_t * instead of making this
	// strange array.
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// loop over the ax array
		// NOTE: I don't understand this - andrewg
		for (i=0; i<set_length; ++i)
		{
			if (bitset_test(reply->rep_group, (i + over_set->s_offset)))
			{
				// Save the AIF data into the ax
				ax[i] = EVENT_DATA(reply->rep_event);
			}
		}
	}
	
	// Note: The reply gets free'ed later on when the token gets cleaned up

	FreeRecSet(rec_set);
	bitset_free(receiver);

	t = NewToken(g, TK_DATA_SET);
	t->t_name = strdup(t2->t_str);
	t->t_data_set = ax;
	t->t_set_length = set_length;
	t->t_set_bits = bitset_dup(t0->t_set_bits);
	
	DPERFLOG(SetTimerStopAndLog("server time"));

	DPERFLOG(SetTimerStart("client time"));

	// Save the reply for cleanup later on
	// FIXME: Modify graph to understand rep_set_t so that this is not
	// necessary.
	t->t_d_shadow_reply = rep_set;

	return t;
}

/*
** Read variable from program.
*/
token_t *
NodeDbgGetVarSet(graph_t *g, node_t *n)
{
	token_t *	t;
	token_t *	t1;
	token_t *	t2;
	token_t *	t3;
	AIF **		ax;
	int		set_length;
	rep_set_t *     rep_set;
	group_reply_t *	reply;
	dbgprocset_t *  procset;
	dbg_over_set_t*  over_set;
	bitset*		receiver = NULL;
	int			i;
	int	     index;
	char *	  procset_name;
	
#ifdef PERFLOG
	char *envDir;
	char logfile[PATH_MAX];

	if ((envDir = getenv("GUARD_LOG_DIR")) == (char *)NULL)
	{
		// user didn't set the environment variable, so lets
		// default to tmp
		envDir = "/tmp";
	}

	// create the fullpath string to the log file using PATH_MAX plus a null term
	snprintf(logfile, PATH_MAX+1, "%s/ccdb_perf_log", envDir);
#endif

	DPERFLOG(CreateTimerLog(logfile));
    DPERFLOG(SetTimerStart("total time"));
    DPERFLOG(SetTimerStart("server time"));

	t1 = GetTokenFromInput(n, 0);
	t2 = GetTokenFromInput(n, 1);
	
	//iAssertID
	t3 = GetTokenFromInput(n, 2);

	set_length = t1->t_set_length;

	ax = (AIF **)Alloc(set_length * sizeof(AIF *));

	procset_name = t1->t_set_name;
	procset = FindProcSet(procset_name);
	if(procset == NULL)
	{
		DFError(n, "NodeDbgGetVarSet cannot find procset for %s\n", procset_name);
		return NULL;
	}

	over_set = GetOverlaySetForProcSet(procset);
	if(over_set == (dbg_over_set_t*)NULL)
	{
		DFError(n, "NodeDbgGetVarSet cannot get Overlay Set for [%s]!\n", procset_name); 
		return (token_t *)NULL;
	}

	receiver = bitset_dup(over_set->s_set_bs);
	if(!bitset_isfull(t1->t_set_bits))
	{
		for(index = 0; index < t1->t_set_length; index++)
		{
			if(!bitset_test(t1->t_set_bits, index))
				bitset_unset(receiver, over_set->s_offset + index);
		}	
	}
		
	rec_set_t* rec_set = NewRecSetByOverSet(over_set, receiver);

	rep_set = DbgEvalExpr(rec_set, t2->t_str, t3->t_int);
	
	ProcessErrorInRepSet(rec_set, rep_set);
	
	// Process the data replies
	// FIXME: Modify graph to understand rep_set_t * instead of making this
	// strange array.
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// loop over the ax array
		// NOTE: I don't understand this - andrewg
		for (i=0; i<set_length; ++i)
		{
			if (bitset_test(reply->rep_group, (i + over_set->s_offset)))
			{
				// Save the AIF data into the ax
				ax[i] = EVENT_DATA(reply->rep_event);
			}
		}
	}
	
	// Note: The reply gets free'ed later on when the token gets cleaned up

	FreeRecSet(rec_set);
	bitset_free(receiver);

	t = NewToken(g, TK_DATA_SET);
	t->t_name = strdup(t2->t_str);
	t->t_data_set = ax;
	t->t_set_length = set_length;
	t->t_set_bits = bitset_dup(t1->t_set_bits);
	// Save the reply for cleanup later on
	// FIXME: Modify graph to understand rep_set_t so that this is not
	// necessary.
	t->t_d_shadow_reply = rep_set;

	DPERFLOG(SetTimerStopAndLog("server time"));
	DPERFLOG(SetTimerStart("client time"));

	return t;
}

/*
** Read variable from program.
*/
token_t *
NodeDbgGetVar(graph_t *g, node_t *n)
{
	token_t *	t;
	token_t *	t1;
	token_t *	t2;
	token_t *	t3;
	AIF *		a = NULL;
	rep_set_t *     rep_set;
	group_reply_t *	reply;
	dbgprocset_t *  procset;
	dbg_over_set_t * over_set;
	bitset*		receiver = NULL;
	int		index = 0;
	char *			procset_name;
	int				num = 0;

	t1 = GetTokenFromInput(n, 0);
	t2 = GetTokenFromInput(n, 1);
	
	//iAssertID
	t3 = GetTokenFromInput(n, 2);

	procset_name = t1->t_set_name;
	
	procset = FindProcSet(procset_name);
	if(procset == NULL)
	{
		DFError(n, "NodeDbgGetVar cannot find procset for %s\n", procset_name);
		return NULL;
	}

	over_set = GetOverlaySetForProcSet(procset);
	if(over_set == (dbg_over_set_t*)NULL)
	{
		DFError(n, "NodeDbgGetVar cannot get Overlay Set for [%s]!\n", procset_name);
		return (token_t *)NULL;
	}

	receiver = bitset_dup(over_set->s_set_bs);
	if(!bitset_isfull(t1->t_set_bits))
	{
		for(index = 0; index < t1->t_set_length; index++)
		{
			if(!bitset_test(t1->t_set_bits, index))
				bitset_unset(receiver, over_set->s_offset + index);
		}	
	}

	rec_set_t* rec_set = NewRecSetByOverSet(over_set, receiver);
	
	rep_set = DbgEvalExpr(rec_set, t2->t_str, t3->t_int);

	ProcessErrorInRepSet(rec_set, rep_set);

	num = GetNumberOfReply(rep_set);
	
	if (num > 1)
	{
		DFError(n, "NodeDbgGetVar called with more than one application rank!\n");
		return NULL;
	}
	
	if (num < 1)
	{
		DFError(n, "NodeDbgGetVar has no results in reply!\n");
		return NULL;
	}
	
	// Process the reply
	// FIXME: This node is unecessary. Instead modify graph to understand rep_set_t
	// and have a single node for this.......... Combine with the NodeDbgGetVarSet.
	if ((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// This node will call AIFFree directly on the aif pointer, so we want
		// to mark this reply as save data to prevent it from being free'ed when
		// we free the reply.
		EVENT_SAVE_DATA(reply->rep_event);
		// point a directly at the event data
		a = EVENT_DATA(reply->rep_event);
	}

	// cleanup
	FreeRecSet(rec_set);
	FreeRepSet(rep_set);
	bitset_free(receiver);

	t = NewToken(g, TK_DATA);
	t->t_name = strdup(t2->t_str);
	t->t_data = a;

	return t;
}

/*
** Dataflow event handler. This routine is installed as an event
** handler before the dataflow graph begins executing a process.
** When an event occurs it will take the appropriate action. In
** the case of a breakpoint, a token is "magically" injected into
** the graph as if it was generated by the DBGGO node.
*/
int
NodeEvent(dbgevent_t *e, bitset* src_agg, bitset* src_indiv, void *arg)
{
	dbgnodesetinfo_t *		node_set;
	void *					tree = arg;
	dbgtreenodesinfo_t *	info = GetNodesInfoByTree(tree);
	node_t *				n = NULL;
	token_t *				t;
	int						index = 0;
	int						number = bitset_count(src_indiv);	
	
	if(info == NULL)
	{
		DFError(n, "ERROR: NodeEvent cannot find nodes info for tree[%p]\n", tree);
		return -1;
	}
	DPRINT("[GRAPH INFO] NodeEvent src_agg:%s, src_indiv:%s\n", bitset_to_set(src_agg), bitset_to_set(src_indiv));

	/*
	** All nodes are associated with the same graph. Therefore, RemoveActiveEvent is conducted only on the first node
	*/
	SetList(info->t_node_list);
	if((node_set = (dbgnodesetinfo_t*)GetListElement(info->t_node_list)) != (dbgnodesetinfo_t*)NULL)
	{
		n = node_set->t_node;
		for(index = 0; index < number; ++index)
			RemoveActiveEvent(n->n_graph);
	}

	switch ( e->ev_event )
	{
	case DBGEV_EXIT:
		if ( e->ev_exit != 0 )
			DFMsg("Program exited with code 0%o.", e->ev_exit);

		ExitDF();
		break;

	case DBGEV_BPHIT:
		SetList(info->t_node_list);
		while((node_set = (dbgnodesetinfo_t*)GetListElement(info->t_node_list)) != (dbgnodesetinfo_t*)NULL)
		{
			if(bitset_compare((bitset*)node_set->t_bitset, src_indiv) != 0)
			{
				n = node_set->t_node;

				if (!strcmp(n->n_name, "DBGGO")) {
					t = NewToken(n->n_graph, TK_INT);
					t->t_int = e->ev_bp->bp_id;
					InjectToken(n->n_graph, n, t);
				} else { /* DBGGO_SET */
					number = bitset_count(bitset_and((bitset*)node_set->t_bitset, src_indiv));
					n->n_nevent -= number;

					if (n->n_nevent == 0) {
						RemoveFromList(info->t_node_list, node_set);
						DPRINT("[GRAPH DEBUG] NodeEvent received BPHIT: bp_id[%d], func[%s]\n", e->ev_bp->bp_id, e->ev_bp->bp_loc.loc_func);
						if (!strcmp(e->ev_bp->bp_loc.loc_func, "exit") && (e->ev_bp->bp_id == 1) && ActiveEvents(n->n_graph))
						{
							ExitDF();
						}
						else {
							t = NewToken(n->n_graph, TK_INT);
							t->t_int = e->ev_bp->bp_id;
							InjectToken(n->n_graph, n, t);
	
							DPRINT("[GRAPH DEBUG] NodeEvent injected a token. bp_id[%d], token_id[%d], node id[%d]\n", t->t_int, t->t_id, n->n_id);
						}
					}
				}
			}
		}
		break;

	case DBGEV_SIGNAL:
	case DBGEV_STEP:
		break;

	case DBGEV_ERROR:
		DFError(n, DbgEventErrorStr(e));
		break;

	default:
		break;
	}

	return 1;
}
/* old code
int
NodeEvent(dbgevent_t *e, void *arg)
{
	node_t * 	n = (node_t *)arg;
	token_t *	t;

	RemoveActiveEvent(n->n_graph);

	switch ( e->ev_event )
	{
	case DBGEV_EXIT:
		if ( e->ev_exit == 0 )
			DFMsg("Program exited normally.");
		else
			DFMsg("Program exited with code 0%o.", e->ev_exit);

		ExitDF();
		break;

	case DBGEV_BPHIT:
		if (!strcmp(n->n_name, "DBGGO")) {
			t = NewToken(n->n_graph, TK_INT);
			t->t_int = e->ev_bp->bp_id;
			InjectToken(n->n_graph, n, t);
		} else { * DBGGO_SET *

			// DONNY FIXME
			n->n_nevent--;

			if (n->n_nevent == 0) {
				t = NewToken(n->n_graph, TK_INT);
				t->t_int = e->ev_bp->bp_id;
				InjectToken(n->n_graph, n, t);
			}
		}
		break;

	case DBGEV_SIGNAL:
	case DBGEV_STEP:
		break;

	case DBGEV_ERROR:
		DFError(n, DbgEventErrorStr(e));
		break;

	default:
		break;
	}

	return 1;
}
*/

/*
** Start/Continue execution. Node does not return anything until an event
** occurs. Then a token "magically" appears.
*/
token_t *
NodeDbgGoSet(graph_t *g, node_t *n)
{
	token_t	*			t;
	int					set_length;
	bitset *			receiver = NULL;
	char *				procset_name;
	int 				index;
	dbgprocset_t *  	procset;
	dbg_over_set_t *	over_set;
	rec_set_t *			rec_set;
	rep_set_t *			rep_set;

	t = GetTokenFromInput(n, 0);

	set_length = t->t_set_length;

	n->n_nevent = 0; /* set the number of pending "magic" events */
	
	procset_name = t->t_set_name;
	procset = FindProcSet(procset_name);
	if(procset == NULL)
	{
		DFError(n, "NodeDbgGoSet cannot find procset for %s\n", procset_name);
		return (token_t *)NULL;
	}

	over_set = GetOverlaySetForProcSet(procset);
	if(over_set == (dbg_over_set_t*)NULL)
	{
		DFError(n, "NodeDbgGoSet cannot get Overlay Set for [%s]!\n", procset_name); 
		return (token_t *)NULL;
	}

	for(index = 0; index < set_length; index++)
	{
		if(bitset_test(t->t_set_bits, index))
		{
			AddActiveEvent(g);
			n->n_nevent++;
		}
	}

	RemoveEventSet(over_set->s_invoke->i_tree, DBGEV_BPHIT);

	// FIXME: This call will alloc some memory and add the object to a list. I 
	//         don't know how to clean this up...
	//         I think it should be cleaned up in the NodeEvent function above.
	AddNodeInfoByTree(over_set->s_invoke->i_tree, (void*)n, (void*)t->t_set_bits);

	receiver = bitset_dup(over_set->s_set_bs);
	if(!bitset_isfull(t->t_set_bits))
	{
		for(index = 0; index < t->t_set_length; index++)
		{
			if(!bitset_test(t->t_set_bits, index))
				bitset_unset(receiver, over_set->s_offset + index);
		}	
	}
		
	rec_set = NewRecSetByOverSet(over_set, receiver);

	if ((rep_set = DbgGo(rec_set)) != NULL)
	{
		ProcessErrorInRepSet(rec_set, rep_set);

		for(index = 0; index < set_length; index++)
			RemoveActiveEvent(g);
			
		FreeRecSet(rec_set);
		FreeRepSet(rep_set);
		
		DFError(n, "NodeDbgGoSet could not continue processes!\n"); 
		return (token_t *)NULL;
	}
	
	// cleanup
	FreeRecSet(rec_set);
	bitset_free(receiver);

	return (token_t *)NULL;
}

/*
** Initialise server
*/
token_t *
NodeDbgInit(graph_t *g, node_t *n)
{
	token_t	*	t;

	t = GetTokenFromInput(n, 0);

	DbgHandleSignal(SIGINT, Cleanup);
	DbgHandleSignal(SIGTERM, Cleanup);
	DbgHandleSignal(SIGHUP, Cleanup);

	return DupToken(t);
}

/*
** Open socket at every dbgsrv.
*/
token_t *
NodeDbgOpenSocketSet(graph_t *g, node_t *n)
{
	token_t	*	t1;
	token_t	*	t2;
	token_t	*	t3;
	token_t	*	t;
	int *		bi_set;
	int		set_length;
	rep_set_t *rep_set;
	int	index;
	group_reply_t *reply;
	bitset* receiver;
	char	*procset_name;
	char	*opset_name;
	int	opset_size;
	dbgbp_t *	bp_t;

	t1 = GetTokenFromInput(n, 0);
	t2 = GetTokenFromInput(n, 1);
	t3 = GetTokenFromInput(n, 2);

	set_length = t1->t_set_length;

	bi_set =  (int *)Alloc(set_length * sizeof(int));

	procset_name = t1->t_set_name;
	opset_name = t2->t_str;
	opset_size = t3->t_int;

	dbgprocset_t *  procset = FindProcSet(procset_name);
	if(procset == NULL)
	{
		DFError(n, "NodeDbgOpenSocketSet cannot find procset for %s\n", procset_name);
		return NULL;
	}

	dbg_over_set_t* over_set = NULL;
	over_set = GetOverlaySetForProcSet(procset);
	if(over_set == (dbg_over_set_t*)NULL)
	{
		DFError(n, "NodeDbgOpenSocketSet cannot get Overlay Set for [%s]!\n", procset_name);
		return (token_t *)NULL;
	}

	receiver = bitset_dup(over_set->s_set_bs);
	if(!bitset_isfull(t1->t_set_bits))
	{
		for(index = 0; index < t1->t_set_length; index++)
		{
			if(!bitset_test(t1->t_set_bits, index))
				bitset_unset(receiver, over_set->s_offset + index);
		}	
	}
		
//	rec_set_t* rec_set = NewRecSetByOverSet(over_set, t1->t_set_bits, -1);
	rec_set_t* rec_set = NewRecSetByOverSet(over_set, receiver);

	rep_set = DbgOpenSocket(rec_set, opset_name, opset_size);

	bitset_free(receiver);
	ProcessErrorInRepSet(rec_set, rep_set);

	// process results
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// point at the breakpoint
		bp_t = EVENT_BP(reply->rep_event);
		for(index = 0; index < set_length; index++)
			if(bitset_test(reply->rep_group, (over_set->s_offset + index)))
				bi_set[index] = bp_t->bp_id;
	}
	FreeRepSet(rep_set);
	
	t = NewToken(g, TK_INT_SET);
	t->t_int_set = bi_set;
	t->t_set_length = set_length;
	t->t_set_bits = bitset_dup(t1->t_set_bits);

	// turn the flag on
	socket_on = 1;

	return t;
}

/*
** close socket at every dbgsrv.
** FIXME: This function is returning void. But it can fail. That is a very
**         bad idea...
*/
void
_close_socket_proc_set(graph_t *g, token_t *t, node_t *n)
{
	int *		bi_set;
	int		set_length;
	rep_set_t *rep_set;
	int	index;
	group_reply_t *reply;
	bitset* receiver;
	char	*procset_name;
	dbgbp_t* bp_t;

	set_length = t->t_set_length;

	bi_set =  (int *)Alloc(set_length * sizeof(int));

	procset_name = t->t_set_name;
	dbgprocset_t *  procset = FindProcSet(procset_name);
	if(procset == NULL)
	{
		DFError(n, "NodeDbgCloseSocketSet cannot find procset for %s\n", procset_name);
		return;
	}

	dbg_over_set_t* over_set = NULL;
	over_set = GetOverlaySetForProcSet(procset);
	if(over_set == (dbg_over_set_t*)NULL)
	{
		DFError(n, "NodeDbgCloseSocketSet cannot get Overlay Set for [%s]!\n", procset_name);
		return;
	}

	receiver = bitset_dup(over_set->s_set_bs);
	if(!bitset_isfull(t->t_set_bits))
	{
		for(index = 0; index < t->t_set_length; index++)
		{
			if(!bitset_test(t->t_set_bits, index))
				bitset_unset(receiver, over_set->s_offset + index);
		}	
	}
		
//	rec_set_t* rec_set = NewRecSetByOverSet(over_set, t->t_set_bits, -1);
	rec_set_t* rec_set = NewRecSetByOverSet(over_set, receiver);

	rep_set = DbgCloseSocket(rec_set);

	bitset_free(receiver);
	ProcessErrorInRepSet(rec_set, rep_set);

	// process results
	SetReplyHeadInSet(rep_set);
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// point at the breakpoint data
		bp_t = EVENT_BP(reply->rep_event);
		for(index = 0; index < set_length; index++)
			if(bitset_test(reply->rep_group, (over_set->s_offset + index)))
				bi_set[index] = bp_t->bp_id;
	}
	FreeRepSet(rep_set);
	
	return;
}

token_t *
NodeDbgCloseSocketSet(graph_t *g, node_t *n)
{
	token_t	*	t1;
	token_t	*	t2;

	if (!socket_on)
	{
		DFError(n, "NodeDbgCloseSocket cannot find any socket open\n");
		return NULL;
	}
	else
	{
		t1 = GetTokenFromInput(n, 0);
		t2 = GetTokenFromInput(n, 1);

		_close_socket_proc_set(g, t1, n);
		_close_socket_proc_set(g, t2, n);

		// turn the flag off
		socket_on = 0;

		return (token_t *)NULL;
	}
}
/*
** Set a breakpoint in program.
*/
token_t *
NodeDbgSetBreakSet(graph_t *g, node_t *n)
{
	char *			file = NULL;
	token_t	*		t1;
	token_t	*		t2;
	token_t	*		t3;
	token_t	*		t;
	int *			bi_set;
	int				set_length;
	rep_set_t *		rep_set;
	int				index;
	group_reply_t *	reply;
	dbgbp_t *		bp_t;

	t1 = GetTokenFromInput(n, 0);
	t2 = GetTokenFromInput(n, 1);

	if ( (t3 = GetTokenFromInput(n, 2)) != (token_t *)NULL )
		file = t3->t_str;

	set_length = t1->t_set_length;

	bi_set =  (int *)Alloc(set_length * sizeof(int));

	bitset* receiver;
	char	*procset_name;
	procset_name = t1->t_set_name;
	dbgprocset_t *  procset = FindProcSet(procset_name);
	if(procset == NULL)
	{
		DFError(n, "NodeDbgSetBreakSet cannot find procset for %s\n", procset_name);
		return NULL;
	}

	dbg_over_set_t* over_set = NULL;
	over_set = GetOverlaySetForProcSet(procset);
	if(over_set == (dbg_over_set_t*)NULL)
	{
		DFError(n, "NodeDbgSetBreakSet cannot get Overlay Set for [%s]!\n", procset_name);
		return (token_t *)NULL;
	}

	receiver = bitset_dup(over_set->s_set_bs);
	if(!bitset_isfull(t1->t_set_bits))
	{
		for(index = 0; index < t1->t_set_length; index++)
		{
			if(!bitset_test(t1->t_set_bits, index))
				bitset_unset(receiver, over_set->s_offset + index);
		}	
	}
		
	rec_set_t* rec_set = NewRecSetByOverSet(over_set, receiver); //TODO: extention for individual process
	switch ( t2->t_type )
	{
	case TK_INT:
		rep_set = DbgSetLineBreak(rec_set, file, t2->t_int);
		break;

	case TK_STR:
		rep_set = DbgSetFuncBreak(rec_set, file, t2->t_str);
		break;

	default:
		DFError(n, "Invalid token type");
		return (token_t *)NULL;
	}

	bitset_free(receiver);
	
	ProcessErrorInRepSet(rec_set, rep_set);

	// process results
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// Point at the bp
		bp_t = EVENT_BP(reply->rep_event);

		//keep breakpoints 
		AddBP(rep_set->tree, bp_t, reply->rep_group);

		for(index = 0; index < set_length; index++)
		{
			if(bitset_test(reply->rep_group, (over_set->s_offset + index)))
			{
				bi_set[index] = bp_t->bp_id;
			}
		}
		DPRINT("[GRAPH DEBUG] NodeDbgSetBreakSet[%d] bp_id[%d]\n", n->n_id, bp_t->bp_id);
	}
	FreeRepSet(rep_set);

	t = NewToken(g, TK_INT_SET);
	t->t_int_set = bi_set;
	t->t_set_length = set_length;
	t->t_set_bits = bitset_dup(t1->t_set_bits);

	DPRINT("[GRAPH DEBUG] NodeDbgSetBreakSet[%d] token id[%d]\n", n->n_id, t->t_id);

	return t;
}

/*
** Terminate execution of program.
*/
token_t *
NodeDbgKill(graph_t *g, node_t *n)
{
	token_t	*			t;
	dbg_over_set_t *	over_set;
	char *				procset_name;
	dbgprocset_t *		procset;
	rec_set_t *			rec_set;
	rep_set_t *			rep_set;

	t = GetTokenFromInput(n, 0);

	procset_name = t->t_set_name;
	procset = FindProcSet(procset_name);
	if(procset == NULL)
	{
		DFError(n, "NodeDbgSetBreak cannot find procset for %s\n", procset_name);
		return NULL;
	}

	over_set = GetOverlaySetForProcSet(procset);
	if(over_set == (dbg_over_set_t*)NULL)
	{
		DFError(n, "NodeDbgKill cannot get Overlay Set for [%s]!\n", procset->s_name);
		return (token_t *)NULL;
	}

	rec_set = NewRecSetByOverSet(over_set, SetIndividualProcess(over_set->s_set_bs, 0, procset->s_indiv_proc));
	
	if ((rep_set = DbgKill(rec_set)) != NULL)
	{
		ProcessErrorInRepSet(rec_set, rep_set);
		FreeRepSet(rep_set);
	}

	FreeRecSet(rec_set);
	
	return (token_t *)NULL;
}

/*
** Extract a program handle from an array.
*/
token_t *
NodeProcSet(graph_t *g, node_t *n)
{
	token_t	*	t;
	token_t	*	t0;
	token_t	*	t1;


	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);


	t = NewToken(g, TK_PROC_SET);
	if ( t->t_set_name == NULL )
		t->t_set_name = strdup(t0->t_str);

	t->t_proc_set = FindProcSet(t->t_set_name);
	if ( t->t_proc_set == (dbgprocset_t *)NULL )
	{
		DFError(n, "Invalid procset reference: %s", t0->t_str);
		DestroyToken(g, t);
		return (token_t *)NULL;
	}

	t->t_set_length = bitset_size(t1->t_bit_set);
	
	DPRINT("[GRAPH] NodeProcSet t1->t_bit_set[%s], length[%d]\n", bitset_to_set(t1->t_bit_set), bitset_size(t1->t_bit_set));

	if ( t->t_set_bits == NULL )
	{
		t->t_set_bits = bitset_dup(t1->t_bit_set);
	}
	
	return t;
}

/*
** Set variable to data.
*/
token_t *
NodeDbgSetVar(graph_t *g, node_t *n)
{
	token_t *	t0;
	token_t *	t1;
	token_t *	t2;

	t0 = GetTokenFromInput(n, 0);
	t1 = GetTokenFromInput(n, 1);
	t2 = GetTokenFromInput(n, 2);

	dbg_over_set_t * over_set = NULL;
	char* procset_name = t0->t_set_name;

	dbgprocset_t *  procset = FindProcSet(procset_name);
	if(procset == NULL)
	{
		DFError(n, "NodeDbgSetBreak cannot find procset for %s\n", procset_name);
		return NULL;
	}

	over_set = GetOverlaySetForProcSet(procset);
	if(over_set == (dbg_over_set_t*)NULL)
	{
		DFError(n, "NodeDbgSetVar cannot get Overlay Set for [%s]!\n", procset->s_name);
		return (token_t *)NULL;
	}

	rec_set_t *	rec_set = NewRecSetByOverSet(over_set, SetIndividualProcess(over_set->s_set_bs, 0, procset->s_indiv_proc));
	rep_set_t *	rep_set = DbgSetVar(rec_set, t1->t_str, t2->t_data);
	
	if (ProcessErrorInRepSet(rec_set, rep_set))
	{
		DFError(n, "DbgSetVar failed.");
		return (token_t *)NULL;
	}
	
	FreeRecSet(rec_set);
	FreeRepSet(rep_set);

	return DupToken(t0);
}
