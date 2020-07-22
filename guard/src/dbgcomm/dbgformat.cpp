/*********************************************************************************\
 * network format routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2012-2020 Cray Inc.
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
#include <utility>
#include <sstream>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "aif.h"
#include "dbg.h"
#include "defs.h"
#include "useful.h"

// Static prototypes
static char *getstring(char **);
static int getint(char **);

static inline const char* StrSoc(const char* s) {
    return s ? s : "";
}

static char *EventBPToSocProto(dbgev, dbgbp_t *, int);
static char *EventSigToSocProto(dbgev, dbgsig_t);
static char *EventStepToSocProto(dbgev, dbgstep_t);
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
static char *EventLocToSocProtoL(dbgloc_t *);

static dbgerr_t SocProtoToErr(char *);
static dbgloc_t *SocProtoToLoc(char *);
static dbgvars_t *SocProtoToVars(char *);
static dbglist_t *SocProtoToList(char *);
static dbgbp_t *SocProtoToBP(char *);
static dbgsig_t SocProtoToSig(char *);
static dbgframe_t *SocProtoToFrame(char *);
static char *SocProtoToString(char *);
static dbgstatus_t *SocProtoToStatus(char *);
static dbgvers_t SocProtoToVers(char *);
static int SocProtoToInt(char *);
static dbgsources_t *SocProtoToSources(char *);

char *AIFToSocProto(dbgev, AIF *a);

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
        Asprintf(&str, "%d %d %s", int(ev), len, buf);
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

static int getint(char **res_buf) {
    int digit;
    char *endptr = NULL;

    *res_buf = skipspace(*res_buf);

    // Get the digit
    digit = (int)strtol(*res_buf, &endptr, 10);

    *res_buf = endptr;

    return digit;
}

static char *getstring(char **res_buf) {
    int len;
    char *endptr = NULL;
    char *res;

    *res_buf = skipspace(*res_buf);

    // First get the length of the string - Note that this can be 0
    len = (int)strtol(*res_buf, &endptr, 10);

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
    *res_buf = endptr + len;

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
    const char* st_exp = StrSoc(be->bp_exp);
    const char* threadId = StrSoc(be->bp_threadId);

    char *stres = nullptr;
    if (!simplified) {
        const char* st_stmt = StrSoc(be->bp_stmt);
        const char* st_addr = StrSoc(be->bp_loc.loc_addr);

        stres = MakeSocStr(ev,
                 "%d %d %d %d %d %d %s %d %d %d %d %s %d %s %d %s %d %d %s %d %s",
                 be->bp_id, simplified, be->bp_pending, be->bp_ignore, be->bp_hits,
                 (int)strlen(st_stmt), st_stmt, be->bp_enabled, be->bp_disp,
                 be->bp_type, (int)strlen(st_file), st_file,
                 (int)strlen(st_func), st_func, (int)strlen(st_addr), st_addr,
                           be->bp_loc.loc_line, (int)strlen(st_exp), st_exp,
                           strlen(threadId), threadId);
    } else {
        stres = MakeSocStr(ev,"%d %d %d %d %d %d %s %d %s %d %d %s %d %s", be->bp_id,
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

static char *EventStepToSocProto(dbgev ev, dbgstep_t se) {
    const char* line = StrSoc(se.step_line);
    char* frame = EventFrameToSocProtoL(se.step_frame);
    char* stres = MakeSocStr(ev, "%d %d %s %d %s", se.step_lno,
                             (int)strlen(line), line,
                             (int)strlen(frame), frame);
    Free(frame);
    return stres;
}

static char *EventFrameToSocProtoL(const dbgframe_t *fe) {
    std::ostringstream os;
    const char* s = " ";

    for (auto tf=fe; tf; tf=tf->getNextThreadFrame()) {
        auto newThread = 1;
        
        for (auto f=tf; f; f = f->getNext()) {
            auto loc = f->getLoc();
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

static char *EventFrameToSocProto(dbgev ev, const dbgframe_t *fe) {
    char *stres = EventFrameToSocProtoL(fe);
    char* wrappedStr = MakeSocStr(ev, "%s", stres);
    Free(stres);
    return wrappedStr;
}

static char *EventLocToSocProtoL(dbgloc_t *le) {

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


char *AIFToSocProto(AIF *a) {
    int fl;
    char *aif_format;
    int dl;
    char *aif_data;
    char *stres;
    int i;
    char *src;
    char *dst;
    char ch;

    if (a == NULL) return strdup("");

    // Can directly print the format
    fl = strlen(AIF_FORMAT(a));
    aif_format = AIF_FORMAT(a);

    // Convert the data to string format
    dl = AIF_LEN(a);
    if (dl <= 0) {
        aif_data = NULL;
    } else {
        aif_data = (char *)Alloc(2 * dl + 1);
        src = AIF_DATA(a);
        dst = aif_data;
        for (i = 0; i < dl; i++) {
            ch = *src++;
            // Need to ensure that the NULL char doesn't appear, hence the '0'
            // offset
            // FIXME: This seems messed up and a dumb way of transferring data.
            // Someone needs to investigate if this is actually necessary...
            *dst++ = ((ch >> 4) & 0xf) + '0';
            *dst++ = (ch & 0xf) + '0';
        }
        *dst = '\0';
    }

    if (fl <= 0 && dl <= 0) {
        Asprintf(&stres, "%d %d", fl, dl);
    } else if (fl <= 0 && dl > 0) {
        Asprintf(&stres, "%d %d %d %s", fl, dl, strlen(aif_data), aif_data);
    } else if (fl > 0 && dl <= 0) {
        Asprintf(&stres, "%d %s %d", fl, aif_format, dl);
    } else if (fl > 0 && dl > 0) {
        Asprintf(&stres, "%d %s %d %d %s", fl, aif_format, dl, strlen(aif_data),
                 aif_data);
    }

    if (aif_data != NULL) Free(aif_data);

    return stres;
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
    AIF* aif = nullptr;
    bool isFinal = false;

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
        aif = e->ev_data->getNextSlice(isFinal);
        res = AIFToSocProto(ev, aif);
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

char *AIFToSocProto(dbgev ev, AIF *a) {
    if (a == NULL) return MakeSocStr(ev, "");

    // Can directly print the format
    int fl = strlen(AIF_FORMAT(a));
    char* aif_format = AIF_FORMAT(a);

    // Convert the data to string format
    int dl = AIF_LEN(a);
    char *aif_data = nullptr;
    if (dl > 0) {
        aif_data = (char *)Alloc(2 * dl + 1);
        char* src = AIF_DATA(a);
        char* dst = aif_data;
        for (int i = 0; i < dl; i++) {
            char ch = *src++;
            // Need to ensure that the NULL char doesn't appear, hence the '0'
            // offset
            // FIXME: This seems messed up and a dumb way of transferring data.
            // Someone needs to investigate if this is actually necessary...
            // !!!!!FIXME FIXME FIXME - HOLY MOTHER IF GOD IT IS **DOUBLING** THE
            // SIZE OF THE DATA!!!!!!!!!!!!
            *dst++ = ((ch >> 4) & 0xf) + '0';
            *dst++ = (ch & 0xf) + '0';
        }
        *dst = '\0';
    }

    char *stres = nullptr;
    if (fl <= 0 && dl <= 0) {
        stres = MakeSocStr(ev, "%d %d", fl, dl);
    } else if (fl <= 0 && dl > 0) {
        stres = MakeSocStr(ev, "%d %d %d %s", fl, dl, strlen(aif_data), aif_data);
    } else if (fl > 0 && dl <= 0) {
        stres = MakeSocStr(ev, "%d %s %d", fl, aif_format, dl);
    } else if (fl > 0 && dl > 0) {
        stres = MakeSocStr(ev, "%d %s %d %d %s", fl, aif_format, dl, strlen(aif_data),
                 aif_data);
    }

    if (aif_data) Free(aif_data);

    return stres;
}

OSocStream::OSocStream(dbgcmd cmd) : m_cmd(cmd) {}

OSocStream& OSocStream::operator<<(int i) { m_str << " " << i; return *this; }
OSocStream& OSocStream::operator<<(unsigned int i) { m_str << " " << i; return *this; }
OSocStream& OSocStream::operator<<(long i) { m_str << " " << i; return *this; }
OSocStream& OSocStream::operator<<(size_t i) { m_str << " " << i; return *this; }


OSocStream& OSocStream::operator<<(const char*s) {
    if (s) {
        m_str << " " << strlen(s) << " " << s;
    } else {
        m_str << " 0 ";
    }
    return *this;
}
OSocStream& OSocStream::operator<<(const std::string& s) {
    m_str << " " << s.size() << " " << s;
    return *this;
}
OSocStream& OSocStream::operator<<(const std::pair<int,char**>& args) {
    int argc = args.first;
    *this << argc;
    for (int i=0; i<argc; ++i) {
        *this << args.second[i];
    }
    return *this;
}
OSocStream& OSocStream::operator<<(AIF* val) {

    auto aifStr = AIFToSocProto(val);
    *this << aifStr;
    free(aifStr);

    return *this;
}

std::string OSocStream::str() {
    auto s = m_str.str();
    m_str.str("");
    m_str << m_cmd;
    if (!s.empty()) {
        // the commands are/were sent like a single string; that is preceded by the
        // length as usual.   I don't quite want to call the normal << operator because s has
        // an extra space on the front.
        m_str << " " << s.size()-1 << s;
    }
    // else, commands with no arguments just send the command id.

    return m_str.str();
}


/*********************************
** From network proto functions **
*********************************/

