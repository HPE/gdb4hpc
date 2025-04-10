// Copyright 2024-2025 Hewlett Packard Enterprise Development LP.

#include <algorithm>
#include <csetjmp>
#include <csignal>
#include <functional>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include <iomanip>
#include <map>
#include <mutex>
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

// opaque types (whitepaper section 5.3)
//
// these are opaque types used by the MQD shared library.
// we just need to pass them around.
using mqs_image_info = void;
using mqs_process_info = void;

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

    // having a reference to the global state is a code smell, but because we're dealing
    // with a C api and it calls back to us with pointers to types, not the global
    // state object, we're forced to smuggle in the global state like this.
    MQD::State& m_mqd_impl;

    bool operator<(const Type& other) const;
};

struct Image;

struct Process {
    int get_global_rank();
    Image* get_image();
    int fetch_data(mqs_taddr_t addr, int size, void* buf);
    void target_to_host(const void* indata, void* outdata, int size);
    void get_type_sizes(mqs_target_type_sizes* sizes);

    Image& m_image;

    // having a reference to the global state is a code smell, but because we're dealing
    // with a C api and it calls back to us with pointers to processes, not the global
    // state object, we're forced to smuggle in the global state like this.
    MQD::State& m_mqd_impl;
};

struct Image {
    int find_function(char* fname, mqs_lang_code lang, mqs_taddr_t* out_addr);
    int find_symbol(char* sname, mqs_taddr_t* out_addr);
    const Type* find_type(char* tname, mqs_lang_code lang);

    // updated by find_type
    std::set<Type> m_valid_types;

    // having a reference to the global state is a code smell, but because we're dealing
    // with a C api and it calls back to us with pointers to images, not the global
    // state object, we're forced to smuggle in the global state like this.
    MQD::State& m_mqd_impl;
};

// bind our types to the names used in the whitepaper

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

    // XXX: the mqd shared library is prone to segfaulting. since we dlopen it
    // to run it, this means that a mqd segfault is a dbgsrv segfault.
    // we handle this by setting up a signal handler for SIGSEGV when using the
    // mqd shared library. of course, this is nasty. but it's the least nasty
    // of the available options. see CPE-9018 for details and discussion of other options.

    // thrown when a segfault happens in the MQD shared library
    class SegfaultException : public std::runtime_error {
    public:
        SegfaultException() : std::runtime_error("A segmentation fault occurred in message queue debugging library provided by MPI. Further message queue debugging has been disabled.") {}
    };

    // function to execute code protected from segfaults. if a segfault happens,
    // throws SegfaultException. further calls the the shared library will
    // be blocked and throw a std::runtime_error.
    template <typename FN, typename ...Args>
    std::invoke_result_t<FN, Args...> call_catch_segv(FN fn, Args&&... args);

public:
    // given a path to the library .so, dlopen it, setup basic callbacks,
    // and bind the functions provided by the MQD shared library.
    SoIface(const std::string& so_path);
    ~SoIface();

    // these functions are described in sections 5.7 - 5.10 of the whitepaper.

    // returned string is owned by MQD.so and should not be free'd
    const char* version_string() { return call_catch_segv(m_version_string_fp); }

    int version_compatibility() { return call_catch_segv(m_version_compatibility_fp); }
    int dll_taddr_width() { return call_catch_segv(m_dll_taddr_width_fp); }
    const char* error_string(int error_code) { return call_catch_segv(m_dll_error_string_fp, error_code); }

    // `SoIface` must live as long as `image`.
    int setup_image(Image* image) { return call_catch_segv(m_setup_image_fp, image, &m_image_callbacks); }
    int image_has_queues(Image* image, char** message) { return call_catch_segv(m_image_has_queues_fp, image, message); }
    int destroy_image_info(mqs_image_info* image_info) { return call_catch_segv(m_destroy_image_info_fp, image_info); }

    int setup_process(Process* process) { return call_catch_segv(m_setup_process_fp, process, &m_process_callbacks); }
    int process_has_queues(Process* process, char** message) { return call_catch_segv(m_process_has_queues_fp, process, message); }
    int destroy_process_info(mqs_process_info* process_info) { return call_catch_segv(m_destroy_process_info_fp, process_info); }

    int update_communicator_list(Process* process) { return call_catch_segv(m_update_communicator_list_fp, process); }
    int setup_communicator_iterator(Process* process) { return call_catch_segv(m_setup_communicator_iterator_fp, process); }
    int get_communicator(Process* process, mqs_communicator* comm) { return call_catch_segv(m_get_communicator_fp, process, comm); }
    int next_communicator(Process* process) { return call_catch_segv(m_next_communicator_fp, process); }

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

    std::mutex m_segv_call_mutex;
    bool m_encountered_segv = false;
};

