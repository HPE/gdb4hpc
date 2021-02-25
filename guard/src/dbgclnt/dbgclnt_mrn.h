#ifndef _DBGCLNT_MRN_H
#define _DBGCLNT_MRN_H

#include "dbgclnt_proto.h"

class MRNClientProtocol : public DebugClientProtocol {
public:
    // MRNet does not support:
    //  * finish function
    //  * checkEvents
    //  * any session functions

    const std::string name() const override;

    int init() const override;
    bitset* sendCommand(struct dbginvoke *, bitset *, const char *) const override;
    struct proto_msg_set* receiveReply(struct dbginvoke *, bitset *,bitset *) const override;
    void dispatchEvents(struct dbginvoke *, int) const override;

    // Client protocol routines
    int attach(struct dbginvoke *, proto_session *, const char *,
        const char *, const char *, int,
        int) const override;
    int launch(struct dbginvoke *, proto_session *, const char *, char **,
        const char *, char **, const char *, const char *, const char*, char **,
        const char *, int, int,
        int, bool) const override;
    int exit(struct dbginvoke *, int detach) const override;
    void destroyClient(struct dbginvoke *) const override;
};

#endif /* _DBGCLNT_MRN_H */
