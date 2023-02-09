// Dummies out version of MPI sufficient to run the functional tests againt
// the localhost CTI implementation.

#ifndef IS_LOCAL_MPI
#define IS_LOCAL_MPI

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define MPI_COMM_WORLD 0
#define MPI_SUCCESS 0
#define MPI_FAILURE (-1)
#define MPI_CHAR 1

#ifdef __cplusplus
extern "C" {
#endif

typedef int MPI_Comm;
typedef int MPI_Status;
    
static int MPI_Init(int* argc, char ***argv) { return MPI_SUCCESS; }

static int MPI_WaitDone = 0;

static void MPI_Wait() {
    if (MPI_WaitDone) return;
    MPI_WaitDone = 1;

    const char* lockFile = getenv("CTI_LOCALHOST_LOCK_FILE");
    if (!lockFile) return;

    struct stat buffer;
    while (stat(lockFile, &buffer) == 0) {
        sleep(1);
    }
}
    
#define MPI_Init(argc, argv) (MPI_Wait(),MPI_Init(argc,argv))


static int MPI_Comm_rank(MPI_Comm comm, int *rank) {
    char* env = getenv("CTI_LOCALHOST_RANK");
    *rank = 0;
    if (env) {
        *rank = atoi(env);
        return MPI_SUCCESS;
    } else {
        return MPI_FAILURE;
    }
}

static int MPI_Comm_size(MPI_Comm comm, int *num_ranks) {
    char* env = getenv("CTI_LOCALHOST_NUM_RANKS");
    *num_ranks = 0;
    if (env) {
        *num_ranks = atoi(env);
        return MPI_SUCCESS;
    } else {
        return MPI_FAILURE;
    }
}

static int MPI_Finalize() {
    return MPI_SUCCESS;
}

static int MPI_Send(const void *buf, int count, int datatype, int dest,
             int tag, MPI_Comm comm) {
    return MPI_SUCCESS;
}

static int MPI_Recv(void *buf, int count, int datatype,
    int source, int tag, MPI_Comm comm, MPI_Status *status)
{
    *status = MPI_SUCCESS;
    ((char*)buf)[0] = '\0';
    return 1;
}

#ifdef __cplusplus
}
#endif
#endif

