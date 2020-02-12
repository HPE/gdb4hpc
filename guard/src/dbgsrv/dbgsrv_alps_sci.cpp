/*********************************************************************************\
 * dbgsrv_alps.c - A wrapper program to launch guard dbgsrv's onto the compute
 *                 nodes.
 *
 * Copyright 2011-2013,2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "dbg_sci.h"
#include "defs.h"
#include "tool_backend.h"
#include "useful.h"

#define DBGSRV "dbgsrv"
#define DBGSRV_ARGC 19
#define PID_BUFSIZ 32

#define DBGSRV_ARGV_NAME 0
#define DBGSRV_ARGV_OPT_PID 1
#define DBGSRV_ARGV_PID 2
#define DBGSRV_ARGV_OPT_HOST 3
#define DBGSRV_ARGV_HOST 4
#define DBGSRV_ARGV_OPT_PORT 5
#define DBGSRV_ARGV_PORT 6
#define DBGSRV_ARGV_OPT_ARCH 7
#define DBGSRV_ARGV_ARCH 8
#define DBGSRV_ARGV_OPT_PROTO 9
#define DBGSRV_ARGV_PROTO 10
#define DBGSRV_ARGV_OPT_DBG 11
#define DBGSRV_ARGV_DBG 12
#define DBGSRV_ARGV_OPT_PROC 13
#define DBGSRV_ARGV_PROC 14
#define DBGSRV_ARGV_OPT_KEY 15
#define DBGSRV_ARGV_KEY 16
#define DBGSRV_ARGV_PROG 17
#define DBGSRV_ARGV_NULL 18

#define DBGSRV_OPT_PID "-i"
#define DBGSRV_OPT_HOST "-h"
#define DBGSRV_OPT_PORT "-c"
#define DBGSRV_OPT_ARCH "-a"
#define DBGSRV_OPT_PROTO "-p"
#define DBGSRV_OPT_DBG "-e"
#define DBGSRV_OPT_PROC "-n"
#define DBGSRV_OPT_KEY "-k"

/*
const struct option long_opts[] = {
                                {"callbackHost",        required_argument, 0,
'h'},
                                {"callbackPort",        required_argument, 0,
'c'},
                                {"networkArch",         required_argument, 0,
'a'},
                                {"networkProtocol",     required_argument, 0,
'p'},
                                {"dbgBackend",          required_argument, 0,
'e'},
                                {"help",                no_argument, 0, 'x'},
                                {0, 0, 0, 0}
                                };

void
usage(char *name)
{
        fprintf(stdout, "Usage: %s [OPTIONS]...\n", name);
        fprintf(stdout, "Launch guard debug servers on compute nodes.\n");
        fprintf(stdout, "\t-h, --callbackHost     Front end hostname\n");
        fprintf(stdout, "\t-c, --callbackPort     Front end callback port\n");
        fprintf(stdout, "\t-a, --networkArch      Network architecture\n");
        fprintf(stdout, "\t-p, --networkProtocol  Network protocol\n");
        fprintf(stdout, "\t-e, --dbgBackend       Debugging program backend\n");
        fprintf(stdout, "\t-x, --help             Display this text and
exit\n");
}*/

#define MAXLINE 1024
#define MAX_RETRIES 10
#define MAX_TMMSG_LEN 1024

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

static struct sembuf postop, waitop;
static char timeMsg[MAX_TMMSG_LEN];

/*void
get_current_time()
{
        time_t curr_time;
        struct tm curr_tm;

        time(&curr_time);
    localtime_r(&curr_time, &curr_tm);
    strftime(timeMsg, MAX_TMMSG_LEN, "%y%m%d-%H:%M:%S", &curr_tm);
}*/
void get_current_time() {
    time_t curr_time;
    struct tm curr_tm;

    time(&curr_time);
    localtime_r(&curr_time, &curr_tm);

    struct timeval current_tv;
    gettimeofday(&current_tv, NULL);

    strftime(timeMsg, MAX_TMMSG_LEN, "%y%m%d-%H:%M:%S", &curr_tm);

    int len = strlen(timeMsg);
    snprintf(timeMsg + len, MAX_TMMSG_LEN - len, "[%lu.%lu]", current_tv.tv_sec,
             current_tv.tv_usec);
}

