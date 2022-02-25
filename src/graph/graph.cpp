/*********************************************************************************\
 * Graph routines
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <limits>
#include <stdexcept>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bitset.h"
#include "client.h"
#include "defs.h"
#include "df.h"
#include "useful.h"

extern int NodeEvent(event_set_t *, const Bitset*);
const clnt_ev_callback_func graph_t::g_dbgevent = NodeEvent;

graph_t::graph_t()
    : g_isactive{false}
    , g_iterations{0}
    , g_pass{0}
    , g_warn{0}
    , g_fail{0}
    , g_diff{NewList()}
    , g_reset{true}
    , g_nid{0}
    , g_store{0}
    , g_startnode{-1}
    , g_initnode{-1}
    , g_di_list{NewList()}
    , g_bp_reply_list{NewList()}
    , g_vars{NewList()}
    , g_assert_cmds{NewList()}
    , g_nodes{nullptr}
    , g_lastNode{&g_nodes}
    , g_active{nullptr}
    , g_currActive{nullptr}
    , g_lastActive{nullptr}
    , g_activeEvents{0}
    , g_token_id{0}
    , g_tokens{nullptr}
    , g_last_token{nullptr}
    , g_tcurrent{nullptr}
{}

// XXX: Stuck with this until g_diff is an stl container...
void destroyGDiff(graph_t::graph_diff *gd) {
    delete gd;
}

graph_t::~graph_t() {
    DestroyNodes(g_nodes);
    DestroyList(g_diff, (void (*)(void *))destroyGDiff);
    DestroyList(g_di_list, (void (*)(void *))NULL);
    DestroyList(g_bp_reply_list, (void (*)(void *))FreeGroupReply);
    DestroyList(g_vars, (void (*)(void *))DestroyVar);
    DestroyList(g_assert_cmds, (void (*)(void *))Free);

    while (g_tokens != (token_t *)NULL) DestroyToken(this, g_tokens);
}

graph_t::graph_inv* graph_t::NewGraphInv(dbgprocset_t *ps) {
    // FIXME: This assumes only a single dbginvoke - this might not be the case
    // We don't have any checks here to fail if so.
    SetOverlaySetForProcSet(ps);
    auto over_set = GetNextOverlaySetForProcSet(ps);

    auto gi = std::make_shared<graph_inv>(over_set->s_di);

    // FIXME: This won't work well for user defined process sets...
    // Put this graph_inv in the global hash table
    if (strcmp(over_set->s_di->d_handle, ps->s_name) != 0) {
        g_inv.insert({ps->s_name, gi});
    }

    // FIXME: We still need to insert the di handle since the graph uses it. It
    // should be modified to use the process set name instead.
    g_inv.insert({over_set->s_di->d_handle, gi});

    // Put this dbginvoke_t in the g_di_list
    if (InList(g_di_list, over_set->s_di) == 0)
        AddToList(g_di_list, over_set->s_di);

    return gi.get();
}

graph_t::graph_inv* graph_t::FindGraphInv(const char *inv) {
    if (inv == nullptr) {
        throw std::runtime_error("Null key in FindGraphInv");
    }

    if (auto it = g_inv.find(std::string{inv}); it != g_inv.end()) {
        return it->second.get();
    } else {
        return nullptr;
    }
}

void graph_t::incActiveEvents() { g_activeEvents++; }

void graph_t::decActiveEvents() { g_activeEvents--; }

bool graph_t::hasActiveEvents() const { return g_activeEvents > 0; }

/*
** Reset a graph. This involves:
** - removing tokens from the inputs of all nodes
** - removing all nodes from the active list
** - recreating start token
*/
void graph_t::reset() {
    node_t *n;

    if (g_reset) return;

    for (n = g_nodes; n != NULL; n = n->n_next) ResetInputs(this, n);

    g_active = (node_t *)NULL;
    g_currActive = (node_t *)NULL;
    g_lastActive = (node_t *)NULL;
    g_activeEvents = 0;

    prepStartNode();

    g_reset = true;
}

