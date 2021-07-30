/******************************************************************************\
 * dbgproxy.h - proxy interface routines
 *
 * Copyright 2013-2015,2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 ******************************************************************************/

#ifndef _DBGPROXY_H
#define _DBGPROXY_H

#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "dbgclnt_proto.h"
#include "dbgclnt_network.h"

// This defines a proxy "session". Only the command stream is shared for a
// single session of the proxy. The event and output stream are unique to each
// invoke/attach.
struct proxy_session {
    // determines if this session is still valid
    int isValid;
    // number of dbginvoke_t objects that reference this session.
    int refCount;
    // next id to assign for proxy_type
    int idCount;
    // size of sockaddr_in for use in socket API calls - this is a constant
    socklen_t slen;
    // listener socket for new event/output streams
    int listenfd;
    struct sockaddr_in listenSock;
    // used for synch commands
    int cmdfd;
    struct sockaddr_in cmdSock;
    // used for error callback
    int errfd;
    struct sockaddr_in errSock;
    FILE *errStream;
    // used for message callback
    int msgfd;
    struct sockaddr_in msgSock;
    FILE *msgStream;
    // this is the temporary job script that should be cleaned up during exit
    char *jobScript;
};
typedef struct proxy_session proxy_session;
#define PROXY_SESSION_VALID(p) (p)->isValid
#define PROXY_SESSION_REFCNT(p) (p)->refCount
#define PROXY_SESSION_IDCNT(p) (p)->idCount
#define PROXY_SESSION_SLEN(p) (p)->slen
#define PROXY_SESSION_LISTENFD(p) (p)->listenfd
#define PROXY_SESSION_LISTENSOCK(p) (p)->listenSock
#define PROXY_SESSION_CMDFD(p) (p)->cmdfd
#define PROXY_SESSION_CMDSOCK(p) (p)->cmdSock
#define PROXY_SESSION_ERRORFD(p) (p)->errfd
#define PROXY_SESSION_ERRORSOCK(p) (p)->errSock
#define PROXY_SESSION_ERRORSTREAM(p) (p)->errStream
#define PROXY_SESSION_MSGFD(p) (p)->msgfd
#define PROXY_SESSION_MSGSOCK(p) (p)->msgSock
#define PROXY_SESSION_MSGSTREAM(p) (p)->msgStream
#define PROXY_SESSION_JOBFILE(p) (p)->jobScript

// used as the network protocol object for the proxy interface
struct proxy_type : public DebugClientNetwork {
    // Every proxy_type references a proxy_session
    proxy_session *sess;
    // unique id for this instance associated with the session
    int id;
    // used for event callback
    int eventfd;
    struct sockaddr_in eventSock;
    // used for output callback
    int outputfd;
    struct sockaddr_in outputSock;
    FILE *outputStream;
};
typedef struct proxy_type proxy_type;
#define PROXY_SESSION(p) (p)->sess
#define PROXY_SLEN(p) (p)->sess->slen
#define PROXY_LISTENFD(p) (p)->sess->listenfd
#define PROXY_CMDFD(p) (p)->sess->cmdfd
#define PROXY_JOBFILE(p) (p)->sess->jobScript
#define PROXY_ID(p) (p)->id
#define PROXY_EVENTFD(p) (p)->eventfd
#define PROXY_EVENTSOCK(p) (p)->eventSock
#define PROXY_OUTPUTFD(p) (p)->outputfd
#define PROXY_OUTPUTSOCK(p) (p)->outputSock
#define PROXY_OUTPUTSTREAM(p) (p)->outputStream

enum msgtype {
    MSG_SEND_CMD,
    MSG_RECV_REPLY,
    MSG_ATTACH,
    MSG_INVOKE,
    MSG_SAVE_SESSION,
    MSG_CLEANUP_SESSION,
    MSG_EXIT,
    MSG_OK,
    MSG_ERROR
};
typedef enum msgtype msgtype;

struct proxy_msg_header {
    int msg_id;
    int msg_len;
    msgtype msg_type;
};
typedef struct proxy_msg_header proxy_msg_header;

struct recv_type {
    std::unique_ptr<Bitset> target;
    std::unique_ptr<Bitset> received;
};
typedef struct recv_type recv_type;

struct attach_args {
    char *app_ident;
    char *arch;
    char *backend;
    int gpu_flag;
    int debug_flag;
};
typedef struct attach_args attach_args;

struct launch_args {
    char *prog;
    char **argv;
    int nproc;
    char *arch;
    char **arch_args;
    char *arch_input;
    char *backend;
    char *gdb;
    char **env;
    char *cwd;
    int gpu_flag;
    int debug_flag;
    int early_entry_flag;
    int non_stop_flag;
};

extern proxy_session *NewProxySession(void);
extern void CleanupProxySession(proxy_session *sess);

extern proxy_type *NewProxyType(proxy_session *sess);
extern void FreeProxyType(proxy_type *p);

// recv_type takes ownership of bitset pointers
extern recv_type *NewRecvType(std::unique_ptr<Bitset> &&target, std::unique_ptr<Bitset> &&recv);
extern void FreeRecvType(recv_type *);

extern attach_args *NewAttachArgs(void);
extern void FreeAttachArgs(attach_args *args);
extern launch_args *NewLaunchArgs(void);
extern void FreeLaunchArgs(launch_args *args);

extern std::optional<ProtoMessage> SocProtoToProtoMsg(char *);
extern recv_type *SocProtoToRecv(char *);
extern attach_args *SocProtoToAttachArgs(char *);
extern launch_args *SocProtoToLaunchArgs(char *);
extern int SocProtoToExitArgs(char *);

extern char *ProtoMsgToSocProto(const ProtoMessage&);
extern char *RecvToSocProto(const Bitset *target, const Bitset *recv);
extern char *AttachArgsToSocProto(const char *app_ident, const char *arch,
                                  const char *backend, int gpu_flag,
                                  int debug_flag);
extern char *LaunchArgsToSocProto(const char *prog, const char * const *argv, int nproc,
                                  const char *arch, const char * const *arch_args,
                                  const char *arch_input, const char *backend, const char* gdb,
                                  char **env, const char *cwd, int gpu_flag,
                                  int debug_flag, int early_entry_flag,
                                  bool isNonStop);
extern char *ExitArgsToSocProto(int detach);

#endif /* ! _DBGPROXY_H */
