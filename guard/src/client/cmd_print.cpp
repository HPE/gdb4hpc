/*********************************************************************************\
 * Print command.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex>
#include <sstream>

#include "client.h"
#include "df.h"
#include "useful.h"

// external exposure
int PrintVal(val_t *val, rec_set_t *rec_set) {
    graph_t *g;
    dbgprocset_t *pr = NULL;
    rep_set_t *rep_set;

    if (val == NULL) return -1;

    switch (RVAL_TYPE(val)) {
        case rval_aif:
            // Retrieve the contents of the val
            if (VAL_RETRIEVE(val, rec_set)) {
                return -1;
            }

            //
            // Print the result
            //
            // AIFPrint doesn't play nice with editline...
            RVAL_AIF(val)->print(Client.cli_out_stream, 0);
            PrintOutMsg("\n");

            break;

        case rval_reply:
            // Retrieve the contents of the val
            if (VAL_RETRIEVE(val, rec_set)) {
                return -1;
            }

            // Print each reply
            rep_set = RVAL_REPLY(val);
            for (auto&& reply : rep_set) {
                // Print the set info - Note that the rep_di can be null here if
                // the reply set was the result of an operation on two different
                // sets.
                PrintMsgWithSetInfo(
                    ((reply->rep_di == NULL) ? (char *)""
                                             : reply->rep_di->d_handle),
                    reply->rep_group, NULL);

                // Print the AIF value
                // AIFPrint doesn't play nice with editline...
                AIFPrint(Client.cli_out_stream, 0,
                         EVENT_DATA(reply->rep_event)->joinAIF());
                PrintOutMsg("\n");
            }

            break;

        case rval_proc:
            // assign the procset directly
            if ((pr = RVAL_PROCSET(val)) == NULL) {
                PrintOutMsg("<proc NULL>");
                break;
            }

            PrintOutMsg("<proc %s[%d]>\n", pr->s_name, pr->s_nproc);

            break;

        case rval_graph:
            // assign the graph directly
            if ((g = RVAL_GRAPH(val)) == NULL) {
                PrintOutMsg("NULL graph\n");
                break;
            }

            PrintGraph(Client.cli_out_stream, g);

            break;

        default:
            // do nothing
            return 0;
    }

    return 0;
}

// Print the message from a use of thread apply with it proc-set and thread-set info.
// \a msg is the output from a thread apply operation and will be formatted like:
// Thread 1-4:
//  <result>
// Thread 5 9:
//  <result>
// etc.  and this will be parsed and combined with diName and group to something like:
// $a{0..5:1..4}: <result>
// $a{0..5:5,9}: <result>
int PrintMultiThreadMsg(const char* diName, const bitset* group, const std::string& msg)
{
    // the form is:
    // Threads 1 3 5-7:
    // <result>
    static auto threadHeadRE=std::regex{R"((^|\n)Thread\s+([0-9 ,-]+):\s*\n)"};

    auto lookStr = msg;
    auto thread = std::string{}, eval = std::string{}, nextThread = std::string{};
    auto match = std::smatch{};
    while (!lookStr.empty()) {
        if (std::regex_search(lookStr, match, threadHeadRE)) {
            eval = match.prefix().str();  // will be empty for the first thread
            nextThread = match[2].str();  // note we're catching the thread id in arrears
            lookStr = match.suffix().str();
        } else {
            // result from final thread
            eval = lookStr;
            nextThread = lookStr = "";
        }
        
        if (!eval.empty()) {
            // translate from gdb form to procset form
            for (size_t i=0; (i=thread.find('-', i)) != std::string::npos; ++i) thread.replace(i,1,"..");
            for (size_t i=0; (i=thread.find(' ',i)) != std::string::npos; ++i) thread.replace(i,1,",");

            auto is = std::istringstream{eval};
            auto line = std::string{};
            while (std::getline(is,line)) {
                line += "\n";
                PrintMsgWithSetAndThreadInfo(diName, group, thread.c_str(), line.c_str());
            }
        }
        thread = nextThread;
    }

    return 0;
}

// print out a reply of type DBGEV_THREADINFO with procset/threadset info.
int PrintMultiThreadReply(group_reply_t* reply)
{
    if (reply->rep_event->ev_event == DBGEV_THREADINFO) {
        auto res = std::string{reply->rep_event->ev_u.ev_u_string};
        auto diName = (reply->rep_di ? reply->rep_di->d_handle : "");
        return PrintMultiThreadMsg(diName, reply->rep_group, res);
    } else {
        return -1;
    }
}

// print the value of \a prefixed with the process and possibly thread info.
static int printValWithThreads(val_t* val, rec_set_t* rec_set) {
    if (!val || !rec_set || RVAL_TYPE(val) != rval_reply) {
        return PrintVal(val, rec_set);
    }

    // Retrieve the contents of the val
    if (VAL_RETRIEVE(val, rec_set)) {
        return -1;
    }

    // Print each reply
    auto rep_set = RVAL_REPLY(val);
    for (auto&& reply : rep_set) {
        // Print the set info - Note that the rep_di can be null here if
        // the reply set was the result of an operation on two different
        // sets.
        auto diName = (reply->rep_di ? reply->rep_di->d_handle : "");
        auto aif = EVENT_DATA(reply->rep_event)->joinAIF();
        char* str = nullptr;
        if (AIFType(aif) == AIF_STRING && AIFToStr(&str,0,aif) == 0 &&
            strncmp(str+1,MULTI_THREAD_HDR,strlen(MULTI_THREAD_HDR)) == 0) {
            // this is the result from thread apply returned back in string form since it
            // is potentially a heterogeneous collection
            auto msg = std::string{str+1+strlen(MULTI_THREAD_HDR)}; // skip begin quote and header
            if (msg.size() >= 1) msg.erase(msg.size()-1);  // end quote

            PrintMultiThreadMsg(diName, reply->rep_group, msg);

        } else {
            // A normal expression result, either from no thread focus, or focus on a single thread.
            PrintMsgWithSetAndThreadInfo(diName, reply->rep_group, nullptr, nullptr);
            // Print the AIF value
            // AIFPrint doesn't play nice with editline...
            AIFPrint(Client.cli_out_stream, 0, aif);
            PrintOutMsg("\n");
        }
        free(str);
    }

    return 0;

}

static int cmd_print_cmd(rec_set_t *rec_set, int argc, char **argv) {
    val_t *val;
    char **ap;
    expr_t *e;
    char *buf;

    if (argc < 2) return 1;

    ap = &argv[1];

    if (argc <= 0) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: print expr\n");
        return 1;
    }

    buf = ArgsToBuf(ap, argc - 1);

    if ((e = ParseExpr(buf)) == NULL) {
        PrintError();
        Free(buf);
        return 0;
    }

    // done with buf
    Free(buf);

    if (EvalExpr(&val, e, rec_set) < 0) {
        PrintError();
        FreeExpr(e);
        return 0;
    }

    FreeExpr(e);

    // Ensure we have a recset provided to us if the lval is a gdb/blockmap
    // variable
    switch (LVAL_TYPE(val)) {
        case lval_gdb:
        case lval_blockmap:
            if (rec_set == NULL) {
                PrintOutError(
                    "No process to print symbol. Use launch/attach first.\n");
                DestroyVal(val);
                return 0;
            }
            break;
        default:
            // We should be able to print this, do nothing.
            break;
    }

    DPERFLOG(SetTimerStart("print"));

    printValWithThreads(val, rec_set);

    DPERFLOG(SetTimerStopAndLog("print"));

    DestroyVal(val);

    return 1;
}

void InitCmd_print(void) {
    AddCommand("print", "p", proc_optional, 0, cmd_print_cmd,
               cmd_print_help, "Print the value of an expression.");
}