void graph_t::debugPrint() const {
    static int print_index = 0;
    char print_name[128];
    FILE *file = Client.cli_out_stream;

    sprintf(print_name, "graph-%d.dot", print_index++);
    file = fopen(print_name, "w");
    print(file);
}

void graph_t::setReset(bool r) {
    g_reset = r;
}

void graph_t::print(FILE *fp) const {
    int i;
    node_t *n;
    output_t *o;
    token_t *t;

    PrintToFd(fp, "digraph a {\n");

    for (n = g_nodes; n != (node_t *)NULL; n = n->n_next) {
        PrintToFd(fp, "%d [", n->n_id);

        if ((t = n->n_store) != (token_t *)NULL) {
            switch (t->t_type) {
                case TK_STR:
                    PrintToFd(fp, " label=\"%s\"", t->t_str);
                    break;

                case TK_INT:
                    PrintToFd(fp, " label=%d", t->t_int);
                    break;

                case TK_BIT_SET:
                    PrintToFd(fp, " label=\"%s\"", t->t_bit_set->toSetString().c_str());
                    break;

                case TK_FLOAT:
                    PrintToFd(fp, " label=%#g", t->t_float);
                    break;
                case TK_DATA:
                    PrintToFd(fp, " label=");
                    AIFPrint(fp, 0, t->t_data);
                    break;
            }

            PrintToFd(fp, " shape=ellipse ]\n");
        } else {
            PrintToFd(fp, " label=\"%s(%d)\"", n->n_def->nd_name, n->n_id);
            PrintToFd(fp, " shape=box ]\n");
        }

        for (i = 0; i < n->n_nout; i++) {
            o = &n->n_outputs[i];
            PrintToFd(fp, "%d -> %d\n", n->n_id, o->o_node->n_id);
        }
    }

    PrintToFd(fp, "}\n");
}

list_t* graph_t::diffsList() {
    return g_diff;
}

int graph_t::numAssertions() const {
    // XXX: too many outside things rely on this returning an int rather than a
    //      size_t right now, so until we can properly fix the interfaces, just
    //      make sure it won't overflow
    if (g_assert_info_tab.size() > std::numeric_limits<int>::max()) {
        throw std::runtime_error("Overflow in graph_t::numAssertions");
    }

    return static_cast<int>(g_assert_info_tab.size());
}

graph_t::ainfo_t* graph_t::NewAssertInfoEntry() {
    g_assert_info_tab.emplace_back(std::make_unique<ainfo_t>());
    return g_assert_info_tab.back().get();
}

graph_t::ainfo_t* graph_t::FindAssertInfo(int id) {
    if (id < 0) return nullptr;
    // temporary to silence warnings until interface properly uses size_t
    auto uid = static_cast<size_t>(id);
    
    if (uid >= g_assert_info_tab.size()) return nullptr;

    return g_assert_info_tab[uid].get();
}

const graph_t::ainfo_t* graph_t::FindAssertInfo(int id) const {
    if (id < 0) return nullptr;
    // temporary to silence warnings until interface properly uses size_t
    auto uid = static_cast<size_t>(id);
    
    if (uid >= g_assert_info_tab.size()) return nullptr;

    return g_assert_info_tab[uid].get();
}

int graph_t::prepStartNode() {
    token_t *t;
    node_t *n;

    /*
    ** Create dummy token.
    */

    if ((n = FindNode(this, startNode())) == (node_t *)NULL) {
        SetError("No such start node \"%d\"\n", startNode());
        return 0;
    }

    /*
    ** Check if node has any inputs. Create one
    ** if not.
    */
    if (n->n_ninp == 0 && NewInput(this, n, 0, TK_ANY) < 0) {
        return 0;
    }

    t = NewToken(this, TK_INT);
    t->t_int = startNode();

    /*
    ** Fake an token on first input of node.
    ** GenNodeInput() duplicates token, and adds
    ** node to active list.
    */

    GenNodeInput(this, n, &n->n_inputs[0], t);

    // DestroyToken(g, t);

    return 1;
}

