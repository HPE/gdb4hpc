/*********************************************************************************\
 * Token handling routines
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitset.h"
#include "client.h"
#include "defs.h"
#include "df.h"
#include "useful.h"

struct tt_t {
    token_type tt_type;
    const char *tt_str;
};

struct tt_t tokenType[] = {
    {TK_DATA, "TK_DATA"},         {TK_BOOL, "TK_BOOL"},
    {TK_INT, "TK_INT"},           {TK_FLOAT, "TK_FLOAT"},
    {TK_ADDR, "TK_ADDR"},         {TK_STR, "TK_STR"},
    {TK_PROC_SET, "TK_PROC_SET"}, {TK_INT_SET, "TK_INT_SET"},
    {TK_DATA_SET, "TK_DATA_SET"}, {TK_BIT_SET, "TK_BIT_SET"},
    {TK_ANY, "TK_ANY"},           {TK_BAD, "TK_BAD"}};

const char *TokenTypeStr(token_type t) {
    struct tt_t *tt;

    for (tt = tokenType; tt->tt_type != 0; tt++)
        if (tt->tt_type == t) break;

    return tt->tt_str;
}

/*
** Create space for new tokens. Will expand existing space if
** necessary. Called to initialise token array, or when we have
** run out of space.
*/
int SpaceForTokens(void) {
#ifdef notdef
    token_t *t;

    if (++_tchunks > T_NCHUNKS) {
        HandleError(E_TOKEN);
        return 0;
    }

    if ((t = (token_t *)realloc(_tokens,
                                sizeof(token_t) * _tchunks * T_CHUNK)) == NULL)

    {
        HandleError(E_MALLOC);
        return 0;
    }

    _tokens = t;

    SpaceForFreeMap(_tchunks * T_CHUNK);
#endif /* notdef */

    return 1;
}

token_t *NewToken(graph_t *g, token_type tt) {
    token_t *t;

    t = (token_t *)Alloc(sizeof(token_t));

    t->t_id = g->g_token_id++;
    t->t_name = (char *)NULL;
    t->t_ref = 0;
    t->t_node = (node_t *)NULL;
    t->t_type = tt;
    t->t_data = (AIF *)NULL;
    t->t_next = (token_t *)NULL;
    t->t_prev = (token_t *)NULL;
    t->t_set_name = (char *)NULL;
    t->t_set_length = 0;
    t->t_set_bits = (bitset *)NULL;

    if (g->g_tokens == (token_t *)NULL) {
        g->g_tokens = t;
        g->g_last_token = t;
    } else {
        g->g_last_token->t_next = t;
        t->t_prev = g->g_last_token;
        g->g_last_token = t;
    }

    return t;
}

token_t *DupToken(token_t *t) {
    t->t_ref++;

    return t;
}

/*
** Free token and deallocate any data.
*/
void DestroyToken(graph_t *g, token_t *t) {
    if (t == (token_t *)NULL) return;

    if (--t->t_ref > 0) return;

    if (t->t_name != NULL) Free(t->t_name);

    if (t->t_set_name != NULL) Free(t->t_set_name);

    if (t->t_set_bits != NULL) bitset_free(t->t_set_bits);

    switch (t->t_type) {
        case TK_DATA:
            AIFFree(t->t_data);
            break;

        case TK_BOOL:
        case TK_INT:
        case TK_FLOAT:
        case TK_ADDR:
            break;

        case TK_STR:
            Free(t->t_str);
            break;

        case TK_PROC_SET:
            t->t_proc_set = NULL;
            break;

        case TK_INT_SET:
            Free(t->t_int_set);
            break;

        case TK_DATA_SET:
            // Free the aif * array without free'ing the aif data. The AIF data
            // gets free'ed with the reply set.
            Free(t->t_data_set);
            // Free the shadow reply
            FreeRepSet(t->t_d_shadow_reply);
            break;

        case TK_BIT_SET:
            if (t->t_bit_set != NULL) bitset_free(t->t_bit_set);
            break;

        case TK_BAD:
            SetError("Tried to destroy a BAD token!");
            break;
    }

    if (t->t_prev != (token_t *)NULL) t->t_prev->t_next = t->t_next;

    if (t->t_next != (token_t *)NULL) t->t_next->t_prev = t->t_prev;

    if (g->g_tokens == t) g->g_tokens = t->t_next;

    if (g->g_last_token == t) g->g_last_token = t->t_prev;

    Free(t);
}

/*
** Get token from input inp. Checks that token type
** matches input type.
*/
token_t *GetTokenFromInput(node_t *n, int inp) {
    token_t *t;
    input_t *i;
    input_t *in = n->n_inputs;

    if (inp < 0) {
        for (inp = 0; inp < n->n_ninp; inp++)
            if (!EmptyQueue(in[inp].i_tokens)) break;
    }

    if (inp >= n->n_ninp) return (token_t *)NULL;

    i = &in[inp];

    if ((t = (token_t *)HeadOfQueue(i->i_tokens)) == NULL)
        return (token_t *)NULL;

    if ((t->t_type & i->i_type) == 0) {
        PrintOutError(
            "Node %d has bad token type %s on input %d, expected %s\n",
            i->i_node->n_id, TokenTypeStr(t->t_type), inp,
            TokenTypeStr(i->i_type));
        DumpNode(i->i_node);
        return (token_t *)NULL;
    }

    return t;
}

int ValidTokenType(token_type t) {
    return (int)(t == TK_ANY || (t & TK_VALID) == t);
}

void SetToken(graph_t *g) { g->g_tcurrent = g->g_tokens; }

token_t *NextToken(graph_t *g) {
    token_t *t = g->g_tcurrent;

    if (t == (token_t *)NULL) return (token_t *)NULL;

    g->g_tcurrent = t->t_next;

    return t;
}

void DumpToken(token_t *t) {
    if (t->t_name != NULL) PrintOutError("%s:", t->t_name);

    switch (t->t_type) {
        case TK_ADDR:
            PrintOutError("<0x%lx>", (long)t->t_addr);
            break;

        case TK_INT:
            PrintOutError("<%d>", t->t_int);
            break;

        case TK_FLOAT:
            PrintOutError("<%#g>", t->t_float);
            break;

        case TK_BOOL:
            PrintOutError("<%s>", t->t_bool ? "true" : "false");
            break;

        case TK_STR:
            PrintOutError("<\"%s\">", t->t_str);
            break;

        case TK_DATA:
            PrintOutError("<data ");

            AIFPrint(Client.cli_err_stream, 0, t->t_data);

            PrintOutError(">");
            break;

        default:
            PrintOutError("<bad type %d>", t->t_type);
    }
}
