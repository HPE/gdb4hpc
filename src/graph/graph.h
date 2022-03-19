/*********************************************************************************\
 * Graph definition
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2022 Hewlett Packard Enterprise Development LP.
 *
 **********************************************************************************/
#pragma once

#include <map>

#include "dense_bitset.h"
#include "dbgevent.h"
#include "hash.h"
#include "list.h"

#include "file.h"
#include "gvar.h"
#include "node.h"
#include "token.h"

/*
** graph_t holds information about the static graph, and is also
** used when executing the graph.
*/
class graph_t {
public:
    struct ainfo_t;
    struct graph_diff;
    struct graph_inv;

    // TODO make this a properly named struct
    // tuple<int, int, int> -> <num passes, num warns, num fails>
    using AssertResult = std::tuple<int, int, int>;

    graph_t();
    ~graph_t();

    graph_t(const graph_t&) = delete;
    graph_t& operator=(const graph_t&) = delete;

    void reset();
    void setReset(bool r);

    void print(FILE *fp) const;
    void debugPrint() const;

    // list of graph_diff structs created on during graph execution when
    // assertions fail. queried via info diffs
    list_t* diffsList();

    // number of assertions in graph
    int numAssertions() const;

    // graph name as saved by user at graph creation time
    void setName(std::string name);
    const std::string& name() const;

    // externally set to true/false when graph execution begins/ends
    bool isActive() const;
    void setActive(bool state);

    // nodes query this function at creation time for a length for their queues
    int queueLength() const;

    // number of assertions evaluated
    int iterations() const;

    // total number of assertion passes, warnings, and failures, combined across
    // all assertions
    int numPasses() const;
    int numWarnings() const;
    int numFailures() const;

    // TODO merge this information with AssertInfo functions
    // look up result of a particular assertion by assertion id
    AssertResult getAssertResult(int id) const;

    // create a graph_inv and record its bginvoke info in an internal list
    graph_inv* NewGraphInv(dbgprocset_t *ps);
    // find a graph_inv by procset name
    graph_inv* FindGraphInv(const char *inv);

    // assert storage
    ainfo_t* NewAssertInfoEntry();
    ainfo_t* FindAssertInfo(int id);
    const ainfo_t* FindAssertInfo(int id) const;

    // view/modify the integer ids of the graph's start/init nodes
    int startNode() const;
    void setStartNode(int id);
    int initNode() const;
    void setInitNode(int id);

    // ?
    int lastStore() const;
    int fetchIncLastStore();

    // XXX these getters don't really provide any protection until the lists are
    //     converted to stl containers

    // XXX node_t and token_t have linked lists baked into them. these should be
    //     removed and the list logic should sit on top of the types

    // unused? dbginvoke_t *? only ever read from, never inserted to
    list_t* diList();
    // group_reply_t, used for breakpoints
    list_t* replyList();
    // var_t
    list_t* varsList();
    // char *, assert strings from command line
    list_t* assertsList();

    // list of all graph nodes
    node_t* nodesList();
    node_t** lastNode();
    void setLastNode(node_t** last);

    // list of active nodes, ready to be fired
    node_t* activeNodesList();
    void    setActiveNodesList(node_t *list);
    node_t* currentActive();
    void    setCurrentActive(node_t *newCurrent);
    node_t* lastActive();
    void    setLastActive(node_t *newLast);

    // active events counter
    void incActiveEvents();
    void decActiveEvents();
    bool hasActiveEvents() const;

    // tokens
    int      nextTokenId();
    token_t* tokensList();
    void     setTokensList(token_t *list);
    token_t* lastToken();
    void     setLastToken(token_t *newLast);
    token_t* currentToken();
    void     setCurrentToken(token_t *newCurrent);

    // graph construction/verification
    int prepStartNode();
    int checkGraph();
    int outNode(const char *nodeDefName);
    int outConstInt(int val);
    int outConstFloat(double val);
    int outConstStr(const char *val);
    int outConstAIF(double val, int type);
    int outConstBitSet(const DenseBitset* val);
    void outEdge(int from, int to, int port);

