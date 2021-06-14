/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include <mutex>
#include <condition_variable>
#include <queue>

#include "zmqnet.hpp"

enum class ZMQNetStream : zmqnet::Stream::Id {
    CommandStream,
    EventStream,
    IOStream
};
