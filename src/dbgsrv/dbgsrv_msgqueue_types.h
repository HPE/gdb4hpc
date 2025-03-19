// Copyright 2024-2025 Hewlett Packard Enterprise Development LP.
//
// low level types used to communicate with the message queue debugging shared library
//
// the following mqs_ types are copied from the whitepaper (https://www.mpi-forum.org/docs/msgq.5.pdf)

#pragma once

namespace mqd {

// types for target independence (whitepaper section 5.2.*)
//
// the MQD shared library is equipped to handle the case where the host
// (dbgsrv) is running on a different architecture than the target (MPI
// application). the whitepaper's given example is MPI might be compiled as 32
// bit and dbgsrv as 64 bit. to facilitate this, the MQD shared library needs
// to know the sizes of certain types on the host (dbgsrv). we don't support
// mixed architectures on cray machines either way, but we still need to define
// the types for the library to work.

using mqs_tword_t = long; // whitepaper section 5.2.1
using mqs_taddr_t = unsigned long; // whitepaper section 5.2.2

struct mqs_target_type_sizes {
    // as described in whitepaper section 5.2.3.
    int short_size = sizeof(short);
    int int_size = sizeof(int);
    int long_size = sizeof(long);
    int long_long_size = sizeof(long long);
    int pointer_size = sizeof(void*);
};

// constants and enums (whitepaper section 5.4)

enum mqs_lang_code {
    // as described in whitepaper section 5.4.1
    mqs_lang_c = 'c',
    mqs_lang_cplus = 'C',
    mqs_lang_f77 = 'f',
    mqs_lang_f90 = 'F'
};

enum mqs_op_class {
    // as described in whitepaper section 5.4.2
    mqs_pending_sends,
    mqs_pending_receives,
    mqs_unexpected_messages
};

enum {
    // weird unnamed single item enum, but copied directly from whitepaper section 5.4.3
    MQS_INTERFACE_COMPATIBILITY = 2,
};

enum mqs_status {
    // as described in whitepaper section 5.4.4
    mqs_st_pending,
    mqs_st_matched,
    mqs_st_complete
};

enum mqs_result {
    // as described in whitepaper section 5.4.5
    mqs_ok = 0,
    mqs_no_information,
    mqs_end_of_list,
    mqs_first_user_code = 100
};

enum {
    // weird unnamed single item enum, but copied directly from whitepaper section 5.4.6
    MQS_INVALID_PROCESS = -1
};

// concrete types (whitepaper section 5.5)

struct mqs_communicator {
    // as described in whitepaper section 5.5.1
    //
    // comments in quotes are directly from the whitepaper.

    // "unique_id uniquely identifies a communicator"
    mqs_taddr_t unique_id;

    // "local_rank identifies the rank of the current MPI process in this communicator."
    mqs_tword_t local_rank;

    // "size holds the size of the communicator."
    mqs_tword_t size;

    // "name contains the name of the communicator if it was given one."
    //
    // in practice, this seems to be "--unamed--" if there is no name,
    // but that's not guaranteed anywhere.
    char name[64];
};

struct mqs_pending_operation {
    // as described in whitepaper section 5.5.2
    //
    // comments in quotes are directly from the whitepaper.
    // some fields are only valid in certain situations (descibed in comments).

    ////////////////////////////////////////////////////////////////////////////
    // "fields for all messages" ///////////////////////////////////////////////

    // "status stores the status of the message. The value of this field should be either mqs_-
    // st_pending, mqs_st_matched, or mqs_st_complete as described in the enumeration
    // mqs_status (section 5.4.4)."
    int status;

    // "desired_local_rank stores the rank of the target or the source for the communicator
    // from which this message was initiated."
    mqs_tword_t desired_local_rank;

    // "desired_global_rank stores the rank of the target or the source with respect to
    // MPI_COMM_WORLD."
    mqs_tword_t desired_global_rank;

    // "tag_wild is a boolean that identifies whether this message is a posted receive with
    // tag MPI_ANY_TAG. If the receive was posted with MPI_ANY_TAG, tag_wild will be set
    // to 1. Otherwise, it it set to 0."
    int tag_wild;

    // "desired_tag holds the tag of the message. This field is ignored if tag_wild is not set."
    mqs_tword_t desired_tag;

    // "desired_length holds the length of the message buffer in bytes."
    mqs_tword_t desired_length;

    // "system_buffer is a boolean that identifies whether this is a user or a system buffer.
    // A value of 1 indicates that it is a system buffer. Otherwise, it is set to 0."
    int system_buffer;

    // "buffer holds the address to the beginning of the message data."
    mqs_taddr_t buffer;

    ////////////////////////////////////////////////////////////////////////////
    // "fields valid if status >= matched or it is a send" /////////////////////
    //
    // "the following fields are only meaningful if the message is a send or if the status fields
    // indicates that this message is either matched (mqs_st_matched), or completed (mqs_st_
    // complete)."

    // "actual_local_rank holds the actual local rank within the communicator (after the
    // message has matched)."
    mqs_tword_t actual_local_rank;

    // "actual_global_rank holds the actual global rank with respect to
    // MPI_COMM_WORLD."
    mqs_tword_t actual_global_rank;

    // "actual_tag holds the actual tag of the message."
    mqs_tword_t actual_tag;

    // "actual_length holds the actual length of the buffer in bytes."
    mqs_tword_t actual_length;

    // "extra_text is an array of five null-terminated strings that can be used by the MQD
    // DLL to provide more information to the user. The debugger does not interpret this
    // field and can display it to the user. This field can be used to give the name of the
    // function causing this request, for example."
    char extra_text[5][64];
};

// names that fit our codebase style better

using Communicator = mqs_communicator;
using PendingOperation = mqs_pending_operation;

} // namespace mqd
