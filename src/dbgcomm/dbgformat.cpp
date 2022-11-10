/*********************************************************************************\
 * network format routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2012-2020,2022 Hewlett Packard Enterprise Development LP.
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
#include <utility>
#include <sstream>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"
#include "defs.h"
#include "useful.h"

#include "unittest_macros.h"

TEST_CASE("EventToSocProto/SocProtoToEvent") {
    SUBCASE("DBGEV_NONE") {
        auto e = NewEvent(DBGEV_NONE);
        auto soc = EventToSocProto(e); // frees e
        auto e2 = SocProtoToEvent(soc);
        CHECK(e2 == nullptr);
        free(soc);
    }

    SUBCASE("Events with nothing on the heap") {
        auto test = [](dbgev type) {
            auto e = NewEvent(type);
            e->ev_u.ev_u_int = 0;
            auto soc = EventToSocProto(e); // frees e
            auto e2 = SocProtoToEvent(soc);

            CHECK(e2->ev_event == type);
            FreeEvent(e2);
            free(soc);
        };

        SUBCASE("DBGEV_OK") {test(DBGEV_OK);}
        SUBCASE("DBGEV_EXIT") {test(DBGEV_EXIT);}
    }

    SUBCASE("String-y events") {
        auto test = [](dbgev type, std::string str) {
            auto e = NewEvent(type);
            e->ev_u.ev_u_string = strdup(str.c_str());
            auto soc = EventToSocProto(e); // frees e and string
            auto e2 = SocProtoToEvent(soc);

            CHECK(e2->ev_event == type);
            CHECK(std::string(e2->ev_u.ev_u_string) == str);

            FreeEvent(e2);
            free(soc);
        };

        SUBCASE("DBGEV_SOURCEPATH") {test(DBGEV_SOURCEPATH, "source path");}
        SUBCASE("DBGEV_TYPE") {test(DBGEV_TYPE, "type");}
        SUBCASE("DBGEV_CMD") {test(DBGEV_CMD, "cmd");}
        SUBCASE("DBGEV_THREADINFO") {test(DBGEV_THREADINFO, "thread info");}
        SUBCASE("DBGEV_STRING") {test(DBGEV_STRING, "string");}
    }

    SUBCASE("Breakpoint-y events") {
        auto test = [](dbgev type, dbgbp_t* bp) {
            auto e = NewEvent(type);
            e->ev_u.ev_u_bp = bp->clone();
            auto soc = EventToSocProto(e); // frees e and bp
            CAPTURE(std::string(soc));
            auto e2 = SocProtoToEvent(soc);

            auto* bp2 = e2->ev_u.ev_u_bp;

            // basic types
            CHECK(e2->ev_event == type);
            CHECK(bp2->bp_fe_id == bp->bp_fe_id);
            CHECK(bp2->bp_be_id == 0);
            CHECK(bp2->bp_enabled == bp->bp_enabled);
            CHECK(bp2->bp_disp == bp->bp_disp);
            CHECK(bp2->bp_type == bp->bp_type);

            // strings
            CHECK(std::string(bp2->bp_exp) == std::string(bp->bp_exp));
            CHECK(std::string(bp2->bp_threadId) == std::string(bp->bp_threadId));

            // loc
            // SocProtoToEvent will find the full path
            CHECK(bp2->bp_loc.loc_file == doctest::Contains(bp->bp_loc.loc_file));
            CHECK(std::string(bp2->bp_loc.loc_func) == std::string(bp->bp_loc.loc_func));
            CHECK(bp2->bp_loc.loc_line == bp->bp_loc.loc_line);

            if (type != DBGEV_BPHIT && type != DBGEV_BPDEL) {
                // BPHIT and BPDEL have certain elements omitted in serialization

                CHECK(bp2->bp_ignore == bp->bp_ignore);
                CHECK(bp2->bp_pending == bp->bp_pending);
                CHECK(std::string(bp2->bp_stmt) == std::string(bp->bp_stmt));
                CHECK(std::string(bp2->bp_loc.loc_addr) == std::string(bp->bp_loc.loc_addr));
                CHECK(bp2->bp_hits == bp->bp_hits);
            }

            FreeEvent(e2);
            free(soc);
        };

        auto bp = new dbgbp_t(7,0);

        bp->bp_ignore = 1;
        bp->bp_deleted = 2; // This is never serialized
        bp->bp_enabled = 3;
        bp->bp_pending = 4;
        bp->bp_hits = 5;

        bp->bp_disp = DBGBP_DELETE_DISP;
        bp->bp_type = DBGBP_CATCH_TYPE;

        bp->bp_loc.loc_file = strdup("main.cpp");
        bp->bp_loc.loc_func = strdup("main");
        bp->bp_loc.loc_addr = strdup("0xffff");
        bp->bp_loc.loc_line = 197;

        bp->bp_exp = "myVar";
        bp->bp_threadId = "0xaaaa";
        bp->bp_stmt = "callMyFunction();";

        // bp_deleted, bp_commands, and bp_comminfo are never serialized

        SUBCASE("DBGEV_BPHIT") {test(DBGEV_BPHIT, bp);}
        SUBCASE("DBGEV_BPINFO") {test(DBGEV_BPINFO, bp);}
        SUBCASE("DBGEV_BPSET") {test(DBGEV_BPSET, bp);}
        SUBCASE("DBGEV_BPDEL") {test(DBGEV_BPDEL, bp);}

        delete bp;
    }

    SUBCASE("Frame-y events") {
        // thread a
        auto loc0a = dbgloc(strdup("dbgformat.cpp"), strdup("FreeEvent"), strdup("0xaaaa"), 180);
        auto loc1a = dbgloc(strdup("dbgformat.cpp"), strdup("FreeBp"), strdup("0xbbbb") ,200);
        auto loc2a = dbgloc(strdup("dbgformat.cpp"), strdup("EventToSocProto"), strdup("0xcccc"), 60);
        // thread b
        auto loc0b = dbgloc(strdup("dbgevent.cpp"), strdup("FreeEvent"), strdup("0xdddd"), 20);
        auto loc1b = dbgloc(strdup("dbgevent.cpp"), strdup("FreeBp"), strdup("0xeeee"), 210);
        auto loc2b = dbgloc(strdup("dbgevent.cpp"), strdup("EventToSocProto"), strdup("0xffff"), 90);

        auto frame2b = new dbgframe_t(2, loc2a, nullptr, "thread b", nullptr);
        auto frame1b = new dbgframe_t(1, loc1a, frame2b, "thread b", nullptr);
        auto frame0b = new dbgframe_t(0, loc0a, frame1b, "thread b", nullptr);

        auto frame2a = new dbgframe_t(2, loc2a, nullptr, "thread a", nullptr);
        auto frame1a = new dbgframe_t(1, loc1a, frame2a, "thread a", nullptr);
        auto frame0a = new dbgframe_t(0, loc0a, frame1a, "thread a", frame0b);

        auto sameFrame = [](const dbgframe_t& l, const dbgframe_t& r) {
            auto str = [](const char* s) { return s ? std::string(s) : ""; };
            auto* lthf = &l;
            auto* rthf = &r;
            for (; lthf && rthf; lthf=lthf->getNextThreadFrame(), rthf=rthf->getNextThreadFrame()) {
                auto* lf = lthf;
                auto* rf = rthf;
                for (; lf && rf; lf = lf->getNext(), rf = rf->getNext()) {
                    const auto &lloc = lf->getLoc();
                    const auto lfile = str(lloc.loc_file);
                    const auto lfunc = str(lloc.loc_func);
                    const auto laddr = str(lloc.loc_addr);
                    const auto lthreads = str(lf->getThreads());

                    const auto &rloc = rf->getLoc();
                    const auto rfile = str(rloc.loc_file);
                    const auto rfunc = str(rloc.loc_func);
                    const auto raddr = str(rloc.loc_addr);
                    const auto rthreads = str(rf->getThreads());

                    CHECK(lfile == rfile);
                    CHECK(lfunc == rfunc);
                    CHECK(laddr == raddr);
                    CHECK(lthreads == rthreads);
                }
                CHECK_MESSAGE(lf == nullptr, "Check that backtraces have same number of frames");
                CHECK_MESSAGE(rf == nullptr, "Check that backtraces have same number of frames");
            }
            CHECK_MESSAGE(lthf == nullptr, "Check that frames have same number of threads");
            CHECK_MESSAGE(rthf == nullptr, "Check that frames have same number of threads");
        };

        SUBCASE("DBGEV_FRAME") {
            auto e = NewEvent(DBGEV_FRAME);
            e->ev_u.ev_u_frame = frame0a->clone();
            auto* soc = EventToSocProto(e); // frees e and frame
            auto* e2 = SocProtoToEvent(soc);

            sameFrame(*frame0a, *e2->ev_u.ev_u_frame);

            FreeEvent(e2);
            free(soc);
        }

        SUBCASE("DBGEV_SIGNAL") {
            auto e = NewEvent(DBGEV_SIGNAL);
            auto& sig = e->ev_u.ev_u_sig;

            sig.sig_type = strdup("SIGCONT");
            sig.sig_line = strdup("77");
            sig.sig_frame = frame0a->clone();

            auto* soc = EventToSocProto(e); // frees e and frame
            auto* e2 = SocProtoToEvent(soc);
            auto& e2sig = e2->ev_u.ev_u_sig;

            CHECK(std::string(e2sig.sig_type) == "SIGCONT");
            CHECK(std::string(e2sig.sig_line) == "77");
            sameFrame(*frame0a, *e2->ev_u.ev_u_sig.sig_frame);

            FreeEvent(e2);
            free(soc);
        }

        SUBCASE("DBGEV_STEP") {
            auto* e = NewEvent(DBGEV_STEP);

            e->ev_u.ev_u_step.step_frame_changed = true;
            e->ev_u.ev_u_step.step_frame = frame0a->clone();

            auto* soc = EventToSocProto(e); // frees e and frame
            auto* e2 = SocProtoToEvent(soc);

            CHECK(e2->ev_u.ev_u_step.step_frame_changed == true);
            sameFrame(*frame0a, *e2->ev_u.ev_u_step.step_frame);

            FreeEvent(e2);
            free(soc);
        }

        delete frame0a;
    }

    SUBCASE("Location-y events") {
        auto tloc = new dbgloc();
        tloc->loc_file = strdup("main.cpp");
        tloc->loc_func = strdup("main");
        tloc->loc_addr = strdup("0xffff");
        tloc->loc_line = 197;

        SUBCASE("DBGEV_CHECKPOINT") {
            auto* e = NewEvent(DBGEV_CHECKPOINT);
            e->ev_u.ev_u_checkpoint = new dbgcheckpoint_t {
                .id = 29,
                .location = *tloc,
            };

            auto* soc = EventToSocProto(e); // frees e and its checkpoint
            auto* e2 = SocProtoToEvent(soc);
            auto* eChp2 = e2->ev_u.ev_u_checkpoint;

            CHECK(eChp2->id == 29);
            // SocProtoToEvent will find the full path of the filename
            CHECK(doctest::Contains("main.cpp") == eChp2->location.loc_file);
            CHECK(std::string(eChp2->location.loc_func) == "main");
            CHECK(std::string(eChp2->location.loc_addr) == "0xffff");
            CHECK(eChp2->location.loc_line == 197);

            FreeEvent(e2);
            free(soc);
        }

        SUBCASE("DBGEV_STAT") {
            auto* e = NewEvent(DBGEV_STAT);
            // FreeStatus calls free()
            e->ev_u.ev_u_status = (dbgstatus_t*)malloc(sizeof(dbgstatus_t));
            e->ev_u.ev_u_status->stat = DBGSTAT_RUNNING;
            e->ev_u.ev_u_status->stat_loc = DupLocation(tloc);

            auto* soc = EventToSocProto(e); // frees e and its status
            auto* e2 = SocProtoToEvent(soc);
            auto* eStat2 = e2->ev_u.ev_u_status;

            CHECK(eStat2->stat == DBGSTAT_RUNNING);
            // SocProtoToEvent will find the full path of the filename
            CHECK(doctest::Contains("main.cpp") == eStat2->stat_loc->loc_file);
            CHECK(std::string(eStat2->stat_loc->loc_func) == "main");
            CHECK(std::string(eStat2->stat_loc->loc_addr) == "0xffff");
            CHECK(eStat2->stat_loc->loc_line == 197);

            FreeEvent(e2);
            free(soc);
        }

        SUBCASE("DBGEV_HALT") {
            auto* e = NewEvent(DBGEV_HALT);
            e->ev_u.ev_u_loc = new dbgloc(*tloc);

            auto* soc = EventToSocProto(e); // frees e and its location
            auto* e2 = SocProtoToEvent(soc);
            auto* eLoc2 = e2->ev_u.ev_u_loc;

            // SocProtoToEvent will find the full path of the filename
            CHECK(doctest::Contains("main.cpp") == eLoc2->loc_file);
            CHECK(std::string(eLoc2->loc_func) == "main");
            CHECK(std::string(eLoc2->loc_addr) == "0xffff");
            CHECK(eLoc2->loc_line == 197);

            FreeEvent(e2);
            free(soc);
        }

        SUBCASE("DBGEV_LOC") {
            auto* e = NewEvent(DBGEV_LOC);
            e->ev_u.ev_u_loc = new dbgloc(*tloc);

            auto* soc = EventToSocProto(e); // frees e and its location
            auto* e2 = SocProtoToEvent(soc);
            auto* eLoc2 = e2->ev_u.ev_u_loc;

            // SocProtoToEvent will find the full path of the filename
            CHECK(doctest::Contains("main.cpp") == eLoc2->loc_file);
            CHECK(std::string(eLoc2->loc_func) == "main");
            CHECK(std::string(eLoc2->loc_addr) == "0xffff");
            CHECK(eLoc2->loc_line == 197);

            FreeEvent(e2);
            free(soc);
        }

        delete tloc;
    }

    SUBCASE("DBGEV_VARS") {
        auto makeVar = [](const char* name, const char* type, dbgvars_t* next) {
            auto* v = (dbgvars_t*)malloc(sizeof(dbgvars_t));
            v->var_name = name ? strdup(name) : nullptr;
            v->var_type = type ? strdup(type) : nullptr;
            v->var_next = next;
            return v;
        };

        auto* v4 = makeVar("var4", "char *", nullptr);
        auto* v3 = makeVar(nullptr, "float", v4); // note that var with a null name will be skipped
        auto* v2 = makeVar("var2", "double", v3);
        auto* v1 = makeVar("var1", "int", v2);
        auto* e = NewEvent(DBGEV_VARS);
        e->ev_u.ev_u_vars = v1;

        auto* soc = EventToSocProto(e); // frees e and vars
        auto* e2 = SocProtoToEvent(soc);

        auto count = 0;
        for (auto* v = e2->ev_u.ev_u_vars; v; v = v->var_next, ++count) {
            if (count == 0) {
                CHECK(std::string(v->var_name) == "var1");
                CHECK(std::string(v->var_type) == "int");
            } else if (count == 1) {
                CHECK(std::string(v->var_name) == "var2");
                CHECK(std::string(v->var_type) == "double");
            } else if (count == 2) {
                CHECK(std::string(v->var_name) == "var4");
                CHECK(std::string(v->var_type) == "char *");
            } else {
                FAIL("count should not be > 2");
            }
        }

        FreeEvent(e2);
        free(soc);
    }

    SUBCASE("DBGEV_LIST") {
        auto makeList = [](int lno, const char* line, const char* threads) {
            auto* l = (dbglist_t*)malloc(sizeof(dbglist_t));
            l->list_lno = lno;
            l->list_line = strdup(line);
            l->list_threads = strdup(threads); // not serialized
            l->list_next = nullptr;
            return l;
        };

        const char* const lines[] = {
            "auto* l = (dbglist_t*)malloc(sizeof(dbglist_t));",
            "l->list_lno = lno;",
            "l->list_line = strdup(line);",
            "l->list_threads = strdup(threads); // not serialized",
            "l->list_next = nullptr;",
            "return l;"
        };

        auto* l381 = makeList(381, lines[0], "0xffff");
        auto* l382 = makeList(382, lines[1], "0xffff");
        auto* l383 = makeList(383, lines[2], "0xffff");
        auto* l384 = makeList(384, lines[3], "0xffff");
        auto* l385 = makeList(385, lines[4], "0xffff");
        auto* l386 = makeList(386, lines[5], "0xffff");

        l381->list_next = l382;
        l382->list_next = l383;
        l383->list_next = l384;
        l384->list_next = l385;
        l385->list_next = l386;

        auto* e = NewEvent(DBGEV_LIST);
        e->ev_u.ev_u_list = l381;
        auto* soc = EventToSocProto(e); // frees e/list
        auto* e2 = SocProtoToEvent(soc);

        auto count = 0;
        for (auto* l = e2->ev_u.ev_u_list; l; l = l->list_next, ++count) {
            CHECK(l->list_lno == 381 + count);
            CHECK(std::string(l->list_line) == std::string(lines[count]));
        }
        CHECK(count == 6);

        FreeEvent(e2);
        free(soc);
    }

    SUBCASE("DBGEV_ERROR") {
        auto* e = NewEvent(DBGEV_ERROR);
        auto& err = e->ev_u.ev_u_err;

        err.err_str = strdup("Your free trial has expired");
        err.err_errno = DBGERR_NOTRUN;

        auto* soc = EventToSocProto(e); // frees e and internals
        auto* e2 = SocProtoToEvent(soc);
        auto& e2err = e2->ev_u.ev_u_err;

        CHECK(std::string(e2err.err_str) == "Your free trial has expired");
        CHECK(e2err.err_errno == DBGERR_NOTRUN);

        FreeEvent(e2);
        free(soc);
    }

    SUBCASE("DBGEV_VERS") {
        auto* e = NewEvent(DBGEV_VERS);
        auto& vers = e->ev_u.ev_u_vers;

        vers.vers_version = strdup("4.14.5");
        vers.vers_arch = strdup("aarch64");

        auto* soc = EventToSocProto(e); // frees e and internals
        auto* e2 = SocProtoToEvent(soc);
        auto& vers2 = e2->ev_u.ev_u_vers;

        CHECK(std::string(vers2.vers_version) == "4.14.5");
        CHECK(std::string(vers2.vers_arch) == "aarch64");

        FreeEvent(e2);
        free(soc);
    }

    SUBCASE("DBGEV_SOURCES") {
        auto makeSources = [](const char* src, dbgsources_t* next) {
            auto* s = (dbgsources_t*)malloc(sizeof(dbgsources_t));
            *s = {
                .sources_file = strdup(src),
                .sources_next = next
            };
            return s;
        };

        const char* const sources[] = {
            "alloc.cpp",
            "args.cpp",
            "debug.cpp",
            "error.cpp",
            "exec.cpp",
        };

        auto* s4 = makeSources(sources[4], nullptr);
        auto* s3 = makeSources(sources[3], s4);
        auto* s2 = makeSources(sources[2], s3);
        auto* s1 = makeSources(sources[1], s2);
        auto* s0 = makeSources(sources[0], s1);

        auto* e = NewEvent(DBGEV_SOURCES);
        e->ev_u.ev_u_sources = s0;
        auto* soc = EventToSocProto(e); // frees e and internals
        auto* e2 = SocProtoToEvent(soc);

        auto i = 0;
        for (auto* s = e2->ev_u.ev_u_sources; s; s = s->sources_next, ++i) {
            CHECK(std::string(s->sources_file) == sources[i]);
        }

        FreeEvent(e2);
        free(soc);
    }
}

TEST_CASE("CommandToSocProto") {
    SUBCASE("Single argument") {
        auto req = CommandToSocProto(DBGCMD_STARTUP);
        auto istr = soc::ISocStream(req);
        CHECK(istr.getCmd() == DBGCMD_STARTUP);
    }

    SUBCASE("Multiple arguments") {
        auto req = CommandToSocProto(DBGCMD_EVALEXPR, "2 + 2", 11ul);

        auto istr = soc::ISocStream(req);

        CHECK(istr.getCmd() == DBGCMD_EVALEXPR);

        std::string expr;
        size_t id;
        istr.get(expr, id);

        CHECK(expr == "2 + 2");
        CHECK(id == 11ul);
    }
}

// Static prototypes
static char *getstring(const char *&);
static unsigned long getUnsignedInt(const char *&);
static int getint(const char *&);

static inline const char* StrSoc(const char* s) {
    return s ? s : "";
}

static char *EventBPToSocProto(dbgev, dbgbp_t *, int);
static char *EventSigToSocProto(dbgev, dbgsig_t);
static char *EventStepToSocProto(dbgev, const dbgstep_t &step);
static char *EventCheckpointToSocProto(dbgev, const dbgcheckpoint_t *);
static char *EventFrameToSocProto(dbgev, const dbgframe_t *);
static char *EventLocToSocProto(dbgev, dbgloc_t *);
static char *EventStatusToSocProto(dbgev, dbgstatus_t *);
static char *EventVersToSocProto(dbgev, dbgvers_t);
static char *EventIntToSocProto(dbgev,int);
static char *EventSourcesToSocProto(dbgev, dbgsources_t *);
static char *EventListToSocProto(dbgev, dbglist_t *);
static char *EventVarsToSocProto(dbgev, dbgvars_t *);
static char *EventStringToSocProto(dbgev, char *);
static char *EventFrameToSocProtoL(const dbgframe_t *);
static char *EventLocToSocProtoL(const dbgloc_t *);
static char *EventDataToSocProto(dbgev, AIFStream *);

static dbgerr_t SocProtoToErr(const char *);
static dbgloc_t *SocProtoToLoc(const char *);
static dbgvars_t *SocProtoToVars(const char *);
static dbglist_t *SocProtoToList(const char *);
static dbgbp_t *SocProtoToBP(const char *);
static dbgsig_t SocProtoToSig(const char *);
static dbgcheckpoint_t *SocProtoToCheckpoint(const char *);
static dbgframe_t *SocProtoToFrame(const char *);
static dbgstep_t SocProtoToStep(const char *);
static char *SocProtoToString(const char *);
static dbgstatus_t *SocProtoToStatus(const char *);
static dbgvers_t SocProtoToVers(const char *);
static int SocProtoToInt(const char *);
static dbgsources_t *SocProtoToSources(const char *);
static AIFStream *SocProtoToAIFStream(const char *);

#define NULL_GUARD(s)              \
    if (s == NULL || *s == '\0') { \
        return NULL;               \
    }

/************************
** Auxillary functions **
************************/