static thread_local jmp_buf g_mqd_segfault_jmp_buf;

static void handle_mqd_segfault(int /* sig */) {
    longjmp(g_mqd_segfault_jmp_buf, 1);
}

template <typename FN, typename ...Args>
std::invoke_result_t<FN, Args...> SoIface::call_catch_segv(FN fn, Args&&... args) {
    if (m_encountered_segv) {
        DVPRINT("mqd: tried to call MQD.so after segfault\n");
        throw std::runtime_error("The message queue debugging library has been disabled due to a segmentation fault.");
    }

    // we're changing the global handler for segv, which is not thread safe
    auto lock = std::scoped_lock(m_segv_call_mutex);

    auto sa = (struct sigaction){};
    sa.sa_handler = handle_mqd_segfault;
    ::sigemptyset(&sa.sa_mask);

    auto old_sa = (struct sigaction){};

    if (sigaction(SIGSEGV, &sa, &old_sa)) {
        throw std::runtime_error("Failed to set signal handler: " + std::string{strerror(errno)});
    }

    if (setjmp(g_mqd_segfault_jmp_buf) == 0) {
        auto result = fn(std::forward<Args>(args)...);
        if (sigaction(SIGSEGV, &old_sa, nullptr)) {
            throw std::runtime_error("Failed to set signal handler: " + std::string{strerror(errno)});
        }
        return result;
    } else {
        DVPRINT("mqd: segfault in MQD.so. disabling\n");
        m_encountered_segv = true;
        if (sigaction(SIGSEGV, &old_sa, nullptr)) {
            throw std::runtime_error("Failed to set signal handler: " + std::string{strerror(errno)});
        }
        throw SegfaultException();
    }
}

struct MQD::State {
    State(const std::string& mqd_so_path, MISession& mi);
    ~State() = default;

    State(const State&) = delete;
    State(State&&) = delete;
    State& operator=(const State&) = delete;
    State& operator=(State&&) = delete;

    std::vector<Communicator> get_communicators();

    SoIface m_so;
    MISession& m_mi;

    Image m_image;
    Process m_process;