    enum class ComparisonResult { PASS, FAIL, WARN, INDETERMINATE, UNKNOWN };
    // XXX: This actually returns a ComparisonResult but the rest of gdb4hpc
    //      still treats it as an int
    // ?
    int compareData(int id, char *name1, AIFStream *data1, char *name2, AIFStream *data2, int cmpop);
    
    // pointer to static event handler function. when the graph starts executing,
    // the client swaps the event handler function to this one
    const static clnt_ev_callback_func g_dbgevent;
private:
    static const int g_qlen = 5;

    int gennode(const char *node, token_t *t);

    bool g_isactive;

    int g_iterations;

    int g_pass;
    int g_warn;
    int g_fail;

    list_t *g_diff;

    bool g_reset;
    int g_nid;
    int g_store;

    std::string g_valname;

    int g_startnode;
    int g_initnode;

    // TODO turn all these lists into stl containers

    list_t *g_di_list;
    list_t *g_bp_reply_list;

    list_t *g_vars;
    list_t *g_assert_cmds;

    node_t *g_nodes;
    // TODO this doesn't need to be a double pointer
    node_t **g_lastNode;

    node_t *g_active;
    node_t *g_currActive;
    node_t *g_lastActive;
    int g_activeEvents;

    int g_token_id;
    token_t *g_tokens;
    token_t *g_last_token;
    token_t *g_tcurrent;

    std::map<size_t, AssertResult> g_assert_results;

    std::vector<std::unique_ptr<ainfo_t>> g_assert_info_tab;

    // All the procsets used in assert or assign.
    // Shared ptrs rather than unique ptrs because in some cases the same key 
    // may point to the same item, see NewGraphInv
    std::map<std::string, std::shared_ptr<graph_inv>> g_inv;
};

struct graph_t::ainfo_t {
    // collection of data representing an assert

    ainfo_t() : a_lower(0.0), a_upper(0.0), a_relative(0), a_stop{1}
    {}

    ainfo_t(const ainfo_t&) = delete;
    ainfo_t& operator=(const ainfo_t&) = delete;

    ~ainfo_t() = default;

    double a_lower;
    double a_upper;
    int a_relative;
    int a_stop;
};

struct graph_t::graph_diff {
    // used to recall saved diff information from a graph execution.

    graph_diff(int id, AIFStream* res, const char* stat) 
        : gd_assert_id{id}, gd_val{res}
    {
        std::ostringstream ss;
        ss << "AssertID " << id + 1 << " " << stat;
        gd_name = ss.str();
    }

    graph_diff(const graph_diff&) = delete;
    graph_diff& operator=(const graph_diff&) = delete;

    ~graph_diff() {
        delete gd_val;
    }

    int gd_assert_id;    /* assert id */
    std::string gd_name; /* difference name */
    AIFStream *gd_val;         /* AIF ptr to difference */
};

struct graph_t::graph_inv {
    // graph_inv is used when bulding a graph to retain state information.
    // TODO: Allow this to be used with DBGSET_DEFSET

    graph_inv(const dbginvoke_t *di)
        : gp_di{di}, gp_unbuilt_bs{std::make_unique<DenseBitset>(di->d_nproc)}
        , gp_files{nullptr}
    {
        // Set every rank to 1, since everything is currently unbuilt.
        gp_unbuilt_bs->invert();

        for (uint d = 0; d < n_invoke_nodes; d++) gp_node[d] = -1;
    }

    graph_inv(const graph_inv&) = delete;
    graph_inv& operator=(const graph_inv&) = delete;

    ~graph_inv() {
        DestroyList(gp_files, (void (*)(void *))DestroyFile);
        gp_files = nullptr;
    }

    static const size_t n_invoke_nodes = 5;

    const dbginvoke_t *gp_di;

    // procset support components
    std::unique_ptr<DenseBitset> gp_unbuilt_bs; /* ranks that have yet to be built */
    list_t *gp_files;                           /* per process list of files */
    int gp_node[n_invoke_nodes];
};