// Function for wrapping an fprintf style encoded message into to the socket
// prototype which is in the form "%d %d %s", event_number, length_of_message, message.
// Since the wrapped message must contain the length of the event message, the
// trick for this function is to try to create the final result without creating two
// potentially huge strings.
static char* MakeSocStr(dbgev ev, const char* fmt, ...) {

    static const size_t BUF_SIZE = 4000;
    char buf[BUF_SIZE];

    // we may need to iterate the var_args twice.   Using va_copy seems to be the
    // more bullet-proof way to do it.
    va_list args, args2;
    va_start(args, fmt);
    va_copy(args2, args);

    // most messages are short so we just try to build the inner string in a
    // fixed size buffer.   Even in the worst case this should outperform
    // calling asprintf twice (note that asprintf already needs to be a two pass
    // operation.
    size_t len = vsnprintf(buf, BUF_SIZE, fmt, args);
    va_end(args);

    char *str = nullptr;
    if (len < BUF_SIZE) {
        // a short message, so we just need to simple copy
        Asprintf(&str, "%d %zu %s", int(ev), len, buf);
    } else {
        // the string is too long for the local buffer but
        // we now know the length of the inside string so we
        // can build the final with just one allocation.

        // note: rather than trying to concatenate the var_args,
        // we just prepend the event number and length to the
        // format string.
        std::ostringstream ostr;
        ostr << int(ev) << " " << len << " " << fmt;
        std::string wrapFmt = ostr.str();    // event + length + fmt

        VAsprintf(&str, wrapFmt.c_str(), args2);
    }
    va_end(args2);

    return str;
}

