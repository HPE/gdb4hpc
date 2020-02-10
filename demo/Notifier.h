#pragma once

#include "Stream.h"

class Notifier 
{
public:
    Notifier() = default;
    virtual ~Notifier() = default;

    virtual void notify(Stream::State) = 0;
};
