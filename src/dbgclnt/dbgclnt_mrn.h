#ifndef _DBGCLNT_MRN_H
#define _DBGCLNT_MRN_H

#include "dbgclnt_proto.h"

#ifdef ALPS
#include "common_tools_fe.h"
#endif /* ALPS */

namespace {
class MRNAttachHandler : public DebugClientProtocolEventHandler {
public:
    int handleEvent(dbginvoke *di, const ProtoMessageSet &set) override;
};

class MRNLaunchHandler : public DebugClientProtocolEventHandler {
public:
    int handleEvent(dbginvoke *di, const ProtoMessageSet &set) override;
};
} // private namespace

class MRNClientProtocol : public DebugClientProtocol {
public:
    // MRNet does not support:
    //  * finish function
    //  * checkEvents
    //  * any session functions

    MRNClientProtocol() : DebugClientProtocol() { registerProtocol(); };

    const char* name() const override { return "mrnet"; };

    int attach(struct dbginvoke *di, proto_session *session, 
               const char *app_ident, const char *arch, const char *backend, 
               const char* gdb, int gpu_flag, int debug_flag) override;

    int launch(struct dbginvoke *di, proto_session *session, 
        const char *prog, const char * const *argv,
        const char *arch, const char * const *arch_args, const char *arch_input, 
        const char *backend, const char* gdb, const char * const *env,
        const char *cwd, int gpu_flag, int debug_flag, int non_mpi_flag,
        int early_entry_flag, bool isNonStop) override;

    int exit(struct dbginvoke *, int detach) override;

    std::unique_ptr<Bitset> sendCommand(struct dbginvoke *di, const Bitset *target, 
        const char *request) override;

    std::optional<ProtoMessageSet> receiveReply(struct dbginvoke *di, 
        const Bitset *target, Bitset &received) override;

    int init() override;
    
    void dispatchEvents(struct dbginvoke *di, int ev_fd) override;

    void destroyClient(struct dbginvoke *di) override;

private:
#ifdef BE_VALGRIND
    int addValgrindFiles(cti_manifest_id_t mid);
#endif
};

// called once by registerProtos
void MRNClientProtocol_init() {
    // will register itself in constructor
    static MRNClientProtocol proto;
};

#endif /* _DBGCLNT_MRN_H */
