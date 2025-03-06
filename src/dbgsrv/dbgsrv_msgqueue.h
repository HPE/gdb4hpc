// Copyright 2024-2025 Hewlett Packard Enterprise Development LP.

#include "MISession.h"

#include <memory>

namespace mqd {

struct MQD_Impl;

class MQD {
public:
    // interface to MPI's message queue debugging library.
    //
    // currently nothing is implemented.

    explicit MQD(const char* mqd_so_path, MISession& mi);
    ~MQD();

private:
    std::unique_ptr<MQD_Impl> m_impl;
};

} // namespace mqd
