/*********************************************************************************\
 * Graph routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013,2020 Cray Inc.
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

#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "defs.h"
#include "df.h"
#include "graph.h"
#include "node.h"
#include "useful.h"

void DumpGraph(graph_t *g) {
    float inc;
    float pos;
    int i;
    node_t *n;
    output_t *o;
    token_t *t;

    PrintOutMsg(("graph ["));
    PrintOutMsg(("Creator \"Guard\""));
    PrintOutMsg(("Version 1"));
    PrintOutMsg(("directed 1"));
    PrintOutMsg(("startnode 0"));

    for (n = g->g_nodes; n != (node_t *)NULL; n = n->n_next) {
        PrintOutMsg(("node [ id %d name \"%s\""), n->n_id, (n->n_def->nd_name));

        if ((t = n->n_store) != (token_t *)NULL) {
            switch (t->t_type) {
                case TK_STR:
                    PrintOutMsg((" label \"%s\""), (t->t_str));
                    PrintOutMsg((" valueStr \"%s\""), (t->t_str));
                    break;

                case TK_INT:
                    PrintOutMsg((" label \"%d\""), t->t_int);
                    PrintOutMsg((" valueInt %d"), t->t_int);
                    break;

                case TK_FLOAT:
                    PrintOutMsg((" label \"%#g\""), t->t_float);
                    PrintOutMsg((" valueReal %#g"), t->t_float);
                    break;

                case TK_DATA:
                    PrintOutMsg((" label \"%#g\""), t->t_name);
                    PrintOutMsg(" valueReal ");
                    AIFPrint(Client.cli_out_stream, 0, t->t_data);
                    break;
            }

            PrintOutMsg((""));
            PrintOutMsg(("       graphics [ type \"oval\" w 50.0 h 15.0 ]"));
        } else {
            PrintOutMsg((" label \"%s(%d)\""), (n->n_def->nd_name), n->n_id);
            PrintOutMsg(
                ("       graphics [ type \"rectangle\" w 50.0 h 10.0 ]"));
        }

        PrintOutMsg(("       ports ["));

        inc = (float)(2.0 / (n->n_ninp + 1));
        pos = -1.0;

        for (i = 0; i < n->n_ninp; i++) {
            pos += inc;
            PrintOutMsg(
                ("               port [ name \"Port%d\" x %0.2f y -1.0 ]"), i,
                pos);
        }

        PrintOutMsg(("             ]"));
        PrintOutMsg(("     ]"));

        for (i = 0; i < n->n_nout; i++) {
            o = &n->n_outputs[i];
            PrintOutMsg(" [ source %d target %d targetPort \"Port%d\" ]",
                        n->n_id, o->o_node->n_id, o->o_inp);
        }
    }
    PrintOutMsg("]");
}