static int getint(const char *&res_buf) {
    int digit;
    char *endptr = NULL;

    res_buf = skipspace(res_buf);

    // Get the digit
    digit = (int)strtol(res_buf, &endptr, 10);

    res_buf = endptr;

    return digit;
}

static unsigned long getUnsignedInt(const char *&res_buf) {
    char *endptr = NULL;

    res_buf = skipspace(res_buf);

    // Get the digit
    auto digit = strtoul(res_buf, &endptr, 10);

    res_buf = endptr;

    return digit;
}

static char *getstring(const char *&res_buf) {
    int len;
    char *endptr = NULL;
    char *res;

    res_buf = skipspace(res_buf);

    // First get the length of the string - Note that this can be 0
    len = (int)strtol(res_buf, &endptr, 10);

    // Skip past the space between the length and the start of the string
    ++endptr;

    // Now create a new buffer and memcpy
    res = (char *)Alloc(len + 1);
    // only copy if len > 0
    if (len > 0) {
        memcpy(res, endptr, len);
    }

    // set the null terminator
    res[len] = '\0';

    // set res_buf past string
    res_buf = endptr + len;

    return res;
}

static std::string getStdString(const char *&res_buf) {
    res_buf = skipspace(res_buf);

    // First get the length of the string - Note that this can be 0
    char *endptr = nullptr;
    auto len = strtol(res_buf, &endptr, 10);

    // Skip past the space between the length and the start of the string
    ++endptr;

    // Now create a new buffer and memcpy
    auto res = std::string{endptr, endptr+len};

    // set res_buf past string
    res_buf = endptr + len;

    return res;
}
    

