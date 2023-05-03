/******************************************************************************\
 * Copyright 2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "dbginvoke.h"
#include "dbgclnt_proto.h"
#include "dbg_client.h"
#include "path.h"

#include <getopt.h>
#include <algorithm>
#include <fstream>
#include <filesystem>

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
        auto r = di.instantiateFrontendGdb("gdbmi", nullptr);
        CHECK(r != nullptr);
        REQUIRE(di.frontendGdb() != nullptr);

        auto r2 = di.instantiateFrontendGdb(nullptr, nullptr);
        CHECK(di.frontendGdb() == nullptr);
        CHECK(r2 == nullptr);
    }

    SUBCASE("Instantiate, deinstantiate with invalid impl") {
        auto r = di.instantiateFrontendGdb("gdbmi", nullptr);
        CHECK(r != nullptr);
        REQUIRE(di.frontendGdb() != nullptr);

        auto r2 = di.instantiateFrontendGdb("not real", nullptr);
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

TEST_CASE("DbgInvoke: Parse LaunchOpts") {
    SUBCASE("Throws exception when missing required args") {
        const char* const argv[] = {};
        auto csi = MockClientSettings();

        CHECK_THROWS(LaunchOpts(0, argv, csi));
    }

    SUBCASE("Settings from global client settings") {
        const char* const argv[] = {
            "launch", "$a{1}", "/tmp/c_inc",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        CHECK(opts.unsafe_flag == false);
        CHECK(opts.non_stop_flag == true);
        CHECK(opts.early_entry_flag == true);
        CHECK(opts.proto_name == "mock_zmqnet");
        CHECK(opts.arch_name == "mock_alps");
        CHECK(opts.backend_name == "mock_gdbmi");
    }

    SUBCASE("Path settings: unquoted") {
        const char* const argv[] = {
            "launch", "$a{1}", "./bin/c_inc",

            "--launcher=/tmp/fake_mpiexec",
            "--launcher-input=/tmp/fake_input",
            "--gdb=/tmp/fake_gdb",
            "--workdir=/tmp/fake_workdir",
            "--qsub=/tmp/fake_qsub",
            "--sbatch=/tmp/fake_sbatch",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        CHECK(opts.launcher_argv[0] == "/tmp/fake_mpiexec");
        CHECK(opts.input_file_path == "/tmp/fake_input");
        CHECK(opts.gdb_path == "/tmp/fake_gdb");
        CHECK(opts.cwd == "/tmp/fake_workdir");
        CHECK(opts.qsub_file_path == "/tmp/fake_qsub");
        CHECK(opts.sbatch_file_path == "/tmp/fake_sbatch");
    }

    SUBCASE("Path settings: quoted") {
        const char* const argv[] = {
            "launch", "$a{1}", "./bin/c_inc",

            "-l", "\"/tmp/fake mpiexec\"",
            "-i", "\"/tmp/fake input\"",
            "-b", "\"/tmp/fake gdb\"",
            // since --workdir is valid even without a passed path,
            // the argument parser treats any "-d" argument as having
            // no argument, even when followed by a string
            "--workdir=\"/tmp/fake workdir\"",
            "-q", "\"/tmp/fake qsub\"",
            "-s", "\"/tmp/fake sbatch\"",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        // quotes should be removed
        CHECK(opts.launcher_argv[0] == "/tmp/fake mpiexec");
        CHECK(opts.input_file_path == "/tmp/fake input");
        CHECK(opts.gdb_path == "/tmp/fake gdb");
        CHECK(opts.cwd == "/tmp/fake workdir");
        CHECK(opts.qsub_file_path == "/tmp/fake qsub");
        CHECK(opts.sbatch_file_path == "/tmp/fake sbatch");
    }

    SUBCASE("Binary flags") {
        const char* const argv[] = {
            "launch", "$a{1}", "./bin/c_inc",
            "--gpu", "--non-mpi", "--debug", "--workdir",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        CHECK(opts.gpu_flag == true);
        CHECK(opts.non_mpi_flag == true);
        CHECK(opts.debug_flag == true);
        CHECK(opts.set_default_cwd == true);
    }

    SUBCASE("Launcher args") {
        const char* const argv[] = {
            "launch", "$a{1}", "./bin/c_inc",
            "--launcher=/tmp/fake_mpiexec",
            "--launcher-args=\"-n3 -N1 --exclusive \"multi word arg\"\"",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        REQUIRE(opts.launcher_argv.argc() == 5);
        CHECK(opts.launcher_argv[0] == "/tmp/fake_mpiexec");
        CHECK(opts.launcher_argv[1] == "-n3");
        CHECK(opts.launcher_argv[2] == "-N1");
        CHECK(opts.launcher_argv[3] == "--exclusive");
        CHECK(opts.launcher_argv[4] == "multi word arg"); // should have no quotes
    }

    SUBCASE("App args") {
        const char* const argv[] = {
            "launch", "$a{1}", "./bin/c_inc",
            "--args=\"input_file output_file 44 \"multi word arg\"\"",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        REQUIRE(opts.app_argv.argc() == 5);
        REQUIRE(opts.run_argv.argc() == 5);

        CHECK(opts.app_argv[0] == "./bin/c_inc");
        CHECK(opts.app_argv[1] == "input_file");
        CHECK(opts.app_argv[2] == "output_file");
        CHECK(opts.app_argv[3] == "44");
        CHECK(opts.app_argv[4] == "multi word arg"); // should have no quotes
                                                     //
        CHECK(opts.run_argv[0] == "./bin/c_inc");
        CHECK(opts.run_argv[1] == "input_file");
        CHECK(opts.run_argv[2] == "output_file");
        CHECK(opts.run_argv[3] == "44");
        CHECK(opts.run_argv[4] == "multi word arg");
    }
}

struct MockFs {
    MockFs() {
        if (std::filesystem::exists("./mockfs")) {
            throw std::runtime_error("./mockfs exists");
        }

        std::filesystem::create_directory("./mockfs");
        std::ofstream{"./mockfs/unittest_binary"};
        std::ofstream{"./mockfs/unittest_gdb"};
        std::ofstream{"./mockfs/unittest_txt"};
    }
    ~MockFs() {
        std::filesystem::remove_all("./mockfs");
    }

    static std::unique_ptr<MockFs> make() {
        try {
            return std::make_unique<MockFs>();
        } catch (const std::exception &e) {
            return nullptr;
        }
    }
};

TEST_CASE("DbgInvoke: LaunchArgs enforceValid() validates that files are present") {
    if (auto fs = MockFs::make()) {
        const char* const argv[] = {
            "launch", "$a{1}", "./mockfs/unittest_binary",
            "--workdir", "--gdb=unittest_gdb",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        using path = std::filesystem::path;

        auto cwd = std::filesystem::current_path();
        REQUIRE(std::filesystem::exists(cwd / path{"mockfs"}));

        // contains unittest_gdb
        setenv("PATH", "./mockfs", 1);

        REQUIRE_NOTHROW(opts.enforceValid());

        CHECK(opts.app_argv[0] == cwd / path{"mockfs/unittest_binary"});
        CHECK(opts.cwd == cwd / path{"mockfs"});
        CHECK(opts.gdb_path == cwd / path{"mockfs/unittest_gdb"});
    } else {
        WARN_MESSAGE(false, "Failed to create mockfs");
    }
}

TEST_CASE("DbgInvoke: launch args enforceValid replaces relative paths with full paths") {
    if (auto fs = MockFs::make()) {
        const char* const argv[] = {
            "launch", "$a{1}", "./mockfs/unittest_binary",

            "--launcher=./mockfs/unittest_binary",
            "--launcher-input=./mockfs/unittest_txt",
            "--gdb=./mockfs/unittest_gdb",
            "--workdir=./mockfs",
            "--qsub=./mockfs/unittest_txt",
            "--sbatch=./mockfs/unittest_txt",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        using path = std::filesystem::path;

        auto cwd = std::filesystem::current_path();
        REQUIRE(std::filesystem::exists(cwd / path{"mockfs"}));

        REQUIRE_NOTHROW(opts.enforceValid());

        CHECK(opts.app_argv[0] == cwd / path{"mockfs/unittest_binary"});
        CHECK(opts.run_argv[0] == cwd / path{"mockfs/unittest_binary"});
        CHECK(opts.launcher_argv[0] == cwd / path{"mockfs/unittest_binary"});
        CHECK(opts.input_file_path == cwd / path{"mockfs/unittest_txt"});
        CHECK(opts.gdb_path == cwd / path{"mockfs/unittest_gdb"});
        CHECK(opts.cwd == cwd / path{"mockfs"});
        CHECK(opts.qsub_file_path == cwd / path{"mockfs/unittest_txt"});
        CHECK(opts.sbatch_file_path == cwd / path{"mockfs/unittest_txt"});
    } else {
        WARN_MESSAGE(false, "Failed to create mockfs");
    }
}

TEST_CASE("DbgInvoke: LaunchArgs --gdb=/path/to/gdb:/path/to/datadir") {
    if (auto fs = MockFs::make()) {
        const char* const argv[] = {
            "launch", "$a{1}", "./mockfs/unittest_binary",
            "--gdb=./mockfs/unittest_gdb:./mockfs",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        using path = std::filesystem::path;

        auto cwd = std::filesystem::current_path();
        REQUIRE(std::filesystem::exists(cwd / path{"mockfs"}));

        REQUIRE_NOTHROW(opts.enforceValid());

        // something like /path/to/mockfs/unittest_gdb:/path/to/mockfs
        auto full_paths = std::string{cwd} + "/mockfs/unittest_gdb:" + std::string{cwd} + "/mockfs";
        CHECK(opts.gdb_path == full_paths);
    } else {
        WARN_MESSAGE(false, "Failed to create mockfs");
    }
}

TEST_CASE("DbgInvoke: LaunchArgs --gdb=/path/to/gdb: edge case") {
    if (auto fs = MockFs::make()) {
        const char* const argv[] = {
            "launch", "$a{1}", "./mockfs/unittest_binary",
            "--gdb=./mockfs/unittest_gdb:",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        using path = std::filesystem::path;

        auto cwd = std::filesystem::current_path();
        REQUIRE(std::filesystem::exists(cwd / path{"mockfs"}));

        REQUIRE_NOTHROW(opts.enforceValid());

        // colon is removed
        CHECK(opts.gdb_path == std::string{cwd} + "/mockfs/unittest_gdb");
    } else {
        WARN_MESSAGE(false, "Failed to create mockfs");
    }
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

bool DbgInvoke::deactivate(const DbgInvoke::Handle& handle) {
    // find it
    auto it = std::find_if(g_allInvokes.begin(), g_allInvokes.end(), [&handle](const auto& diptr) {
        return handle == diptr->handle();
    });

    if (it == g_allInvokes.end()) return false;

    // remove it
    auto expiring = std::move(*it);
    g_allInvokes.erase(it);

    rebuildCachedHandles();

    return true;
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
    o.m_proto_outp = nullptr;
    o.m_frontend_gdb = nullptr;
}

DbgInvoke::~DbgInvoke() {
    HoldSignals();

    // Quit the debug frontend if there is one
    if (m_frontend_gdb) {
        auto e = DbgDestroyFE(m_frontend_gdb);
        FreeEvent(e);
    }

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

const std::vector<int>& DbgInvoke::eventFds() const { return m_evfds; }
void DbgInvoke::addEventFd(int fd) { m_evfds.push_back(fd); }
void DbgInvoke::initializeEventFds() {
    m_evfds.clear();
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

const LaunchOpts& DbgInvoke::launchOptions() const { return m_launch_opts; }
LaunchOpts& DbgInvoke::launchOptions() { return m_launch_opts; }

DebugClientProtocol& DbgInvoke::proto() { return m_proto; }

DebugClientNetwork* DbgInvoke::network() const { return m_proto_net.get(); }

void DbgInvoke::setNetwork(std::unique_ptr<DebugClientNetwork>&& net) {
    m_proto_net = std::move(net);
}

dbgfrontend_sw* DbgInvoke::instantiateFrontendGdb(const char* impl, const char* gdbPath) {
    if (m_frontend_gdb) {
        auto e = DbgDestroyFE(m_frontend_gdb);
        FreeEvent(e);
    }

    if (impl) {
        m_frontend_gdb = DbgInstantiateFE(impl, gdbPath);
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

LaunchOpts::LaunchOpts(int argc, const char* const constArgv[], ClientSettingsInterface& csi) {
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

    int parsed_gpu_flag, parsed_debug_flag, parsed_non_mpi_flag;
    parsed_gpu_flag = parsed_debug_flag = parsed_non_mpi_flag = 0;

    launcher_argv << ""; // reserve launcher name
    app_argv << "";      // reserve binary name

    const struct option launch_long_opts[] = {
        {"args", required_argument, 0, 'a'},
        {"launcher-args", required_argument, 0, 'g'},
        {"launcher-input", required_argument, 0, 'i'},
        {"workdir", optional_argument, 0, 'd'},
        {"env", required_argument, 0, 'e'},
        {"gpu", no_argument, &parsed_gpu_flag, 1},
        {"debug", no_argument, &parsed_debug_flag, 1},
        {"qsub", required_argument, 0, 'q'},
        {"sbatch", required_argument, 0, 's'},
        {"non-mpi", no_argument, &parsed_non_mpi_flag, 1},
        {"launcher", required_argument, 0, 'l'},
        {"gdb", required_argument, 0, 'b'},
        {"gdb-data-dir", required_argument, 0, 'B'},
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
    while ((opt = getopt_long(argc, argv.v.data(), "a:g:i:d::b:l:e:q:s:", launch_long_opts, nullptr)) != -1) {
        switch (opt) {
            case 0:
                // if this is a flag, do nothing
                break;

            case 'a':
                if (optarg == nullptr) {
                    throw std::runtime_error("--args provided, but missing arguments.");
                }
                app_argv.parseAndAppendArgs(withoutQuotes(optarg));

                // remove quotes from individual args
                for (size_t i = 0; i < app_argv.argc(); i++) {
                    app_argv[i] = withoutQuotes(app_argv[i]);
                }

                break;

            case 'l':
                if (optarg == nullptr) {
                    throw std::runtime_error("--launcher provided, but missing arguments.");
                }

                // replace on first instance of --launcher only
                if (launcher_argv[0] == "") {
                    launcher_argv[0] = withoutQuotes(optarg);
                }
                break;

            case 'g':
                if (optarg == nullptr) {
                    throw std::runtime_error("--launcher-args provided, but missing arguments.");
                }
                launcher_argv.parseAndAppendArgs(withoutQuotes(optarg));

                // remove quotes from individual args
                for (size_t i = 0; i < launcher_argv.argc(); i++) {
                    launcher_argv[i] = withoutQuotes(launcher_argv[i]);
                }
                break;

            case 'i':
                if (optarg == nullptr) {
                    throw std::runtime_error("--launcher-input provided, but missing arguments.");
                }
                input_file_path = withoutQuotes(optarg);
                break;

            case 'd':
                if (optarg == nullptr) {
                    // --workdir provided without argument, so default to chdir
                    // to the directory that the binary is located in.
                    set_default_cwd = true;
                } else {
                    cwd = withoutQuotes(optarg);
                }
                break;

            case 'e': {
                if (optarg == nullptr) {
                    throw std::runtime_error("--env provided, but missing arguments.");
                }

                // Check formatting
                auto view = std::string_view{optarg};
                if (view.size() < 3 || std::count(view.begin(), view.end(), '=') != 1){
                    using namespace std::string_literals;
                    throw std::runtime_error("Invalid --env string\""s + optarg
                        + "\". Expected \"KEY=VALUE\"");
                }

                env << optarg;
                break;
            }

            case 'b':
                if (optarg == nullptr) {
                    throw std::runtime_error("--gdb provided, but missing argument.");
                }
                gdb_path = withoutQuotes(optarg);
                break;

            case 'B':
                if (std::filesystem::is_directory(optarg)) {
                    gdb_data_dir = optarg;
                } else {
                    throw std::runtime_error("--gdb-data-dir is not a directory.");
                }
                break;
                    

            case 'q':
                if (optarg == nullptr) {
                    throw std::runtime_error("--qsub provided, but missing arguments.");
                }
                qsub_file_path = withoutQuotes(optarg);
                break;

            case 's':
                if (optarg == nullptr) {
                    throw std::runtime_error("--sbatch provided, but missing arguments.");
                }
                sbatch_file_path = withoutQuotes(optarg);
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
        throw std::runtime_error("No process set specified");
    }

    // The second argument is the name of the a.out
    if (auto binary_path_cstr = argv.v[optind++]) {
        app_argv[0] = binary_path_cstr;
    } else {
        throw std::runtime_error("Invalid binary (No binary passed)");
    }

    if (set_default_cwd) {
        // --workdir provided without argument, so default to chdir to
        // the directory that the binary is located in.
        auto binary_path = std::filesystem::path{app_argv[0]};
        cwd = binary_path.parent_path();
    }

    gpu_flag = parsed_gpu_flag;
    debug_flag = parsed_debug_flag;
    non_mpi_flag = parsed_non_mpi_flag;
    run_argv = app_argv;

    non_stop_flag = csi.isNonStop();
    early_entry_flag = csi.isEarlyEntry();
    unsafe_flag = csi.isUnsafe();
    proto_name = csi.getProto();
    arch_name = csi.getArch();
    backend_name = csi.getBackend();
}

void LaunchOpts::enforceValid() {
    namespace fs = std::filesystem;
    using namespace std::string_literals;

    const auto fTypeStr = [](fs::file_type t) {
        std::string fType = "";
        switch(t) {
            case fs::file_type::none: fType = "is not evaluated"; break;
            case fs::file_type::not_found: fType = "does not exist"; break;
            case fs::file_type::regular: fType = "is a regular file"; break;
            case fs::file_type::directory: fType = "is a directory"; break;
            case fs::file_type::symlink: fType = "is a symlink"; break;
            case fs::file_type::block: fType = "is a block device"; break;
            case fs::file_type::character: fType = "is a character device"; break;
            case fs::file_type::fifo: fType = "is a named IPC pipe"; break;
            case fs::file_type::socket: fType = "is a named IPC socket"; break;
            case fs::file_type::unknown: fType = "has `unknown` type"; break;
        }
        return fType;
    };

    const auto canonFile = [fTypeStr](std::string& path) {
        if (!path.empty()) {
            // throws if path doesn't exist
            auto canon_path = fs::canonical(path);
            auto status = fs::status(canon_path); // follows links
            if (!fs::is_regular_file(status)) {
                throw std::runtime_error(
                    canon_path.string() + " is not a regular file, it " + fTypeStr(status.type())
                );
            }
            path = canon_path;
        }
    };

    canonFile(app_argv[0]);
    canonFile(run_argv[0]);
    canonFile(launcher_argv[0]);
    canonFile(input_file_path);
    canonFile(qsub_file_path);
    canonFile(sbatch_file_path);

    if (!cwd.empty()) {
        // throws if cwd doesn't exist
        auto canon_cwd = fs::canonical(cwd);
        auto status = fs::status(canon_cwd); // follows links
        if (!fs::is_directory(status)) {
            throw std::runtime_error(
                canon_cwd.string() + " is not a directory, it " + fTypeStr(status.type())
            );
        }
        cwd = canon_cwd;
    }

    if (!gdb_path.empty()) {
        // the user can specify this option as --gdb=/path/to/gdb:/path/to/gdb/datadir or
        // just --gdb=/path/to/gdb
        auto colon_idx = gdb_path.find(":");

        auto gdb_binary = gdb_path.substr(0, colon_idx);
        auto data_dir = std::string{""};
        if (colon_idx != std::string::npos) {
            data_dir = gdb_path.substr(colon_idx + 1);
        }

        // check gdb path
        if (auto p = pathFind(gdb_binary.c_str(), nullptr)) {
            namespace fs = std::filesystem;
            // make sure p is cleaned up in case fs:canonical throws
            auto p_str = std::string{p};
            free(p);
            gdb_binary = fs::canonical(p_str);
            canonFile(gdb_binary);
        } else {
            using namespace std::string_literals;
            throw std::runtime_error(
                "Could not find --gdb provided ("s + gdb_path + ") in PATH."
            );
        }

        // check datadir path if present
        if (!data_dir.empty()) {
            // throws if data_dir doesn't exist
            auto canon_data_dir = fs::canonical(data_dir);
            auto status = fs::status(canon_data_dir); // follows links
            if (!fs::is_directory(status)) {
                throw std::runtime_error(
                    canon_data_dir.string() + " is not a directory, it " + fTypeStr(status.type())
                );
            }
            data_dir = canon_data_dir;
        }

        // compose full path string
        gdb_path = gdb_binary;
        if (!data_dir.empty()) gdb_path += ":" + data_dir;
    }
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