int init_sem(key_t key) {
    static int initFlag = 0;

    int i;
    union semun arg;
    struct semid_ds seminfo;
    //	struct sembuf sb;
    int semid;

    if (initFlag == 0) {
        semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);

        if (semid >= 0) { /* we got it first */
            arg.val = 1;
            semctl(semid, 0, SETVAL, arg);
        } else if (errno == EEXIST) { /* someone else got it first */
            int ready = 0;

            semid = semget(key, 1, 0);   /* get the id */
            if (semid < 0) return semid; /* error, check errno */

            /* wait for other process to initialize the semaphore: */
            arg.buf = &seminfo;
            for (i = 0; i < MAX_RETRIES && !ready; i++) {
                semctl(semid, 1, IPC_STAT, arg);
                if (arg.buf->sem_otime != 0) {
                    ready = 1;
                } else {
                    sleep(1);
                }
            }

            if (!ready) {
                errno = ETIME;
                return -1;
            }

        } else {
            return semid; /* error, check errno */
        }
    }

    initFlag = 1;

    postop.sem_num = 0;
    postop.sem_op = 1;
    postop.sem_flg = SEM_UNDO;

    waitop.sem_num = 0;
    waitop.sem_op = 1;
    waitop.sem_flg = SEM_UNDO;

    return semid;
}

int rm_sem(int semid) {
    union semun arg;

    if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        return -1;
    }

    return 0;
}

void lock(int semid) { semop(semid, &waitop, 1); /* down by 1 */ }

void unlock(int semid) { semop(semid, &postop, 1); /* up by 1 */ }

int init_shm(key_t key, int size) {
    int shmid;

    shmid = shmget(key, size, 0644 | IPC_CREAT);
    return shmid;
}

void *attach_shm(int shmid) {
    void *data;

    data = shmat(shmid, (void *)0, 0);

    return data;
}

void detach_shm(int shmid, void *data) {
    int res;
    res = shmdt(data);
    if (res < 0) fprintf(stderr, "detach_shm: shmdt error\n");
}

void rm_shm(int shmid, void *data) {
    int res;
    res = shmdt(data);
    if (res < 0) fprintf(stderr, "rm_shm: shmdt error\n");

    res = shmctl(shmid, IPC_RMID, NULL);
    if (res < 0) fprintf(stderr, "rm_shm: shmctl error\n");
}

/*int
write_shm(int shmid, void* data, pid_t pid)
{
        struct shmid_ds buf;
        int seq;
        pid_t	*pid_buf;
        int		*seq_buf;

        shmctl(shmid, IPC_STAT, &buf);
        seq_buf = (int*)data;
        seq = *seq_buf;
        (*seq_buf)++;

        pid_buf = (pid_t*)((char*)data + sizeof(int) + seq*(sizeof(pid_t)));
        *pid_buf = pid;

        seq = *seq_buf;
        return seq;
}*/

int write_shm(int shmid, void *data, int value) {
    struct shmid_ds buf;
    int seq;
    pid_t *pid_buf;
    int *seq_buf;

    shmctl(shmid, IPC_STAT, &buf);
    seq_buf = (int *)data;
    *seq_buf = value;

    seq = *seq_buf;
    return seq;
}

int count_shm(int shmid, void *data) {
    struct shmid_ds buf;
    int res;
    int *res_buf;

    shmctl(shmid, IPC_STAT, &buf);
    res_buf = (int *)data;
    res = *res_buf;

    return res;
}

void read_shm(int shmid) {
    struct shmid_ds buf;
    int res;

    shmctl(shmid, IPC_STAT, &buf);
}

void lock_file(int fd) {
    struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    fcntl(fd, F_SETLKW, &lock);
}

void unlock_file(int fd) {
    struct flock lock;

    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    fcntl(fd, F_SETLK, &lock);
}