/*******************************
** To network proto functions **
*******************************/

/*
** The following are helper functions for converting events into a string for
** network transfer
*/

static char *EventErrorToSocProto(dbgev ev, dbgerr_t e) {
    const char* err_str = StrSoc(e.err_str);
    return MakeSocStr(ev, "%d %d %s", e.err_errno, (int)strlen(err_str), err_str);
}

static char *EventBPToSocProto(dbgev ev, dbgbp_t *be, int simplified) {
    if (!be) return MakeSocStr(ev, "");

    const char* st_file = StrSoc(be->bp_loc.loc_file);
    const char* st_func = StrSoc(be->bp_loc.loc_func);
    const char* st_exp = be->bp_exp.c_str();
    const char* threadId = be->bp_threadId.c_str();

    char *stres = nullptr;
    if (!simplified) {
        const char* st_stmt = be->bp_stmt.c_str();
        const char* st_addr = StrSoc(be->bp_loc.loc_addr);

        stres = MakeSocStr(ev,
                 "%d %d %d %d %d %d %s %d %d %d %d %s %d %s %d %s %d %d %s %d %s",
                 be->bp_fe_id, simplified, be->bp_pending, be->bp_ignore, be->bp_hits,
                 (int)strlen(st_stmt), st_stmt, be->bp_enabled, be->bp_disp,
                 be->bp_type, (int)strlen(st_file), st_file,
                 (int)strlen(st_func), st_func, (int)strlen(st_addr), st_addr,
                           be->bp_loc.loc_line, (int)strlen(st_exp), st_exp,
                           strlen(threadId), threadId);
    } else {
        stres = MakeSocStr(ev,"%d %d %d %d %d %d %s %d %s %d %d %s %d %s", be->bp_fe_id,
                 simplified, be->bp_enabled, be->bp_disp, be->bp_type,
                 (int)strlen(st_file), st_file, (int)strlen(st_func), st_func,
                           be->bp_loc.loc_line, (int)strlen(st_exp), st_exp,
                           strlen(threadId), threadId);
    }
    return stres;
}


