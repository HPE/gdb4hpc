// Copyright 2024-2025 Hewlett Packard Enterprise Development LP.

#pragma once

#include "MISession.h"
#include "dbgsrv_msgqueue_types.h"

#include <memory>
#include <vector>

namespace mqd {

class MQD {
public:
    // interface to MPI's message queue debugging library.
    //
    // currently nothing is implemented.

    MQD(const std::string& mqd_so_path, MISession& mi);
    ~MQD();

    std::vector<Communicator> get_communicators();

    // opaque internal global state
    struct State;

private:
    std::unique_ptr<State> m_impl;
};

} // namespace mqd
