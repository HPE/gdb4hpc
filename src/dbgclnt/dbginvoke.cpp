/******************************************************************************\
 * Copyright 2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "dbginvoke.h"
#include "dbgclnt_proto.h"
#include "dbg_client.h"
#include "path.h"
#include "str.h"
#include "dense_bitset.h"

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
        auto r = di.instantiateFrontendGdb(nullptr);
        CHECK(r != nullptr);
        REQUIRE(di.frontendGdb() != nullptr);

        di.resetFrontendGdb();
        CHECK(di.frontendGdb() == nullptr);
    }
}

TEST_CASE("DbgInvoke: set/call/unset event callback function") {
    auto fn = [](DbgInvoke::EventSet&, const Bitset&) {
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

TEST_CASE("rep_set_t and group_recv operators") {
    // rep_set_t::operator== uses the operator== of the contained group_recvs.
    // That operator in turn relies on DbgInvoke::== and Bitset::==.
    //
    // DbgInvoke::== relies on the DbgInvoke having a handle. DbgInvokes get
    // handles by registering themselves in the global DbgInvoke list.

    auto Launched = DbgInvoke::ProcessType::Launched;
    auto di0 = std::make_unique<DbgInvoke>(Launched, 3, *findClientProtoByName("zmqnet"));
    auto di1 = std::make_unique<DbgInvoke>(Launched, 4, *findClientProtoByName("zmqnet"));
    auto dih0 = DbgInvoke::activate("test_di_0", std::move(di0));
    auto dih1 = DbgInvoke::activate("test_di_1", std::move(di1));

    const auto make_bitset = [](size_t size, size_t first, size_t second) {
        auto bitset = std::make_unique<DenseBitset>(size);
        bitset->set(first);
        bitset->set(second);
        return bitset;
    };

    auto bs_011 = make_bitset(3, 0, 1);
    auto bs_110 = make_bitset(3, 1, 2);
    auto bs_101 = make_bitset(3, 0, 2);
    auto bs_0110 = make_bitset(4, 1, 2);

    // pointers are used for these tests because the rec_set_t code is pointer based.

    auto di0_011 = std::make_unique<group_recv>(
        group_recv{ .rec_di = *dih0.lock(), .rec_group = bs_011->clone() });
    auto di0_110 = std::make_unique<group_recv>(
        group_recv{ .rec_di = *dih0.lock(), .rec_group = bs_110->clone() });
    auto di1_0110 = std::make_unique<group_recv>(
        group_recv{ .rec_di = *dih1.lock(), .rec_group = bs_0110->clone() });

    SUBCASE("group_recv ==") {
        CHECK(*di0_011 == *di0_011);
        auto di0_011_clone = std::make_unique<group_recv>(
            group_recv{ .rec_di = *dih0.lock(), .rec_group = bs_011->clone() });
        CHECK(*di0_011 == *di0_011_clone); // check that == is not comparing addresses
        CHECK(*di0_011 != *di0_110);
        CHECK(*di0_011 != *di1_0110);
    }

    SUBCASE("group_recv ordering") {
        // same elements, different initial order
        auto left_set = std::vector<group_recv*>{ di0_011.get(), di0_110.get(), di1_0110.get() };
        auto right_set = std::vector<group_recv*>{ di0_110.get(), di1_0110.get(), di0_011.get() };

        // duplicate elements
        auto dupe_set = std::vector<group_recv*>{ di0_011.get() /*1*/, di0_011.get() /*2*/, di1_0110.get() };
        auto dupe_set_2 = std::vector<group_recv*>{ di0_011.get() /*1*/, di1_0110.get(), di0_011.get() /*2*/};

        REQUIRE(left_set != right_set);
        REQUIRE(left_set != dupe_set);
        REQUIRE(left_set != dupe_set_2);
        REQUIRE(right_set != dupe_set);
        REQUIRE(right_set != dupe_set_2);
        REQUIRE(dupe_set != dupe_set_2);

        const auto deref = [](const auto* ptr) -> const auto& { return *ptr; };

        std::ranges::sort(left_set, {}, deref);
        std::ranges::sort(right_set, {}, deref);
        std::ranges::sort(dupe_set, {}, deref);
        std::ranges::sort(dupe_set_2, {}, deref);

        CHECK(std::ranges::equal(left_set, right_set, {}, deref, deref));
        CHECK(std::ranges::equal(dupe_set, dupe_set_2, {}, deref, deref));
    }

    SUBCASE("rec_set ==") {
        auto left_rset = rec_set_t();
        auto right_rset = rec_set_t();

        // addReceiver takes ownership of the passed group, so we make some clones
        auto di0_011_clone = std::make_unique<group_recv>(
            group_recv{ .rec_di = *dih0.lock(), .rec_group = bs_011->clone() });
        auto di0_110_clone = std::make_unique<group_recv>(
            group_recv{ .rec_di = *dih0.lock(), .rec_group = bs_110->clone() });
        auto di1_0110_clone = std::make_unique<group_recv>(
            group_recv{ .rec_di = *dih1.lock(), .rec_group = bs_0110->clone() });

        REQUIRE(left_rset == right_rset);

        SUBCASE("1") {
            left_rset.addReceiver(di0_011.release());
            right_rset.addReceiver(di0_110_clone.release());
            REQUIRE(left_rset != right_rset);

            // both sets will merge the passed bitsets into single di0_111
            left_rset.addReceiver(di0_110.release());
            right_rset.addReceiver(di0_011_clone.release());
            REQUIRE(left_rset == right_rset);

            left_rset.addReceiver(di1_0110.release());
            REQUIRE(left_rset != right_rset);

            right_rset.addReceiver(di1_0110_clone.release());
            REQUIRE(left_rset == right_rset);
        }

        SUBCASE("2") {
            left_rset.addReceiver(di0_011.release());
            left_rset.addReceiver(di1_0110.release());
            REQUIRE(left_rset != right_rset);

            right_rset.addReceiver(di0_011_clone.release());
            REQUIRE(left_rset != right_rset);

            right_rset.addReceiver(di1_0110_clone.release());
            REQUIRE(left_rset == right_rset);

            left_rset.addReceiver(di0_110.release());
            REQUIRE(left_rset != right_rset);

            right_rset.addReceiver(di0_110_clone.release());
            REQUIRE(left_rset == right_rset);
        }

        SUBCASE("3") {
            // verify that order doesn't matter
            left_rset.addReceiver(di0_011.release());
            left_rset.addReceiver(di1_0110.release());
            right_rset.addReceiver(di1_0110_clone.release());
            right_rset.addReceiver(di0_011_clone.release());
            REQUIRE(left_rset == right_rset);
        }
    }

    // clean up as to not affect other tests
    DbgInvoke::deactivateAll();
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