static char *EventSigToSocProto(dbgev ev, dbgsig_t se) {
    const char* sig = StrSoc(se.sig_type);
    const char* line = StrSoc(se.sig_line);

    // get the frame
    char* frame = EventFrameToSocProtoL(se.sig_frame);

    char* stres = MakeSocStr(ev, "%d %s %d %s %d %s", (int)strlen(sig), sig,
             (int)strlen(line), line, (int)strlen(frame), frame);
    Free(frame);
    return stres;
}

static char *EventStepToSocProto(dbgev ev, const dbgstep_t &step) {
    char *stres = EventFrameToSocProtoL(step.step_frame);
    char* wrappedStr = MakeSocStr(ev, "%d %s %d", strlen(stres), stres, step.step_frame_changed);
    Free(stres);
    return wrappedStr;
}

static char *EventFrameToSocProtoL(const dbgframe_t *fe) {
    std::ostringstream os;
    const char* s = " ";

    for (auto tf=fe; tf; tf=tf->getNextThreadFrame()) {
        auto newThread = 1;

        for (auto f=tf; f; f = f->getNext()) {
            const auto &loc = f->getLoc();
            const char* file = StrSoc(loc.loc_file);
            const char* func = StrSoc(loc.loc_func);
            const char* addr = StrSoc(loc.loc_addr);
            const char* threads = StrSoc(f->getThreads());

            // append the next frame
            os << f->getLevel() << s << strlen(file) << s << file << s
               << strlen(func) << s << func << s << strlen(addr) << s << addr << s
               << loc.loc_line << s << newThread << s << strlen(threads) << s << threads;

            newThread = 0;
        }
    }
    return strdup(os.str().c_str());
}

static char *EventCheckpointToSocProto(dbgev ev, const dbgcheckpoint_t *ce) {
    char* loc = EventLocToSocProtoL(&ce->location);
    char* wrapped = MakeSocStr(ev, "%lu %d %s", ce->id, (int)strlen(loc), loc);
    free(loc);
    return wrapped;
}

static char *EventFrameToSocProto(dbgev ev, const dbgframe_t *fe) {
    char *stres = EventFrameToSocProtoL(fe);
    char* wrappedStr = MakeSocStr(ev, "%s", stres);
    Free(stres);
    return wrappedStr;
}

static char *EventLocToSocProtoL(const dbgloc_t *le) {

    if (le == (dbgloc_t *)NULL) return strdup("");

    const char* file = StrSoc(le->loc_file);
    const char* func = StrSoc(le->loc_func);
    const char* addr = StrSoc(le->loc_addr);

    char *stres;
    Asprintf(&stres, "%d %s %d %s %d %s %d", (int)strlen(file), file,
             (int)strlen(func), func, (int)strlen(addr), addr, le->loc_line);

    return stres;
}

static char *EventVarsToSocProto(dbgev ev, dbgvars_t *ve) {
    char *stres = strdup("");

    for (; ve; ve = ve->var_next) {
        if (!ve->var_name) {
            continue;
        }

        const char* type = StrSoc(ve->var_type);

        // append  the name and type
        char* tmp = nullptr;;
        Asprintf(&tmp, "%s%d %s %d %s ", stres,
                 (int)strlen(ve->var_name), ve->var_name,
                 (int)strlen(type), type);

        std::swap(tmp, stres);
        Free(tmp);
    }

    char* wrappedStr = MakeSocStr(ev, stres);
    Free(stres);

    return wrappedStr;
}

static char *EventStringToSocProto(dbgev ev, char *te) {
    const char* type = StrSoc(te);
    return MakeSocStr(ev, "%d %s", (int)strlen(type), type);
}

