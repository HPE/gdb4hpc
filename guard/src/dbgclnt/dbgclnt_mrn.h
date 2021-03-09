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

    int attach(struct dbginvoke *di, proto_session *session, 
        const char *app_ident, const char *arch, const char *backend, 
        int gpu_flag, int debug_flag) override;

    int launch(struct dbginvoke *di, proto_session *session, 
        const char *prog, char **argv,
        const char *arch, char **arch_args, const char *arch_input, 
        const char *backend, const char* gdb, char **env,
        const char *cwd, int gpu_flag, int debug_flag, int early_entry_flag, 
        bool isNonStop) override;

    int exit(struct dbginvoke *, int detach) override;

    bitset* sendCommand(struct dbginvoke *di, bitset *target, 
        const char *request) override;

    struct proto_msg_set* receiveReply(struct dbginvoke *di, 
        bitset *target, bitset *received) override;

    int init() override;
    
    void dispatchEvents(struct dbginvoke *di, int ev_fd) override;

    void destroyClient(struct dbginvoke *di) override;

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
