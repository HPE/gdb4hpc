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

    int attach(DbgInvoke *di, const AttachOpts& opts) override;

    int launch(DbgInvoke *di, const LaunchOpts& opts) override;

    int syncExit(DbgInvoke *, std::function<void(void)>&& cleanupCallback) override;
    int forceExit(DbgInvoke *, std::function<void(void)>&& cleanupCallback) override;

    bool sendCommand(DbgInvoke *di, const Bitset *target, const char *request) override;

    std::variant<ProtoMessageSet, ProtoNothing> receiveReply(DbgInvoke *di, 
        const Bitset *target, std::chrono::seconds timeout) override;

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
