// Copyright 2024-2025 Hewlett Packard Enterprise Development LP.

#include <cstdio>
#include <dlfcn.h>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>

#include "dbgsrv_defs.h"
#include "mi_utils.h"
#include "str.h"

#include "dbgsrv_msgqueue.h"

namespace {

template <typename FN>
void try_send_command_impl(MISession& sess, MICommand& cmd, FN fail_callback, const char* file, int line) {
    DVPRINT("mqd mi command from %s:%d\n", file, line);
    SendMICommand(&sess, &cmd);
    if (!MICommandResultOK(&cmd)) {
        DVPRINT("mqd mi command failed: %s:%d\n", file, line);
        fail_callback();
    }
}

// using MISession& `sess`, run the mi command `cmd` and check
// MICommandResultOK. if it's not ok, call `fail_callback`.
//
// this is a macro so __file__ and __line__ reflect the callsite
#define try_send_command(sess, cmd, fail_callback) try_send_command_impl(sess, cmd, fail_callback, __FILE__, __LINE__);

} // anon namespace

namespace mqd {

// the following mqs_ types are copied from the whitepaper (https://www.mpi-forum.org/docs/msgq.5.pdf).

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

// opaque types (whitepaper section 5.3)
//
// these are opaque types used by the MQD shared library.
// we just need to pass them around.
using mqs_image_info = void;
using mqs_process_info = void;

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

// end of whitepaper types

struct MQD_Impl;

// Type, Image, and Process are a containers for state associated with the MQD
// shared library's calls to functions that start with an
// mqs_type*/mqs_image*/mqs_process argument. for example, Type::field_offset is
// what is ultimately called by the mqs_field_offset(mqs_type* type, char* name)
// callback that is set up below in ImageCallbacks. the Type object that implements
// the function is created by the MQD shared library calling ImageCallbacks::find_type.
// and passed as the first argument of mqs_field_offset.

struct Type {
    int field_offset(char* fname);
    int get_sizeof();

    std::string m_name;
    MQD_Impl& m_mqd_impl;

    bool operator<(const Type& other) const;
};

struct Image;

struct Process {
    int get_global_rank();
    Image* get_image();
    int fetch_data(mqs_taddr_t /* addr */, int /* size */, void* /* buf */);
    void target_to_host(const void* /* indata */, void* /* outdata */, int /* size */);
    void get_type_sizes(mqs_target_type_sizes* /* sizes */);

    Image& m_image;
    MQD_Impl& m_mqd_impl;
};

struct Image {
    int find_function(char* fname, mqs_lang_code lang, mqs_taddr_t* out_addr);
    int find_symbol(char* sname, mqs_taddr_t* out_addr);
    const Type* find_type(char* tname, mqs_lang_code lang);

    // updated by find_type
    std::set<Type> m_valid_types;

    MQD_Impl& m_mqd_impl;
};

using mqs_image = Image;
using mqs_process = Process;
using mqs_type = Type;

// callback implementations from whitepaper section 5.6
//
// the whitepaper asks the debugger to implement callbacks by passing a struct
// of function pointers to the MQD shared library during setup. these stucts
// contain no state, they only serve as a list of function pointers for the MQD
// shared library to use.

struct BasicCallbacks {
    // implements mqs_basic_callbacks struct specified in whitepaper section 5.6.1

    // signatures for basic callbacks specified in whitepaper sections 5.6.2 - 5.6.9
    using mqs_malloc_ft = void* (*)(size_t size);
    using mqs_free_ft = void (*)(void* buf);
    using mqs_dprints_ft = void (*)(const char* msg);
    using mqs_errorstring_ft = const char* (*)(int error_code);
    using mqs_put_image_info_ft = void (*)(mqs_image* image, mqs_image_info* info);
    using mqs_get_image_info_ft = mqs_image_info* (*)(mqs_image* image);
    using mqs_put_process_info_ft = void (*)(mqs_process* process, mqs_process_info* info);
    using mqs_get_process_info_ft = mqs_process_info* (*)(mqs_process* process);

