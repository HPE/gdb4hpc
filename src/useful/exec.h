/*********************************************************************************\
 * Copyright 2023 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#pragma once

#include "args.h"

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <optional>
#include <memory>

// wrapper around the result of a ::pipe syscall with some provided utilities
// for file descriptor modifying syscalls.
//
// closes both ends of the pipe on destruction. use the move constructor to keep
// the pipe alive between scopes.
class Pipe {
public:
    enum class End: unsigned {
        Read = 0,
        Write = 1,
    };

    // create a Pipe.
    // return empty if the pipe creation failed, e.g. from a failed syscall.
    static std::optional<Pipe> make();

    Pipe(Pipe&& expiring);
    ~Pipe();

    // read `len` bytes from the read end of the pipe into `buffer`.
    // return the number of bytes read.
    //
    // if the read end of the pipe is non-blocking and the underlying ::read
    // returns EAGAIN, this function will attempt to read again after a short
    // (< 1s) delay. after enough successive EAGAIN failures, the function will
    // stop attempts and return the amount of bytes that it was able to
    // successfully read.
    size_t read(char* buffer, size_t len);

    // write `len` bytes from `buffer` to the write end of the pipe.
    // return the number of bytes written.
    //
    // if the write end of the pipe is non-blocking and the underlying ::write
    // returns EAGAIN, this function will attempt to write again after a short
    // (< 1s) delay. after enough successive EAGAIN failures, the function will
    // stop attempts and return the amount of bytes that it was able to
    // sucessfully write.
    //
    // SIGPIPE is ignored during the duration of this function.
    size_t write(const char* buffer, size_t len);

    // call ::dup2(end, fd) and return the result.
    int dup2(End end, int fd);

    // set the FD_CLOEXEC file descriptor flag on the specified pipe `end`.
    // this function makes multiple calls to ::fcntl. on success, it will return
    // 0, and on failure, it will return the result of the first failing ::fcntl
    // call.
    int setCloseOnExec(End end);

    // call ::close(end) and return the result.
    int close(End end);

private:
    Pipe(int read_end, int write_end);

    int m_pipe_fds[2];
};

// wrapper around a fork'd and exec'd subprocess. the subprocess shares stdout
// and stderr with the parent (calling) process, and maps stdin to the
// `stdin_pipe` member.
//
// SIGKILLs the underlying process on destruction. use the move constructor to
// keep the process alive between scopes.
class SharedForegroundSubprocess {
public:
    // create a subprocess by forking and execing `argv`.
    // return empty if the subprocess creation failed, e.g. from a failed syscall.
    static std::unique_ptr<SharedForegroundSubprocess> make(const ArgvStream& argv);

    SharedForegroundSubprocess(SharedForegroundSubprocess&& expring);
    ~SharedForegroundSubprocess();

    // wait forever for the subprocess to exit.
    void waitForExit();

    Pipe stdin_pipe;
private:
    SharedForegroundSubprocess(pid_t pid, Pipe&& in_pipe);

    pid_t m_pid;
};
