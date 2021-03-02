#ifndef _DBGCLNT_MRN_H
#define _DBGCLNT_MRN_H

#include "dbgclnt_proto.h"

class MRNClientProtocol : public DebugClientProtocol {
public:
    // MRNet does not support:
    //  * finish function
    //  * checkEvents
    //  * any session functions

    const std::string name() const override { return "mrnet"; };

    int init() const override;
    virtual bool hasInit() const { return true; };

    bitset* sendCommand(struct dbginvoke *, bitset *, const char *) const override;
    virtual bool hasSendCommand() const { return true; };

    struct proto_msg_set* receiveReply(struct dbginvoke *, bitset *,bitset *) const override;
    virtual bool hasReceiveReply() const { return true; };

    void dispatchEvents(struct dbginvoke *, int) const override;
    virtual bool hasDispatchEvents() const { return true; };

    // Client protocol routines
    int attach(struct dbginvoke *, proto_session *, const char *,
        const char *, const char *, int,
        int) const override;
    virtual bool hasAttach() const { return true; };

    int launch(struct dbginvoke *, proto_session *, const char *, char **,
        const char *, char **, const char *, const char *, const char*, char **,
        const char *, int, int,
        int, bool) const override;
    virtual bool hasLaunch() const { return true; };

    int exit(struct dbginvoke *, int detach) const override;
    virtual bool hasExit() const { return true; };

    void destroyClient(struct dbginvoke *) const override;
    virtual bool hasDestroyClient() const { return true; };
};

#endif /* _DBGCLNT_MRN_H */
