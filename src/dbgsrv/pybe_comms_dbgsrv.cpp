// Copyright 2024 Hewlett Packard Enterprise Development LP.

#include "pybe_comms_dbgsrv.h"

std::unique_ptr<dbgframe_t> to_dbgframe(const pybe::Frame& frame, int level) {
    // dbgloc owns the ptrs given to it
    auto location = dbgloc(::strdup(frame.file_name.c_str()),
                           ::strdup(frame.function_name.c_str()),
                           ::strdup("") /* address */,
                           frame.line_number);
    return std::make_unique<dbgframe_t>(level, location, nullptr, nullptr, nullptr);
}

std::unique_ptr<dbgframe_t> to_dbgframe(const pybe::Backtrace& bt) {
    // XXX: some raw pointers are thrown around here since dbgframe_t uses them internally
    // and it's cumbersome to keep creating unique ptrs only to immediately release them when
    // building the linked list.

    int current_level = 0;
    auto first_frame = static_cast<dbgframe_t*>(nullptr); // "first" meaning "deepest"
    auto* last_frame = static_cast<dbgframe_t*>(nullptr); // "last" meaning "shallowest" (e.g. main)

    for (const auto& py_frame : bt.frames) {
        // dbgloc owns the ptrs given to it
        auto* frame = to_dbgframe(py_frame, current_level).release();
        current_level++;

        if (!first_frame) {
            first_frame = frame;
            last_frame = frame;
        } else {
            last_frame->setNext(frame);
            last_frame = frame;
        }
    }

    if (first_frame == nullptr) {
        // if the user requests a backtrace before python has started, we won't
        // get any frames. in that case give the user something informative
        auto location = dbgloc(nullptr, ::strdup("No python backtrace"), nullptr, 0);
        first_frame = new dbgframe_t(0, location, nullptr, nullptr, nullptr);
    }

    return std::unique_ptr<dbgframe_t>(first_frame);
}

std::unique_ptr<dbgbp_t> to_dbgbp(const pybe::Breakpoint& bp) {
    auto dbgbp = std::make_unique<dbgbp_t>(bp.feid, 0);

    dbgbp->bp_loc.loc_file = !bp.location.file_name.empty() ? ::strdup(bp.location.file_name.c_str()) : nullptr;
    dbgbp->bp_loc.loc_line = bp.location.line_number;
    dbgbp->bp_loc.loc_func = !bp.location.function_name.empty() ? ::strdup(bp.location.function_name.c_str()) : nullptr;

    dbgbp->bp_type = DBGBP_BREAK_TYPE;
    dbgbp->bp_disp = DBGBP_KEEP_DISP;

    return dbgbp;
}

