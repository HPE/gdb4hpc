// Copyright 2024 Hewlett Packard Enterprise Development LP.

#include "pybe_comms_dbgsrv.h"

std::unique_ptr<dbgframe_t> to_dbgframe(const pybe::Frame& frame) {
    // dbgloc owns the ptrs given to it
    auto location = dbgloc(::strdup(frame.location.file_name.c_str()),
                           ::strdup(frame.location.function_name.c_str()),
                           ::strdup("") /* address */,
                           frame.location.line_number);
    return std::make_unique<dbgframe_t>(frame.level, location, nullptr, nullptr, nullptr);
}

std::unique_ptr<dbgframe_t> to_dbgframe(const pybe::Backtrace& bt) {
    // XXX: some raw pointers are thrown around here since dbgframe_t uses them internally
    // and it's cumbersome to keep creating unique ptrs only to immediately release them when
    // building the linked list.

    auto first_frame = static_cast<dbgframe_t*>(nullptr); // "first" meaning "deepest"
    auto* last_frame = static_cast<dbgframe_t*>(nullptr); // "last" meaning "shallowest" (e.g. main)

    for (const auto& py_frame : bt.frames) {
        // dbgloc owns the ptrs given to it
        auto* frame = to_dbgframe(py_frame).release();

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
        first_frame = new dbgframe_t(/*level=*/ 0, location, nullptr, nullptr, nullptr);
    }

    return std::unique_ptr<dbgframe_t>(first_frame);
}

std::unique_ptr<dbgbp_t> to_dbgbp(const pybe::Breakpoint& bp, bool pending) {
    auto dbgbp = std::make_unique<dbgbp_t>(bp.feid, 0);

    dbgbp->bp_loc.loc_file = !bp.location.file_name.empty() ? ::strdup(bp.location.file_name.c_str()) : nullptr;
    dbgbp->bp_loc.loc_line = bp.location.line_number;
    dbgbp->bp_loc.loc_func = !bp.location.function_name.empty() ? ::strdup(bp.location.function_name.c_str()) : nullptr;

    dbgbp->bp_pending = pending;
    // XXX: in order for the frontend to consider a breakpoint as pending, it needs to have
    //      a nonempty bp_stmt too
    if (dbgbp->bp_pending) {
        if (dbgbp->bp_loc.loc_file && dbgbp->bp_loc.loc_line > 0) {
            dbgbp->bp_stmt = std::string{dbgbp->bp_loc.loc_file} + ":" + std::to_string(dbgbp->bp_loc.loc_line);
        } else if (dbgbp->bp_loc.loc_func) {
            dbgbp->bp_stmt = dbgbp->bp_loc.loc_func;
        } else {
            // set something so the frontend will still display it as a pending breakpoint.
            // "Pending at unknown locaion"
            dbgbp->bp_stmt = "unknown location";
        }
    }

    dbgbp->bp_type = DBGBP_BREAK_TYPE;
    dbgbp->bp_disp = DBGBP_KEEP_DISP;

    return dbgbp;
}

