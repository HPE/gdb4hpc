#ifndef _DBGCLNT_MRN_H
#define _DBGCLNT_MRN_H

#include "dbgclnt_proto.h"

class MRNClientProtocol : public DebugClientProtocol {
public:
    // MRNet does not support:
    //  * finish
    //  * checkEvents
    //  * any session functions

    static const std::string name;

    int init() override;
    bitset* sendCommand(struct dbginvoke *, bitset *, const char *) override;
    struct proto_msg_set* receiveReply(struct dbginvoke *, bitset *,bitset *) override;
    void dispatchEvents(struct dbginvoke *, int) override;

    // Client protocol routines
    int attach(struct dbginvoke *, proto_session *, const char *,
        const char *, const char *, int,
        int) override;
    int launch(struct dbginvoke *, proto_session *, const char *, char **,
        const char *, char **, const char *, const char *, const char*, char **,
        const char *, int, int,
        int, bool) override;
    int exit(struct dbginvoke *, int detach) override;
    void destroyClient(struct dbginvoke *) override;
};

#endif /* _DBGCLNT_MRN_H */
