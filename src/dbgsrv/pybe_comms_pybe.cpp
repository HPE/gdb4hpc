// Copyright 2024 Hewlett Packard Enterprise Development LP.

#include "pybe_comms_pybe.h"

namespace pybe {

Frame to_frame(PyFrameObject& pyframe) {
    auto code = StrongRef(PyFrame_GetCode(&pyframe));

    // note that PyUnicode_AsUTF8 does not give us a pointer that we have to
    // free. it generates the internal utf8 representation of the object, caches
    // it, then returns a pointer to that. it will be freed with the rest of the
    // code object.
    //
    // XXX: these member accesses are not ABI portable and might break with a
    // python update
    return Frame{
        .function_name = String(PyUnicode_AsUTF8(code->co_name)),
        .file_name = String(PyUnicode_AsUTF8(code->co_filename)),
        .line_number = PyFrame_GetLineNumber(&pyframe),
    };
}

} // namespace pybe
