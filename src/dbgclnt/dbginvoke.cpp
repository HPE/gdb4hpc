/******************************************************************************\
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "dbginvoke.h"
#include "dbgclnt_proto.h"
#include "dbg_client.h"
#include "path.h"

#include <getopt.h>
#include <algorithm>
#include <filesystem>
#include <cassert>

#include "unittest_macros.h"

TEST_CASE("DbgInvoke: operator==") {
    // operator== relies on the DbgInvoke having a handle. DbgInvokes get handles by
    // registering themselves in the global DbgInvoke list.

    auto Launched = DbgInvoke::ProcessType::Launched;
    auto di = std::make_unique<DbgInvoke>(Launched, 4, *findClientProtoByName("zmqnet"));
    auto di2 = std::make_unique<DbgInvoke>(Launched, 4, *findClientProtoByName("zmqnet"));

    auto dih = DbgInvoke::activate("test_di", std::move(di));
    auto dih2 = DbgInvoke::activate("test_di_2", std::move(di2));

    SUBCASE("operator==") {
        auto dip = dih.lock();
        auto dip2 = dih.lock(); // same di
        REQUIRE(dip != nullptr);
        REQUIRE(dip2 != nullptr);

        CHECK(*dip == *dip2);
    }

    SUBCASE("operator!=") {
        auto dip = dih.lock();
        auto dip2 = dih2.lock(); // different di
        REQUIRE(dip != nullptr);
        REQUIRE(dip2 != nullptr);

        CHECK(*dip != *dip2);
    }

    // clean up as to not affect other tests
    DbgInvoke::deactivateAll();
}

TEST_CASE("DbgInvoke: set/get/unset frontendGdb") {
    using DI = DbgInvoke;
    auto di = DbgInvoke(DI::ProcessType::Launched, 4, *findClientProtoByName("zmqnet"));

    REQUIRE(di.frontendGdb() == nullptr);

    SUBCASE("Instantiate, deinstantiate with nullptr") {
        auto r = di.instantiateFrontendGdb("gdbmi");
        CHECK(r != nullptr);
        REQUIRE(di.frontendGdb() != nullptr);

        auto r2 = di.instantiateFrontendGdb(nullptr);
        CHECK(di.frontendGdb() == nullptr);
        CHECK(r2 == nullptr);
    }

    SUBCASE("Instantiate, deinstantiate with invalid impl") {
        auto r = di.instantiateFrontendGdb("gdbmi");
        CHECK(r != nullptr);
        REQUIRE(di.frontendGdb() != nullptr);

        auto r2 = di.instantiateFrontendGdb("not real");
        CHECK(r2 == nullptr);
        CHECK(di.frontendGdb() == nullptr);
    }
}

TEST_CASE("DbgInvoke: set/call/unset event callback function") {
    auto fn = [](DbgInvoke::EventSet& es, const Bitset& bitset) {
        return 1;
    };
    auto Launched = DbgInvoke::ProcessType::Launched;
    auto di = DbgInvoke(Launched, 4, *findClientProtoByName("zmqnet"));
    auto bs = DenseBitset(8);
    auto es = DbgInvoke::EventSet(di, bs, nullptr);

    REQUIRE(di.getAsyncEventCallback() == nullptr);

    // set
    di.setAsyncEventCallback(fn);
    REQUIRE(di.getAsyncEventCallback() != nullptr);
    CHECK(di.callAsyncEventCallback(es, bs) == 1);

    // unset
    di.setAsyncEventCallback(nullptr);
    REQUIRE(di.getAsyncEventCallback() == nullptr);
    CHECK_THROWS_AS(di.callAsyncEventCallback(es, bs), std::bad_function_call);
}

TEST_CASE("DbgInvoke: check that no other tests polluted global registry") {
    CHECK(DbgInvoke::getAll().size() == 0);
}

// Public API for accessing dbginvoke table

namespace {

// private table of all active dbginvokes
std::vector<std::shared_ptr<DbgInvoke>> g_allInvokes;

// cached list of dbginvoke handles to return via getAllDbgInvokes()
// to avoid rebuilding the list in every call to DbgCheckEvents etc.
std::vector<DbgInvoke::Handle> g_allHandles;
void rebuildCachedHandles() {
    g_allHandles.clear();
    for (const auto& di : g_allInvokes) {
        g_allHandles.emplace_back(di->handle());
    }
}

} // anonymous namespace

DbgInvoke::Handle DbgInvoke::activate(std::string name, std::unique_ptr<DbgInvoke>&& di) {
    if (!di || name == "") return nullptr;

    // disallow duplicate names
    for (const auto& existingDi : g_allInvokes) {
        if (name == existingDi->handle().name()) {
            return nullptr;
        }
    }

    auto sptr = std::shared_ptr<DbgInvoke>(std::move(di));
    auto handle = DbgInvoke::Handle(name, sptr);

    sptr->m_handle = handle;

    g_allInvokes.push_back(std::move(sptr));
    g_allHandles.push_back(handle);

    return handle;
}

std::shared_ptr<DbgInvoke> DbgInvoke::deactivate(const DbgInvoke::Handle& handle) {
    // find it
    auto it = std::find_if(g_allInvokes.begin(), g_allInvokes.end(), [&handle](const auto& diptr) {
        return handle == diptr->handle();
    });

    if (it == g_allInvokes.end()) return nullptr;

    // remove it
    auto r = std::move(*it);
    g_allInvokes.erase(it);

    rebuildCachedHandles();

    return r;
}

DbgInvoke::Handle DbgInvoke::rename(DbgInvoke::Handle handle, std::string newName) {
    if (!handle.lock() || newName.empty()) return nullptr;

    for (auto& existingDi : g_allInvokes) {
        if (existingDi->handle().name() == handle.name()) {
            existingDi->m_handle = DbgInvoke::Handle(newName, existingDi);
            rebuildCachedHandles();
            return existingDi->handle();
        }
    }

    return nullptr;
}

void DbgInvoke::deactivateAll() {
    g_allInvokes.clear();
    rebuildCachedHandles();
}

std::vector<DbgInvoke::Handle>& DbgInvoke::getAll() {
    return g_allHandles;
}

DbgInvoke::DbgInvoke(ProcessType type, int nproc, DebugClientProtocol& proto)
    : m_type{type}
    , m_nproc{nproc}
    , m_proto{proto}
{};

DbgInvoke::DbgInvoke(DbgInvoke&& o)
    : m_appid{o.m_appid}
    , m_type{o.m_type}
    , m_started_flag{o.m_started_flag}
    , m_at_init_breakpoint_flag{o.m_at_init_breakpoint_flag}
    , m_shutdown_flag{o.m_shutdown_flag}
    , m_launch_opts{std::move(o.m_launch_opts)}
    , m_nproc{o.m_nproc}
    , m_evfds{o.m_evfds}
    , m_proto_outp{o.m_proto_outp}
    , m_bitset_full{std::move(o.m_bitset_full)}
    , m_bitset_launched{std::move(o.m_bitset_launched)}
    , m_proto{o.m_proto}
    , m_proto_net{std::move(o.m_proto_net)}
    , m_frontend_gdb{o.m_frontend_gdb}
    , m_clnt_ev_callback{std::move(o.m_clnt_ev_callback)}
    , m_source_paths{std::move(o.m_source_paths)}
    , m_breakpoints{std::move(o.m_breakpoints)}
    , m_checkpoints{std::move(o.m_checkpoints)}
    , m_eventset_list{std::move(o.m_eventset_list)}
    , m_handle{std::move(o.m_handle)}
{
    o.m_appid = 0;
    o.m_started_flag = false;
    o.m_shutdown_flag = true;
    o.m_nproc = 0;
    o.m_evfds = nullptr;
    o.m_proto_outp = nullptr;
    o.m_frontend_gdb = nullptr;
}

DbgInvoke::~DbgInvoke() {
    HoldSignals();

    // XXX has large side effects. does it belong in this dtor?
    proto().destroyClient(this);

    // Quit the debug frontend if there is one
    if (m_frontend_gdb) {
        auto e = DbgDestroyFE(m_frontend_gdb);
        FreeEvent(e);
    }

    // remove any event fds from the proto
    DestroyList(m_evfds, free);

    ReleaseSignals();
}

bool DbgInvoke::operator==(const DbgInvoke& other) const {
    return m_handle == other.m_handle;
}

bool DbgInvoke::operator!=(const DbgInvoke& other) const {
    return !(*this == other);
}

DbgInvoke::Handle DbgInvoke::handle() {
    if (m_handle.lock() == nullptr) {
        // The handle is put in place by registerDbgInvoke. Don't call handle() before that.
        throw std::runtime_error("Bad dbginvoke handle");
    }
    return m_handle;
}

const DbgInvoke::Handle DbgInvoke::handle() const {
    // OK as long as handle() never gains any side effects
    return const_cast<DbgInvoke*>(this)->handle();
}

uint64_t DbgInvoke::appId() const { return m_appid; }
void DbgInvoke::setAppId(uint64_t id) { m_appid = id; }

DbgInvoke::ProcessType DbgInvoke::processType() const { return m_type; }

bool DbgInvoke::isStarted() const { return m_started_flag; }
void DbgInvoke::setStarted() { m_started_flag = true; }

bool DbgInvoke::isAtInitialBreakpoint() const { return m_at_init_breakpoint_flag; }
void DbgInvoke::setAtInitialBreakpoint(bool at) { m_at_init_breakpoint_flag = at; }

bool DbgInvoke::isShutdown() const { return m_shutdown_flag; }
void DbgInvoke::setShutdown() { m_shutdown_flag = true; }

int DbgInvoke::numRanks() const { return m_nproc; }
void DbgInvoke::setNumRanks(int n) { m_nproc = n; }

list_t* DbgInvoke::eventFds() { return m_evfds; }
void DbgInvoke::initializeEventFds() {
    DestroyList(m_evfds, free); // nullptr is OK
    m_evfds = NewList();
}

const Bitset* DbgInvoke::fullBitset() const { return m_bitset_full.get(); }
void DbgInvoke::setFullBitset(std::unique_ptr<Bitset>&& bitset) {
    m_bitset_full = std::move(bitset);
}

const Bitset* DbgInvoke::launchedBitset() const { return m_bitset_launched.get(); }
void DbgInvoke::setLaunchedBitset(std::unique_ptr<Bitset>&& bitset) {
    m_bitset_launched = std::move(bitset);
}
void DbgInvoke::orLaunchedBitset(const Bitset& rhs) {
    if (!m_bitset_launched) return;
    *m_bitset_launched |= rhs;
}

g_progoutp_t* DbgInvoke::protoOutput() { return m_proto_outp; }
void DbgInvoke::initializeProtoOutput() {
    destroyProtoOutput();
    m_proto_outp = DbgCreateGenericProgOutp();
}
void DbgInvoke::destroyProtoOutput() {
    DbgFreeGenericProgOutp(m_proto_outp);
    m_proto_outp = nullptr;
}

const DbgInvoke::LaunchOpts& DbgInvoke::launchOptions() const { return m_launch_opts; }
DbgInvoke::LaunchOpts& DbgInvoke::launchOptions() { return m_launch_opts; }

DebugClientProtocol& DbgInvoke::proto() { return m_proto; }

DebugClientNetwork* DbgInvoke::network() const { return m_proto_net.get(); }

void DbgInvoke::setNetwork(std::unique_ptr<DebugClientNetwork>&& net) {
    m_proto_net = std::move(net);
}

dbgfrontend_sw* DbgInvoke::instantiateFrontendGdb(const char* impl) {
    if (m_frontend_gdb) {
        auto e = DbgDestroyFE(m_frontend_gdb);
        FreeEvent(e);
    }

    if (impl) {
        m_frontend_gdb = DbgInstantiateFE(impl);
    } else {
        m_frontend_gdb = nullptr;
    }

    return m_frontend_gdb;
}

dbgfrontend_sw* DbgInvoke::frontendGdb() const { return m_frontend_gdb; }

DbgInvoke::AsyncEventCallback DbgInvoke::getAsyncEventCallback() const {
    return m_clnt_ev_callback;
}

void DbgInvoke::setAsyncEventCallback(DbgInvoke::AsyncEventCallback func) {
    m_clnt_ev_callback = func;
}

int DbgInvoke::callAsyncEventCallback(DbgInvoke::EventSet& es, const Bitset& bitset) const {
    return m_clnt_ev_callback(es, bitset);
}

DbgInvoke::SourcePaths& DbgInvoke::sourcePaths() { return m_source_paths; }
const DbgInvoke::SourcePaths& DbgInvoke::sourcePaths() const { return m_source_paths; }

BreakpointManager& DbgInvoke::breakpoints() { return m_breakpoints; }
const BreakpointManager& DbgInvoke::breakpoints() const { return m_breakpoints; }

CheckpointManager& DbgInvoke::checkpoints() { return m_checkpoints; }
const CheckpointManager& DbgInvoke::checkpoints() const { return m_checkpoints; }

std::list<DbgInvoke::EventSet>& DbgInvoke::eventSets() { return m_eventset_list; }

DbgInvoke::Handle::Handle(std::string name, std::shared_ptr<DbgInvoke> parent) {
    if (parent == nullptr) {
        // use ::Handle(nullptr_t)
        throw std::runtime_error("Can't construct named DbgInvoke::Handle from nullptr");
    }

    m_name = std::move(name);
    m_ptr = parent;
}

DbgInvoke::Handle::Handle(nullptr_t) {
    // Creates a null handle. lock() will return nullptr
}

const std::string& DbgInvoke::Handle::name() const {
    if (lock()) {
        // If this handle is still valid, fetch name from parent.
        // It might have changed via DbgInvoke::rename.
        m_name = lock()->handle().m_name;
    }
    return m_name;
}

const char* DbgInvoke::Handle::c_str() const {
    return name().c_str();
}

DbgInvoke* DbgInvoke::Handle::lock() const {
    return m_ptr.lock().get();
}

DbgInvoke::LaunchOpts::LaunchOpts(int argc, const char* const constArgv[], ClientSettingsInterface& csi) {
    if (argc < 3) {
        throw std::runtime_error("Not enough command arguments.\nExpected: launch $handle ./program <args>");
    }

    const auto withoutQuotes = [](const std::string& str) {
        const auto quotes = {'"', '\''};
        if (str.size() > 1) {
            for (const auto& q : quotes) {
                if (str.front() == q && str.back() == q) {
                    return std::string_view(str.c_str() + 1, str.size() - 2);
                }
            }
        }

        return std::string_view{str};
    };

    int gpu_flag, debug_flag, non_mpi_flag;
    gpu_flag = debug_flag = non_mpi_flag = 0;

    launcher_argv << ""; // reserve launcher name
    app_argv << "";      // reserve binary name

    const struct option launch_long_opts[] = {
        {"args", optional_argument, 0, 'a'},
        {"launcher-args", optional_argument, 0, 'g'},
        {"launcher-input", optional_argument, 0, 'i'},
        {"workdir", optional_argument, 0, 'd'},
        {"env", optional_argument, 0, 'e'},
        {"gpu", no_argument, &gpu_flag, 1},
        {"debug", no_argument, &debug_flag, 1},
        {"qsub", required_argument, 0, 'q'},
        {"sbatch", required_argument, 0, 's'},
        {"non-mpi", no_argument, &non_mpi_flag, 1},
        {"launcher", optional_argument, 0, 'l'},
        {"gdb", required_argument, 0, 'b'},
        {0, 0, 0, 0}
    };

    int opt;
    /* extern int */ optind = 0;

    // getopt can change argv so we work with a local copy
    struct CStrVector {
        // ensure memory is cleaned up if something throws
        ~CStrVector() {
            for (auto& str : v) free(str);
        }
        std::vector<char*> v;
    };
    CStrVector argv;
    std::transform(constArgv, constArgv + argc, std::back_inserter(argv.v), [](const auto& strp) {
        return strdup(strp);
    });

    // This will permute the non-option stuff to the end, so parse that stuff last.
    while ((opt = getopt_long(argc, argv.v.data(), "a:g:i:d::e:q:", launch_long_opts, nullptr)) != -1) {
        switch (opt) {
            case 0:
                // if this is a flag, do nothing
                break;

            case 'a':
                if (optarg == nullptr) {
                    throw std::runtime_error("--args provided, but missing arguments.");
                }
                app_argv.parseAndAppendArgs(withoutQuotes(optarg));
                break;

            case 'l':
                if (optarg == nullptr) {
                    throw std::runtime_error("--launcher provided, but missing arguments.");
                }

                // replace on first instance of --launcher only
                if (launcher_argv[0] == "") {
                    launcher_argv[0] = optarg;
                }
                break;

            case 'g':
                if (optarg == nullptr) {
                    throw std::runtime_error("--launcher-args provided, but missing arguments.");
                }
                launcher_argv.parseAndAppendArgs(withoutQuotes(optarg));
                break;

            case 'i':
                if (optarg == nullptr) {
                    throw std::runtime_error("--launcher-input provided, but missing arguments.");
                }
                input_file_path = optarg;
                break;

            case 'd':
                if (optarg == nullptr) {
                    // --workdir provided without argument, so default to chdir
                    // to the directory that the binary is located in.
                    set_default_cwd = true;
                } else {
                    // convert to absolute path
                    try {
                        // throws if directory doesn't exist
                        cwd = std::filesystem::canonical(withoutQuotes(optarg));
                        if (!std::filesystem::is_directory(cwd)) {
                            throw std::runtime_error(cwd + " is not a directory.");
                        }
                    } catch (const std::exception& e) {
                        using namespace std::string_literals;
                        throw std::runtime_error(
                            "invalid --workdir provided. ("s + e.what() + ")"
                        );
                    }
                }
                break;

            case 'e': {
                if (optarg == nullptr) {
                    throw std::runtime_error("--env provided, but missing arguments.");
                }

                // Check formatting
                auto view = std::string_view{optarg};
                if (view.size() < 3 || std::count(view.begin(), view.end(), '=') != 1){
                    throw std::runtime_error("Invalid --env string. Expected \"KEY=VALUE\"");
                }

                env << optarg;
                break;
            }

            case 'b':
                if (optarg == nullptr) {
                    throw std::runtime_error("--gdb provided, but missing argument.");
                }
                // convert to absolute path
                namespace fs = std::filesystem;
                // throws if file doesn't exist
                if (auto p = pathFind(optarg, nullptr)) {
                    gdb_path = fs::canonical(p);
                    free(p);
                } else {
                    throw std::runtime_error("Could not find --gdb provided gdb in $PATH.");
                }
                if (!fs::is_regular_file(gdb_path) && !fs::is_symlink(gdb_path)) {
                    throw std::runtime_error("--gdb provided, but doesn't point to a normal file.");
                }
                break;

            case 'q':
                if (optarg == nullptr) {
                    throw std::runtime_error("--qsub provided, but missing arguments.");
                }
                qsub_file_path = optarg;
                break;

            case 's':
                if (optarg == nullptr) {
                    throw std::runtime_error("--sbatch provided, but missing arguments.");
                }
                sbatch_file_path = optarg;
                break;

            default:
                throw std::runtime_error("Invalid launch argument provided. See \"help launch\" for usage.");
        }
    }

    // Ensure the user provided two non-option arguments
    if (argc - optind != 2) {
        using namespace std::string_literals;
        throw std::runtime_error(
            "Invalid number of non-option arguments. See \"help launch\" for usage.\n"s
            + "Expected 2, found " + std::to_string(argc - optind) + "."
        );
    }

    // The first argument should be the name of the process set
    if (auto procset_name_cstr = argv.v[optind++]) {
        procset_name = procset_name_cstr;
    } else {
        throw std::runtime_error("No process set specified.");
    }

    // The second argument is the name of the a.out
    try {
        if (auto binary_path_cstr = argv.v[optind++]) {
            namespace fs = std::filesystem;
            // throws if file doesn't exist
            app_argv[0] = fs::canonical(binary_path_cstr);
            if (!fs::is_regular_file(app_argv[0]) && !fs::is_symlink(app_argv[0])) {
                throw std::runtime_error("Incorrect filetype");
            }
        } else {
            throw std::runtime_error("No binary passed");
        }
    } catch (const std::exception &e) {
        using namespace std::string_literals;
        throw std::runtime_error("Invalid binary ("s + e.what() + ")");
    }

    if (set_default_cwd) {
        // --workdir provided without argument, so default to chdir to
        // the directory that the binary is located in.
        auto binary_path = std::filesystem::path{app_argv[0]};

        // argument parsing function should always canonicalize path
        assert(binary_path.is_absolute());

        cwd = binary_path.parent_path();
    }

    this->gpu_flag = gpu_flag;
    this->debug_flag = debug_flag;
    this->non_mpi_flag = non_mpi_flag;
    run_argv = app_argv;

    non_stop_flag = csi.isNonStop();
    early_entry_flag = csi.isEarlyEntry();
    unsafe_flag = csi.isUnsafe();
    proto_name = csi.getProto();
    arch_name = csi.getArch();
    backend_name = csi.getBackend();
}

void DbgInvoke::SourcePaths::storeSourcePath(const char * str) {
    if(str!=nullptr){
        m_sourceList.push_back(std::filesystem::path(str));
    }
}

std::string DbgInvoke::SourcePaths::getFullPath(const std::string& relativePath) const {
    if (!relativePath.empty() && !m_sourceList.empty()){
        for(auto &source : m_sourceList){
            if (source.find(relativePath) != std::string::npos){
                return source;
            }
        }
    }
    
    //Cannot find full path. Return the relative path as the full path.
    return relativePath;
}

DbgInvoke::EventSet::EventSet(DbgInvoke& di, const Bitset& group, const dbgevent_t* event)
    : m_di{di}, m_group{group.clone()}, m_event{DupEvent(event)}
{}

DbgInvoke::EventSet::~EventSet() {
    if (m_event) FreeEvent(m_event);
}