static char *EventListToSocProto(dbgev ev, dbglist_t *le) {
    // we haven't added thread information to the socket protocol because list
    // is usually handled on the front end.
    char *stres = strdup("");
    for (; le; le = le->list_next) {
        const char* line = StrSoc(le->list_line);
        char* tmp = nullptr;
        Asprintf(&tmp, "%s%d %d %s ", stres, le->list_lno, (int)strlen(line), line);
        std::swap(tmp,stres);
        Free(tmp);
    }

    char* wrappedStr = MakeSocStr(ev, stres);
    Free(stres);
    return wrappedStr;
}

static char *EventStatusToSocProto(dbgev ev, dbgstatus_t *se) {
    if (!se) return MakeSocStr(ev, "");

    // Get the location
    char* loc = EventLocToSocProtoL(se->stat_loc);
    char* stres = MakeSocStr(ev, "%d %d %s", se->stat, (int)strlen(loc), loc);
    Free(loc);
    return stres;
}

static char *EventVersToSocProto(dbgev ev, dbgvers_t ve) {
    const char* vers = StrSoc(ve.vers_version);
    const char* arch = StrSoc(ve.vers_arch);
    char* stres = MakeSocStr(ev, "%d %s %d %s", (int)strlen(vers), vers,
                             (int)strlen(arch), arch);
    return stres;
}

static char *EventIntToSocProto(dbgev ev, int pe) {
    return MakeSocStr(ev, "%d", pe);
}

static char *EventSourcesToSocProto(dbgev ev, dbgsources_t *se) {
    char *stres = strdup("");
    for (; se != NULL; se = se->sources_next) {
        const char* file = StrSoc(se->sources_file);

        // append the next source
        char* tmp;
        Asprintf(&tmp, "%s%d %s ", stres, (int)strlen(file), file);

        std::swap(tmp,stres);
        Free(tmp);
    }

    char* wrappedStr = MakeSocStr(ev, stres);
    Free(stres);
    return wrappedStr;
}

/*
** This is the main function that converts an event to network protocol
*/

char* EventToSocProto(dbgevent_t *e)
{
    char *res = nullptr;
    dbgev ev = (e ? e->ev_event : DBGEV_NONE);

    switch (ev) {
    case DBGEV_ERROR:
        res = EventErrorToSocProto(ev, e->ev_err);
        break;

    case DBGEV_NONE:
    case DBGEV_OK:
        // There is no data that goes along with this event
        Asprintf(&res, "%d", ev);
        break;

        // This handles the generic string events
    case DBGEV_SOURCEPATH:
    case DBGEV_TYPE:
    case DBGEV_CMD:
    case DBGEV_THREADINFO:
    case DBGEV_STRING:
        res = EventStringToSocProto(ev, e->ev_string);
        break;

        // This handles the generic int events
    case DBGEV_EXIT:
        res = EventIntToSocProto(ev, e->ev_int);
        break;

    case DBGEV_BPHIT:
        res = EventBPToSocProto(ev, e->ev_bp, 1);
        break;

    case DBGEV_BPINFO:
        res = EventBPToSocProto(ev, e->ev_bp, 0);
        break;

    case DBGEV_BPSET:
        res = EventBPToSocProto(ev, e->ev_bp, 0);
        break;

    case DBGEV_BPDEL:
        res = EventBPToSocProto(ev, e->ev_bp, 1);
        break;

    case DBGEV_SIGNAL:
        res = EventSigToSocProto(ev, e->ev_sig);
        break;

    case DBGEV_STEP:
        res = EventStepToSocProto(ev, e->ev_step);
        break;

    case DBGEV_CHECKPOINT:
        res = EventCheckpointToSocProto(ev, e->ev_checkpoint);
        break;

    case DBGEV_FRAME:
        res = EventFrameToSocProto(ev, e->ev_frame);
        break;

    case DBGEV_HALT:
    case DBGEV_LOC:
        res = EventLocToSocProto(ev, e->ev_loc);
        break;

    case DBGEV_VARS:
        res = EventVarsToSocProto(ev, e->ev_vars);
        break;

    case DBGEV_LIST:
        res = EventListToSocProto(ev, e->ev_list);
        break;

    case DBGEV_DATA:
    case DBGEV_DATA_SLICE:
        res = EventDataToSocProto(ev, e->ev_data);
        break;

    case DBGEV_STAT:
        res = EventStatusToSocProto(ev, e->ev_status);
        break;

    case DBGEV_VERS:
        res = EventVersToSocProto(ev, e->ev_vers);
        break;

    case DBGEV_SOURCES:
        res = EventSourcesToSocProto(ev, e->ev_sources);
        break;
    }

    FreeEvent(e);
    return res;
}


/*
** The following are helper functions for converting events into a string for
** network transfer
*/

static char *EventLocToSocProto(dbgev ev, dbgloc_t *le) {
    if (!le) return MakeSocStr(ev, "");

    const char* file = StrSoc(le->loc_file);
    const char* func = StrSoc(le->loc_func);
    const char* addr = StrSoc(le->loc_addr);

    char* stres = MakeSocStr(ev, "%d %s %d %s %d %s %d", (int)strlen(file), file,
             (int)strlen(func), func, (int)strlen(addr), addr, le->loc_line);
    return stres;
}

static char *EventDataToSocProto(dbgev ev, AIFStream *stream) {
    if (stream == nullptr) return MakeSocStr(ev, "");

    bool isFinal = false;
    auto slice = stream->getNextSlice(isFinal);
    return MakeSocStr(ev, "%s", slice.c_str());
}

/*********************************
** From network proto functions **
*********************************/

/*
** The following are helper functions for converting a network string back into
** an event
*/

static dbgerr_t SocProtoToErr(const char *s) {
    dbgerr_t res;
    char *tmp_str;

    if (s == NULL || *s == '\0') {
        res.err_errno = DBGERR_UNKNOWN;
        res.err_str = strdup("");

        return res;
    }

    // Get the errno
    res.err_errno = (dbgerr)getint(s);

    // Get the errstr
    res.err_str = getstring(s);

    // Don't want to modify the err_str if its length is 0
    if (strlen(res.err_str) == 0) {
        return res;
    }

    // Replace any \\t or \\n in the err_str with \t and \n respectively
    tmp_str = ReplaceStr(res.err_str, "\\t", "\t");
    // Free the old string
    Free(res.err_str);
    // Set the new string
    res.err_str = ReplaceStr(tmp_str, "\\n", "\n");
    // Free the tmp_str
    Free(tmp_str);

    return res;
}

