// Copyright (c) 2024 Hewlett Packard Enterprise Development LP.

#include "str.h"

#include "dbgsrv_defs.h"

#include "mi_utils.h"

void SendMICommand(MISession *sess, MICommand *cmd) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    MISessionSendCommand(sess, cmd);
    do {
        if (MISessionProgress(sess) == -1) {
            if (auto buf = MallocedCharPtr(MICommandResultErrorMessage(cmd))) {
                DVPRINT("SendMICommand: Debugger error: %s\n", buf.get());
            } else {
                DVPRINT("SendMICommand: Error from GDB, but no message\n");
            }

            break;
        }

        if (sess->out_fd == -1) {
            DVPRINT("SendMICommand: sess->out_fd = -1\n");
            break;
        }

    } while (!MICommandCompleted(cmd));
}

std::unique_ptr<MICommand, decltype(&MICommandFree)>
make_MICmd(MICommand* cmd) {
    return std::unique_ptr<MICommand, decltype(&MICommandFree)>(cmd, MICommandFree);
}
