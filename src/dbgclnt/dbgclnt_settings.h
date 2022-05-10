/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <string>

// abstracted interface for struct clnt_settings so that layers
// below don't need to know the details of the whole client layer
struct ClientSettingsInterface {
    virtual std::string getProto() = 0;
    virtual std::string getArch() = 0;
    virtual std::string getBackend() = 0;
    virtual bool isUnsafe() = 0;
    virtual bool isEarlyEntry() = 0;
    virtual bool isNonStop() = 0;
};

