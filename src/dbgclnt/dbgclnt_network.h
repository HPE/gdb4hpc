/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

class DebugClientNetwork {
    // Protocol specific frontend networks do not have a unified interface other than the
    // fact that they will have some cleaning up to do when they are deleted.
public:
    virtual ~DebugClientNetwork() = 0;
};