static dbgloc_t *SocProtoToLoc(const char *s) {
    char *tmp_file;

    NULL_GUARD(s);

    // allocate the res
    auto* res = new dbgloc;

    // set the members
    res->loc_file = getstring(s);
    res->loc_func = getstring(s);
    res->loc_addr = getstring(s);
    res->loc_line = getint(s);

    // We should try to resolve the realpath of file on this end. If it fails,
    // we will use the original value.
    if ((tmp_file = realpath(res->loc_file, NULL)) != NULL) {
        // Free the old loc_file
        Free(res->loc_file);
        // Set the new loc_file
        res->loc_file = tmp_file;
    }

    return res;
}

static dbgvars_t *SocProtoToVars(const char *s) {
    dbgvars_t *res = NULL;
    dbgvars_t **vp;

    NULL_GUARD(s);

    vp = &res;

    // Loop until we find a null terminator
    for (; *s != '\0'; s = skipspace(s)) {
        *vp = (dbgvars_t *)Alloc(sizeof(dbgvars_t));
        memset(*vp, 0, sizeof(dbgvars_t));

        (*vp)->var_name = getstring(s);
        (*vp)->var_type = getstring(s);
        (*vp)->var_next = (dbgvars_t *)NULL;

        vp = &(*vp)->var_next;
    }

    return res;
}

static AIFStream *SocProtoToAIFStream(const char *s) {
    return new AIFStream{s, false};
}

static dbglist_t *SocProtoToList(const char *s) {
    dbglist_t *res = NULL;
    dbglist_t **lp;

    NULL_GUARD(s);

    lp = &res;

    // Loop until we find a null terminator
    for (; *s != '\0'; s = skipspace(s)) {
        *lp = (dbglist_t *)Alloc(sizeof(dbglist_t));
        memset(*lp, 0, sizeof(dbglist_t));

        (*lp)->list_lno = getint(s);
        (*lp)->list_line = getstring(s);
        (*lp)->list_threads = nullptr;
        (*lp)->list_next = (dbglist_t *)NULL;

        lp = &(*lp)->list_next;
    }

    return res;
}

static dbgbp_t *SocProtoToBP(const char *s) {
    dbgbp_t *res;
    int id;
    int simple;
    char *tmp_file;

    NULL_GUARD(s);

    // Get the breakpoint id
    id = getint(s);

    // Create the new breakpoint
    res = new dbgbp_t(id,0); // only the front-end id serialized

    // Get value of simplified
    simple = getint(s);

    // simple breakpoints don't contain this information
    if (!simple) {
        res->bp_pending = getint(s);
        res->bp_ignore = getint(s);
        res->bp_hits = getint(s);
        res->bp_stmt = getstring(s);
    }

    res->bp_enabled = getint(s);
    res->bp_disp = (dbgbp_disp)getint(s);
    res->bp_type = (dbgbp_type)getint(s);
    res->bp_loc.loc_file = getstring(s);
    res->bp_loc.loc_func = getstring(s);

    // simple breakpoints don't contain this information
    if (!simple) {
        res->bp_loc.loc_addr = getstring(s);
    }

    res->bp_loc.loc_line = getint(s);
    res->bp_exp = getStdString(s);
    res->bp_threadId = getStdString(s);

    // We should try to resolve the realpath of file on this end. If it fails,
    // we will use the original value.
    if ((tmp_file = realpath(res->bp_loc.loc_file, NULL)) != NULL) {
        // Free the old loc_file
        Free(res->bp_loc.loc_file);
        // Set the new loc_file
        res->bp_loc.loc_file = tmp_file;
    }

    return res;
}

static dbgsig_t SocProtoToSig(const char *s) {
    dbgsig_t res;
    char *fstr;

    if (s == NULL || *s == '\0') {
        res.sig_type = strdup("");
        res.sig_line = strdup("");
        res.sig_frame = NULL;

        return res;
    }

    // get the type
    res.sig_type = getstring(s);

    // get the line
    res.sig_line = getstring(s);

    // get the frame string and convert it to a frame pointer
    fstr = getstring(s);
    res.sig_frame = SocProtoToFrame(fstr);
    Free(fstr);

    return res;
}

static dbgcheckpoint_t *SocProtoToCheckpoint(const char *s) {
    if (s == nullptr) return nullptr;

    auto id = getUnsignedInt(s);

    // get the location string and convert it to a loc pointer
    auto lstr = getstring(s);
    auto loc = SocProtoToLoc(lstr);

    auto res = new dbgcheckpoint_t {
        .id{id},
        .location{std::move(*loc)}
    };

    free(lstr);
    FreeLocation(loc);

    return res;
}

static dbgframe_t *SocProtoToFrame(const char *s) {
    NULL_GUARD(s);

    dbgframe_t *res = nullptr, *tail = nullptr, *threadTail = nullptr;

    // Loop until we find a null terminator
    for (; *s != '\0'; s = skipspace(s)) {
        auto loc = dbgloc_t{};
        auto level = getint(s);
        loc.loc_file = getstring(s);
        loc.loc_func = getstring(s);
        loc.loc_addr = getstring(s);
        loc.loc_line = getint(s);

        auto newThread = getint(s);
        auto threads = getstring(s);

        // We should try to resolve the realpath of file on this end. If it
        // fails, we will use the original value.
        if (auto tmp_file = realpath(loc.loc_file, nullptr)) {
            std::swap(tmp_file,  loc.loc_file);
            free(tmp_file);
        }

        auto frame = new dbgframe_t(level, loc, nullptr, threads[0] ? threads : nullptr, nullptr);

        free(threads);

        if (!tail) {
            res = tail = threadTail = frame;
        } else if (newThread) {
            threadTail->setNextThreadFrame(frame);
            threadTail = tail = frame;
        } else {
            tail->setNext(frame);
            tail = frame;
        }
    }

    return res;
}

static dbgstep_t SocProtoToStep(const char *s) {
    dbgstep_t res{false, nullptr};

    if (!s || *s == '\0') {
        return res;
    }

    auto fstr = getstring(s);
    res.step_frame = SocProtoToFrame(fstr);
    free(fstr);

    auto changed = getint(s);
    res.step_frame_changed = static_cast<bool>(changed);

    return res;
}

static char *SocProtoToString(const char *s) {
    char *res;

    if (s == NULL || *s == '\0') {
        return strdup("");
    }

    res = getstring(s);

    return res;
}