int graph_t::checkGraph() {
    if (CheckNodes(this) == 0) {
        return 0;
    }

    return 1;
}

int graph_t::gennode(const char *node, token_t *t) {
    nodedef_t *nd;

    if ((nd = FindNodeDef(node)) == (nodedef_t *)NULL) {
        PrintOutError("No such node \"%s\"\n", node);
        return -1;
    }

    (void)NewNode(this, g_nid, nd, t);

    return g_nid++;
}

int graph_t::outNode(const char *node) {
    return gennode(node, (token_t *)NULL);
}

int graph_t::outConstInt(int val) {
    token_t *t;
    char buf[BUFSIZ];

    sprintf(buf, "%d", val);

    t = NewToken(this, TK_INT);
    t->t_int = val;
    t->t_name = strdup(buf);

    return gennode("CONST", t);
}

int graph_t::outConstFloat(double val) {
    token_t *t;
    char buf[BUFSIZ];

    sprintf(buf, "%#g", val);

    t = NewToken(this, TK_FLOAT);
    t->t_float = val;
    t->t_name = strdup(buf);

    return gennode("CONST", t);
}

int graph_t::outConstStr(const char *val) {
    token_t *t;

    t = NewToken(this, TK_STR);
    t->t_str = strdup(val);

    return gennode("CONST", t);
}

int graph_t::outConstAIF(double val, int type)  // just int for now
{
    token_t *t;
    char buf[BUFSIZ];

    t = NewToken(this, TK_DATA);

    if (type == AIF_INTEGER) {
        sprintf(buf, "%d", (int)val);
        t->t_data = IntToAIF((int)val);
    } else if (type == AIF_FLOATING) {
        sprintf(buf, "%f", val);
        t->t_data = IntToAIF(val);
    }

    t->t_name = strdup(buf);

    return gennode("CONST", t);
}

int graph_t::outConstBitSet(const DenseBitset *val) {
    token_t *t;

    t = NewToken(this, TK_BIT_SET);
    DASSERT_NOT_NULL(val);
    t->t_bit_set = val->typedClone();

    return gennode("CONST", t);
}

void graph_t::outEdge(int from, int to, int port) {
    if (!Connect(this, from, to, port)) {
        PrintOutError("Could not connect node %d to %d:%d\n", from, to, port);
        PrintError();
    }
}

