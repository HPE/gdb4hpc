#ifndef _DBGCLNT_MRN_H
#define _DBGCLNT_MRN_H

#include "dbgclnt_proto.h"

#ifdef ALPS
#include "common_tools_fe.h"
#endif /* ALPS */

class MRNClientProtocol : public DebugClientProtocol {
public:
    // MRNet does not support:
    //  * finish function
    //  * checkEvents
    //  * any session functions

    const char* name() const override { return "mrnet"; };

    int init() override;
    virtual bool hasInit() const { return true; };

    bitset* sendCommand(struct dbginvoke *, bitset *, const char *) override;
    virtual bool hasSendCommand() const { return true; };

    struct proto_msg_set* receiveReply(struct dbginvoke *, bitset *,bitset *) override;
    virtual bool hasReceiveReply() const { return true; };

    void dispatchEvents(struct dbginvoke *, int) override;
    virtual bool hasDispatchEvents() const { return true; };

    // Client protocol routines
    int attach(struct dbginvoke *, proto_session *, const char *,
        const char *, const char *, int,
        int) override;
    virtual bool hasAttach() const { return true; };

    int launch(struct dbginvoke *, proto_session *, const char *, char **,
        const char *, char **, const char *, const char *, const char*, char **,
        const char *, int, int,
        int, bool) override;
    virtual bool hasLaunch() const { return true; };

    int exit(struct dbginvoke *, int detach) override;
    virtual bool hasExit() const { return true; };

    void destroyClient(struct dbginvoke *) override;
    virtual bool hasDestroyClient() const { return true; };

private:
    // MRNet specific helper functions
    bool DbgMRNAddOmpd(cti_manifest_id_t mid);
    bool DbgMRNAddGpuGdbs(cti_manifest_id_t mid);

    // startup event callback functions
    // these are static so that they can be pointed to by the callback function
    // fields of a dbginvoke_t
    static int cbMRNInvoke(dbginvoke_t *di, proto_msg_set *set);
    static int cbMRNAttach(dbginvoke_t *di, proto_msg_set *set);

#ifdef BE_VALGRIND
    int DbgAddValgrindFiles(cti_manifest_id_t mid);
#endif
};

#endif /* _DBGCLNT_MRN_H */
