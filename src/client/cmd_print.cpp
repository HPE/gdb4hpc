/*********************************************************************************\
 * Print command.
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
#include "help_dict.h"
#include "useful.h"

extern void PrintWithBlockMap(AIFStream* aif, g_block_map_t* bm, std::ostream& out);

void ClientSetPrintElements(int elements, rec_set_t* rec_set, bool force) {
    // only do work if we need to
    if (Client.print_elements != elements || force) {
        Client.print_elements = elements;

        // it is valid to call this function with a null rec_set, before a
        // gdb instance is launched
        if (rec_set != nullptr) {
            rep_set_t *rep_set = DbgSetPrintElements(rec_set, std::to_string(elements).c_str());
            ProcessErrorInRepSet(rep_set);
            FreeRepSet(rep_set);
        }
    }
}

// external exposure
int PrintVal(val_t *val, rec_set_t *rec_set, const std::string& format) {
    dbgprocset_t *pr = NULL;
    rep_set_t *rep_set;

    if (val == NULL) return -1;

    if (RVAL_TYPE(val) == rval_aif || RVAL_TYPE(val) == rval_reply) {
        // Retrieve the contents of the val
        // Note: take a little care because VAL_RETRIEVE can change the RVAL_TYPE.
        if (VAL_RETRIEVE(val, rec_set)) {
            return -1;
        }
    }
        

    switch (RVAL_TYPE(val)) {
        case rval_aif:
            // Print the result
            RVAL_AIF(val)->print(Client.cli_out.ostream(), "", format);
            PrintOutMsg("\n");

            break;

        case rval_reply:
            // Print each reply
            rep_set = RVAL_REPLY(val);
            for (auto &&reply : rep_set) {
                // Print the set info - Note that the rep_di can be null here if
                // the reply set was the result of an operation on two different
                // sets.
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(), NULL);

                // Print the AIF value
                // AIFPrint doesn't play nice with editline...
                auto aif = EVENT_DATA(reply->rep_event);
                auto& os = Client.cli_out.ostream();
                if (LVAL_TYPE(val) == lval_blockmap) {
                    PrintWithBlockMap(aif, LVAL_BM_BLOCKMAP(val), os);
                } else {
                    aif->print(os);
                }
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

        default:
            // do nothing
            return 0;
    }

    return 0;
}

// Print the message from a use of thread apply with it proc-set and thread-set
// info. \a msg is the output from a thread apply operation and will be
// formatted like: Thread 1-4:
//  <result>
// Thread 5 9:
//  <result>
// etc.  and this will be parsed and combined with diName and group to something
// like: $a{0..5:1..4}: <result> $a{0..5:5,9}: <result>
int PrintMultiThreadMsg(const char *diName, const Bitset *group,
                        const std::string &msg) {
    // the form is:
    // Threads 1 3 5-7:
    // <result>
    for (ThreadMsgParser parser(msg); auto res = parser.nextResult();) {
        auto [thread, val] = *res;

        // translate from gdb form to procset form
        for (size_t i = 0; (i = thread.find('-', i)) != std::string::npos; ++i)
            thread.replace(i, 1, "..");
        for (size_t i = 0; (i = thread.find(' ', i)) != std::string::npos; ++i)
            thread.replace(i, 1, ",");

        auto is = std::istringstream{std::move(val)};
        auto line = std::string{};
        while (std::getline(is, line)) {
            line += "\n";
            PrintMsgWithSetAndThreadInfo(diName, group, thread.c_str(),
                                         line.c_str());
        }
    }

    return 0;
}

// print the value of \a prefixed with the process and possibly thread info.
static int printValWithThreads(val_t *val, rec_set_t *rec_set, const std::string& format) {
    if (!val || !rec_set || RVAL_TYPE(val) != rval_reply) {
        return PrintVal(val, rec_set, format);
    }

    // Retrieve the contents of the val
    if (VAL_RETRIEVE(val, rec_set)) {
        return -1;
    }

    // Print each reply
    auto rep_set = RVAL_REPLY(val);
    auto& os = Client.cli_out.ostream();
    
    for (auto &&reply : rep_set) {
        // Print the set info - Note that the rep_di can be null here if
        // the reply set was the result of an operation on two different
        // sets.
        auto stream = EVENT_DATA(reply->rep_event);
        auto diName = reply->rep_di.c_str();

        auto label = GetProcSetString(diName, reply->rep_group.get(), rec_set->getThreads());
        if (!label.empty()) label += ": ";
        if (Client.MI_mode){
            // temporarily naming variables "var"+"number" to get distinct names, need to figure out
            // how to get better naming convention.
            static int var_count=0;
            var_count++;
            std::string s = "var" + std::to_string(var_count);
            PrintOutMIResult(MIResult::DONE, "name=\"%s\",numchild=\"0\", in_scope=\"true\", has_more=\"0\", value=\"", s.c_str());
            stream->print(os, label, format);
            PrintOutMIResult(MIResult::CONTINUATION, "\"\n(gdb)\n");
        }else{
            PrintOutMsg("%s", ""); // currently need for edit_line interaction
            stream->print(Client.cli_out.ostream(), label, format);
            PrintOutMsg("\n");
        }
    }

    return 0;
}

static int cmd_print_cmd(rec_set_t *rec_set, int argc, char **argv) {

    if (argc < 2) return 1;

    auto format = std::string{}, args = std::string{};

    // Check for print format modifier
    if (auto slash = strchr(argv[0], '/')) {
        format = std::string{slash+1};
        args = ArgsToString(argv+1, argc-1);
    } else if (argv[1][0] == '/') {
        if (argc < 3) return 1;
        format = std::string{argv[1]+1};
        args = ArgsToString(argv+2, argc-2);
    } else {
        args = ArgsToString(argv+1, argc-1);
    }

    if (auto bad = format.find_first_not_of("xotz"); bad != std::string::npos) {
        PrintOutError("Unrecognized format \'%c\'.\n", format[bad]);
        return 0;
    }

    auto e = ParseExpr(args.c_str());
    if (!e) {
        PrintError();
        return 0;
    }

    val_t *val;
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

    printValWithThreads(val, rec_set, format);

    DPERFLOG(SetTimerStopAndLog("print"));

    DestroyVal(val);

    return 1;
}

static int class_set_print_subcmd_elements_cmd(rec_set_t *rec_set, int argc,
                                               char **argv) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Usage: set print elements <positive integer or \"unlimited\">\n");
        return 0;
    }

    int elements = -1;

    if (strcmp("unlimited", argv[1]) == 0) {
        elements = 0;
    } else {
        try {
            elements = std::stoi(argv[1]);
        } catch (const std::invalid_argument &e) {
            PrintOutError("Error: couldn't convert \"%s\" to an integer. (%s)\n",
                          argv[1], e.what());
            return 0;
        } catch (const std::out_of_range &e) {
            PrintOutError("Error: %s is out of range for an integer. (%s)\n",
                          argv[1], e.what());
            return 0;
        }
    }

    if (elements < 0) {
        PrintOutError("Error: negative values are invalid.\n");
        return 0;
    }

    ClientSetPrintElements(elements, rec_set);

    return 1;
}

static int class_show_print_subcmd_elements_cmd(rec_set_t *rec_set, int argc,
                                               char **argv) {
    if (Client.print_elements == 0) {
        PrintOutMsg("Limit on string chars or array elements to print is unlimited.\n");
    } else {
        PrintOutMsg("Limit on string chars or array elements to print is %d.\n", Client.print_elements);
    }

    return 1;
}

void InitCmd_print(void) {
    AddCommand("print", "p", proc_optional, 0, cmd_print_cmd, cmd_print_help,
               "Print the value of an expression.");

    AddSetPrintCommand(
        "elements", proc_optional, 0, class_set_print_subcmd_elements_cmd,
        class_set_print_subcmd_elements_help,
        "Specify number of elements to be printed in arrays or c-strings.");

    AddShowPrintCommand(
        "elements", proc_optional, 0, class_show_print_subcmd_elements_cmd,
        class_show_print_subcmd_elements_help,
        "Show number of elements to print when an array is printed.");
}
