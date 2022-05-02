// Copyright 2022 Hewlett Packard Enterprise Development LP.

// This is a tiny program that can be used in cases where gdb4hpc launch
// cannot reliably get a launch barrier, which is currently the case
// for non-MPI programs or where the WLM launch barrier isn't working
// right.

// Basically, this programs transforms the problem into an "attach".
// The tool launches this program with real program and its arguments
// in the argument list.   Then this progam just waits around
// for gdb to have a chance to connect to it, then after gdb
// set the variable dbgsrv_trampoline_connected to non-zero it
// execv with the real program.  Gdb by default just follows the exec
// and Bob's your uncle.

#include <unistd.h>

static int theArgc;
static char **theArgv;
static char *theApplication;

// Setting this value via the debugger triggers the exec.
int dbgsrv_trampoline_connected = 0;
int dbgsrv_wait_cnt = 0;

void doExec() {
    int argc = theArgc;
    char **argv = theArgv;
    char* prog = theApplication;

    for (int i=0; i+1 < argc; ++i) argv[i] = argv[i+1];
    argv[argc-1] = 0;
    execv(prog, argv);
}

void MPI_Init() {}

void connectHook() {
    // setting a breakpoint on an entirely empty function
    // can be a little unreliable - so give it something to do.
    ++dbgsrv_wait_cnt;  
}

void doWait() {
    for (int i=0; !dbgsrv_trampoline_connected && i<400; ++i) {
        sleep(1);
        connectHook();
    }
    if (dbgsrv_trampoline_connected)
        doExec();
}

int main(int argc, char* argv[]) {
    theArgc = argc;
    theArgv = argv;
    theApplication = argv[1];
    doWait();
    return -1;
}
