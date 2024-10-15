#pragma once

/*********************************************************************************\
 * GDB interface routines
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
#include <vector>
#include <map>
#include "dbgfn.h"
#include "dbgcmd.h"
#include "MI.h"
#include "dbgsrv_pybe.h"

class DbgGdbMI {
public:
    DbgGdbMI();
    dbgevent_t *init(int *dfd, int *iofd, char **env);
    dbgevent_t *startup();
    int read(int);
    bool bufferReady();
    int progioHook(int);   //!!! careful - potentially unimplemented
    dbgevent_t *setLineBreak(int id, const int allowPending, const char *file, int lno, const char* threadId, const dbgcomm_info *commInfo);
    dbgevent_t *setFuncBreak(int id, const int allowPending, const char *file, const char *func, const char* threadId, const dbgcomm_info *commInfo);
    dbgevent_t *setWatch(int id, int type, const char *expr, const dbgcomm_info *commInfo);
    dbgevent_t *setCatch(int id, const std::string &event, const std::string &regex, const dbgcomm_info *commInfo); // TODO is commInfo needed?
    dbgevent_t *setCheckpoint();
    dbgevent_t *restoreCheckpoint(unsigned int id);
    dbgevent_t *deleteCheckpoint(unsigned int id);
    dbgevent_t *deleteBreak(int bid);
    dbgevent_t *disableBreak(int bid);
    dbgevent_t *enableBreak(int bid, bool once, bool del);
    dbgevent_t *ignoreBreak(int bid, int n);
    dbgevent_t *conditionBreak(int bid, const std::string& cond);
    dbgevent_t *infoBreak();
    dbgevent_t *list(const char *str);
    dbgevent_t *getSources();
    dbgevent_t *go(bool all, const char* threads);
    dbgevent_t *finishFunc(const char* threads);
    dbgevent_t *setArgs(const char *arg); // set run arguments
    dbgevent_t *step(int const, bool in, const char* threads);
    dbgevent_t *moveFrame(int count, DbgFrameDir dir, const char* threads);

    // If count is not zero then return only that many frames. If count is zero then
    // return an unlimited number of frames. If count is negative return the
    // outermost frames.
    //
    // if in python mode, returns a python backtrace. use `showElfFrames` or `pythonBEBacktrace`
    // to manually specify the type of backtrace to use.
    dbgevent_t *showFrame(int count, const char* threads);
    dbgevent_t* showElfFrames(int count, const char* threads);

    dbgevent_t *getFrame(bool verbose, const char* threads);
    dbgevent_t *setVar(const char *var, const char* val, const char* threads);
    dbgevent_t *evalExpr(const char *expr, long exprIdx, const char* threads);
    dbgevent_t *getType(const char *var, bool isType, const char* threads);
    dbgevent_t *getLocation();   //!! potentially unimplemented
    dbgevent_t *getVars(const char* threads);
    dbgevent_t *getArgs(const char* threads);
    dbgevent_t *getScope(const char *location);
    dbgevent_t *getStatus(bool getLocation);
    dbgevent_t *quit(bool detach);
    dbgevent_t *interrupt(bool all);
    dbgevent_t *launch(const char *host, int cp, const char *proto, const char *prog,
                               const char *args, char **env, const char *main_function);
    dbgevent_t *attach(int pid, const char *prog, const char *args, char **env, const char *main_function);
    dbgevent_t *targetRemote(const char* target_remote);
    void finalize();
    bool isReadyForCmd();
    bool isOk();
    bool isStopped();
    dbgevent_t *setDebuggerVar(const char *var, const char *val);
    dbgevent_t *setSourcePath(DbgDirOp op, const char *path);
    dbgevent_t *getSourcePath();
    dbgevent_t *getDebuggerVar(const char *var);
    dbgevent_t *setPrintElements(const char *elements);
    dbgevent_t *resetBeforeDirectCmd();
    dbgevent_t *restoreAfterDirectCmd();


    // Pass command directly to gdb/mi through console command
    dbgevent_t *directCmd(int argc, char **argv);
    dbgevent_t *openSocket(const char *opset_name, int opset_size);
    dbgevent_t *closeSocket();
    dbgevent_t *setThread(const char* threadId);
    dbgevent_t *getThreadInfo(const char* threadId, bool anonymize, bool collect);
    dbgevent_t *setThreadName(const char* name);
    dbgevent_t *findThread(const char* pattern);
    dbgevent_t *threadApply(const char* op);
    dbgevent_t *setSchedulerLocking(const char* op);
    dbgevent_t *getSchedulerLocking();
    dbgevent_t *setNonStop(const char* op);
    dbgevent_t *getNonStop();
    dbgevent_t *ompd(const char* op);
    dbgevent_t *cuda(const char* op);
    dbgevent_t *sleep(int seconds);
    dbgevent_t *shell(int argc, char **argv);
    dbgevent_t *signal(char const* signame);

    // pyExt* functions use gdb python extensions, pyBE* functions use pybe. see python.md

    // these python features require that the backend gdb has loaded python extensions. see python.md
    dbgevent_t* pyExtBacktrace();
    dbgevent_t* pyExtList(const std::string& arg);
    dbgevent_t* pyExtMoveFrame(DbgFrameDir dir);
    dbgevent_t* pyExtPrint(const std::string& name);
    dbgevent_t* pyExtLocals();

    // endpoints of `get/set python` commands
    dbgevent_t* pyBEGetPython();
    dbgevent_t* pyBESetPython(PyMode setting);

    // these python features use the pybe injected library. see python.md
    dbgevent_t* pyBEInject();
    dbgevent_t* pyBEBacktrace();
    dbgevent_t* pyBEGetGIL();
    dbgevent_t* pyBEEvalExpr(std::string_view expr);
    dbgevent_t* pyBESetLineBreakpoint(int feid, std::string_view filename, int line);
    dbgevent_t* pyBESetFuncBreakpoint(int feid, std::string_view filename, std::string_view func);
    dbgevent_t* pyBEDeleteBreakpoint(int feid);
    dbgevent_t* pyBEStep(bool step_in);
    dbgevent_t* pyBEFinish();

    // pybe breakpoint callback
    dbgevent_t* pyBEHandleAsyncStop(MIEvent* event);

    // initialize pybe after attaching to an already running python app
    dbgevent_t* pyBEAttach();

    // return true if we should prefer the pybe versions of commands when possible
    bool isPython() const;
    void setPython(bool isPython);

    MIEvent* waitForAsyncEvent();
    dbgevent_t* waitAndProcessAsyncEvent();
    int flushAsyncEvents();
    static void setAsyncEvent(MIEvent* e);
    bool isRunning() const;
    bool isAtExit() const;
    
    void setIsInNonStopMode(bool inNonStop);
    void setSourceOpen(bool doOpen);
    dbgevent_t* waitEvent(dbgevent_t *e);

private:
    class Initializer;

    dbgevent_t* setBreak(int feId,
                         const int allowPending, const std::string& where,
                         const char* threadSet, const dbgcomm_info* commInfo);
    dbgevent_t* evalExprWithThreads(const char *expr, const char* threads);
    std::vector<int> getThreadSet(const std::string& threadId, std::string* suffix, std::string& error);
    std::string parseThreadApplyOp(const std::string& op, bool& isSilent, bool& isQuiet,
                                   bool &isVerbose, size_t& numThreads);
    void sendCmd(MICommand* cmd);

    dbgevent_t*  initOmpd();
    bool initOmpdL();
    dbgevent_t* setPrintElementsL(int elems);
    int getPrintElementsL();
    std::string getDebuggerVarL(const char* var);

    std::unique_ptr<Initializer> m_initializer;
    bool m_ompdIsInitialized;
    bool m_ompdIsAvail;
    std::string m_beFileDir;  // location of manifest files

    bool m_isPython;

    // ensure that the python backend is ready to use. after successfully calling this function:
    //
    // - the pybe shared library will have been injected into the inferior.
    // - a new thread will have been spawned in the inferior for pybe use.
    //
    // this function is idempotent and can safely be called multiple times.
    //
    // throws a dbgevent_t* error on failure.
    //
    // XXX: ensurePybe has become a suboptimal name. it was originally designed to assume
    // that it could be called at any point in the program, but now we assume that it will
    // be called right after launch/attach - only *then* is it safe to call at any point
    // afterwards. i'll revisit the name in the future.
    Pybe& ensurePybe();

    // prefer ensurePybe instead of accessing this directly
    std::unique_ptr<Pybe> m_pybe;
};

class DbgGdbMI::Initializer {
public:
    Initializer(DbgGdbMI& dbg);
    dbgevent_t* init(int *dfd, int *iofd, char **env);
    dbgevent_t* startup();
    dbgevent_t* initDbg(const char *prog, const char *args, const char *main_function);
    dbgevent_t* initDbgAttach(int pid, const char *prog, const char *main_function);
    dbgevent_t* initDbgTargetRemote(const char *target_remote);
    dbgevent_t* setArgs(const char* args);
    dbgevent_t* quit(bool detach);
    dbgevent_t* interrupt(bool all);

    void setSourceOpen(bool doOpen);
        
    MISession* getMISession() const { return m_session; }
    const std::vector<dbgbp_t*>& getExitBPs() const { return m_exitBP; }
    void resetPId() { m_pid = 0; }

    // If true, the initializer is in a state during startup where SIGSTOPS should be ignored.
    bool ignoreSigstop();
private:
    // Send a raw SIGSTOP with ::kill. gdbmi will catch this signal,
    // stopping one thread, and generate an async event. The signal will
    // never actually make it to the inferior because of the signal handling settings
    // we set at initialization.
    // Returns the result of ::kill(m_pid, SIGSTOP).
    int sendAndCatchSigstop();

    dbgevent_t* setup_gdbmi_environment();
    dbgevent_t* setup_gdbmi_signals();
    dbgevent_t* delete_internal_breakpoints(std::vector<dbgbp_t*>& bplist);
    std::pair<bool,dbgevent_t*>  setEntryBPs();
    dbgevent_t* continueToEntryPoint();
    void setup_pretty_printing();
    void initGpu();
    void setDataDir(const char* gdbType);
    bool entryBPsAreSet();
    bool hasAmdGpus();
    std::pair<bool,dbgevent_t*> setEntryBPsL();
    void quitGdb(bool detach);

    using MIEventPtr = std::unique_ptr<MIEvent,decltype(&MIEventFree)>;
    enum ContinueType { CONTINUE, NEXTI, FINISH };
    std::pair<MIEventPtr, dbgevent_t*> continueToEvent(ContinueType ctype=CONTINUE);

    DbgGdbMI& m_dbg;
    MISession* m_session;
    // XXX: these dbgbp_t* are never free'd. but the contained pointers are copied around
    // pretty freely throughout the codebase so I'm not sure who is actually responsible
    // for freeind them. leaving it alone for now since it's barely a leak (matches the
    // lifetime of the entire dbgsrv)
    std::vector<dbgbp_t*> m_exitBP;
    int m_pid;
    char* m_runArgs;
    std::string m_executable;
    std::string m_mainFunction;
    bool m_ignoreSigstop = false;
    bool m_isNonMPI = false;
};

