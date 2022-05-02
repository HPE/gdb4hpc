/*********************************************************************************\
 * Server commands dispatch routines
 *
 * Copyright (c) 1996-2012 by Monash Uni.
 *
 * Copyright 2013,2020-2022 Hewlett Packard Enterprise Development LP.
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

#include "dbg.h"
#include "dbgfn.h"
#include "dbgcmd.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_proto.h"
#include "dbgsrv_util.h"
#include "useful.h"

#include <string>
#include <memory>

namespace {
// Holder struct for argc/argv.
// (Doing a one-off as the stuff in useful/args.h doesn't manage memory)
struct CmdArgs {
    int argc;
    char** argv;
    CmdArgs() : argc(0), argv(nullptr) {}
    ~CmdArgs();
    CmdArgs(const CmdArgs&) = delete;
    CmdArgs& operator=(const CmdArgs&) = delete;
};

struct OptionalString {
    std::string str;
    operator const char*() { return str.empty() ? nullptr : str.c_str(); }
};

// Socket prototype string reader.
class ISocStream {
    dbgcmd m_cmd;
    std::istringstream m_str;
public:
    // Prepare to read the socket string
    ISocStream(const std::string& str);

    // Get the index of the command
    dbgcmd getCmd() { return m_cmd; }

    // Get the rest of the commands; the caller just has to
    // declare the arguments with the correct type, and it will
    // fill it in.
    // Fixme - add error checking here.
    template<typename ...Targs>
    void get(Targs&&... args) {
        (void) (*this >> ... >> std::forward<Targs>(args));
    };

private:
    // read typed arguments from the string.  When a new type of data is
    // passed through the socket, we need to add the appopriate data type here.
    ISocStream& operator>>(int& i);
    ISocStream& operator>>(unsigned int& ui);
    ISocStream& operator>>(bool& b);
    ISocStream& operator>>(size_t& sz);
    ISocStream& operator>>(std::string& s);
    ISocStream& operator>>(OptionalString& s);
    ISocStream& operator>>(CmdArgs& args);
};
}

std::unique_ptr<char, decltype(&std::free)> dbgsrv_dispatch_cmds(const char *msg, const dbgcomm_info *ci) {

    DVPRINT("dbgsrv_dispatch_cmds received: %s\n", msg);

    // ISocStream manages the parsing
    ISocStream is(msg);

    dbgcmd cmd = is.getCmd();

    // switch on the command
    dbgevent_t *e = nullptr;

    switch (cmd) {
        case DBGCMD_INVOKE: {
            int numbe;
            is.get(numbe);
            e = DbgInvokeNetwork(numbe);
            break;
        }

        case DBGCMD_STARTUP: {
            e = DbgStartup();
            break;
        }

        case DBGCMD_SETLINEBREAK: {
            int allowPending; std::string file; int line; std::string threadSet;
            DVPRINT("std\n");
            is.get(allowPending, file, line, threadSet);
            e = DbgSetLineBreak(allowPending, file.c_str(), line, threadSet.c_str(), ci);
            break;
        }

        case DBGCMD_SETFUNCBREAK: {
            int allowPending; std::string file, func, threadSet;
            is.get(allowPending, file, func, threadSet);
            e = DbgSetFuncBreak(allowPending, file.c_str(), func.c_str(),
                                threadSet.c_str(), ci);
            break;
        }

        case DBGCMD_SETWATCH: {
            int type; std::string expr;
            is.get(type, expr);
            e = DbgSetWatch(type, expr.c_str(), ci);
            break;
        }

        case DBGCMD_SETCATCH: {
            std::string eventType;
            std::string regex;
            is.get(eventType);
            is.get(regex);
            e = DbgSetCatch(eventType, regex, ci);
            break;
        }

        case DBGCMD_SETCHECKPOINT: {
            e = DbgSetCheckpoint();
            break;
        }

        case DBGCMD_RESTORECHECKPOINT: {
            unsigned int id;
            is.get(id);
            e = DbgRestoreCheckpoint(id);
            break;
        }

        case DBGCMD_DELETECHECKPOINT: {
            unsigned int id;
            is.get(id);
            e = DbgDeleteCheckpoint(id);
            break;
        }

        case DBGCMD_DELETEBREAK: {
            int bpid;
            is.get(bpid);
            e = DbgDeleteBreak(bpid);
            break;
        }

        case DBGCMD_DISABLEBREAK: {
            int bpid;
            is.get(bpid);
            e = DbgDisableBreak(bpid);
            break;
        }

        case DBGCMD_ENABLEBREAK: {
            int bpid; bool once, del;
            is.get(bpid, once, del);
            e = DbgEnableBreak(bpid, once, del);
            break;
        }

        case DBGCMD_LASTBREAK: {
            e = DbgLastBreak();
            break;
        }

        case DBGCMD_LIST: {
            std::string expr;
            is.get(expr);
            e = DbgList(expr.c_str());
            break;
        }

        case DBGCMD_GETSOURCES: {
            e = DbgGetSources();
            break;
        }

        case DBGCMD_GO: {
            bool all; OptionalString threads;
            is.get(all, threads);
            e = DbgGo(all, threads);
            break;
        }

        case DBGCMD_FINISH: {
            OptionalString threads;
            is.get(threads);
            e = DbgFinishFunc(threads);
            break;
        }

        case DBGCMD_SETARGS: {
            std::string args;
            is.get(args);
            e = DbgSetArgs(args.c_str());
            break;
        }

        case DBGCMD_STEP: {
            int count, stepInto; OptionalString threads;
            is.get(count,stepInto, threads);
            e = DbgStep(count, stepInto, threads);
            break;
        }

        case DBGCMD_KILL: {
            e = DbgKill();
            break;
        }

        case DBGCMD_MOVEFRAME: {
            int count; bool up; OptionalString threads;
            is.get(count, up, threads);
            e = DbgMoveFrame(count, up, threads);
            break;
        }

        case DBGCMD_SHOWFRAME: {
            int count; OptionalString threads;
            is.get(count, threads);
            e = DbgShowFrame(count, threads);
            break;
        }

        case DBGCMD_GETFRAME: {
            OptionalString threads;
            is.get(threads);
            e = DbgGetFrame(threads);
            break;
        }

        case DBGCMD_SETDEBUGGERVAR: {
            std::string var, val;
            is.get(var, val);
            e = DbgSetDebuggerVar(var.c_str(), val.c_str());
            break;
        }

        case DBGCMD_SETSOURCEPATH: {
            std::string path;
            is.get(path);
            e = DbgSetSourcePath(path.c_str());
            break;
        }

        case DBGCMD_GETSOURCEPATH: {
            e = DbgGetSourcePath();
            break;
        }

        case DBGCMD_GETDEBUGGERVAR: {
            std::string var;
            is.get(var);
            e = DbgGetDebuggerVar(var.c_str());
            break;
        }

        case DBGCMD_SETPRINTELEMENTS: {
            std::string elements;
            is.get(elements);
            e = DbgSetPrintElements(elements.c_str());
            break;
        }

        case DBGCMD_RESETBEFOREDIRECTCMD: {
            e = DbgResetBeforeDirectCmd();
            break;
        }

        case DBGCMD_RESTOREAFTERDIRECTCMD: {
            e = DbgRestoreAfterDirectCmd();
            break;
        }

        case DBGCMD_SETVAR: {
            std::string var, val; OptionalString threads;
            is.get(var, val, threads);
            e = DbgSetVar(var.c_str(), val.c_str(), threads);
            break;
        }

        case DBGCMD_EVALEXPR:
        case DBGCMD_CONTINUEEVALEXPR: {
            // note - go back and make the proto order == argument order
            size_t exprId; std::string expr, threads;
            is.get(exprId, expr, threads);
            e = DbgEvalExpr(expr.c_str(), exprId, threads.c_str());
            break;
        }

        case DBGCMD_GETTYPE: {
            std::string var; bool isType; OptionalString threads;
            is.get(var, isType, threads);
            e = DbgGetType(var.c_str(), isType, threads);
            break;
        }

        case DBGCMD_GETLOCATION: {
            e = DbgGetLocation();
            break;
        }

        case DBGCMD_GETVARS: {
            OptionalString threads;
            is.get(threads);
            e = DbgGetVars(threads);
            break;
        }

        case DBGCMD_GETSCOPE: {
            std::string location;
            is.get(location);
            e = DbgGetScope(location.c_str());
            break;
        }

        case DBGCMD_STATUS: {
            int getLocation;
            is.get(getLocation);
            e = DbgStatus(getLocation);
            break;
        }

        case DBGCMD_DIRECT: {
            CmdArgs args;
            is.get(args);
            e = DbgDirectCmd(args.argc, args.argv);
            break;
        }

        case DBGCMD_QUIT: {
            bool detach;
            is.get(detach);
            e = DbgQuit(detach);
            break;
        }

        case DBGCMD_INTERRUPT: {
            bool all;
            is.get(all);
            e = DbgInterrupt(all);
            break;
        }
     
        case DBGCMD_OPENSOCKET: {
            std::string name; int size;
            is.get(name, size);
            e = DbgOpenSocket(name.c_str(), size);
            break;
        }

        case DBGCMD_CLOSESOCKET: {
            e = DbgCloseSocket();
            break;
        }

        case DBGCMD_SETCOMMPARAM: {
            e = NULL;  // this command should be caught by the communication level
            break;
        }

        case DBGCMD_THREAD: {
            std::string threadId;
            is.get(threadId);
            e = DbgSetThread(threadId.c_str());
            break;
        }

        case DBGCMD_GETTHREADINFO: {
            std::string threadId;
            is.get(threadId);
            e = DbgGetThreadInfo(threadId.c_str());
            break;
        }

        case DBGCMD_SETTHREADNAME: {
            std::string name;
            is.get(name);
            e = DbgSetThreadName(name.c_str());
            break;
        }

        case DBGCMD_FINDTHREAD: {
            std::string pattern;
            is.get(pattern);
            e = DbgFindThread(pattern.c_str());
            break;
        }

        case DBGCMD_THREADAPPLY: {
            std::string op;
            is.get(op);
            e = DbgThreadApply(op.c_str());
            break;
        }

        case DBGCMD_SETSCHEDULERLOCKING: {
            std::string mode;
            is.get(mode);
            e = DbgSetSchedulerLocking(mode.c_str());
            break;
        }

        case DBGCMD_GETSCHEDULERLOCKING: {
            e = DbgGetSchedulerLocking();
            break;
        }

        case DBGCMD_SETNONSTOP: {
            std::string mode;
            e = DbgSetNonStop(mode.c_str());
            break;
        }

        case DBGCMD_GETNONSTOP: {
            e = DbgGetNonStop();
            break;
        }

        case DBGCMD_OMPD: {
            std::string op;
            is.get(op);
            e = DbgOmpd(op.c_str());
            break;
        }

        case DBGCMD_CUDA: {
            std::string op;
            is.get(op);
            e = DbgCuda(op.c_str());
            break;
        }


    }

    // Convert the event to network protocol and send it
    // TODO: remove the null event check here when the DbgInvokeNetwork function
    // gets fixed for MRNet. Every command should be returning a response.
    if (!e) {
        DVPRINT("event was null\n");
        return {nullptr, &std::free};
    }

    std::unique_ptr<char, decltype(&std::free)> resp(EventToSocProto(e), &std::free);
    
    if (resp) {
        DbgSendReply(resp.get());
    } else {
        DVPRINT("did not get an event response\n");
    }

    DVPRINT("done with dispatch\n");

    return resp;
}

ISocStream::ISocStream(const std::string& str) : m_str(str) {
    int cmdId;
    m_str >> cmdId;
    m_cmd = static_cast<dbgcmd>(cmdId);

    // the protocol has put the arguments into a string -- if there are any.   We're not currently
    // using that to skip over the result, so here we'll just read the arg len and ignore it.
    int len;
    m_str >> len;  // this command will fail for commands with no arguments, but it's harmless
}

ISocStream& ISocStream::operator>>(int& i) { m_str >> i; return *this; }
ISocStream& ISocStream::operator>>(unsigned int& ui) { m_str >> ui; return *this; }
ISocStream& ISocStream::operator>>(bool& b) { int i; m_str >> i; b = i; return *this; }
ISocStream& ISocStream::operator>>(size_t& sz) { m_str >> sz; return *this; }
ISocStream& ISocStream::operator>>(std::string& s) {
    size_t len = 0;
    m_str >> len;
    
    char c = '\0';
    m_str.get(c); // the space between the length an the string
    s.resize(len);
    if (len) m_str.read(&s[0], len);

    return *this;
}

ISocStream& ISocStream::operator>>(OptionalString& s) {
    return *this >> s.str;
}

ISocStream& ISocStream::operator>>(CmdArgs& args) {
    int n;
    *this >> n;
    args.argc = n;

    if (n <= 0) {
        args.argv = nullptr;
        return *this;
    }
    args.argv = (char**)malloc(n * sizeof(char*));

    for (int i=0; i<n; ++i) {
        std::string s;
        *this >> s;
        args.argv[i] = strdup(s.c_str());
    }

    return *this;
}

CmdArgs::~CmdArgs()
{
    for (int i=0; i<argc; ++i) free(argv[i]);
    free(argv);
}