    // actual callback implementations
    static void* malloc(size_t size);
    static void free(void* buf);
    static void dprints(const char* msg);
    static const char* errorstring(int error_code);
    static void put_image_info(mqs_image* image, mqs_image_info* info);
    static mqs_image_info* get_image_info(mqs_image* image);
    static void put_process_info(mqs_process* process, mqs_process_info* info);
    static mqs_process_info* get_process_info(mqs_process* process);

    // function pointers used by MQD shared library
    mqs_malloc_ft mqs_malloc_fp = BasicCallbacks::malloc;
    mqs_free_ft mqs_free_fp = BasicCallbacks::free;
    mqs_dprints_ft mqs_dprints_fp = BasicCallbacks::dprints;
    mqs_errorstring_ft mqs_errorstring_fp = BasicCallbacks::errorstring;
    mqs_put_image_info_ft mqs_put_image_info_fp = BasicCallbacks::put_image_info;
    mqs_get_image_info_ft mqs_get_image_info_fp = BasicCallbacks::get_image_info;
    mqs_put_process_info_ft mqs_put_process_info_fp = BasicCallbacks::put_process_info;
    mqs_get_process_info_ft mqs_get_process_info_fp = BasicCallbacks::get_process_info;
};

struct ImageCallbacks {
    // implements mqs_image_callbacks in whitepaper section 5.6.10
    //
    // note that despite the name, some of these callbacks deal with Processes and Types.

    // signatures for image callbacks specified in whitepaper sections 5.6.11 - 5.6.16
    using mqs_get_type_sizes_ft = void (*)(mqs_process* process, mqs_target_type_sizes* sizes);
    using mqs_find_function_ft = int (*)(mqs_image* image, char* fname, mqs_lang_code lang, mqs_taddr_t* addr);
    using mqs_find_symbol_ft = int (*)(mqs_image* image, char* sname, mqs_taddr_t* addr);
    using mqs_find_type_ft = const mqs_type* (*)(mqs_image* image, char* tname, mqs_lang_code lang);
    using mqs_field_offset_ft = int (*)(mqs_type* type, char* fname);
    using mqs_sizeof_ft = int (*)(mqs_type* type);

    // actual callback implementations. these forward to member functions of Image/Process/Type.
    static void get_type_sizes(mqs_process* process, mqs_target_type_sizes* sizes) { return process->get_type_sizes(sizes); }
    static int find_function(mqs_image* image, char* fname, mqs_lang_code lang, mqs_taddr_t* addr) { return image->find_function(fname, lang, addr); }
    static int find_symbol(mqs_image* image, char* sname, mqs_taddr_t* addr) { return image->find_symbol(sname, addr); }
    static const mqs_type* find_type(mqs_image* image, char* tname, mqs_lang_code lang) { return image->find_type(tname, lang); }
    static int field_offset(mqs_type* type, char* fname) { return type->field_offset(fname); }
    static int get_sizeof(mqs_type* type) { return type->get_sizeof(); }

    // function pointers used by MQD shared library
    mqs_get_type_sizes_ft mqs_get_type_sizes_fp = ImageCallbacks::get_type_sizes;
    mqs_find_function_ft mqs_find_function_fp = ImageCallbacks::find_function;
    mqs_find_symbol_ft mqs_find_symbol_fp = ImageCallbacks::find_symbol;
    mqs_find_type_ft mqs_find_type_fp = ImageCallbacks::find_type;
    mqs_field_offset_ft mqs_field_offset_fp = ImageCallbacks::field_offset;
    mqs_sizeof_ft mqs_sizeof_fp = ImageCallbacks::get_sizeof;
};

struct ProcessCallbacks {
    // implements mqs_process_callbacks in whitepaper 5.6.17

    // signatures for process callbacks specified in whitepaper sections 5.6.18 - 5.6.22
    using mqs_get_global_rank_ft = int (*)(mqs_process* process);
    using mqs_get_image_ft = mqs_image* (*)(mqs_process* process);
    using mqs_fetch_data_ft = int (*)(mqs_process* process, mqs_taddr_t addr, int size, void* buf);
    using mqs_target_to_host_ft = void (*)(mqs_process* process, const void* indata, void* outdata, int size);

