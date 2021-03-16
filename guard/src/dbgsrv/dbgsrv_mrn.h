#ifndef _DBGSRV_MRN_H
#define _DBGSRV_MRN_H

#include "dbgsrv_proto.h"
#include "dbg_mrn.h"

#ifdef ALPS
#include "common_tools_be.h"
#endif // ALPS

class MRNServerProtocol : public DebugServerProtocol {
public:
    const char * name() override;
    int initProto(int procnum, int argc, char **argv,
        const char *key, const char *host, int port) override;
    void endProto() override;
    fd_set* getFdSet() override;
    void runProto(fd_set *fds) override;
    int invokeCallback(char *chost, int cport, char *host, int prog,
        int pnum, int port, dbgevent_t *e) override;
    int eventCallback(dbgevent_t *ev) override;
    int setupIo(int *fd, int *port) override;
    int transferIo(int fd, char *buffer, int size) override;
    dbgevent_t* invokeNetwork(int numbe) override;
    int sendReply(char *response) override;
private:
    /*
    we use mpd to launch MRNet backends on non-Cray platform.
    Not sure why "PMI_RANK" does not work.
    We use "MPD_JRANK" instead.
    */
#ifndef ALPS
    static char *MPIRankVars[] = {
        "OMPI_COMM_WORLD_RANK",   /* Open MPI 1.6 */
        "OMPI_MCA_orte_ess_vpid", /* Open MPI 1.3 */
        "OMPI_MCA_ns_nds_vpid",   /* Open MPI 1.2 */
        "PMI_RANK",               /* MPICH2. PMI_RANK does not work for MPICH2. */
        "MPD_JRANK",              /* MPICH2. MPD_JRANK works on MPICH2. */
        "MP_CHILD",               /* IBM PE */
        "SLURM_PROCID",           /* SLURM */
        NULL};
#endif // ALPS

    MRNet_Stream *m_stream;
    int m_tag;
    int m_my_mrn_rank;
    int m_mrn_event_fd;
};

#endif // _DBGSRV_MRN_H