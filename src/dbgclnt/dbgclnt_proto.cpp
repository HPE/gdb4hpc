/*********************************************************************************\
 * Debug client protocol switch routines.
 *
 * Copyright 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <vector>

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <filesystem>

#include "bitset.h"
#include "dbg_client.h"
#include "dbgclnt_proto.h"
#include "dbgevent.h"
#include "defs.h"
#include "list.h"
#include "useful.h"

#include "common_tools_fe.h"

/*
 * don't use this function!
 * this function is only here to make sure the linker pulls in the protocol
 * translation units so each protocol calls its constructor and registers itself.
*/
void registerProtos() {
    #ifdef MRNET
    extern void MRNClientProtocol_init();
    MRNClientProtocol_init();
    #endif
    #ifdef ZMQNET
    extern void ZMQClientProtocol_init();
    ZMQClientProtocol_init();
    #endif
    // proxy is always present
    extern void ProxyClientProtocol_init();
    ProxyClientProtocol_init();
}

// Used for the protocol wait for events function
static fd_set proto_dispatch_fds;
static int proto_high_fd;

std::unordered_map<std::string, DebugClientProtocol *> DebugClientProtocol::registry;

const std::unordered_map<std::string, DebugClientProtocol *>& DebugClientProtocol::getRegistry() {
    return DebugClientProtocol::registry;
}

DebugClientProtocol * const findClientProtoByName(const char* protoName) {
    if (DebugClientProtocol::getRegistry().find(protoName) != DebugClientProtocol::getRegistry().end()) {
        return DebugClientProtocol::getRegistry().at(protoName);
    }

    return nullptr;
}

DebugClientProtocol::DebugClientProtocol()
    : m_protoEventHandler{} {}

void DebugClientProtocol::registerProtocol() {
    // called by derived class constructors to register themselves in the registry
    if (registry.insert({name(), this}).second == true) {
        // this protocol doesn't exist in the registry yet, add it
        DPRINT("Registered server protocol: %s\n", name());
    } else {
        // this protocol already exists
        DPRINT("Duplicate protocol: %s\n", name());
    }
}

// Used during launch/attach to wait for the initial setup to complete
static int CheckForProtoStartupEvent(dbginvoke_t *di) {
    int *ev_fd;
    int status;

    if (di == NULL) return -1;

    // reset the fd_set
    FD_ZERO(&proto_dispatch_fds);
    proto_high_fd = -1;

    /*
     Note: This should be used for protocols that do not use an event
     notification fd. If a event notification fd can be used, then it should 
     be added to the sw_evfd list and sw_checkevents should simply return 0.
    */
    if (DBG_PROTO_SW(di)->checkEvents(di) > 0) {
        DPRINT("DbgCheckEvents: sw_checkevents returned 1\n");
        return 1;  // there is a proto event to handle. Return immediately.
    }

    /*
    ** If there is a d_evfds fd set, we want to add it to the fd_set to select
    *on.
    ** This enables protocol events.
    */
    if (di->d_evfds != NULL) {
        SetList(di->d_evfds);

        while ((ev_fd = (int *)GetListElement(di->d_evfds)) != (int *)NULL) {
            if (proto_high_fd < *ev_fd) {
                proto_high_fd = *ev_fd;
            }
            FD_SET(*ev_fd, &proto_dispatch_fds);
        }
    }

    /*
    ** Main select event polling loop.
    */
    for (;;) {
        // from the select manpage, nfds is the highest-numbered file descriptor
        // in any of the three sets, plus 1.
        status =
            select(proto_high_fd + 1, &proto_dispatch_fds, NULL, NULL, NULL);

        switch (status) {
            case -1:
                switch (errno) {
                    case EINTR:
                        DPRINT("DbgCheckEvents: EINTR\n");
                        continue;

                    // We expect EINVAL if there are no fd's in the fd_set. The
                    // high_fd should be negative in this case and select will
                    // return with -1 and errno set to EINVAL.
                    case EINVAL:
                        DPRINT("DbgCheckEvents: EINVAL\n");
                        PrintOutError("No way to checkevents.\n");
                        return -1;

                    default:
                        PrintOutError("Select failed with %s.\n",
                                      strerror(errno));
                        return -1;
                }
                break;

            case 0:
                return 0;

            default:
                return 1;
        }
    }

    // We should never reach here...
    DPRINT("DbgCheckEvents: At final return\n");
    return 1;
}

