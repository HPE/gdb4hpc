/******************************************************************************\
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP. 
 ******************************************************************************/

#pragma once

#include "dbgclnt_proto.h"
#include "dbg_zmq_network.h"
#include "dbginvoke.h"

class ZMQNetClientProtocol : public DebugClientProtocol {
public:
    // ZMQnet does not support:
    //  * finish function

    ZMQNetClientProtocol();

    const char* name() const override { return "zmqnet"; };

    int attach(DbgInvoke *di,
        const char *app_ident, const char *arch, const char *backend, const char* vgdb_file,
        const char* gdb, int gpu_flag, int debug_flag) override;

    int launch(DbgInvoke *di,
        const char *prog, const char * const *argv,
        const char *arch, const char * const *arch_args, const char *arch_input, 
        const char *backend, const char* gdb, const char* const *env,
        const char *cwd, int gpu_flag, int debug_flag, int non_mpi_flag,
        int early_entry_flag, 
        bool isNonStop) override;

    int syncExit(DbgInvoke *) override;
    int forceExit(DbgInvoke *) override;

    std::unique_ptr<Bitset> sendCommand(DbgInvoke *di, const Bitset *target, 
        const char *request) override;

    std::variant<ProtoMessageSet, ProtoNothing> receiveReply(DbgInvoke *di,
        const Bitset *target, std::chrono::seconds timeout, Bitset &received) override;

    int init() override;
    
    int checkEvents(DbgInvoke *di) override;
    void dispatchEvents(DbgInvoke *di, int ev_fd) override;
private:
    // separate attach function so we can reuse its attach functionality within the launch function
    int internalAttach(DbgInvoke *di, const char *prog,
                const char *app_ident, uint64_t appid,
                ZMQNetNetwork::OutputPtr &&output, const char *arch,
                const char *backend, const char* vgdb_file, const char *gdb, int gpu_flag,
                int debug_flag, int early_entry_flag, bool isNonStop,
                bool isNonMpi);
};

// called once by registerProtos
void ZMQClientProtocol_init() {
    // will register itself in constructor
    static ZMQNetClientProtocol proto;
};
