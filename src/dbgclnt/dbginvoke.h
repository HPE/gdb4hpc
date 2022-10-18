/******************************************************************************\
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <list>
#include <vector>
#include <functional>
#include <memory>

#include "dbgclnt_settings.h"
#include "dbgclnt_breakpoints.h"
#include "dbgclnt_network.h"
#include "dbgclnt_debugfe.h"
#include "dbgoutp.h"
#include "bitset.h"
#include "args.h"

class DebugClientProtocol;

class DbgInvoke {
    // A DbgInvoke (di) is a handle for a HPC application. The application is made of a
    // collection of ranks, where each rank has a single instance of gdb attached to it.
    //
    // Dis are created by the DbgLaunch and DbgAttach functions.
public:
    // Types

    // Used to store a dbginvoke long term in other classes
    class Handle {
    public:
        Handle(std::string name, std::shared_ptr<DbgInvoke> parent);
        Handle(nullptr_t); // construct an empty handle. lock() will return nullptr

        const std::string& name() const;
        const char* c_str() const; // helper for name().c_str()

        // Returns a pointer to parent DbgInvoke. If parent DbgInvoke has
        // expired, returns nullptr. Don't store returned pointer long-term.
        DbgInvoke* lock() const;

        bool operator==(const Handle& other) const {
            const auto* lhs = m_ptr.lock().get();
            const auto* rhs = other.m_ptr.lock().get();
            if (lhs && rhs) return lhs == rhs; // both alive
            else if (lhs || rhs) return false; // one expired, one alive
            return name() == other.name();     // both expired
        }

        bool operator!=(const Handle& other) const {
            return !(*this == other);
        }

        bool operator<(const Handle& other) const {
            const auto* lhs = m_ptr.lock().get();
            const auto* rhs = other.m_ptr.lock().get();
            if (lhs && rhs) return name() < other.name(); // both alive
            else if (lhs && !rhs) return false;           // rhs expired. nullptr is ordered first
            else if (!lhs && rhs) return true;            // lhs expired. nullptr is ordered first
            return false;                                 // both expired
        }

    private:
        mutable std::string m_name;
        std::weak_ptr<DbgInvoke> m_ptr;
    };

    // Used to track async events and how many ranks have reported a given event
    class EventSet {
    public:
        // Clones group and event
        EventSet(DbgInvoke& di, const Bitset& group, const dbgevent_t* event);
        ~EventSet();

        DbgInvoke& di() { return m_di; }
        Bitset& group() { return *m_group; }
        dbgevent_t* event() { return m_event; }

        const DbgInvoke& di() const { return m_di; }
        const Bitset& group() const { return *m_group; }
        const dbgevent_t* event() const { return m_event; }
    private:
        DbgInvoke& m_di;
        std::unique_ptr<Bitset> m_group;
        dbgevent_t* m_event;
    };

    // This is the client callback function prototype
    // Arguments are the event_set for this event and a bitset containing the new
    // ranks to report this event
    using AsyncEventCallback = std::function<int(EventSet&, const Bitset&)>;

    class LaunchOpts {
        // LaunchOpts stores arguments initially passed to the launch
        // command that created the dbginvoke.
    private:
        // to allow a default dbginvoke constructor
        LaunchOpts() = default;
        friend DbgInvoke;
    public:
        // throws std::runtime_error upon parse error
        LaunchOpts(int argc, const char* const argv[], ClientSettingsInterface& csi);

        ArgvStream launcher_argv;          // --launcher="custom_launcher"
                                           // --launcher-args="launcher argv"
                                           // if processType() == DBG_LAUNCHED, stores passed WLM launcher
                                           // arguments. if the user passes --launcher, argv[0] will
                                           // be set and CTI_LAUNCHER_NAME_ENV_VAR should be set before
                                           // launch. these arguments can't be changed by the user after launch.
                                           //
        ArgvStream app_argv;               // --args="application argv"
                                           // if processType() == DBG_LAUNCHED, holds app's argv,
                                           // with the name of the binary as argv[0].
                                           //
        ArgvStream run_argv;               // args to apply when a `run` command is invoked. initially
                                           // holds app_argv, but args can be changed with "set args" 
                                           // command. argv[0] should never change.
                                           //
        std::string gdb_path = "";         // --gdb /path/to/custom/gdb
        std::string input_file_path = "";  // --launcher-input /path/to/input/file
        std::string cwd = "";              // --workdir /path/to/workdir
        bool set_default_cwd = false;      // true if --workdir is passed with no argument. if true,
                                           // the cwd of the program should be set to the directory
                                           // the actual binary file is located in.
        ArgvStream env;                    // --env "name=value"
        bool gpu_flag = false;             // --gpu
        bool non_mpi_flag = false;         // --non-mpi
        bool debug_flag = false;           // --debug
        std::string qsub_file_path = "";   // --qsub /path/to/script
        std::string sbatch_file_path = ""; // --sbatch /path/to/script
        std::string procset_name;          // full name of passed procset, including rank count, e.g. $a{3}
                                           //
        bool non_stop_flag = true;         // settings copied from passed ClientSettingsInterface
        bool early_entry_flag = false;     //
        bool unsafe_flag = false;          //
        std::string proto_name = "";       //
        std::string arch_name = "";        // arch and proto are legacy settings that don't change
        std::string backend_name = "";     // any more, but are still passed down
    };

    class SourcePaths{
    public:
        void storeSourcePath(const char * str);
        std::string getFullPath(const std::string& relativePath) const;

        SourcePaths() = default;
        SourcePaths(SourcePaths&& other) = default;

        SourcePaths(const SourcePaths&) = delete;
        SourcePaths& operator=(const SourcePaths&)= delete;

    private:
        std::vector<std::string> m_sourceList; // Stores all values in order of arrival.
    };

    // Used to determine type of dbginvoke, currently there is either launched or
    // attached launched means the application was launched by the debugger,
    // attached means it was an existing application attached onto.
    enum class ProcessType {
        Attached,
        Launched,
    };

public:
    DbgInvoke(ProcessType type, int nproc, DebugClientProtocol& proto);
    DbgInvoke(DbgInvoke&& other);

    DbgInvoke(const DbgInvoke& other) = delete;
    DbgInvoke& operator=(const DbgInvoke& other) = delete;

    ~DbgInvoke();

    // return true if `other` refers to the same set of compute node applications/gdb instances
    bool operator==(const DbgInvoke& other) const;
    bool operator!=(const DbgInvoke& other) const;

    // Get handle for this DbgInvoke. Handles are safe to store long term elsewhere (raw
    // pointers are not). A DbgInvoke has to be activated before a handle can be
    // generated. See DbgInvoke::activate(...).
    DbgInvoke::Handle handle();
    const DbgInvoke::Handle handle() const;

    // App id for use with CTI
    uint64_t appId() const;
    void setAppId(uint64_t id);

    // Returns the process type associated with this DbgInvoke, either launched or attached.
    ProcessType processType() const;

    // Returns true if this di has been started.
    bool isStarted() const;
    // Set started flag to true. Can never be un-set.
    void setStarted();

    // Returns true if this di's state has not been changed with continue/step/etc
    bool isAtInitialBreakpoint() const;
    void setAtInitialBreakpoint(bool at_breakpoint);

    // Returns true if this di has been shut down.
    bool isShutdown() const;
    // Sets shutdown flag to true. Can never be un-set.
    void setShutdown();

    // Returns the options that the DbgInvoke was constructed with.
    const LaunchOpts& launchOptions() const;
    LaunchOpts& launchOptions();

    // Returns the number of ranks this DbgInvoke is associated with.
    int numRanks() const;
    void setNumRanks(int n);

    // Returns this DbgInvoke's list of file descriptors used for protocol events. If
    // initializeEventFds is not called before this, returns nullptr.
    //
    // TODO change to std::list or std::vector
    // TODO export a more fine-grained API for this
    list_t* eventFds();

    // Initilize the inner list of fds to an empty list. If there already exists a list,
    // it is destroyed along with its contents.
    void initializeEventFds();

    // Returns a bitset representing all ranks associated with the DbgInvoke. Returns
    // nullptr until the DbgInvoke is actually launched.
    const Bitset* fullBitset() const;
    void setFullBitset(std::unique_ptr<Bitset>&& bitset);

    // Returns a subset of fullBitset() representing which ranks have actually launched
    // successfully. Returns nullptr until the DbgInvoke is actually launched.
    //
    // TODO: any reason that these bitsets can't be initialized in the constructor? They
    // only depend on nproc for initialization.
    const Bitset* launchedBitset() const;
    void setLaunchedBitset(std::unique_ptr<Bitset>&& bitset);
    // Bitwise OR the `rhs` with the bitset that is returned by launchedBitset().
    void orLaunchedBitset(const Bitset& rhs);

    // Returns a data structure used to deliver application output from ranks to terminal.
    //
    // XXX: Also something that might make sense to be owned by the proto() object.
    g_progoutp_t* protoOutput();
    void initializeProtoOutput();
    void destroyProtoOutput();

    // The client protocol api. This api defines what operations are available in
    // communicating with the gdb4hpc backends.
    DebugClientProtocol& proto();

    // The client network api. This api defines more low level network operations than
    // proto(). Prefer using proto() when talking to the gdb4hpc backends. The returned
    // pointer is owned by the DbgInvoke and should not be stored long-term.
    //
    // If something goes wrong during the initial network setup step (initiated by
    // functions in proto()), this function will nullptr.
    //
    // XXX: A case can be made to have this owned by proto() instead of the di, it
    // probably shouldn't be observable to normal users of DbgInvoke.
    DebugClientNetwork* network() const;

    // Only used in proto() launch/attach functions.
    void setNetwork(std::unique_ptr<DebugClientNetwork>&& net);

    // Create an interface to a local instance of gdb for future utility usecases like
    // listing source files etc. The interface's implementation is determined by the
    // frontend_impl_type argument. See dbgclnt_debugfe.* files for availble interfaces.
    // If the DbgInvoke already has a frontend gdb interface, it is destroyed. Pass
    // nullptr to remove the interface without replacing it. If an invalid implementation
    // type is passed, the existing interface is destroyed and the interface is set to
    // nullptr.
    //
    // On success, this function returns the newly instantiated interface. On failure, it
    // returns nullptr.
    //
    // XXX: The only available implementation right now is "gdbmi".
    dbgfrontend_sw* instantiateFrontendGdb(const char* frontend_impl_type);

    // Interface to an instance of gdb running on the client machine for use in things
    // like source code listing. Create the interface with instantiateFrontendGdb first.
    // The creation of the frontend gdb is allowed to silently fail. In that case, this
    // function returns nullptr.
    dbgfrontend_sw* frontendGdb() const;

    // Get/set a funcion as the function to be called with callAsyncEventCallback. This is
    // typically in the main loop to tell a DbgInvoke that an async event (like a
    // breakpoint being hit) has happened.
    AsyncEventCallback getAsyncEventCallback() const;
    void setAsyncEventCallback(AsyncEventCallback func);

    // Call the registered event callback function and return the result. If no callback
    // was registered, throws std::bad_function_call.
    int callAsyncEventCallback(EventSet& eventset, const Bitset& bitset) const;

    SourcePaths& sourcePaths();
    const SourcePaths& sourcePaths() const;

    BreakpointManager& breakpoints();
    const BreakpointManager& breakpoints() const;

    CheckpointManager& checkpoints();
    const CheckpointManager& checkpoints() const;

    // Returns internal list of EventSets.
    //
    // XXX: Using std::list because some legacy functions return pointers directly into
    // the list
    // XXX: The list is only used in static functions in dbg_client.cpp. Probably doesn't
    // need to be owned by a DbgInvoke.
    std::list<EventSet>& eventSets();

    // API to access internal dbginvoke list

    // Transfers ownership of passed di to internal dbginvoke list.
    // If the di is inserted successfully, returns its handle.
    // Else returns a null Handle (.lock() returns nullptr).
    // Reasons that the insertion could fail are:
    // - a di with the passed name already exists
    // - the passed di is invalid (nullptr, invalid/empty name, etc)
    // On failure, the passed DbgInvoke pointer is untouched and is still usable.
    static Handle activate(std::string name, std::unique_ptr<DbgInvoke>&& di);

    // Removes di matching the given Handle from the internal list and returns it, if it exists.
    // If it doesn't exist, returns nullptr.
    // Any Handles pointing to the removed di become expired.
    static std::shared_ptr<DbgInvoke> deactivate(const DbgInvoke::Handle& handle);

    // Finds the DbgInvoke matching `handle` in the internal list and changes its
    // name to `newName` and returns the new handle. If renaming fails, returns
    // a null Handle. Any Handles pointing to the renamed di are still valid.
    static Handle rename(Handle handle, std::string newName);

    // Frees and invalidates all dbginvokes in the list
    static void deactivateAll();

    // Return list of handles of all active dbginvokes.
    // Note: this function returns a reference for speed, but the returned list's
    // size should not be changed with functions like .erase or .push_back.
    static std::vector<DbgInvoke::Handle>& getAll();

private:
    uint64_t m_appid = 0;

    ProcessType m_type;

    bool m_started_flag = false;
    bool m_at_init_breakpoint_flag = false;
    bool m_shutdown_flag = false;

    LaunchOpts m_launch_opts;

    int m_nproc;
    list_t *m_evfds = nullptr;
    g_progoutp_t* m_proto_outp = nullptr;

    std::unique_ptr<Bitset> m_bitset_full;
    std::unique_ptr<Bitset> m_bitset_launched;

    DebugClientProtocol& m_proto;
    std::unique_ptr<DebugClientNetwork> m_proto_net;

    dbgfrontend_sw* m_frontend_gdb = nullptr; // TODO convert to smart pointer

    AsyncEventCallback m_clnt_ev_callback;

    SourcePaths m_source_paths;

    BreakpointManager m_breakpoints; // logical breakpoints for this di
    CheckpointManager m_checkpoints; // logical checkpoints for this di
    std::list<EventSet> m_eventset_list; // partial async event responses

    // register() will set up the di's internal m_handle when it is registered
    Handle m_handle = nullptr;
};

namespace std {

template <>
struct hash<DbgInvoke::Handle> {
    std::size_t operator()(const DbgInvoke::Handle& key) const {
        return std::hash<std::string>{}(key.name());
    }
};

} // namespace std