void setLog(char *logDir, char *nodeName, int suffix) {
    //#ifdef DEBUG
    static int logFd = -1;

    char *dir;
    char logFileName[PATH_MAX];

    // Create to complete log file name
    sprintf(logFileName, "%s/dbgsrv.%s.%d.log", logDir, nodeName, suffix);

    // Open the log file
    if (logFd != -1)  // Been here before? Clean up.
        close(logFd);

    logFd = open(logFileName, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (logFd == -1) {
        fprintf(stderr, "Cannot open log[%s]\n", logFileName);
        return;
    }

    // Redirect stderr/stdout to the file
    dup2(logFd, STDERR_FILENO);  // also clears close-on-exec flag!
    dup2(logFd, STDOUT_FILENO);  // also clears close-on-exec flag!
    //#endif
}

int main(int argc, char **argv) {
    // dbgsrv arguments
    int opt_ind = 0;
    int c;
    int jobKey_num = -1;
    char *jobKey = NULL;
    char *callbackHost = NULL;
    char *callbackPort = NULL;
    char *networkArch = NULL;
    char *networkProto = NULL;
    char *dbgBackend = NULL;
    char *prog;
    char pidbuff[PID_BUFSIZ];
    // node specific info
    uint64_t apid = 0;
    nodeAppPidList_t *myPids;
    // dbgsrv info
    int index;
    char **dbgsrv_argv;
    char **tmp;
    pid_t mypid;
    char path[MAXPATHLEN];

    key_t ipc_key;
    int shmid, semid;
    int nserver, nserver_started, myseq, res, numRetry;
    char *data;
    FILE *shared;
    int shared_fd, rn;
    char shared_name[MAXPATHLEN];
    char line[MAXLINE + 1];
    int failed;

    get_current_time();
    mypid = getpid();

    char *nidName;
    char *logDir;

    // get this nodes nidName
    if ((nidName = getNodeNidName()) == (char *)NULL) {
        fprintf(stderr, "getNodeNidName failed.\n");
        nidName = (char *)malloc(1024);
        sprintf(nidName, "nid-pid-%d", mypid);
    }

    logDir = getenv(GUARD_SCI_BE_LOG_DIR);

    if (logDir == NULL) {
        fprintf(stderr, "getenv GUARD_SCI_BE_LOG_DIR failed.\n");
        // try to use DBG_LOG_DIR
        logDir = getenv(DBG_LOG_DIR);
        if (logDir == NULL) {
            fprintf(stderr, "getenv DBG_LOG_DIR failed.\n");
            logDir = (char *)malloc(1024);
            sprintf(logDir, "/lus/scratch/n13942/dbgsrv_log");
        }
    }
    setenv(DBG_LOG_ENV_VAR, logDir, 1);
#ifdef DEBUG
    setLog(logDir, nidName, mypid);
#endif

    //	DPRINT("Time[%s]: I am started!\n", timeMsg);
    //	DPRINT("My nidName: %s\n", nidName);
    fprintf(stderr, "Time[%s]: I am started! nidName: %s\n", timeMsg, nidName);

    jobKey = getenv(GUARD_SCI_BE_JOBKEY);
    sscanf(jobKey, "%d", &jobKey_num);

    if (jobKey == (char *)NULL || jobKey_num == -1) {
        fprintf(stderr, "Invalid job key.\n");
        return 1;
    }

    callbackHost = getenv(GUARD_SCI_BE_HOSTNAME);
    if (callbackHost == (char *)NULL) {
        fprintf(stderr, "Invalid callback hostname.\n");
        return 1;
    }

    callbackPort = getenv(GUARD_SCI_BE_HOSTPORT);
    if (callbackPort == (char *)NULL) {
        fprintf(stderr, "Invalid callback hostname.\n");
        return 1;
    }

    networkArch = getenv(GUARD_SCI_BE_ARCH);
    if (networkArch == (char *)NULL) {
        fprintf(stderr, "Invalid launch architecture.\n");
        return 1;
    }

    networkProto = getenv(GUARD_SCI_BE_PROTO);
    if (networkProto == (char *)NULL) {
        fprintf(stderr, "Invalid network protocol.\n");
        return 1;
    }

    dbgBackend = getenv(GUARD_SCI_BE_DBGBE);
    if (dbgBackend == (char *)NULL) {
        fprintf(stderr, "Invalid debug backend.\n");
        return 1;
    }

    prog = getenv(GUARD_SCI_BE_PROGRAM);
    if (prog == (char *)NULL) {
        fprintf(stderr, "Invalid program name.\n");
        return 1;
    }

    // get the apid from the toolhelper path from argv[0]
    sscanf(argv[0], "/var/spool/alps/%*d/toolhelper%llu/%*s",
           (long long unsigned int *)&apid);
    sprintf(shared_name, "/var/spool/alps/%ld/toolhelper%ld/shared.txt", apid,
            apid);

    get_current_time();
    //	DPRINT("Time[%s]: My apid: %llu\n", timeMsg, (long long unsigned
    //int)apid); 	DPRINT("My key: %s\n", jobKey);
    fprintf(stderr, "Time[%s]: My apid: %llu, key: %s\n", timeMsg,
            (long long unsigned int)apid, jobKey);

    // get the app pids for this node
    if ((myPids = findAppPids(apid)) == (nodeAppPidList_t *)NULL) {
        fprintf(stderr, "findAppPids failed.\n");
        return 1;
    }
    get_current_time();
    //	DPRINT("Time[%s]: Number of app pid_t's detected on this node: %d\n",
    //timeMsg, myPids->numPairs);
    fprintf(stderr,
            "Time[%s]: Number of app pid_t's detected on this node: %d\n",
            timeMsg, myPids->numPairs);

#ifdef DEBUG
    for (index = 0; index < myPids->numPairs; index++) {
        DPRINT("\trank pid pair: [%d]-[%d]\n", myPids->rankPidPairs[index].rank,
               myPids->rankPidPairs[index].pid);
    }
#endif

    nserver = myPids->numPairs;
    ipc_key = jobKey_num;

    // wait on all dbgsrv start

    /*	semid = init_sem(ipc_key);

            lock(semid);*/

    /* shared memory codes
    //	shmid = init_shm(key, nserver*sizeof(pid_t) + sizeof(int));
            shmid = init_shm(ipc_key, sizeof(int));
            DPRINT("init_shm is done! shmid[%d]\n", shmid);

            data = attach_shm(shmid);
            myseq = count_shm(shmid, data);
            DPRINT("attach_shm is done! seq[%d]\n", myseq);

            nserver_started = myseq + 1;
            nserver_started = write_shm(shmid, data, nserver_started);
            DPRINT("write_shm is done! nserver_started[%d]\n", nserver_started);

            detach_shm(shmid, data);
            DPRINT("detach_shm is done! nserver_started[%d]\n",
    nserver_started);
    */

    //	DPRINT("tyring to open %s\n", shared_name);
    //	fprintf(stderr, "tyring to open %s\n", shared_name);
    /*	shared = fopen(shared_name, "a+");
            if(shared == NULL)
            {
                    fprintf(stderr, "ERROR: failed to open %s\n", shared_name);
                    failed = 1;
            } else {
                    nserver_started = 0;

                    while(EOF != fscanf(shared, "%d\n", &nserver_started)) ;
                    myseq = nserver_started;
                    nserver_started++;

                    fprintf(shared, "%d\n", nserver_started);
                    fflush(shared);

    //		DPRINT("shared_file is done! seq[%d], nserver_started[%d]\n",
    myseq, nserver_started); fprintf(stderr, "shared_file is done! seq[%d],
    nserver_started[%d]\n", myseq, nserver_started);
            }

    _failed:

            if(shared != NULL)
                    fclose(shared); */
    // file lock
    failed = 0;
    shared_fd = open(shared_name, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    if (shared_fd == -1) {
        fprintf(stderr, "ERROR: failed to open %s. errno[%d]\n", shared_name,
                errno);
        failed = 1;
    } else {
        nserver_started = 0;

        lock_file(shared_fd);
        get_current_time();
        //		DPRINT("Time[%s]: starting critical operation\n",
        //timeMsg);
        fprintf(stderr, "Time[%s]: enterring critical operation\n", timeMsg);

        lseek(shared_fd, 0L, SEEK_SET);
        rn = read(shared_fd, line, MAXLINE);
        if (rn > 0) {
            line[rn] = '\0';
            sscanf(line, "%d\n", &nserver_started);
        }
        /*		do {
                                rn = read(shared_fd, line, MAXLINE);
                                line[rn] = '\0';
                                sscanf(line, "%d\n", &nserver_started);
                        } while (rn > 0);*/

        //		while(EOF != fscanf(shared, "%d\n", &nserver_started)) ;
        myseq = nserver_started;
        nserver_started++;
        sprintf(line, "%d\n", nserver_started);

        lseek(shared_fd, 0L, SEEK_SET);
        write(shared_fd, line, strlen(line));

        fsync(shared_fd);
        //		fprintf(shared, "%d\n", nserver_started);
        //		fflush(shared);

        //		DPRINT("shared_file is done! seq[%d],
        //nserver_started[%d]\n", myseq, nserver_started);
        fprintf(stderr, "shared_file is done! seq[%d], nserver_started[%d]\n",
                myseq, nserver_started);

        get_current_time();
        //		DPRINT("Time[%s]: ending critical operation\n",
        //timeMsg);
        fprintf(stderr, "Time[%s]: exiting critical operation\n", timeMsg);
        unlock_file(shared_fd);
    }

_failed:

    if (shared_fd != -1) close(shared_fd);

    if (failed) exit(0);

    //	unlock(semid);

    /*	numRetry = 0;
            while(nserver_started != nserver && numRetry < MAX_RETRIES)
            {
                    sleep(1);
                    nserver_started = count_shm(shmid, data);
                    numRetry++;
                    DPRINT("sleep & count_shm is done! nserver_started[%d]\n",
       nserver_started);
            }

            if(nserver_started < nserver)
            {
                    DPRINT("ERROR: only [%d]/[%d] dbgsrv_alps are started!\n",
       nserver_started, nserver); return 1;
            }

            DPRINT("all[%d] dbgsrv_alps are started!\n", nserver_started);*/
    // determine my rank/pid pair
    index = myseq;

    // calloc memory for the dbgsrv argv array
    if ((dbgsrv_argv = calloc(DBGSRV_ARGC, sizeof(char *))) == (void *)0) {
        fprintf(stderr, "calloc failed.\n");
        return 1;
    }

    // set the static members of dbgsrv_argv
    dbgsrv_argv[DBGSRV_ARGV_NAME] = strdup(DBGSRV);
    dbgsrv_argv[DBGSRV_ARGV_OPT_PID] = strdup(DBGSRV_OPT_PID);
    dbgsrv_argv[DBGSRV_ARGV_PID] = (char *)NULL;
    dbgsrv_argv[DBGSRV_ARGV_OPT_HOST] = strdup(DBGSRV_OPT_HOST);
    dbgsrv_argv[DBGSRV_ARGV_HOST] = strdup(callbackHost);
    dbgsrv_argv[DBGSRV_ARGV_OPT_PORT] = strdup(DBGSRV_OPT_PORT);
    dbgsrv_argv[DBGSRV_ARGV_PORT] = strdup(callbackPort);
    dbgsrv_argv[DBGSRV_ARGV_OPT_ARCH] = strdup(DBGSRV_OPT_ARCH);
    dbgsrv_argv[DBGSRV_ARGV_ARCH] = strdup(networkArch);
    dbgsrv_argv[DBGSRV_ARGV_OPT_PROTO] = strdup(DBGSRV_OPT_PROTO);
    dbgsrv_argv[DBGSRV_ARGV_PROTO] = strdup(networkProto);
    dbgsrv_argv[DBGSRV_ARGV_OPT_DBG] = strdup(DBGSRV_OPT_DBG);
    dbgsrv_argv[DBGSRV_ARGV_DBG] = strdup(dbgBackend);
    dbgsrv_argv[DBGSRV_ARGV_OPT_PROC] = strdup(DBGSRV_OPT_PROC);
    dbgsrv_argv[DBGSRV_ARGV_PROC] = (char *)NULL;
    dbgsrv_argv[DBGSRV_ARGV_OPT_KEY] = strdup(DBGSRV_OPT_KEY);
    dbgsrv_argv[DBGSRV_ARGV_KEY] = strdup(jobKey);
    dbgsrv_argv[DBGSRV_ARGV_PROG] = strdup(prog);
    dbgsrv_argv[DBGSRV_ARGV_NULL] = (char *)NULL;

    // Launch a dbgsvr for each pid

    // write this instances rank into the pidbuff and create the str
    snprintf(pidbuff, PID_BUFSIZ, "%d", myPids->rankPidPairs[index].rank);
    dbgsrv_argv[DBGSRV_ARGV_PROC] = strdup(pidbuff);
    memset(pidbuff, 0, PID_BUFSIZ);
    // write this instances pid into the pidbuff and create the str
    snprintf(pidbuff, PID_BUFSIZ, "%d", myPids->rankPidPairs[index].pid);
    dbgsrv_argv[DBGSRV_ARGV_PID] = strdup(pidbuff);
    memset(pidbuff, 0, PID_BUFSIZ);
    //	DPRINT("app rank: %s\n", dbgsrv_argv[DBGSRV_ARGV_PROC]);
    //	DPRINT("app pid: %s\n", dbgsrv_argv[DBGSRV_ARGV_PID]);
    fprintf(stderr, "app rank: %s, app pid: %s\n",
            dbgsrv_argv[DBGSRV_ARGV_PROC], dbgsrv_argv[DBGSRV_ARGV_PID]);

#ifdef DEBUG
    FILE *log = (FILE *)NULL;
    // create a log if debug is turned on
    if ((log = create_log(myPids->rankPidPairs[index].rank, nidName)) ==
        (FILE *)NULL) {
        fprintf(stderr, "Could not create dbgsrv logfile.\n");
    }
#endif

#ifdef BE_VALGRIND
    FILE *valgrind_log = (FILE *)NULL;
    char valgrind_log_str[BUFSIZ];
    char **valgrind_argv;

    // create a log if valgrind is turned on
    if ((valgrind_log = create_log(index, "valgrind")) == (FILE *)NULL) {
        fprintf(stderr, "Could not create valgrind logfile.\n");
    }
    snprintf(valgrind_log_str, BUFSIZ, "--log-fd=%d", fileno(valgrind_log));

    // create an argv for valgrind
    valgrind_argv =
        NewArgv("valgrind", "--read-var-info=yes", "--track-origins=yes",
                "--leak-check=full", valgrind_log_str, NULL);
    valgrind_argv = AppendArgv(valgrind_argv, dbgsrv_argv);
#endif

    get_current_time();
    DPRINT("Time[%s]: starting fork\n", timeMsg);

    // fork and exec
    mypid = fork();

    get_current_time();
    DPRINT("Time[%s]: ending fork\n", timeMsg);

    // error case
    if (mypid < 0) {
        fprintf(stderr, "Fatal fork error.\n");
        // cleanup dbgsrv_argv array
        tmp = dbgsrv_argv;
        while (*tmp != (char *)NULL) {
            free(*tmp++);
        }
        free(dbgsrv_argv);
        // cleanup pid list
        destroy_nodeAppPidList(myPids);
#ifdef DEBUG
        // close logfile
        close_log(log);
        // cleanup nidName
        free(nidName);
#endif
#ifdef BE_VALGRIND
        // close valgrind logfile
        close_log(valgrind_log);
        // cleanup argv
        FreeArgv(valgrind_argv);
#endif

        return 1;
    }

    // child case
    if (mypid == 0) {
        sprintf(path, "/var/spool/alps/%ld/toolhelper%ld", apid, apid);
        setenv("PATH", path, 1);

#ifdef DEBUG
        // if debug is on, hook into the logfile
        if (log != (FILE *)NULL) hook_stdoe(log);
#endif
#ifdef BE_VALGRIND
        // exec valgrind
        execvp("valgrind", valgrind_argv);
#else
        // exec dbgsrv
        execvp(DBGSRV, dbgsrv_argv);
#endif

        // exec shouldn't return
        fprintf(stderr, "Return from exec.\n");
        exit(1);
    }

    // parent case
    free(dbgsrv_argv[DBGSRV_ARGV_PID]);
    free(dbgsrv_argv[DBGSRV_ARGV_PROC]);

    DPRINT("Done launching dbgsrv [%d]\n", index);

    // free the dbgsrv_argv array
    free(dbgsrv_argv[DBGSRV_ARGV_NAME]);
    free(dbgsrv_argv[DBGSRV_ARGV_OPT_PID]);
    free(dbgsrv_argv[DBGSRV_ARGV_OPT_HOST]);
    free(dbgsrv_argv[DBGSRV_ARGV_HOST]);
    free(dbgsrv_argv[DBGSRV_ARGV_OPT_PORT]);
    free(dbgsrv_argv[DBGSRV_ARGV_PORT]);
    free(dbgsrv_argv[DBGSRV_ARGV_OPT_ARCH]);
    free(dbgsrv_argv[DBGSRV_ARGV_ARCH]);
    free(dbgsrv_argv[DBGSRV_ARGV_OPT_PROTO]);
    free(dbgsrv_argv[DBGSRV_ARGV_PROTO]);
    free(dbgsrv_argv[DBGSRV_ARGV_OPT_DBG]);
    free(dbgsrv_argv[DBGSRV_ARGV_DBG]);
    free(dbgsrv_argv[DBGSRV_ARGV_OPT_PROC]);
    free(dbgsrv_argv[DBGSRV_ARGV_PROG]);

    //#ifdef DEBUG
    // cleanup nidName
    free(nidName);
    //#endif

    //	if(myseq == (nserver - 1))
    if (nserver_started == nserver) {
        // cleanup pid list
        destroy_nodeAppPidList(myPids);

        //		rm_shm(shmid, data);
        //		DPRINT("rm_shm is done!\n");
        //		fprintf(stderr, "rm_shm is done!\n");

        //		res = rm_sem(semid);
        //		DPRINT("rm_sem is done!\n");
        //		fprintf(stderr, "rm_sem is done!\n");
    }

    get_current_time();
    //	DPRINT("Time[%s]: dbgsrv_alps is done!\n", timeMsg);
    fprintf(stderr, "Time[%s]: dbgsrv_alps is done!\n", timeMsg);

    return 0;
}
