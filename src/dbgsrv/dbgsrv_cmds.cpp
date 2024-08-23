/*********************************************************************************\
 * Server commands dispatch routines
 *
 * Copyright (c) 1996-2012 by Monash Uni.
 *
 * Copyright 2013,2020-2023 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#include "config.h"

#include "dbg.h"
#include "dbgfn.h"
#include "dbgcmd.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_proto.h"
#include "dbgsrv_util.h"
#include "useful.h"

#include <string>
#include <memory>

std::unique_ptr<char, decltype(&std::free)> dbgsrv_dispatch_cmds(const char *msg, const dbgcomm_info *ci) {

    DVPRINT("dbgsrv_dispatch_cmds received: %s\n", msg);

    // ISocStream manages the parsing
    soc::ISocStream is(msg);

    dbgcmd cmd = is.getCmd();

    // switch on the command
    dbgevent_t *e = nullptr;

    try {
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
                int allowPending; std::string file; int line, id; std::string threadSet;
                DVPRINT("std\n");
                is.get(id, allowPending, file, line, threadSet);
                e = DbgSetLineBreak(id, allowPending, file.c_str(), line, threadSet.c_str(), ci);
                break;
            }

            case DBGCMD_SETFUNCBREAK: {
                int allowPending, id; std::string file, func, threadSet;
                is.get(id, allowPending, file, func, threadSet);
                e = DbgSetFuncBreak(id, allowPending, file.c_str(), func.c_str(),
                                    threadSet.c_str(), ci);
                break;
            }

            case DBGCMD_SETWATCH: {
                int id, type; std::string expr;
                is.get(id, type, expr);
                e = DbgSetWatch(id, type, expr.c_str(), ci);
                break;
            }

            case DBGCMD_SETCATCH: {
                int id;
                std::string eventType, regEx;
                is.get(id, eventType, regEx);
                e = DbgSetCatch(id, eventType, regEx, ci);
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

            case DBGCMD_IGNOREBREAK: {
                int bpid, n;
                is.get(bpid, n);
                e = DbgIgnoreBreak(bpid, n);
                break;
            }

            case DBGCMD_CONDITIONBREAK: {
                int bpid; std::string cond;
                is.get(bpid, cond);
                e = DbgConditionBreak(bpid, cond);
                break;
            }


            case DBGCMD_INFOBREAK: {
                e = DbgInfoBreak();
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
                bool all; soc::OptionalString threads;
                is.get(all, threads);
                e = DbgGo(all, threads);
                break;
            }

            case DBGCMD_FINISH: {
                soc::OptionalString threads;
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
                int count, stepInto; soc::OptionalString threads;
                is.get(count,stepInto, threads);
                e = DbgStep(count, stepInto, threads);
                break;
            }

            case DBGCMD_MOVEFRAME: {
                int count, dir; soc::OptionalString threads;
                is.get(count, dir, threads);
                e = DbgMoveFrame(count, DbgFrameDir(dir), threads);
                break;
            }

            case DBGCMD_SHOWFRAME: {
                int count; soc::OptionalString threads;
                is.get(count, threads);
                e = DbgShowFrame(count, threads);
                break;
            }

            case DBGCMD_GETFRAME: {
                bool verbose; soc::OptionalString threads;
                is.get(verbose, threads);
                e = DbgGetFrame(verbose, threads);
                break;
            }

            case DBGCMD_SETDEBUGGERVAR: {
                std::string var, val;
                is.get(var, val);
                e = DbgSetDebuggerVar(var.c_str(), val.c_str());
                break;
            }

            case DBGCMD_SETSOURCEPATH: {
                int op; std::string path;
                is.get(op, path);
                e = DbgSetSourcePath(DbgDirOp{op}, path.c_str());
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
                std::string var, val; soc::OptionalString threads;
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
                std::string var; bool isType; soc::OptionalString threads;
                is.get(var, isType, threads);
                e = DbgGetType(var.c_str(), isType, threads);
                break;
            }

            case DBGCMD_GETLOCATION: {
                e = DbgGetLocation();
                break;
            }

            case DBGCMD_GETVARS: {
                soc::OptionalString threads;
                is.get(threads);
                e = DbgGetVars(threads);
                break;
            }

            case DBGCMD_GETARGS: {
                soc::OptionalString threads;
                is.get(threads);
                e = DbgGetArgs(threads);
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
                soc::CmdArgs args;
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

            case DBGCMD_SLEEP: {
                int seconds;
                is.get(seconds);
                e = DbgSleep(seconds);
                break;
            }

            case DBGCMD_SHELL: {
                soc::CmdArgs args;
                is.get(args);
                e = DbgShell(args.argc, args.argv);
                break;
            }

            case DBGCMD_SIGNAL: {
                std::string signame;
                is.get(signame);
                e = DbgSignal(signame.c_str());
                break;
            }

            case DBGCMD_PYEXTBT: {
                e = DbgPyExtBacktrace();
                break;
            }

            case DBGCMD_PYEXTLIST: {
                std::string arg;
                is.get(arg);
                e = DbgPyExtList(arg);
                break;
            }

            case DBGCMD_PYEXTMOVEFRAME: {
                int dir;
                is.get(dir);
                e = DbgPyExtMoveFrame(DbgFrameDir(dir));
                break;
            }

            case DBGCMD_PYEXTPRINT: {
                std::string name;
                is.get(name);
                e = DbgPyExtPrint(name);
                break;
            }

            case DBGCMD_PYEXTLOCALS: {
                e = DbgPyExtLocals();
                break;
            }

            case DBGCMD_PYBEGETPYTHON: {
                e = DbgPyBEGetPython();
                break;
            }

            case DBGCMD_PYBESETPYTHON: {
                int onoff;
                is.get(onoff);
                e = DbgPyBESetPython(static_cast<PyMode>(onoff));
                break;
            }

            case DBGCMD_PYBEINJECT: {
                e = DbgPyBEInject();
                break;
            }

            case DBGCMD_PYBEGETGIL: {
                e = DbgPyBEGetGIL();
                break;
            }

            case DBGCMD_PYBEATTACH: {
                e = DbgPyBEAttach();
                break;
            }
        }
    } catch (dbgevent_t* error) {
        DVPRINT("Caught top level dbgsrv error: %s\n", DbgEventErrorStr(error));
        e = error;
    } catch (const std::runtime_error &exc) {
        DVPRINT("Caught top level dbgsrv exception: %s\n", exc.what());
        e = NewErrorEvent(DBGERR_INTERNAL, exc.what());
    } catch (...) {
        DVPRINT("Caught unknown top level dbgsrv error\n");
        e =  NewErrorEvent(DBGERR_INTERNAL, "Internal error");
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