/*
** Called to perform a comParison and display the result when the
** dataflow interpreter is executing.
**
** Returns 1 if interpreter is to continue, 0 to stop the interpreter.
**
** g - the graph that is executing
**
** id - the assertion id. Can be used to associate assertion specific
**      data with the comparison. For example this might be used to
**      associate upper and lower bounds, display type and an output file
**      with the assertion.
**
** name - the name of the variable being compared.
**
** data - the data being compared.
**
** compare operator
*/
int graph_t::compareData(int id, char *name1, AIF *data1, char *name2, AIF *data2, int cmpop) {
    // temporary cast until interpace properly uses unsigned
    if (id < 0) {
        PrintOutError("Bad assertion ID\n");
        return 0;
    }

    auto uid = static_cast<size_t>(id);
    
    const auto OpResult = [](int aifResult, int cmpop) {
        if (aifResult == 3)
            return ComparisonResult::INDETERMINATE;
        else if (aifResult == -2 || aifResult == 2)
            return ComparisonResult::WARN;

        // the aifResult is reversed from the expected:
        // -1: A > B
        //  0: A == B
        //  1: A < B
        bool pass = true;
        switch (cmpop) {
            case EXPR_OPNOTEQUAL:
                pass = (aifResult != 0);
                break;
            case EXPR_OPLEQ:
                pass = (aifResult != -1);
                break;
            case EXPR_OPGEQ:
                pass = (aifResult != 1);
                break;
            case EXPR_OPLT:
                pass = (aifResult == 1);
                break;
            case EXPR_OPGT:
                pass = (aifResult == -1);
                break;
            case EXPR_OPEQUAL:
                pass = (aifResult == 0);
                break;
            default:
                return ComparisonResult::UNKNOWN;
        }
        return (pass ? ComparisonResult::PASS : ComparisonResult::FAIL);
    };

    int rtn = 1;
    int aifResult;
    AIF *ar = NULL;
    graph_t::ainfo_t *ai;

    // XXX: using id rather than uid since FindAssertInfo still wants signed for now
    if ((ai = FindAssertInfo(id)) == nullptr) {
        PrintOutError("No assertion information found!\n");
        return 0;
    }

#ifdef DEBUG
/*	fprintf(Client.cli_out_stream, "data1 is:\n");
        AIFPrint(Client.cli_out_stream, 0, data1);
        fprintf(Client.cli_out_stream, "\ndata2 is:\n");
        AIFPrint(Client.cli_out_stream, 0, data2);
        fprintf(Client.cli_out_stream, "\n");
        fflush(Client.cli_out_stream);*/
#endif

    // Ensure the error ranges get created
    if (ai->a_aifupper == NULL || ai->a_aiflower == NULL) {
        ai->a_aiflower = DoubleToAIF(ai->a_lower);
        ai->a_aifupper = DoubleToAIF(ai->a_upper);
    }

    // Call the compare function. This handles EPS/diff calculation for us.
    if (AIFCompare(data1, data2, &aifResult, &ar, ai->a_aiflower,
                   ai->a_aifupper)) {
        PrintOutError("AIFCompare Error: %s\n", AIFErrorStr());
        if (ai->a_stop)
            return 0;
        else
            return 1;
    }

    /*
    ** save result information that is independent from display type.
    */
    ComparisonResult res = OpResult(aifResult, cmpop);
    switch (res) {
        case ComparisonResult::PASS:
            // no differences
            g_pass++;
            std::get<0>(g_assert_results[uid])++;
            // continue on pass
            rtn = 1;
            break;
        case ComparisonResult::WARN:
            // between lo and hi
            g_warn++;
            std::get<1>(g_assert_results[uid])++;
            AddToList(g_diff, (void*)new graph_diff(id, CopyAIF(ar), "Warning"));
            // continue on warning
            rtn = 1;
            break;
        case ComparisonResult::FAIL:
            // fatal difference
            g_fail++;
            std::get<2>(g_assert_results[uid])++;
            AddToList(g_diff, (void*)new graph_diff(id, CopyAIF(ar), "Failure"));
            // if stop is true, we want to stop
            if (ai->a_stop)
                rtn = 0;
            else
                rtn = 1;
            break;
        case ComparisonResult::INDETERMINATE:
            PrintOutError(
                "Warning: A function type, pointer, or some other type that "
                "has an \n");
            PrintOutError(
                "         indeterminate comparison result was encountered.\n");
            PrintOutError(
                "No comparison took place. Continuing graph execution.\n");
            break;
        case ComparisonResult::UNKNOWN:
            PrintOutError("Unknown difference result.\n");
            AIFFree(ar);
            if (ai->a_stop)
                return 0;
            else
                return 1;
    }

    // increment the iteration count
    // we only increment the count when a full assertion has completed its
    // evauluation.
    g_iterations++;

    // Always display XML results if XML mode is on
    if (Client.XML_mode) {
        // XXX: using id rather than uid since getAssertResult etc still wants 
        //      signed for now
        auto [pass, warn, fail] = getAssertResult(id);
        PrintOutLine(XML_RESULT, name().c_str(), id + 1, pass, warn, fail);
    }

    switch (res) {
        case ComparisonResult::WARN:
            // display a warning
            PrintOutLine(
                "*** Tolerable difference found in AssertID %lu ***\n",
                uid + 1);
            break;
        case ComparisonResult::FAIL:
            // display an error
            PrintOutLine("*** Difference found in AssertID %lu ***\n",
                            uid + 1);
            /*
            #ifdef DEBUG
                    fprintf(Client.cli_out_stream, "data1 is:\n");
                    AIFPrint(Client.cli_out_stream, 0, data1);
                    fprintf(Client.cli_out_stream, "\ndata2 is:\n");
                    AIFPrint(Client.cli_out_stream, 0, data2);
                    fprintf(Client.cli_out_stream, "\n");
                    fprintf(Client.cli_out_stream, "\ndiff is:\n");
                    AIFPrint(Client.cli_out_stream, 0, ar);
                    fprintf(Client.cli_out_stream, "\n");
                    fflush(Client.cli_out_stream);
            #endif
            */
            break;
        default:
            // do nothing
            break;
    }

    AIFFree(ar);
    return rtn;
}

