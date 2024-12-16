// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// specialize pybe_comms for dbgsrv use

#pragma once

#define I_HAVE_PERMISSION_TO_INCLUDE_PYBE_COMMS
#include "pybe_comms.h"
#undef I_HAVE_PERMISSION_TO_INCLUDE_PYBE_COMMS

#include <vector>
#include <string>
#include <string_view>

#include "dbgevent.h"

namespace pybe::detail {

    // Plugging std::vector directly into the CommsTs template makes the compiler
    // unhappy for some reason, citing that std::vector has a different number of
    // template arguments than the pybe::Vector_T we are plugging it in to. I think
    // this has to do with default template arguments, but I'm not sure. If that was
    // true, it should happen for std::string too, but it doesn't? Either way,
    // defining a vector type with only one template parameter gets it to work.
    template <typename T>
    using DefaultVector = std::vector<T>;

    using DbgsrvLowLevels = pybe::detail::CommsLowLevelTs<std::string, std::string_view, DefaultVector>;

} // namespace pybe::detail

namespace pybe {
    using Channel = detail::Channel_T<detail::DbgsrvLowLevels>;
    using Server = detail::Server_T<detail::DbgsrvLowLevels>;

    using Frame = detail::Frame_T<detail::DbgsrvLowLevels>;
    using Backtrace = detail::Backtrace_T<detail::DbgsrvLowLevels>;
    using Breakpoint = detail::Breakpoint_T<detail::DbgsrvLowLevels>;
};

std::unique_ptr<dbgframe_t> to_dbgframe(const pybe::Frame& frame);
std::unique_ptr<dbgframe_t> to_dbgframe(const pybe::Backtrace& bt);
std::unique_ptr<dbgbp_t> to_dbgbp(const pybe::Breakpoint& bp, bool pending);