    // actual callback implementations. these forward to member functions of Image/Process.
    static int get_global_rank(mqs_process* process) { return process->get_global_rank(); }
    static mqs_image* get_image(mqs_process* process) { return process->get_image(); }
    static int fetch_data(mqs_process* process, mqs_taddr_t addr, int size, void* buf) { return process->fetch_data(addr, size, buf); }
    static void target_to_host(mqs_process* process, const void* indata, void* outdata, int size) { return process->target_to_host(indata, outdata, size); }

    // function pointers used by MQD shared library
    mqs_get_global_rank_ft mqs_get_global_rank_fp = ProcessCallbacks::get_global_rank;
    mqs_get_image_ft mqs_get_image_fp = ProcessCallbacks::get_image;
    mqs_fetch_data_ft mqs_fetch_data_fp = ProcessCallbacks::fetch_data;
    mqs_target_to_host_ft mqs_target_to_host_fp = ProcessCallbacks::target_to_host;
};

// end MQD shared library callbacks

class SoIface {
    // interface to the MQD shared library.
public:
    // given a path to the library .so, dlopen it, setup basic callbacks,
    // and bind the functions provided by the MQD shared library.
    SoIface(const char* so_path);
    ~SoIface();

    // these functions are described in sections 5.7 - 5.10 of the whitepaper.

    // returned string is owned by MQD.so and should not be free'd
    const char* version_string() { return m_version_string_fp(); }

    int version_compatibility() { return m_version_compatibility_fp(); }
    int dll_taddr_width() { return m_dll_taddr_width_fp(); }
    const char* error_string(int error_code) { return m_dll_error_string_fp(error_code); }

    // `SoIface` must live as long as `image`.
    int setup_image(Image* image) { return m_setup_image_fp(image, &m_image_callbacks); }
    int image_has_queues(Image* image, char** message) { return m_image_has_queues_fp(image, message); }
    int destroy_image_info(mqs_image_info* image_info) { return m_destroy_image_info_fp(image_info); }

    int setup_process(Process* process) { return m_setup_process_fp(process, &m_process_callbacks); }
    int process_has_queues(Process* process, char** message) { return m_process_has_queues_fp(process, message); }
    int destroy_process_info(mqs_process_info* process_info) { return m_destroy_process_info_fp(process_info); }

    int update_communicator_list(Process* process) { return m_update_communicator_list_fp(process); }
    int setup_communicator_iterator(Process* process) { return m_setup_communicator_iterator_fp(process); }
    int get_communicator(Process* process, mqs_communicator* comm) { return m_get_communicator_fp(process, comm); }
    int next_communicator(Process* process) { return m_next_communicator_fp(process); }

private:
    const char* (*m_version_string_fp)();
    int (*m_version_compatibility_fp)();
    int (*m_dll_taddr_width_fp)();
    const char* (*m_dll_error_string_fp)(int);

    int (*m_setup_image_fp)(Image*, const ImageCallbacks*);
    int (*m_image_has_queues_fp)(Image*, char**);
    int (*m_destroy_image_info_fp)(mqs_image_info*);

    int (*m_setup_process_fp)(Process*, const ProcessCallbacks*);
    int (*m_process_has_queues_fp)(Process*, char**);
    int (*m_destroy_process_info_fp)(mqs_process_info*);

    int (*m_update_communicator_list_fp)(Process*);
    int (*m_setup_communicator_iterator_fp)(Process*);
    int (*m_get_communicator_fp)(Process*, mqs_communicator*);
    int (*m_next_communicator_fp)(Process*);

    BasicCallbacks m_basic_callbacks = {};
    ImageCallbacks m_image_callbacks = {};
    ProcessCallbacks m_process_callbacks = {};
    void* m_raw_so = nullptr;
};

struct MQD_Impl {
    explicit MQD_Impl(const char* mqd_so_path, MISession& mi);
    ~MQD_Impl() = default;

