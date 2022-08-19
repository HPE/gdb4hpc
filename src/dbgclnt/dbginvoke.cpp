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

DbgInvoke::DbgInvoke(dbgproc_type type, int nproc, DebugClientProtocol * const proto)
    : d_appid{0}
    , d_type{type}
    , d_started{0}
    , d_isShutdown{0}
    , d_nproc{nproc}
    , d_stat{0}
    , d_evfds{nullptr}
    , d_bitset_full{nullptr}
    , d_bitset_launched{nullptr}
    , d_proto_id{0}
    , d_proto_net{nullptr}
    , d_proto_outp{nullptr}
    , d_proto{proto}
    , d_clnt_ev_callback{nullptr}
    , d_frontend{nullptr}
    , d_eventset_list{}
    , d_breakpoints{}
    , d_checkpoints{}
    , m_handle(nullptr)
{};

DbgInvoke::DbgInvoke(DbgInvoke&& o)
    : d_appid{o.d_appid}
    , d_type{o.d_type}
    , d_started{o.d_started}
    , d_isShutdown{o.d_isShutdown}
    , d_launch{std::move(o.d_launch)}
    , d_nproc{o.d_nproc}
    , d_stat{o.d_stat}
    , d_evfds{o.d_evfds}
    , d_bitset_full{std::move(o.d_bitset_full)}
    , d_bitset_launched{std::move(o.d_bitset_launched)}
    , d_proto_id{o.d_proto_id}
    , d_proto_net{std::move(o.d_proto_net)}
    , d_proto_outp{o.d_proto_outp}
    , d_proto{o.d_proto}
    , d_clnt_ev_callback{std::move(o.d_clnt_ev_callback)}
    , d_frontend{o.d_frontend}
    , d_eventset_list{std::move(o.d_eventset_list)}
    , d_breakpoints{std::move(o.d_breakpoints)}
    , d_checkpoints{std::move(o.d_checkpoints)}
    , m_handle{std::move(o.m_handle)}
{
    o.d_appid = 0;
    o.d_started = 0;
    o.d_isShutdown = 1;
    o.d_nproc = 0;
    o.d_evfds = nullptr;
    o.d_proto_outp = nullptr;
    o.d_frontend = nullptr;
}

DbgInvoke::~DbgInvoke() {
    HoldSignals();

    // XXX has large side effects. does it belong in this dtor?
    d_proto->destroyClient(this);

    // Quit the debug frontend if there is one
    if (d_frontend != nullptr) {
        auto e = DbgDestroyFE(d_frontend);
        FreeEvent(e);
    }

    // remove any event fds from the proto
    DestroyList(d_evfds, free);

    ReleaseSignals();
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

DbgInvoke* DbgInvoke::Handle::lock() {
    return m_ptr.lock().get();
}

const DbgInvoke* DbgInvoke::Handle::lock() const {
    // OK as long as lock() never gains any side effects
    return const_cast<DbgInvoke::Handle*>(this)->lock();
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