void graph_t::setName(std::string name) {
    g_valname = std::move(name);
}

const std::string& graph_t::name() const {
    return g_valname;
}

bool graph_t::isActive() const {
    return g_isactive;
}

void graph_t::setActive(bool state) {
    g_isactive = state;
}

int graph_t::queueLength() const {
    return g_qlen;
}

int graph_t::iterations() const {
    return g_iterations;
}

int graph_t::numPasses() const {
    return g_pass;
}

int graph_t::numWarnings() const {
    return g_warn;
}

int graph_t::numFailures() const {
    return g_fail;
}

graph_t::AssertResult graph_t::getAssertResult(int id) const {
    if (id < 0) {
        throw std::runtime_error("Negative index in getAssertResult");
    }
    // temporary to silence warnings until interface properly uses size_t
    auto uid = static_cast<size_t>(id);
    if (g_assert_results.count(uid) == 0) {
        throw std::runtime_error("Out of bounds in getAssertResult");
    }

    return g_assert_results.at(uid);
}

int graph_t::startNode() const {
    return g_startnode;
}

void graph_t::setStartNode(int id) {
    g_startnode = id;
}

int graph_t::initNode() const {
    return g_initnode;
}

void graph_t::setInitNode(int id) {
    g_initnode = id;
}

int graph_t::lastStore() const {
    return g_store;
}

int graph_t::fetchIncLastStore() {
    return g_store++;
}

list_t* graph_t::diList() {
    return g_di_list;
}

list_t* graph_t::replyList() {
    return g_bp_reply_list;
}

list_t* graph_t::varsList() {
    return g_vars;
}

list_t* graph_t::assertsList() {
    return g_assert_cmds;
}

node_t* graph_t::nodesList() {
    return g_nodes;
}

node_t** graph_t::lastNode() {
    return g_lastNode;
}

void graph_t::setLastNode(node_t** last) {
    g_lastNode = last;
}

node_t* graph_t::activeNodesList() {
    return g_active;
}

void graph_t::setActiveNodesList(node_t *list) {
    g_active = list;
}

node_t* graph_t::currentActive() {
    return g_currActive;
}

void graph_t::setCurrentActive(node_t *newCurrent) {
    g_currActive = newCurrent;
}

node_t* graph_t::lastActive() {
    return g_lastActive;
}

void graph_t::setLastActive(node_t *newLast) {
    g_lastActive = newLast;
}

int graph_t::nextTokenId() {
    return g_token_id++;
}

token_t* graph_t::tokensList() {
    return g_tokens;
}

void graph_t::setTokensList(token_t *list) {
    g_tokens = list;
}

token_t* graph_t::lastToken() {
    return g_last_token;
}

void graph_t::setLastToken(token_t *newLast) {
    g_last_token = newLast;
}

token_t* graph_t::currentToken() {
    return g_tcurrent;
}

void graph_t::setCurrentToken(token_t *newCurrent) {
    g_tcurrent = newCurrent;
}