// Automatically join thread in destructor to allow collection of
// cleanup threads from expiring DbgInvoke objects
std::vector<DbgInvoke::WaitableThread> g_allThreads;

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

DbgInvoke::~DbgInvoke() {
    HoldSignals();
    ReleaseSignals();

    // Disown exit thread for later collection
    if (m_exitThread) {
        g_allThreads.emplace_back(std::move(*m_exitThread));
    }
}

bool DbgInvoke::operator==(const DbgInvoke& other) const {
    return m_handle == other.m_handle;
}

bool DbgInvoke::operator!=(const DbgInvoke& other) const {
    return !(*this == other);
}

DbgInvoke::Handle DbgInvoke::handle() {
    if (m_handle.lock() == nullptr) {
        // The handle is put in place by DbgInvoke::activate. Don't call handle() before that.
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
    m_currently_python = m_bitset_full->clone();
    m_currently_python->clear();
}

const Bitset* DbgInvoke::launchedBitset() const { return m_bitset_launched.get(); }
void DbgInvoke::setLaunchedBitset(std::unique_ptr<Bitset>&& bitset) {
    m_bitset_launched = std::move(bitset);
}
void DbgInvoke::orLaunchedBitset(const Bitset& rhs) {
    if (!m_bitset_launched) return;
    *m_bitset_launched |= rhs;
}

const Bitset* DbgInvoke::getPythonBitset() const {
    return m_currently_python.get();
}

void DbgInvoke::setPython(const Bitset& bitset) {
    if (!m_currently_python || m_currently_python->sizeInBits() != bitset.sizeInBits()) return;
    *m_currently_python |= bitset;
}

void DbgInvoke::unsetPython(const Bitset& bitset) {
    if (!m_currently_python || m_currently_python->sizeInBits() != bitset.sizeInBits()) return;
    *m_currently_python -= bitset;
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

std::unique_ptr<DebugClientNetwork> DbgInvoke::ejectNetwork() {
    return std::move(m_proto_net);
}

void DbgInvoke::adoptExitThread(DbgInvoke::WaitableThread&& wt){
    m_exitThread = std::move(wt);
}

DbgFE* DbgInvoke::instantiateFrontendGdb(const char* gdbPath) {
    m_frontend_gdb.reset(new DbgFE(gdbPath));
    return m_frontend_gdb.get();
}
void DbgInvoke::resetFrontendGdb() { m_frontend_gdb.reset(); }

DbgFE* DbgInvoke::frontendGdb() const { return m_frontend_gdb.get(); }

void DbgInvoke::clearFrontendGdb() {
    if (m_frontend_gdb) m_frontend_gdb->clearListState();
}

void DbgInvoke::initCachedFrames() {
    if (m_bitset_full) {
        m_recent_frames = std::make_unique<BitsetMRUCache<dbgloc_t>>(*m_bitset_full);
    }
}

const BitsetMRUCache<dbgloc_t>* DbgInvoke::getCachedFrames() const {
    return m_recent_frames.get();
}

void DbgInvoke::cacheFrame(std::unique_ptr<Bitset>&& ranks, dbgloc_t&& frame) {
    if (!m_recent_frames) return;
    m_recent_frames->push(std::move(ranks), std::move(frame));
}

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

DbgInvoke::WaitableThread::WaitableThread(std::thread&& t, std::shared_ptr<std::mutex> mtx,
    std::shared_ptr<std::condition_variable> cv, std::shared_ptr<bool> joinable)
    : m_thread{std::move(t)}
    , m_mtx{std::move(mtx)}
    , m_cv{std::move(cv)}
    , m_joinable{std::move(joinable)}
    , m_threadValid{true}
{}

DbgInvoke::WaitableThread::WaitableThread(WaitableThread&& expiring)
    : m_thread{std::move(expiring.m_thread)}
    , m_mtx{std::move(expiring.m_mtx)}
    , m_cv{std::move(expiring.m_cv)}
    , m_joinable{std::move(expiring.m_joinable)}
    , m_threadValid{expiring.m_threadValid}
{
    expiring.m_threadValid = false;
}

DbgInvoke::WaitableThread& DbgInvoke::WaitableThread::operator= (WaitableThread&& rhs) {
    m_thread = std::move(rhs.m_thread);
    m_mtx = std::move(rhs.m_mtx);
    m_cv = std::move(rhs.m_cv);
    m_joinable = std::move(rhs.m_joinable);
    m_threadValid = rhs.m_threadValid;

    rhs.m_threadValid = false;

    return *this;
}

DbgInvoke::WaitableThread::~WaitableThread()
{
    if (!m_threadValid) {
        return;
    }

    try {
        auto lck = std::unique_lock<std::mutex>{*m_mtx};

        // Wait for cleanup thread to complete
        if (m_cv->wait_for(lck, std::chrono::seconds(10),
            [this]{ return *m_joinable; })) {

            m_thread.join();

        // Timed out, detach thread
        } else {
            PrintOutMsg("warning: cleanup timed out, your job may still be running\n");
            m_thread.detach();
        }

    } catch (...) {
        // Ignore exception
    }
}