    MQD_Impl(const MQD_Impl&) = delete;
    MQD_Impl(MQD_Impl&&) = delete;
    MQD_Impl& operator=(const MQD_Impl&) = delete;
    MQD_Impl& operator=(MQD_Impl&&) = delete;

    SoIface m_so;
    MISession& m_mi;

    // maps used by MQD.so. values are opaque pointers to private data in MQD.so.
    // the keys are raw pointers. the images and processes they reference live in ___.
    //
    // XXX: (At the moment "___" is "the stack", since the whole process only lives
    //       as long as a single function call for beta purposes.)

    std::map<Image*, mqs_image_info*> m_image_map = {};
    std::map<Process*, mqs_process_info*> m_process_map = {};
};

void* BasicCallbacks::malloc(size_t size) {
    auto ptr = new char[size];
    DVPRINT("mqd: malloc(%lu) -> %p\n", size, ptr);
    return ptr;
}

void BasicCallbacks::free(void* buf) {
    DVPRINT("mqd: free(%p)\n", buf);
    delete[] static_cast<char*>(buf);
}

void BasicCallbacks::dprints(const char* msg) {
    DVPRINT("from mqd .so: %s\n", msg);
}

const char* BasicCallbacks::errorstring(int error_code) {
    DVPRINT("mqd: errorstring\n");
    (void)error_code;
    return "sorry mqd, unimplemented";
}

void BasicCallbacks::put_image_info(mqs_image* image, mqs_image_info* info) {
    DVPRINT("mqd put_image_info %p, %p\n", image, info);
    if (image == nullptr) {
        DVPRINT("mqd: put_image_info called with null image\n");
        return;
    }

    image->m_mqd_impl.m_image_map.insert({image, info});
}

mqs_image_info* BasicCallbacks::get_image_info(mqs_image* image) {
    DVPRINT("mqd get_image_info %p\n", image);
    if (image == nullptr) {
        DVPRINT("mqd: get_image_info called with null image\n");
        return nullptr;
    }

    auto& map = image->m_mqd_impl.m_image_map;
    if (auto it = map.find(image); it != map.end()) {
        DVPRINT("mqd get_image_info found %p\n", it->second);
        return it->second;
    } else {
        DVPRINT("mqd: no image found for %p\n", image);
        return nullptr;
    }
}

void BasicCallbacks::put_process_info(mqs_process* process, mqs_process_info* info) {
    DVPRINT("mqd put_process_info\n");
    if (process == nullptr) {
        DVPRINT("mqd: put_process_info called with null process\n");
        return;
    }

    process->m_mqd_impl.m_process_map.insert({process, info});
}

mqs_process_info* BasicCallbacks::get_process_info(mqs_process* process) {
    DVPRINT("mqd get_process_info(%p)\n", process);
    if (process == nullptr) {
        DVPRINT("mqd: get_process_info called with null process\n");
        return nullptr;
    }

    auto& map = process->m_mqd_impl.m_process_map;
    if (auto it = map.find(process); it != map.end()) {
        DVPRINT("mqd get_process_info(%p) -> %p\n", process, it->second);
        return it->second;
    } else {
        DVPRINT("mqd: no process found for %p\n", process);
        return nullptr;
    }
}

int Process::get_global_rank() {
    DVPRINT("mqd: process get_global_rank(%p)\n", this);
    return 1; // TODO
}

Image* Process::get_image() {
    DVPRINT("mqd: process get_image(%p)\n", this);
    return &m_image;
}

int Process::fetch_data(mqs_taddr_t addr, int size, void* buf) {
    DVPRINT("mqd: process fetch_data(%p, 0x%lx, %d, %p)\n", this, addr, size, buf);

    auto cmd = make_MICmd(MIDataReadMemoryBytes(reinterpret_cast<unsigned long>(addr), 0, size));

    SendMICommand(&m_mqd_impl.m_mi, cmd.get());
    if (!MICommandResultOK(cmd.get())) {
        DVPRINT("mqd: process fetch_data(%p, 0x%lx, %d, %p) -> fail\n", this, addr, size, buf);
        return mqs_no_information;
    }

    const auto result = MIGetMemoryBytesInfo(cmd.get());
    MIMemoryBytes* bytes = nullptr;

    for (MIListSet(result); (bytes = (MIMemoryBytes*)MIListGet(result)) != nullptr; ) {
        // assume one item for now
        ::memcpy(buf, bytes->contents, size);

        auto hex = std::ostringstream{};
        for (auto i = 0lu; i < bytes->contents_len; i++) {
            hex << std::hex << std::setw(2) << std::setfill('0') << (int)bytes->contents[i];
        }
        DVPRINT("mqd: got %s\n", hex.str().c_str());
    }

    MIListFree(result, (void(*)())MIMemoryBytesFree);

    DVPRINT("mqd: process fetch_data(%p, 0x%lx, %d, %p) -> success\n", this, addr, size, buf);

    return mqs_ok;
}

void Process::target_to_host(const void *indata, void* outdata, int size) {
    DVPRINT("mqd: target_to_host(%p, %p, %p, %d)\n", this, indata, outdata, size);
    // XXX: we assume the target and host are the same for now
    ::memcpy(outdata, indata, size);
    DVPRINT("mqd: target_to_host(%p, %p, %p, %d) -> [", this, indata, outdata, size);
    for (auto i = 0; i < size; i++) {
        fprintf(stderr, "%02x", ((unsigned char*)outdata)[i]);
    }
    fprintf(stderr, "]\n");
}

void Process::get_type_sizes(mqs_target_type_sizes *out_sizes) {
    DVPRINT("mqd: process get_type_sizes(%p, %p)\n", this, out_sizes);
    if (out_sizes) {
        *out_sizes = mqs_target_type_sizes{};
    }
}

int Image::find_function(char* fname, mqs_lang_code lang, mqs_taddr_t* addr) {
    DVPRINT("mqd: image: find_function(%p, %s, %c, %p)\n", this, fname, lang, addr);
    return find_symbol(fname, addr);
}

int Image::find_symbol(char* sname, mqs_taddr_t* out_addr) {
    DVPRINT("mqd: image: find_symbol(%p, %s, %p)\n", this, sname, out_addr);

    auto expr = std::string{"&"} + sname;
    auto cmd = make_MICmd(MIDataEvaluateExpression(expr.c_str()));

    SendMICommand(&m_mqd_impl.m_mi, cmd.get());
    if (!MICommandResultOK(cmd.get())) {
        DVPRINT("mqd: image: find_symbol(%p, %s) -> not found\n", this, sname);
        return mqs_no_information;
    }

    // sometimes mqd.so just wants a yes or no answer and passes nullptr as out_addr
    if (out_addr != nullptr) {
        const auto result = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
        const auto result_addr = ::strtoul(result.get(), nullptr, 16);
        DVPRINT("mqd: image: find_symbol(%p, %s) -> %lx\n", this, sname, result_addr);
        *out_addr = result_addr;
    }

    return mqs_ok;
}

const Type* Image::find_type(char* tname, [[maybe_unused]] mqs_lang_code lang) {
    DVPRINT("mqd: image: find_type(%p, %s, %c)\n", this, tname, lang);

    // XXX: we ignore lang for now

    // since we use gdbmi to implement the rest of the Type functions, we just need
    // to verify that gdb knows about the given type. if it does, we can just store
    // the type name to be used in future expressions in the Type member functions.

    auto potential_type = Type { .m_name = tname, .m_mqd_impl = m_mqd_impl };

    // check if we already have seen it
    if (auto it = m_valid_types.find(potential_type); it != m_valid_types.end()) {
        DVPRINT("mqd: image: find_type(%p, %s, %c) -> already found\n", this, tname, lang);
        return &(*it);
    }

    // check if gdb knows about it
    {
        auto cmd = make_MICmd(CLIWhatis(tname));
        SendMICommand(&m_mqd_impl.m_mi, cmd.get());
        if (!MICommandResultOK(cmd.get())) {
            DVPRINT("mqd: image: find_type(%p, %s, %c) -> fail\n", this, tname, lang);
            return nullptr;
        }
    }

    DVPRINT("mqd: image: find_type(%p, %s, %c) -> success\n", this, tname, lang);

    auto [it, inserted] = m_valid_types.insert(std::move(potential_type));
    return &(*it);
}

int Type::field_offset(char* fname) {
    DVPRINT("mqd: type: field_offset(%p (%s), %s)\n", this, m_name.c_str(), fname);

    // (int)(&((T*)0)->fname)
    auto expr = std::string{"(int)(&(("} + m_name + "*)0)->" + fname + ")";
    auto cmd = make_MICmd(MIDataEvaluateExpression(expr.c_str()));

    SendMICommand(&m_mqd_impl.m_mi, cmd.get());
    if (!MICommandResultOK(cmd.get())) {
        DVPRINT("mqd: type: field_offset(%p (%s), %s) -> fail\n", this, m_name.c_str(), fname);
        return -1;
    }

    const auto result = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
    const auto result_offset = ::strtoul(result.get(), nullptr, 10);
    DVPRINT("mqd: type: field_offset(%p (%s), %s) -> %d\n", this, m_name.c_str(), fname, (int)result_offset);
    return result_offset;
}

int Type::get_sizeof() {
    DVPRINT("mqd: type: get_sizeof(%p (%s))\n", this, m_name.c_str());

    // sizeof(m_name)
    auto expr = std::string{"sizeof("} + m_name + ")";
    auto cmd = make_MICmd(MIDataEvaluateExpression(expr.c_str()));

    SendMICommand(&m_mqd_impl.m_mi, cmd.get());
    if (!MICommandResultOK(cmd.get())) {
        DVPRINT("mqd: type: get_sizeof(%p (%s)) -> fail\n", this, m_name.c_str());
        return -1;
    }

    const auto result = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));
    const auto result_size = ::strtoul(result.get(), nullptr, 10);
    DVPRINT("mqd: type: get_sizeof(%p (%s)) -> %d\n", this, m_name.c_str(), (int)result_size);
    return result_size;
}

