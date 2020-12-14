#pragma once

/*********************************************************************************\
 * GDB interface routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
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
#include "MI.h"

class DbgGdbMI : public dbgbackend_sw {
public:
    DbgGdbMI();
    virtual dbgevent_t *init(int *dfd, int *iofd, char **env) override;
    virtual dbgevent_t *startup() override;
    virtual int read(int) override;
    virtual bool bufferReady() override;
    virtual int progioHook(int) override;   //!!! careful - potentially unimplemented
    virtual dbgevent_t *setLineBreak(const int allowPending, const char *file, int lno, const char* threadId, const dbgcomm_info *commInfo) override;
    virtual dbgevent_t *setFuncBreak(const int allowPending, const char *file, const char *func, const char* threadId, const dbgcomm_info *commInfo) override;
    virtual dbgevent_t *setWatch(int type, const char *expr, const dbgcomm_info *commInfo) override;
    virtual dbgevent_t *deleteBreak(int bid) override;
    virtual dbgevent_t *disableBreak(int bid) override;
    virtual dbgevent_t *enableBreak(int bid, bool once, bool del) override;
    virtual dbgevent_t *lastBreak() override;
    virtual dbgevent_t *list(const char *str) override;
    virtual dbgevent_t *getSources() override;
    virtual dbgevent_t *go(bool all, const char* threads) override;
    virtual dbgevent_t *finishFunc(const char* threads) override;
    virtual dbgevent_t *setArgs(const char *arg) override; // set run arguments
    virtual dbgevent_t *step(int const, bool in, const char* threads) override;
    virtual dbgevent_t *kill() override;
    virtual dbgevent_t *moveFrame(int count, bool up, const char* threads) override;

    // If count is not zero then return only that many frames. If count is zero then
    // return an unlimited number of frames. If count is negative return the
    // outermost frames.
    virtual dbgevent_t *showFrame(int count, const char* threads) override;

    virtual dbgevent_t *getFrame(const char* threads) override;
    virtual dbgevent_t *setVar(const char *var, AIF *a, const char* threads) override;
    virtual dbgevent_t *evalExpr(const char *expr, long exprIdx, const char* threads) override;
    virtual dbgevent_t *getType(const char *var, bool isType, const char* threads) override;
    virtual dbgevent_t *getLocation() override;   //!! potentially unimplemented
    virtual dbgevent_t *getVars(const char* threads) override;
    virtual dbgevent_t *getScope(const char *location) override;
    virtual dbgevent_t *getStatus(bool getLocation) override;
    virtual dbgevent_t *quit(bool detach) override;
    virtual dbgevent_t *interrupt(bool all) override;
    virtual dbgevent_t *launch(const char *host, int cp, const char *proto, const char *prog,
                               const char *args, char **env, int arch) override;
    virtual dbgevent_t *attach(int pid, const char *prog, const char *args, char **env, int arch) override;
    virtual void finalize() override;
    virtual bool isReadyForCmd() override;
    virtual bool isOk() override;
    virtual bool isStopped() override;
    virtual dbgevent_t *setDebuggerVar(const char *var, AIF *val) override;
    virtual dbgevent_t *setSourcePath(const char *path) override;
    virtual dbgevent_t *getSourcePath() override;
    virtual dbgevent_t *getDebuggerVar(const char *var) override;
    virtual dbgevent_t *setPrintElements(const char *elements) override;
    virtual dbgevent_t *resetBeforeDirectCmd() override;
    virtual dbgevent_t *restoreAfterDirectCmd() override;


    // Pass command directly to gdb/mi through console command
    virtual dbgevent_t *directCmd(int argc, char **argv) override;
    virtual dbgevent_t *openSocket(const char *opset_name, int opset_size) override;
    virtual dbgevent_t *closeSocket() override;
    virtual dbgevent_t *setThread(const char* threadId) override;
    virtual dbgevent_t *getThreadInfo(const char* threadId) override;
    virtual dbgevent_t *setThreadName(const char* name) override;
    virtual dbgevent_t *findThread(const char* pattern) override;
    virtual dbgevent_t *threadApply(const char* op) override;
    virtual dbgevent_t *setSchedulerLocking(const char* op) override;
    virtual dbgevent_t *getSchedulerLocking() override;
    virtual dbgevent_t *setNonStop(const char* op) override;
    virtual dbgevent_t *getNonStop() override;
    virtual dbgevent_t *ompd(const char* op) override;

    MIEvent* waitForAsyncEvent();
    dbgevent_t* waitAndProcessAsyncEvent();
    static void setAsyncEvent(MIEvent* e);
    bool isRunning() const;
    
    void setIsInNonStopMode(bool inNonStop);
    void setSourceOpen(bool doOpen);
    dbgevent_t* waitEvent(dbgevent_t *e);

private:
    class Initializer;

    dbgevent_t* setBreak(const int allowPending, const std::string& where,
                         const char* threadSet, const dbgcomm_info* commInfo);
    dbgevent_t* evalExprWithThreads(const char *expr, const char* threads);
    std::vector<int> getThreadSet(const std::string& threadId, std::string* suffix, std::string& error);
    std::string parseThreadApplyOp(const std::string& op, bool& isSilent, bool& isQuiet,
                                   bool &isVerbose, size_t& numThreads);
    std::vector<int> getMultiThreadBreaks(int bid, bool remove);
    template<typename FUNC> void applyMultiThreadBreak(int bid, bool remove, FUNC&& op) {
        auto ids = getMultiThreadBreaks(bid,remove);
        for (auto it = ids.rbegin(); it != ids.rend(); ++it) op(*it);
    }
    dbgevent_t*  initOmpd();
    bool initOmpdL();

    std::map<int,std::vector<int>> m_multiThreadBreaks;
    std::unique_ptr<Initializer> m_initializer;
    bool m_ompdIsInitialized;
    bool m_ompdIsAvail;
    std::string m_beFileDir;  // location of manifest files
};

class DbgGdbMI::Initializer {
public:
    Initializer(DbgGdbMI& dbg);
    dbgevent_t* init(int *dfd, int *iofd, char **env);
    dbgevent_t* startup();
    dbgevent_t* initDbg(const char *prog, const char *args);
    dbgevent_t* initDbgAttach(int pid, const char *prog);
    dbgevent_t* setArgs(const char* args);
    dbgevent_t* quit(bool detach);
    dbgevent_t* interrupt(bool all);

    void setSourceOpen(bool doOpen);
        
    MISession* getMISession() const { return m_session; }
    list_t* getExitBPs() const { return m_exitBP; }
    void resetPId() { m_pid = 0; }
private:
    dbgevent_t* setup_gdbmi_entry_points();
    dbgevent_t* setup_gdbmi_main_entry_point();
    dbgevent_t *setup_gdbmi_exit_points();
    dbgevent_t* setup_gdbmi_environment();
    dbgevent_t* setup_gdbmi_signals();
    dbgevent_t* delete_internal_breakpoints(list_t *bplist);
    void setup_pretty_printing();
    DbgGdbMI& m_dbg;
    MISession* m_session;
    list_t *m_exitBP;
    int m_pid;
    char* m_runArgs;
};

