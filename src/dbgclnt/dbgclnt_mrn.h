#ifndef _DBGCLNT_MRN_H
#define _DBGCLNT_MRN_H

#include "dbgclnt_proto.h"

#ifdef ALPS
#include "common_tools_fe.h"
#endif /* ALPS */

namespace {
class MRNAttachHandler : public DebugClientProtocolEventHandler {
public:
    int handleEvent(DbgInvoke *di, const ProtoMessageSet &set) override;
};

class MRNLaunchHandler : public DebugClientProtocolEventHandler {
public:
    int handleEvent(DbgInvoke *di, const ProtoMessageSet &set) override;
};
} // private namespace

class MRNClientProtocol : public DebugClientProtocol {
public:
    // MRNet does not support:
    //  * finish function
    //  * checkEvents

    MRNClientProtocol() : DebugClientProtocol() { registerProtocol(); };

    const char* name() const override { return "mrnet"; };

    int attach(DbgInvoke *di,
        const char *app_ident, const char *arch, const char *backend,
        const char *gdb, const char *vgdb_file, int gpu_flag, int debug_flag) override;

    int launch(DbgInvoke *di,
        const char *prog, const char * const *argv,
        const char *arch, const char * const *arch_args, const char *arch_input, 
        const char *backend, const char* gdb, const char * const *env,
        const char *cwd, int gpu_flag, int debug_flag, int non_mpi_flag,
        int early_entry_flag, bool isNonStop) override;

    int syncExit(DbgInvoke *) override;
    int forceExit(DbgInvoke *) override;

    std::unique_ptr<Bitset> sendCommand(DbgInvoke *di, const Bitset *target, 
        const char *request) override;

    std::optional<ProtoMessageSet> receiveReply(DbgInvoke *di, 
        const Bitset *target, Bitset &received) override;

    int init() override;
    
    void dispatchEvents(DbgInvoke *di, int ev_fd) override;

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