bool Type::operator<(const Type& rhs) const {
    return m_name < rhs.m_name;
}

MQD_Impl::MQD_Impl(const char* mqd_so_path, MISession& mi)
    : m_so(mqd_so_path)
    , m_mi{mi}
{
    DVPRINT("mqd vers: %s\n", m_so.version_string());
    DVPRINT("mqd compat: %d\n", m_so.version_compatibility());
    DVPRINT("mqd bitwidth: %d\n", m_so.dll_taddr_width());

    {
        // temporary beta code

        // for each mqs_image, call mqs_setup_image
        Image img { .m_valid_types = {}, .m_mqd_impl = *this };
        m_so.setup_image(&img);

        {
            char* msg;
            auto r = m_so.image_has_queues(&img, &msg);
            DVPRINT("img has queues (0 == success): %d\n", r);
            if (r != mqs_ok && msg) {
                DVPRINT("mqd: ");
                DVPRINT(msg, "default image");
                DVPRINT("\n");
            }
        }

        Process proc { .m_image = img, .m_mqd_impl = *this };
        m_so.setup_process(&proc);

        {
            char* msg2;
            auto r2 = m_so.process_has_queues(&proc, &msg2);
            DVPRINT("proc has queues (0 == success): %d\n", r2);
            if (r2 != mqs_ok && msg2) {
                DVPRINT("mqd: ");
                DVPRINT(msg2, "default process");
                DVPRINT("\n");
            }
        }

        DVPRINT("mqd: update comm list\n");
        auto update_r = m_so.update_communicator_list(&proc);
        if (update_r != mqs_ok) {
            DVPRINT("update_communicator_list did not return ok\n");
        }
        DVPRINT("mqd: done update comm list\n");

        DVPRINT("mqd: iterate comm list\n");
        auto setup_r = m_so.setup_communicator_iterator(&proc);
        if (setup_r == mqs_end_of_list) {
            DVPRINT("end of list before we got into the loop\n");
        }
        DVPRINT("mqd: done iterate comm list\n");

        mqs_communicator comm;
        while(m_so.get_communicator(&proc, &comm) == mqs_ok){
            DVPRINT("mqd: communicator: %s\n", comm.name);
            DVPRINT("mqd: - local_rank: %ld\n", comm.local_rank);
            DVPRINT("mqd: - unique_id: 0x%lx\n", comm.unique_id);
            DVPRINT("mqd: - size: %ld\n", comm.size);
            auto next_r = m_so.next_communicator(&proc);
            if (next_r == mqs_end_of_list) {
                DVPRINT("mqd: end of list\n");
            }
        }
    }
}

