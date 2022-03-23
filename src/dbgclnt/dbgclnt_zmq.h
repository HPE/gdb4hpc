/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#pragma once

#include "dbgclnt_proto.h"
#include "dbg_zmq_network.h"
#include "dbginvoke.h"

class ZMQNetClientProtocol : public DebugClientProtocol {
public:
    // ZMQnet does not support:
    //  * finish function
    //  * any session functions

    ZMQNetClientProtocol();

    const char* name() const override { return "zmqnet"; };

    int attach(struct dbginvoke *di, proto_session *session, 
        const char *app_ident, const char *arch, const char *backend, 
        int gpu_flag, int debug_flag) override;

    int launch(struct dbginvoke *di, proto_session *session, 
        const char *prog, const char * const *argv,
        const char *arch, const char * const *arch_args, const char *arch_input, 
        const char *backend, const char* gdb, char **env,
        const char *cwd, int gpu_flag, int debug_flag, int non_mpi_flag,
        int early_entry_flag, 
        bool isNonStop) override;
        
    void destroyClient(dbginvoke *) override;
    int exit(struct dbginvoke *, int detach) override;

    std::unique_ptr<Bitset> sendCommand(struct dbginvoke *di, const Bitset *target, 
        const char *request) override;

    std::optional<ProtoMessageSet> receiveReply(struct dbginvoke *di, 
        const Bitset *target, Bitset &received) override;

    int init() override;
    
    int checkEvents(struct dbginvoke *di) override;
    void dispatchEvents(struct dbginvoke *di, int ev_fd) override;
private:
    // separate attach function so we can reuse its attach functionality within the launch function
    int internalAttach(dbginvoke_t *di, proto_session *sess, const char *prog,
                const char *app_ident, uint64_t appid,
                ZMQNetNetwork::OutputPtr &&output, const char *arch,
                const char *backend, const char *gdb, int gpu_flag,
                int debug_flag, int early_entry_flag, bool isNonStop);
};

// called once by registerProtos
void ZMQClientProtocol_init() {
    // will register itself in constructor
    static ZMQNetClientProtocol proto;
};
