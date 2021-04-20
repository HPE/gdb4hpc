#ifndef _DBGCLNT_PROXY_H
#define _DBGCLNT_PROXY_H

#include <libssh2.h>

#include "dbgclnt_proto.h"

namespace {
class ProxyAttachHandler : public DebugClientProtocolEventHandler {
public:
    int handleEvent(dbginvoke *di, proto_msg_set *set) override;
};

class ProxyLaunchHandler : public DebugClientProtocolEventHandler {
public:
    int handleEvent(dbginvoke *di, proto_msg_set *set) override;
};
} // private namespace

class ProxyClientProtocol : public DebugClientProtocol {
public:
    // proxy supports everything except checkEvents

    ProxyClientProtocol() : DebugClientProtocol() { registerProtocol(); };

    const char* name() const override { return "proxy"; };

    int attach(struct dbginvoke *di, proto_session *session, 
        const char *app_ident, const char *arch, const char *backend, 
        int gpu_flag, int debug_flag) override;

    int launch(struct dbginvoke *di, proto_session *session, 
        const char *prog, char **argv,
        const char *arch, char **arch_args, const char *arch_input, 
        const char *backend, const char* gdb, char **env,
        const char *cwd, int gpu_flag, int debug_flag, int early_entry_flag, 
        bool isNonStop) override;

    int exit(struct dbginvoke *di, int detach) override;

    bitset* sendCommand(struct dbginvoke *di, bitset *target, 
        const char *request) override;

    struct proto_msg_set* receiveReply(struct dbginvoke *di, 
        bitset *target, bitset *received) override;

    int init() override;
    void finish() override;

    void dispatchEvents(struct dbginvoke *di, int ev_fd) override;

    // Client protocol routines
    proto_session *createSession(DebugClientProtocol * const proto, 
        WLM_type manager, const char *batch_template, const char *hostname, 
        const char *username) override;

    void saveSession(proto_session *session) override;

    void cleanupSession(proto_session *session) override;

    void destroyClient(struct dbginvoke *di) override;

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
    proto_msg_set *recvProtoMsg(int net_fd);

    // Proxy specific variables
    int callback_failure = 0;
};

 
void _ProxyClientProtocol_link() {};

#endif /* _DBGCLNT_PROXY_H */
