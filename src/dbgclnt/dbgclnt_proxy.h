#ifndef _DBGCLNT_PROXY_H
#define _DBGCLNT_PROXY_H

#include <libssh2.h>

#include "dbgclnt_proto.h"

namespace {
class ProxyAttachHandler : public DebugClientProtocolEventHandler {
public:
    int handleEvent(DbgInvoke *di, const ProtoMessageSet &set) override;
};

class ProxyLaunchHandler : public DebugClientProtocolEventHandler {
public:
    int handleEvent(DbgInvoke *di, const ProtoMessageSet &set) override;
};
} // private namespace

class ProxyClientProtocol : public DebugClientProtocol {
public:
    // proxy supports everything except checkEvents

    ProxyClientProtocol() : DebugClientProtocol() { registerProtocol(); };

    const char* name() const override { return "proxy"; };

    int attach(DbgInvoke *di, proto_session *session, 
        const char *app_ident, const char *arch, const char *backend, 
        const char *gdb, int gpu_flag, int debug_flag) override;

    int launch(DbgInvoke *di, proto_session *session, 
               const char *prog, const char * const *argv,
               const char *arch, const char * const *arch_args, const char *arch_input, 
               const char *backend, const char* gdb, const char * const *env,
               const char *cwd, int gpu_flag, int debug_flag, int non_mpi_flag,
               int early_entry_flag, bool isNonStop) override;

    int exit(DbgInvoke *di, int detach) override;

    std::unique_ptr<Bitset> sendCommand(DbgInvoke *di, const Bitset *target, 
        const char *request) override;

    std::optional<ProtoMessageSet> receiveReply(DbgInvoke *di, 
        const Bitset *target, Bitset &received) override;

    int init() override;
    void finish() override;

    void dispatchEvents(DbgInvoke *di, int ev_fd) override;

    // Client protocol routines
    proto_session *createSession(DebugClientProtocol * const proto, 
        WLM_type manager, const char *batch_template, const char *hostname, 
        const char *username) override;

    void saveSession(proto_session *session) override;

    void cleanupSession(proto_session *session) override;

    void destroyClient(DbgInvoke *di) override;

private:
    // Proxy specific helper functions
    int setupConfigDirectory();
    char *parseBatchScript(const char *script_file, const char *launcher_argv_str);
    int ssh_start_proxy(const char *hostname, const char *username,
        const char *lhost, const char *lport);
    
    static void interactive_auth_callback(
        const char *name, int name_len, const char *instruction,
        int instruction_len, int num_prompts,
        const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
        LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses, void **abstract);
    std::optional<ProtoMessageSet> recvProtoMsg(int net_fd);

    // Proxy specific variables
    int callback_failure = 0;
};

// called once by registerProtos
void ProxyClientProtocol_init() {
    // will register itself in constructor
    static ProxyClientProtocol proto;
};

#endif /* _DBGCLNT_PROXY_H */
