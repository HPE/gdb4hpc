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
    MRNet_Stream *m_stream;
    int m_tag;
    int m_my_mrn_rank;
    int m_mrn_event_fd;
};

#endif // _DBGSRV_MRN_H