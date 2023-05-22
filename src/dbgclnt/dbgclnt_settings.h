/*******************************************************************************
 * Copyright 2022-2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <string>

// abstracted interface for struct clnt_settings so that layers
// below don't need to know the details of the whole client layer
struct ClientSettingsInterface {
    virtual std::string getProto() = 0;
    virtual std::string getBackend() = 0;
    virtual bool isUnsafe() = 0;
    virtual bool isEarlyEntry() = 0;
    virtual bool isNonStop() = 0;
};

struct MockClientSettings : public ClientSettingsInterface {
    // Used in unit tests

    std::string getProto() override { return "mock_zmqnet"; }
    std::string getBackend() override { return "mock_gdbmi"; }
    // these should be opposite to the defaults of LaunchOpts for testing
    bool isUnsafe() override { return false; }
    bool isEarlyEntry() override { return true; }
    bool isNonStop() override { return true; }
};