/*
** The following are helper functions for converting a network string back into
** an event
*/

static dbgerr_t SocProtoToErr(char *s) {
    dbgerr_t res;
    char *tmp_str;

    if (s == NULL || *s == '\0') {
        res.err_errno = DBGERR_UNKNOWN;
        res.err_str = strdup("");

        return res;
    }

    // Get the errno
    res.err_errno = (dbgerr)getint(&s);

    // Get the errstr
    res.err_str = getstring(&s);

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

static dbgloc_t *SocProtoToLoc(char *s) {
    dbgloc_t *res;
    char *tmp_file;

    NULL_GUARD(s);

    // allocate the res
    res = (dbgloc_t *)Alloc(sizeof(dbgloc_t));

    // set the members
    res->loc_file = getstring(&s);
    res->loc_func = getstring(&s);
    res->loc_addr = getstring(&s);
    res->loc_line = getint(&s);

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

static dbgvars_t *SocProtoToVars(char *s) {
    dbgvars_t *res = NULL;
    dbgvars_t **vp;

    NULL_GUARD(s);

    vp = &res;

    // Loop until we find a null terminator
    for (; *s != '\0'; s = skipspace(s)) {
        *vp = (dbgvars_t *)Alloc(sizeof(dbgvars_t));
        memset(*vp, 0, sizeof(dbgvars_t));

        (*vp)->var_name = getstring(&s);
        (*vp)->var_type = getstring(&s);
        (*vp)->var_next = (dbgvars_t *)NULL;

        vp = &(*vp)->var_next;
    }

    return res;
}

AIF *SocProtoToAIF(char *s) {
    AIF *res;
    char *aif_fmt;
    int dl;
    char *aif_data;
    char *dptr;
    char *aptr;
    int i;

    NULL_GUARD(s);

    res = (AIF *)Alloc(sizeof(AIF));

    // get the format
    aif_fmt = getstring(&s);

    // set the format
    if (*aif_fmt != '\0') {
        AIF_FORMAT(res) = aif_fmt;
    } else {
        Free(aif_fmt);
        AIF_FORMAT(res) = (char *)NULL;
    }

    // get the length of the actual aif data
    dl = getint(&s);
    AIF_LEN(res) = dl;

    if (dl <= 0) {
        AIF_DATA(res) = (char *)NULL;
    } else {
        // Get the aif_data string
        aif_data = getstring(&s);

        if (*aif_data == '\0') {
            // Data is empty, so reset to empty
            AIF_LEN(res) = 0;
            AIF_DATA(res) = (char *)NULL;
        } else {
            AIF_DATA(res) = (char *)Alloc(dl);
            aptr = AIF_DATA(res);
            dptr = aif_data;
            // FIXME: This seems messed up and a dumb way of transferring data.
            // Someone needs to investigate if this is actually necessary...
            for (i = 0; i < dl; i++) {
                *aptr = (*dptr - '0') * 0x10 + (*(dptr + 1) - '0');
                aptr++;
                dptr += 2;
            }
        }

        Free(aif_data);
    }

    return res;
}

static dbglist_t *SocProtoToList(char *s) {
    dbglist_t *res = NULL;
    dbglist_t **lp;

    NULL_GUARD(s);

    lp = &res;

    // Loop until we find a null terminator
    for (; *s != '\0'; s = skipspace(s)) {
        *lp = (dbglist_t *)Alloc(sizeof(dbglist_t));
        memset(*lp, 0, sizeof(dbglist_t));

        (*lp)->list_lno = getint(&s);
        (*lp)->list_line = getstring(&s);
        (*lp)->list_threads = nullptr;
        (*lp)->list_next = (dbglist_t *)NULL;

        lp = &(*lp)->list_next;
    }

    return res;
}

static dbgbp_t *SocProtoToBP(char *s) {
    dbgbp_t *res;
    int id;
    int simple;
    char *tmp_file;

    NULL_GUARD(s);

    // Get the breakpoint id
    id = getint(&s);

    // Create the new breakpoint
    res = NewBP(id);

    // Get value of simplified
    simple = getint(&s);

    // simple breakpoints don't contain this information
    if (!simple) {
        res->bp_pending = getint(&s);
        res->bp_ignore = getint(&s);
        res->bp_hits = getint(&s);
        res->bp_stmt = getstring(&s);
    }

    res->bp_enabled = getint(&s);
    res->bp_disp = (dbgbp_disp)getint(&s);
    res->bp_type = (dbgbp_type)getint(&s);
    res->bp_loc.loc_file = getstring(&s);
    res->bp_loc.loc_func = getstring(&s);

    // simple breakpoints don't contain this information
    if (!simple) {
        res->bp_loc.loc_addr = getstring(&s);
    }

    res->bp_loc.loc_line = getint(&s);
    res->bp_exp = getstring(&s);

    char* t = getstring(&s);
    if (t && !t[0]) {
        free(t);
        t = nullptr;
    }
    res->bp_threadId = t;

    res->bp_next = (dbgbp_t *)NULL;

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

static dbgsig_t SocProtoToSig(char *s) {
    dbgsig_t res;
    char *fstr;

    if (s == NULL || *s == '\0') {
        res.sig_type = strdup("");
        res.sig_line = strdup("");
        res.sig_frame = NULL;

        return res;
    }

    // get the type
    res.sig_type = getstring(&s);

    // get the line
    res.sig_line = getstring(&s);

    // get the frame string and convert it to a frame pointer
    fstr = getstring(&s);
    res.sig_frame = SocProtoToFrame(fstr);
    Free(fstr);

    return res;
}

static dbgframe_t *SocProtoToFrame(char *s) {
    NULL_GUARD(s);

    dbgframe_t *res = nullptr, *tail = nullptr, *threadTail = nullptr;

    // Loop until we find a null terminator
    for (; *s != '\0'; s = skipspace(s)) {
        auto loc = dbgloc_t{};
        auto level = getint(&s);
        loc.loc_file = getstring(&s);
        loc.loc_func = getstring(&s);
        loc.loc_addr = getstring(&s);
        loc.loc_line = getint(&s);

        auto newThread = getint(&s);
        auto threads = getstring(&s);

        // We should try to resolve the realpath of file on this end. If it
        // fails, we will use the original value.
        if (auto tmp_file = realpath(loc.loc_file, nullptr)) {
            std::swap(tmp_file,  loc.loc_file);
            free(tmp_file);
        }

        auto frame = new dbgframe_t(level, loc, nullptr, threads[0] ? threads : nullptr, nullptr);

        free(loc.loc_file);
        free(loc.loc_func);
        free(loc.loc_addr);
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

dbgstep_t SocProtoToStep(char *s) {
    // FIXME: No way to check for error.......
    dbgstep_t res;
    char *fstr;

    if (s == NULL || *s == '\0') {
        res.step_lno = 0;
        res.step_line = strdup("");
        res.step_frame = NULL;

        return res;
    }

    // get the lno
    res.step_lno = getint(&s);

    // get the line
    res.step_line = getstring(&s);

    // get the frame string
    fstr = getstring(&s);
    res.step_frame = SocProtoToFrame(fstr);
    Free(fstr);

    return res;
}

static char *SocProtoToString(char *s) {
    char *res;

    if (s == NULL || *s == '\0') {
        return strdup("");
    }

    res = getstring(&s);

    return res;
}

static dbgstatus_t *SocProtoToStatus(char *s) {
    dbgstatus_t *res;
    char *lstr;

    NULL_GUARD(s);

    // allocate the res
    res = (dbgstatus_t *)Alloc(sizeof(dbgstatus_t));

    res->stat = (dbgstat)getint(&s);

    // get the location string and convert it to a loc pointer
    lstr = getstring(&s);
    res->stat_loc = SocProtoToLoc(lstr);
    Free(lstr);

    return res;
}

static dbgvers_t SocProtoToVers(char *s) {
    dbgvers_t res;

    if (s == NULL || *s == '\0') {
        res.vers_version = strdup("");
        res.vers_arch = strdup("");
        return res;
    }

    // Get the version string
    res.vers_version = getstring(&s);

    // Get the arch string
    res.vers_arch = getstring(&s);

    return res;
}

static int SocProtoToInt(char *s) {
    int res;

    if (s == NULL || *s == '\0') {
        return 0;
    }

    res = getint(&s);

    return res;
}

static dbgsources_t *SocProtoToSources(char *s) {
    dbgsources_t *res = NULL;
    dbgsources_t **sp;

    NULL_GUARD(s);

    sp = &res;

    // Loop until we find a null terminator
    for (; *s != '\0'; s = skipspace(s)) {
        *sp = (dbgsources_t *)Alloc(sizeof(dbgsources_t));
        memset(*sp, 0, sizeof(dbgsources_t));

        (*sp)->sources_file = getstring(&s);
        (*sp)->sources_next = (dbgsources_t *)NULL;

        sp = &(*sp)->sources_next;
    }

    return res;
}

/*
** This is the main function that converts a network protocol string back into
** an event.
*/

dbgevent_t *SocProtoToEvent(char *s) {
    dbgevent_t *e;
    dbgev ev;
    char *evdata;
    // The following are for non-pointer types
    // FIXME: At some point these should be changed to be pointers to make
    // a sane interface.
    dbgerr_t st_err;
    dbgsig_t st_sig;
    dbgstep_t st_step;
    dbgvers_t st_vers;

    if (s == NULL) {
        return (dbgevent_t *)NULL;
    }

    s = skipspace(s);

    if (*s == '\0') {
        return (dbgevent_t *)NULL;
    }

    // We expect the first entry to be an int which is the dbgev
    ev = (dbgev)getint(&s);

    // Create the event
    e = NewEvent(ev);

    switch (ev) {
        case DBGEV_NONE:
            FreeEvent(e);
            return (dbgevent_t *)NULL;

        case DBGEV_OK:
            // OK has no data
            e = NewEvent(ev);
            break;

        case DBGEV_ERROR:
            // Get the string and convert to dbgerr_t
            evdata = getstring(&s);
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
            // Get the string and convert to char * and set directly
            evdata = getstring(&s);
            e->ev_string = SocProtoToString(evdata);
            Free(evdata);
            break;

        // This handles the generic int events
        case DBGEV_EXIT:
            // Get the string and convert to int and set directly
            evdata = getstring(&s);
            e->ev_int = SocProtoToInt(evdata);
            Free(evdata);
            break;

        case DBGEV_BPINFO:
        case DBGEV_BPSET:
        case DBGEV_BPDEL:
        case DBGEV_BPHIT:
            // Get the string and convert to dbgbp_t * and set directly
            evdata = getstring(&s);
            e->ev_bp = SocProtoToBP(evdata);
            Free(evdata);
            break;

        case DBGEV_SIGNAL:
            // Get the string and convert to dbgsig_t
            evdata = getstring(&s);
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
            evdata = getstring(&s);
            e->ev_loc = SocProtoToLoc(evdata);
            Free(evdata);
            break;

        case DBGEV_STEP:
            // Get the string and convert to dbgstep_t
            evdata = getstring(&s);
            st_step = SocProtoToStep(evdata);
            Free(evdata);
            // Set step members
            e->ev_step_lno = st_step.step_lno;
            e->ev_step_line = st_step.step_line;
            e->ev_step_frame = st_step.step_frame;
            break;

        case DBGEV_STAT:
            // Get the string and convert to dbgstatus_t * and set directly
            evdata = getstring(&s);
            e->ev_status = SocProtoToStatus(evdata);
            Free(evdata);
            break;

        case DBGEV_FRAME:
            // Get the string and convert to dbgframe_t * and set directly
            evdata = getstring(&s);
            e->ev_frame = SocProtoToFrame(evdata);
            Free(evdata);
            break;

        case DBGEV_VARS:
            // Get the string and convert to dbgvars_t * and set directly
            evdata = getstring(&s);
            e->ev_vars = SocProtoToVars(evdata);
            Free(evdata);
            break;

        case DBGEV_LIST:
            // Get the string and convert to dbglist_t * and set directly
            evdata = getstring(&s);
            e->ev_list = SocProtoToList(evdata);
            Free(evdata);
            break;

        case DBGEV_DATA:
        case DBGEV_DATA_SLICE:
            // Get the string and convert to AIF * and set directly
            evdata = getstring(&s);
            e->ev_data = new AIFStream(SocProtoToAIF(evdata));
            Free(evdata);
            break;

        case DBGEV_VERS:
            // Get the string and conver to dbgvers_t
            evdata = getstring(&s);
            st_vers = SocProtoToVers(evdata);
            Free(evdata);
            // Set vers
            e->ev_version = st_vers.vers_version;
            e->ev_arch = st_vers.vers_arch;
            break;

        case DBGEV_SOURCES:
            // Get the string and convert to dbgsources_t * and set directly
            evdata = getstring(&s);
            e->ev_sources = SocProtoToSources(evdata);
            Free(evdata);
            break;
    }

    return e;
}