static auto bind_fn(auto& fnptr, const char* name, void* raw_so) {
    auto* symbol = ::dlsym(raw_so, name);
    if (symbol == nullptr) {
        throw std::runtime_error(std::string{"mqd.so: Failed to bind "} + name);
    }
    // XXX: unfortunately there is no way to statically check the correctness of this! :-(
    fnptr = reinterpret_cast<decltype(fnptr)>(symbol);
}

SoIface::SoIface(const char* so_path) {
    m_raw_so = ::dlopen(so_path, RTLD_NOW | RTLD_LOCAL);
    if (!m_raw_so) {
        throw std::runtime_error("Failed to open mqd.so");
    }

    // setup fns
    bind_fn(m_version_string_fp, "mqs_version_string", m_raw_so);
    bind_fn(m_version_compatibility_fp, "mqs_version_compatibility", m_raw_so);
    bind_fn(m_dll_taddr_width_fp, "mqs_dll_taddr_width", m_raw_so);
    bind_fn(m_dll_error_string_fp, "mqs_dll_error_string", m_raw_so);

    // image fns
    bind_fn(m_setup_image_fp, "mqs_setup_image", m_raw_so);
    bind_fn(m_image_has_queues_fp, "mqs_image_has_queues", m_raw_so);
    bind_fn(m_destroy_image_info_fp, "mqs_destroy_image_info", m_raw_so);

    // process fns
    bind_fn(m_setup_process_fp, "mqs_setup_process", m_raw_so);
    bind_fn(m_process_has_queues_fp, "mqs_process_has_queues", m_raw_so);
    bind_fn(m_destroy_process_info_fp, "mqs_destroy_process_info", m_raw_so);

    // communicator fns
    bind_fn(m_update_communicator_list_fp, "mqs_update_communicator_list", m_raw_so);
    bind_fn(m_setup_communicator_iterator_fp, "mqs_setup_communicator_iterator", m_raw_so);
    bind_fn(m_get_communicator_fp, "mqs_get_communicator", m_raw_so);
    bind_fn(m_next_communicator_fp, "mqs_next_communicator", m_raw_so);

    // basic callbacks. this is called only once, so we don't need to store the function
    // pointer or expose the function as part of the API. so we just bind it, call it, and forget it here.
    void (*setup_basic_callbacks_fp)(const BasicCallbacks* cbs);
    bind_fn(setup_basic_callbacks_fp, "mqs_setup_basic_callbacks", m_raw_so);
    setup_basic_callbacks_fp(&m_basic_callbacks);

    // the image and process callbacks must be passed to the MQD shared library
    // with each call to setup_image and setup_process (which in turn is called
    // for each Image and Process), so they are omitted from the ctor.
}

SoIface::~SoIface() {
    ::dlclose(m_raw_so);
}

} // namespace mqd

// implementation of the public API
namespace mqd {

MQD::MQD(const char* mqd_so_path, MISession& mi)
    : m_impl{std::make_unique<MQD_Impl>(mqd_so_path, mi)}
{}

MQD::~MQD() = default;

} // namespace mqd