    // maps used by MQD.so. values are opaque pointers to private data in MQD.so.
    // the keys are raw pointers to the Image/Process member variables.
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

static void ensure_mqd_debugging_enabled(Image& img, Process& proc, SoIface& mqd_so, MISession& mi) {
    // use the mqd.so and other heuristics to ensure message queue debugging is available and enabled

    {   // use image_has_queues to check for required symbols
        char* msg;
        auto r = mqd_so.image_has_queues(&img, &msg);
        DVPRINT("img has queues (0 == success): %d\n", r);

        if (msg) {
            // the mqd whitepaper says that a message might be set on success or failure.
            // in practice, the cray-mpich implementation seems to always provide
            // an error message, even on success.
            DVPRINT("mqd: ");
            DVPRINT(msg, "default image");
            DVPRINT("\n");
            // mqd.so owns msg so we don't free it
        }
        if (r != mqs_ok) {
            DVPRINT("mqd: mqd.so image_has_queues failed\n");
            throw std::runtime_error("Message queue debugging not supported (missing symbols)");
        }
    }

    // XXX: this check is useless - we ignore the result since cray-mpich always
    // fails. but we keep it for completeness/documentation sake and hope to
    // un-ignore it in the future.

    {   // use process_has_queues to check for required runtime settings
        char* msg2;
        // note that process_has_queues is only safe if image_has_queues returned mqs_ok
        auto r2 = mqd_so.process_has_queues(&proc, &msg2);
        DVPRINT("proc has queues (0 == success): %d\n", r2);

        if (msg2) {
            // same success/failure behavior as image_has_queues
            DVPRINT("mqd: ");
            DVPRINT(msg2, "default process");
            DVPRINT("\n");
        }
        if (r2 != mqs_ok) {
            // XXX: this should throw and exception, but cray-mpich seems to
            // fail every time, so ignore it for now.
            DVPRINT("mqd: mqd.so process_has_queues failed. continuing anyway.\n");
        }
    }

    // check that one of the appropriate environment variables are set
    //
    // XXX: process_has_queues is already supposed to do this check for us
    // in a way that's agnostic to the MPI implementation, but since process_has_queues
    // always fails for cray-mpich, we do it manually here for now.

    const std::array evars = {
        "MPICH_MSG_QUEUE_DBG",
        "MPIR_CVAR_MSG_QUEUE_DBG",
    };

    auto any_vars = std::ranges::any_of(evars, [&](const char* evar) {
        // if the --env flag is used to launch the app, the variable will be
        // set in the application's environment but not dbgsrv's. so we need
        // to use gdb to check.

        // get the value of the environment variable
        const auto setting = std::invoke([&] {
            // note that we don't use the `show env <evar>` command because it
            // is populated with gdb's env vars, not the app's and the same --env
            // limitation described above applies.
            auto expr = "\"(char *)getenv(\\\"" + std::string{evar} + "\\\")\"";
            auto cmd = make_MICmd(MIDataEvaluateExpression(expr.c_str()));

            SendMICommand(&mi, cmd.get());
            if (!MICommandResultOK(cmd.get())) {
                DVPRINT("mqd: failed to get %s\n", evar);
                return std::string{};
            }

            const auto result = MallocedCharPtr(MIGetDataEvaluateExpressionInfo(cmd.get()));

            // e.g '$1 = 0x7ffc408ec7a4 "1"'
            // or '$3 = 0x0'

            auto result_view = result.get_view();
            const auto quote_idx = result_view.find('"');
            if (quote_idx == std::string_view::npos || quote_idx + 1 >= result_view.size()) {
                return std::string{};
            }

            const auto unquoted = StripQuotes(result_view.substr(quote_idx));
            DVPRINT("mqd: found %s = %s\n", evar, unquoted.c_str());
            return unquoted;
        });

        return !setting.empty() && setting != "0";
    });

    if (!any_vars) {
        DVPRINT("mqd: missing environment variable. none of these were found:\n");
        for (const auto& evar : evars) {
            DVPRINT("- %s\n", evar);
        }
        throw std::runtime_error(
            "Message queue debugging not enabled. Try setting one of these "
            "environment variables to '1': " +
            std::string{evars[0]} + ", " + evars[1]);
    }
}

MQD::State::State(const std::string& mqd_so_path, MISession& mi)
    : m_so(mqd_so_path)
    , m_mi{mi}
    , m_image{ .m_valid_types = {}, .m_mqd_impl = *this }
    , m_process { .m_image = m_image, .m_mqd_impl = *this }
{
    DVPRINT("mqd vers: %s\n", m_so.version_string());
    DVPRINT("mqd compat: %d\n", m_so.version_compatibility());
    DVPRINT("mqd bitwidth: %d\n", m_so.dll_taddr_width());

    // set up state
    if (auto r = m_so.setup_image(&m_image); r != mqs_ok) {
        throw std::runtime_error(
            "Message queue debugging not enabled. Internal error: setup_image failed");
    }
    if (auto r = m_so.setup_process(&m_process); r != mqs_ok) {
        throw std::runtime_error(
            "Message queue debugging not enabled. Internal error: setup_process failed");
    }

    // throws on failure
    ensure_mqd_debugging_enabled(m_image, m_process, m_so, mi);
}

std::vector<Communicator> MQD::State::get_communicators() {
    auto result = std::vector<Communicator>{};

    DVPRINT("mqd: update comm list\n");
    auto update_r = m_so.update_communicator_list(&m_process);
    if (update_r != mqs_ok) {
        DVPRINT("update_communicator_list did not return ok\n");
    }
    DVPRINT("mqd: done update comm list\n");

    DVPRINT("mqd: iterate comm list\n");
    auto setup_r = m_so.setup_communicator_iterator(&m_process);
    if (setup_r == mqs_end_of_list) {
        DVPRINT("end of list before we got into the loop\n");
    }

    mqs_communicator comm;
    while(m_so.get_communicator(&m_process, &comm) == mqs_ok){
        result.push_back(comm);
        auto next_r = m_so.next_communicator(&m_process);
        if (next_r == mqs_end_of_list) {
            DVPRINT("mqd: end of list\n");
        }
    }
    DVPRINT("mqd: done iterate comm list\n");

    return result;
}

static auto bind_fn(auto& fnptr, const char* name, void* raw_so) {
    auto* symbol = ::dlsym(raw_so, name);
    if (symbol == nullptr) {
        throw std::runtime_error(std::string{"mqd.so: Failed to bind "} + name);
    }
    // XXX: unfortunately there is no way to statically check the correctness of this! :-(
    fnptr = reinterpret_cast<decltype(fnptr)>(symbol);
}

SoIface::SoIface(const std::string& so_path) {
    DVPRINT("mqd: opening %s\n", so_path.c_str());
    m_raw_so = ::dlopen(so_path.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (!m_raw_so) {
        throw std::runtime_error("Failed to open " + so_path + ": " + ::dlerror());
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

MQD::MQD(const std::string& mqd_so_path, MISession& mi)
    : m_impl{std::make_unique<MQD::State>(mqd_so_path, mi)}
{}

MQD::~MQD() = default;

std::vector<Communicator> MQD::get_communicators() {
    return m_impl->get_communicators();
}

} // namespace mqd
