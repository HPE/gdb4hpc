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

// Used to determine type of dbginvoke, currently there is either launched or
// attached launched means the application was launched by the debugger,
// attached means it was an existing application attached onto.
enum dbgproc_type { DBG_ATTACHED, DBG_LAUNCHED };
typedef enum dbgproc_type dbgproc_type;

class DbgInvoke {
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
        DbgInvoke* lock();
        const DbgInvoke* lock() const;

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
                                           // if d_type == DBG_LAUNCHED, stores passed WLM launcher
                                           // arguments. if the user passes --launcher, argv[0] will
                                           // be set and CTI_LAUNCHER_NAME_ENV_VAR should be set before
                                           // launch. these arguments can't be changed by the user after launch.
                                           //
        ArgvStream app_argv;               // --args="application argv"
                                           // if d_type == DBG_LAUNCHED, holds app's argv,
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

        SourcePaths() {};
        SourcePaths(const SourcePaths&) = delete;
        SourcePaths& operator=(const SourcePaths&)= delete;

    private:
        std::vector<std::string> m_sourceList; // Stores all values in order of arrival.
    };

public: // TODO privatize
    DbgInvoke(dbgproc_type type, int nproc, DebugClientProtocol * const proto);
    DbgInvoke(DbgInvoke&& other);

    DbgInvoke(const DbgInvoke& other) = delete;
    DbgInvoke& operator=(const DbgInvoke& other) = delete;

    ~DbgInvoke();

    bool operator==(const DbgInvoke& other) const {
        return m_handle == other.m_handle;
    }

    bool operator!=(const DbgInvoke& other) const {
        return !(*this == other);
    }

    DbgInvoke::Handle handle();
    const DbgInvoke::Handle handle() const;
    uint64_t d_appid;     // app identifier - this should be generic
    dbgproc_type d_type;  // launched or attached?
    int d_started;        // indicates if this di has been started
    int d_isShutdown;  // Some networks can use this during error handling. If
                       // this is true, the network is already shutdown.

    LaunchOpts d_launch;
    int d_nproc;       // number of processes
    int d_stat;   // exit/signal status of proc
    list_t *d_evfds;        // event notification fds specific to this instance
    std::unique_ptr<Bitset> d_bitset_full;     // represents all dbgsrvs
    std::unique_ptr<Bitset> d_bitset_launched; // for invoke cmd: represents dbgsrvs that has been launched

    int d_proto_id;  // di specific protocol id - optionally used by certain
                     // protocols
    std::unique_ptr<DebugClientNetwork> d_proto_net;  // di specific protocol network object
    g_progoutp_t* d_proto_outp; // data used by the protocol layer for application output
    DebugClientProtocol * const d_proto;     // client protocol switch

    AsyncEventCallback d_clnt_ev_callback;

    dbgfrontend_sw *d_frontend;  // debug frontend object

    SourcePaths source_paths;

    BreakpointManager& breakpoints() { return d_breakpoints; }
    CheckpointManager& checkpoints() { return d_checkpoints; }
    const BreakpointManager& breakpoints() const { return d_breakpoints; }
    const CheckpointManager& checkpoints() const { return d_checkpoints; }

    // XXX: Using std::list because some legacy functions return pointers directly into the list
    std::list<EventSet> d_eventset_list; // partial async event responses

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
    BreakpointManager d_breakpoints; // logical breakpoints for this di
    CheckpointManager d_checkpoints; // logical checkpoints for this di

    // register() will set up the di's internal m_handle when it is registered
    Handle m_handle;
};

namespace std {

template <>
struct hash<DbgInvoke::Handle> {
    std::size_t operator()(const DbgInvoke::Handle& key) const {
        return std::hash<std::string>{}(key.name());
    }
};

} // namespace std