static void DispatchProtoStartupEvent(dbginvoke_t *di) {
    int *ev_fd;

    if (di == NULL) return;

    // If there is an d_evfds list, then the protol layer is using event
    // notification fd's. We should try to check to see if each one is set,
    // and if so call the protocol specific dispatch function with the event
    // fd as an argument
    if (di->d_evfds != NULL) {
        SetList(di->d_evfds);

        while ((ev_fd = (int *)GetListElement(di->d_evfds)) !=
                (int *)NULL) {
            if (FD_ISSET(*ev_fd, &proto_dispatch_fds)) {
                // dispatch each of the set notification fds one at a time.
                DBG_PROTO_SW(di)->dispatchEvents(di, *ev_fd);
            }
        }
    } else {
        // event notification fd's not in use by this protocol, so call
        // the generic dispatch function with -1 for event fd arg.
        DBG_PROTO_SW(di)->dispatchEvents(di, -1);
    }
}

int WaitForProtoStartupEvent(dbginvoke_t *di) {
    for (;;) {
        switch (CheckForProtoStartupEvent(di)) {
            case -1:
                return -1;

            case 0:
                return 0;

            default:
                DispatchProtoStartupEvent(di);
                return 1;
        }
    }
}

// public facing function to set the handler back to the default generic
void DebugClientProtocol::resetProtocolEventHandler() {
    setProtocolEventHandler(new DebugClientDefaultProtocolEventHandler());
}

void DebugClientProtocol::setProtocolEventHandler(DebugClientProtocolEventHandler *handler) {
    // since this is a unique pointer, this will delete any old handler.
    m_protoEventHandler.reset(handler);
}

int DebugClientProtocol::handleProtocolEvent(dbginvoke *di, const ProtoMessageSet &set) { 
    if (m_protoEventHandler) return m_protoEventHandler->handleEvent(di, set);
    return 1;
};

// common exit actions to all protocols. protocol exit functions should call this
// function in their own exit function after doing protocol specific exit actions.
int DebugClientProtocol::exit(struct dbginvoke *di, int detach) {
    if (!detach) {
        // send a kill signal using the CTI
        if (cti_killApp(di->d_appid, SIGKILL)) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError("Failed to kill application process!\n");
            return -1;
        }
    }

    cti_deregisterApp(di->d_appid);

    di->d_appid = 0;

    // Destroy the program output object
    DbgFreeGenericProgOutp(di->d_proto_outp);

    // clear the program output object
    di->d_proto_outp = nullptr;

    return 0;
}

int DebugClientDefaultProtocolEventHandler::handleEvent(dbginvoke *di, const ProtoMessageSet &set) {
    // by default, just forward to generic handler
    return DbgDispatchProtoEvents(di, set);
}

