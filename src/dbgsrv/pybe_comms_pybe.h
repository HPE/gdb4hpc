// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// specialize pybe_comms for pybe use

#pragma once

#include "pybe_stl.h"

#define I_HAVE_PERMISSION_TO_INCLUDE_PYBE_COMMS
#include "pybe_comms.h"
#undef I_HAVE_PERMISSION_TO_INCLUDE_PYBE_COMMS

namespace pybe::detail {
    // String/Vector aliases to TINYSTL defined in pybe_stl.h
    using LowLevels = pybe::detail::CommsLowLevelTs<String, StringView, Vector>;

} // namespace pybe::detail


namespace pybe {
    using Channel = detail::Channel_T<detail::LowLevels>;
    using Client = detail::Client_T<detail::LowLevels>;

    using Frame = detail::Frame_T<detail::LowLevels>;
    using Backtrace = detail::Backtrace_T<detail::LowLevels>;

} // namespace pybe;
