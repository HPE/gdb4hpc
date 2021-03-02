#ifndef _DBGCLNT_PROXY_H
#define _DBGCLNT_PROXY_H

#include "dbgclnt_proto.h"

class ProxyClientProtocol : public DebugClientProtocol {
public:
    // proxy supports everything except checkEvents

    const std::string name() const override { return "proxy"; };

    int init() override;
    bool hasInit() const { return true; };

    void finish() override;
    bool hasFinish() const { return true; };

    bitset* sendCommand(struct dbginvoke *, bitset *, const char *) override;
    bool hasSendCommand() const { return true; };

    struct proto_msg_set* receiveReply(struct dbginvoke *, bitset *,bitset *) override;
    bool hasReceiveReply() const { return true; };

    void dispatchEvents(struct dbginvoke *, int) override;
    bool hasDispatchEvents() const { return true; };

    // Client protocol routines
    proto_session *createSession(DebugClientProtocol * const, WLM_type, 
        const char *, const char *,const char *) override;
    bool hasCreateSession() const { return true; };

    void saveSession(proto_session *) override;
    bool hasSaveSession() const { return true; };

    void cleanupSession(proto_session *) override;
    bool hasCleanupSession() const { return true; };

    int attach(struct dbginvoke *, proto_session *, const char *,
        const char *, const char *, int, int) override;
    bool hasAttach() const { return true; };

    int launch(struct dbginvoke *, proto_session *, const char *, char **,
        const char *, char **, const char *, const char *, const char*, char **,
        const char *, int, int, int, bool) override;
    bool hasLaunch() const { return true; };

    int exit(struct dbginvoke *, int detach) override;
    bool hasExit() const { return true; };

    void destroyClient(struct dbginvoke *) override;
    bool hasDestroyClient() const { return true; };
};

#endif /* _DBGCLNT_PROXY_H */