uint64_t DebugClientProtocol::parseAndRegisterAppId(const char *appIdent) {
    // get the wlm type
    auto wlm = cti_current_wlm();
    uint64_t appId;

    switch (wlm) {
        case CTI_WLM_SLURM: {
            const char *sep;
            uint32_t jobid;
            uint32_t stepid;
            char *eptr;

            // find '.' in app_ident
            if ((sep = strchr(appIdent, '.')) == nullptr) {
                PrintOutError(
                    "Invalid slurm job step argument. Expected "
                    "<job_id>.<step_id>\n");
                return 0;
            }
            
            // point to second digit (no need to modify string, strtol will stop at '.')
            ++sep;

            // convert the jobid
            errno = 0;
            jobid = (uint32_t)strtoul(appIdent, &eptr, 10);

            // check for error
            if ((errno == ERANGE && (jobid == ULONG_MAX)) ||
                (errno != 0 && jobid == 0)) {
                PrintOutError("Invalid slurm jobid argument.\n");
                return 0;
            }

            // check for invalid input
            if (eptr == appIdent) {
                PrintOutError("Invalid slurm jobid argument.\n");
                return 0;
            }

            // convert the stepid
            errno = 0;
            stepid = (uint32_t)strtoul(sep, &eptr, 10);

            // check for error
            if ((errno == ERANGE && (stepid == ULONG_MAX)) ||
                (errno != 0 && stepid == 0)) {
                PrintOutError("Invalid slurm stepid argument.\n");
                return 0;
            }

            // check for invalid input
            if (eptr == sep) {
                PrintOutError("Invalid slurm stepid argument.\n");
                return 0;
            }

            // Register the slurm job step with the tool interface
            cti_slurm_ops_t *ops = nullptr;
            cti_wlm_type_t ret = cti_open_ops((void **)&ops);
            if (ret != CTI_WLM_SLURM) {
                PrintOutError("cti_open_ops returned invalid wlm!\n");
                return 0;
            }
            if ((appId = ops->registerJobStep(jobid, stepid)) == 0) {
                PrintOutError("CTI error: %s\n", cti_error_str());
                PrintOutError("Failed to register slurm job step with CTI.\n");
                return 0;
            }
        } break;
        case CTI_WLM_SSH: {
            pid_t launcher_pid;
            char *eptr;

            // convert the string to an int
            errno = 0;
            launcher_pid = strtol(appIdent, &eptr, 10);

            // check for error
            if ((launcher_pid < 0) || (errno != 0)) {
                PrintOutError("Invalid launcher pid argument.\n");
                return 0;
            }

            // check for invalid input
            if (eptr == appIdent) {
                PrintOutError("Invalid launcher pid argument.\n");
                return 0;
            }

            // Register the pid with the tool interface
            cti_ssh_ops_t *ops = nullptr;
            cti_wlm_type_t ret = cti_open_ops((void **)&ops);
            if (ret != CTI_WLM_SSH) {
                PrintOutError("cti_open_ops returned invalid wlm!\n");
                return 0;
            }
            if ((appId = ops->registerJob(launcher_pid)) == 0) {
                PrintOutError("CTI error: %s\n", cti_error_str());
                PrintOutError("Failed to register launcher pid with CTI.\n");
                return 0;
            }
        } break;
        case CTI_WLM_ALPS: {
            uint64_t apid;
            char *eptr;

            // convert the string to an apid
            errno = 0;
            apid = (uint64_t)strtoull(appIdent, &eptr, 10);

            // check for error
            if ((errno == ERANGE && (apid == ULLONG_MAX)) ||
                (errno != 0 && apid == 0)) {
                PrintOutError("Invalid alps apid argument.\n");
                return 0;
            }

            // check for invalid input
            if (eptr == appIdent) {
                PrintOutError("Invalid alps apid argument.\n");
                return 0;
            }

            // Register the alps apid with the tool interface
            cti_alps_ops_t *ops = nullptr;
            cti_wlm_type_t ret = cti_open_ops((void **)&ops);
            if (ret != CTI_WLM_ALPS) {
                PrintOutError("cti_open_ops returned invalid wlm!\n");
                return 0;
            }
            if ((appId = ops->registerApid(apid)) == 0) {
                PrintOutError("CTI error: %s\n", cti_error_str());
                PrintOutError("Failed to register alps apid with CTI.\n");
                return 0;
            }
        }break;
        case CTI_WLM_PALS: {
            // check for invalid input
            if (appIdent == nullptr) {
                PrintOutError("Invalid pals apid argument.\n");
                return 0;
            }

            // Register the pals apid with the tool interface
            cti_pals_ops_t *ops = nullptr;
            cti_wlm_type_t ret = cti_open_ops((void **)&ops);
            if (ret != CTI_WLM_PALS) {
                PrintOutError("cti_open_ops returned invalid wlm!\n");
                return 0;
            }
            if ((appId = ops->registerApid(appIdent)) == 0) {
                PrintOutError("CTI error: %s\n", cti_error_str());
                PrintOutError("Failed to register pals apid with CTI.\n");
                return 0;
            }
        }break;
        default:
            PrintOutError("Unsupported wlm %s\n", cti_wlm_type_toString(wlm));
            return 0;
    }

    return appId;
}
