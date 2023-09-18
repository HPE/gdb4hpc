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

#include "exec.h"
#include "signal_util.h"

#include <fcntl.h>
#include <stdio.h>

#include <thread>
#include <chrono>

std::optional<Pipe> Pipe::make() {
    int fds[2];
    if (::pipe(fds)) return {};

    return Pipe(fds[0], fds[1]);
}

Pipe::Pipe(int read_end, int write_end)
    : m_pipe_fds{read_end, write_end}
{}

Pipe::Pipe(Pipe&& expiring)
    : m_pipe_fds{expiring.m_pipe_fds[0], expiring.m_pipe_fds[1]}
{
    expiring.m_pipe_fds[0] = -1;
    expiring.m_pipe_fds[1] = -1;
}

Pipe::~Pipe() {
    close(End::Read);
    close(End::Write);
}

size_t Pipe::read(char* buffer, size_t len) {
    auto bytes_read = size_t{0};
    auto successive_failures = 0;

    // note: 3 sucessive_failures is about .75 seconds, but
    // *only if the pipe is set to non-blocking.*

    while (bytes_read < len && successive_failures < 3) {
        auto rc = ::read(m_pipe_fds[0], buffer, len);
        if (rc > 0) {
            bytes_read += rc;
            successive_failures = 0;
        } else {
            switch (errno) {
                case EAGAIN:
                    successive_failures += 1;
                    // fall through
                case EINTR: {
                    using namespace std::chrono_literals;
                    std::this_thread::sleep_for(250ms);
                    continue;
                }

                // assume all other errors are fatal
                default: return bytes_read;
            }
        }
    }

    return bytes_read;
}

void ignoreSIGPIPE(int) {
    // ignore...
}

size_t Pipe::write(const char* buffer, size_t len) {
    auto bytes_written = size_t{0};
    auto successive_failures = size_t{0};

    // SIGPIPE is sent if read end is closed when we try to write
    TmpSignalHandler sigpipe_handler(SIGPIPE, ignoreSIGPIPE);

    // note: 3 sucessive_failures is about .75 seconds, but
    // *only if the pipe is set to non-blocking.*

    while (bytes_written < len && successive_failures < 3) {
        auto rc = ::write(m_pipe_fds[1], buffer + bytes_written, len - bytes_written);
        if (rc > 0) {
            bytes_written += rc;
            successive_failures = 0;
        } else {
            switch (errno) {
                case EAGAIN:
                    successive_failures += 1;
                    // fall through
                case EINTR: {
                    using namespace std::chrono_literals;
                    std::this_thread::sleep_for(250ms);
                    continue;
                }

                // assume all other errors are fatal
                default: return bytes_written;
            }
        }
    }

    return bytes_written;
}

int Pipe::dup2(End end, int fd) {
    return ::dup2(m_pipe_fds[static_cast<unsigned>(end)], fd);
}

int Pipe::setCloseOnExec(End end) {
    const auto e = static_cast<unsigned>(end);

    const auto current_flags = ::fcntl(m_pipe_fds[e], F_GETFD);
    if (current_flags == -1) return -1;

    return ::fcntl(m_pipe_fds[e], F_SETFD, current_flags | FD_CLOEXEC);
}

int Pipe::close(End end) {
    const auto e = static_cast<unsigned>(end);
    const auto rc = ::close(m_pipe_fds[e]);
    if (rc == 0)
        m_pipe_fds[e] = -1;
    return rc;
}

SharedForegroundSubprocess::SharedForegroundSubprocess(pid_t pid, Pipe&& in_pipe)
    : stdin_pipe{std::move(in_pipe)}
    , m_pid{pid}
{}

SharedForegroundSubprocess::SharedForegroundSubprocess(SharedForegroundSubprocess&& expiring)
    : stdin_pipe{std::move(expiring.stdin_pipe)}
    , m_pid{expiring.m_pid}
{
    expiring.m_pid = -1;
}

SharedForegroundSubprocess::~SharedForegroundSubprocess() {
    if (m_pid != -1) {
        ::kill(m_pid, SIGKILL);
        waitForExit();
    }
}

void SharedForegroundSubprocess::waitForExit() {
    while (true) {
        int status;
        const auto r = ::waitpid(m_pid, &status, 0);

        if (r > 0) {
            if (WIFEXITED(status) || WIFSIGNALED(status))
                break;
            // otherwise, pid's status changed for some other reason
            // (e.g. SIGCONT) and it's still alive.
        } else if (errno == EINTR) {
            continue;
        } else {
            // all other error cases mean the process is gone
            break;
        }
    }

    m_pid = -1;
}

std::unique_ptr<SharedForegroundSubprocess> SharedForegroundSubprocess::make(const ArgvStream& argv) {
    auto in_pipe = Pipe::make();
    auto health_pipe = Pipe::make();
    if (!in_pipe || !health_pipe) return nullptr;

    // set close on exec on the health pipe to let us confirm from the parent
    // process if the execvp is sucessful
    health_pipe->setCloseOnExec(Pipe::End::Write);

    const auto pid = ::fork();
    if (pid < 0) return nullptr;

    if (pid == 0) {
        health_pipe->close(Pipe::End::Read);
        in_pipe->close(Pipe::End::Write);
        in_pipe->dup2(Pipe::End::Read, STDIN_FILENO);

        if (argv.argc() > 0)
            ::execvp(const_cast<char *>(argv[0].c_str()),
                     const_cast<char **>(argv.c_strs().data()));

        // failed to exec
        health_pipe->write("x", 1);
        ::_exit(-1);
    } else {
        health_pipe->close(Pipe::End::Write);
        in_pipe->close(Pipe::End::Read);

        // if the execvp is sucessful, the other end of the the health pipe will
        // be closed by the subprocess and read will read nothing and return 0.
        // if the execvp failes, we'll read the byte the child process sends
        // before exiting.
        char is_dead[1] = {'\0'};
        health_pipe->read(is_dead, 1);
        if (is_dead[0] != '\0')
            return nullptr;

        return std::unique_ptr<SharedForegroundSubprocess>(
            new SharedForegroundSubprocess(pid, std::move(*in_pipe)));
    }
}
