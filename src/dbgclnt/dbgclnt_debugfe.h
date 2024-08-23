/******************************************************************************\
 * Frontend debugger interface definition
 * TODO: This does not support asynchronous commands.
 *
 * Copyright 2013-2024 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#pragma once

#include "bitset.h"
#include "dbgevent.h"
#include "dbgcmd.h"

class DbgFE {
public:
    DbgFE(const char* gdbPath);
    ~DbgFE();

    dbgevent_t *quit();
    dbgevent_t *setFile(const char *);
    dbgevent_t *checkStartupFunc();
    dbgevent_t *list(const char*, const char *);
    dbgevent_t *getSources();
    dbgevent_t *setSourcePath(DbgDirOp op, const char *);
    dbgevent_t *getSourcePath();
    dbgevent_t *getScope(const char *);

    // runs -symbol-info-functions --include-nondebug --name "^`name`" --max-results 200.
    // returned event is DBGEV_SYMBOLS, or nullptr/DBGEV_ERROR on error.
    dbgevent_t *getFunctionSymbolNames(std::string_view name);
    // runs -symbol-info-variables --include-nondebug --name "^`name`" --max-results 200.
    // returned event is DBGEV_SYMBOLS, or nullptr/DBGEV_ERROR on error.
    dbgevent_t *getVariableSymbolNames(std::string_view name);
    // Get struct members for variable with fully-qualified name fq_name.
    // Prefix `name` to member results.
    // returned event is DBGEV_VARS, or nullptr/DBGEV_ERROR on error.
    dbgevent_t *getStructMemberNames(std::string_view fq_name, std::string_view name);

    //--------------------------------------------------
    // for managing a set of pending locations to list
    //--------------------------------------------------
    struct Location {
      std::unique_ptr<Bitset> set;
      std::unique_ptr<char, decltype(&std::free)> threads;
      std::unique_ptr<char, decltype(&std::free)> loc_str;
      int count;
    };

    dbgevent_t *list(const Location& loc);

    bool listIsPending() const { return m_listIsPending; }
    void addListStateLocation(const Bitset *set, const char* threads, const char *str);
    std::vector<Location>& getListStateLocations() { return m_listLocs; }
    void clearListState();

 private:
    dbgevent_t* setup_gdbmi_environment();
    void sendCmd(struct MICommand* cmd);
    dbgevent_t* fetch_symbols(MICommand& cmd);

    struct MISession* m_session;
    bool m_listIsPending = false;
    std::vector<Location> m_listLocs;
};