static dbgstatus_t *SocProtoToStatus(const char *s) {
    dbgstatus_t *res;
    char *lstr;

    NULL_GUARD(s);

    // allocate the res
    res = (dbgstatus_t *)Alloc(sizeof(dbgstatus_t));

    res->stat = (dbgstat)getint(s);

    // get the location string and convert it to a loc pointer
    lstr = getstring(s);
    res->stat_loc = SocProtoToLoc(lstr);
    Free(lstr);

    return res;
}

static dbgvers_t SocProtoToVers(const char *s) {
    dbgvers_t res;

    if (s == NULL || *s == '\0') {
        res.vers_version = strdup("");
        res.vers_arch = strdup("");
        return res;
    }

    // Get the version string
    res.vers_version = getstring(s);

    // Get the arch string
    res.vers_arch = getstring(s);

    return res;
}

static int SocProtoToInt(const char *s) {
    int res;

    if (s == NULL || *s == '\0') {
        return 0;
    }

    res = getint(s);

    return res;
}

static dbgsources_t *SocProtoToSources(const char *s) {
    dbgsources_t *res = NULL;
    dbgsources_t **sp;

    NULL_GUARD(s);

    sp = &res;

    // Loop until we find a null terminator
    for (; *s != '\0'; s = skipspace(s)) {
        *sp = (dbgsources_t *)Alloc(sizeof(dbgsources_t));
        memset(*sp, 0, sizeof(dbgsources_t));

        (*sp)->sources_file = getstring(s);
        (*sp)->sources_next = (dbgsources_t *)NULL;

        sp = &(*sp)->sources_next;
    }

    return res;
}

/*
** This is the main function that converts a network protocol string back into
** an event.
*/

dbgevent_t *SocProtoToEvent(const char *s) {
    dbgevent_t *e;
    dbgev ev;
    char *evdata;
    // The following are for non-pointer types
    // FIXME: At some point these should be changed to be pointers to make
    // a sane interface.
    dbgerr_t st_err;
    dbgsig_t st_sig;
    dbgvers_t st_vers;

    if (s == NULL) {
        return (dbgevent_t *)NULL;
    }

    s = skipspace(s);

    if (*s == '\0') {
        return (dbgevent_t *)NULL;
    }

    // We expect the first entry to be an int which is the dbgev
    ev = (dbgev)getint(s);

    // Create the event
    e = NewEvent(ev);

    switch (ev) {
        case DBGEV_NONE:
            FreeEvent(e);
            return (dbgevent_t *)NULL;

        case DBGEV_OK:
            // OK has no data
            break;

        case DBGEV_ERROR:
            // Get the string and convert to dbgerr_t
            evdata = getstring(s);
            st_err = SocProtoToErr(evdata);
            Free(evdata);
            // Set the event members
            e->ev_errno = st_err.err_errno;
            e->ev_errstr = st_err.err_str;
            break;

        // This handles the generic string events
        case DBGEV_SOURCEPATH:
        case DBGEV_TYPE:
        case DBGEV_CMD:
        case DBGEV_THREADINFO:
        case DBGEV_STRING:
            // Get the string and convert to char * and set directly
            evdata = getstring(s);
            e->ev_string = SocProtoToString(evdata);
            Free(evdata);
            break;

        // This handles the generic int events
        case DBGEV_EXIT:
            // Get the string and convert to int and set directly
            evdata = getstring(s);
            e->ev_int = SocProtoToInt(evdata);
            Free(evdata);
            break;

        case DBGEV_BPINFO:
        case DBGEV_BPSET:
        case DBGEV_BPDEL:
        case DBGEV_BPHIT:
            // Get the string and convert to dbgbp_t * and set directly
            evdata = getstring(s);
            e->ev_bp = SocProtoToBP(evdata);
            Free(evdata);
            break;

        case DBGEV_SIGNAL:
            // Get the string and convert to dbgsig_t
            evdata = getstring(s);
            st_sig = SocProtoToSig(evdata);
            Free(evdata);
            // Set the sig members
            e->ev_sig_type = st_sig.sig_type;
            e->ev_sig_line = st_sig.sig_line;
            e->ev_sig_frame = st_sig.sig_frame;
            break;

        case DBGEV_HALT:
        case DBGEV_LOC:
            // get the string and convert to dbgloc_t * and set directly
            evdata = getstring(s);
            e->ev_loc = SocProtoToLoc(evdata);
            Free(evdata);
            break;

        case DBGEV_STEP: {
            evdata = getstring(s);
            e->ev_step = SocProtoToStep(evdata);
            free(evdata);
            break;
        }

        case DBGEV_STAT:
            // Get the string and convert to dbgstatus_t * and set directly
            evdata = getstring(s);
            e->ev_status = SocProtoToStatus(evdata);
            Free(evdata);
            break;

        case DBGEV_CHECKPOINT: {
            evdata = getstring(s);
            e->ev_checkpoint = SocProtoToCheckpoint(evdata);
            free(evdata);
            break;
        }

        case DBGEV_FRAME:
            // Get the string and convert to dbgframe_t * and set directly
            evdata = getstring(s);
            e->ev_frame = SocProtoToFrame(evdata);
            Free(evdata);
            break;

        case DBGEV_VARS:
            // Get the string and convert to dbgvars_t * and set directly
            evdata = getstring(s);
            e->ev_vars = SocProtoToVars(evdata);
            Free(evdata);
            break;

        case DBGEV_LIST:
            // Get the string and convert to dbglist_t * and set directly
            evdata = getstring(s);
            e->ev_list = SocProtoToList(evdata);
            Free(evdata);
            break;

        case DBGEV_DATA:
        case DBGEV_DATA_SLICE:
            // Get the string and convert to AIF * and set directly
            evdata = getstring(s);
            e->ev_data = SocProtoToAIFStream(evdata);
            Free(evdata);
            break;

        case DBGEV_VERS:
            // Get the string and conver to dbgvers_t
            evdata = getstring(s);
            st_vers = SocProtoToVers(evdata);
            Free(evdata);
            // Set vers
            e->ev_version = st_vers.vers_version;
            e->ev_arch = st_vers.vers_arch;
            break;

        case DBGEV_SOURCES:
            // Get the string and convert to dbgsources_t * and set directly
            evdata = getstring(s);
            e->ev_sources = SocProtoToSources(evdata);
            Free(evdata);
            break;
    }

    return e;
}
