// Copyright 2024-2025 Hewlett Packard Enterprise Development LP.

#pragma once

#include "MISession.h"
#include "dbgsrv_msgqueue_types.h"

#include <memory>
#include <vector>

namespace mqd {

struct PendingOperation {
    // a PendingOperation is an entry in one of MPI's pending operation queues
    // with commonly used metadata attached.

    // `op` is the raw pending operation directly from an MPI queue.
    mqs_pending_operation op;

    // `queue` is the queue that `op` originally came from.
    mqs_op_class queue;

    // `comm` is the communicator that was being examined when `op` was found.
    mqs_communicator comm;
};

class MQD {
public:
    // interface to MPI's message queue debugging library.

    MQD(const std::string& mqd_so_path, int my_rank, MISession& mi);
    ~MQD();

    // `get_communicators` returns a list of the communicators that currently exist.
    std::vector<mqs_communicator_group> get_communicators();

    // `get_pending_operations` returns a list of the currently pending MPI operations.
    // these are the message queues in message queue debugging.
    std::vector<PendingOperation> get_pending_operations();

    // opaque internal global state
    struct State;

private:
    std::unique_ptr<State> m_impl;
};

} // namespace mqd
