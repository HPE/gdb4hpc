// Copyright 2024-2025 Hewlett Packard Enterprise Development LP.

#include "client.h"
#include "help_dict.h"
#include <algorithm>

namespace {

int cmd_info_mpi_communicators(rec_set_t* rec_set, int argc, const char* const argv[]) {
    // if -v or --verbose is passed, included unnamed communicators
    auto verbose = (argc > 1 && (::strcmp(argv[1], "-v") == 0 || ::strcmp(argv[1], "--verbose") == 0));

    auto rep_set = DbgMQDGetCommunicators(rec_set, verbose);

    {
        auto pager = cli_pager{};

        long not_shown = 0;
        std::map<std::string, std::vector<std::string>> commMap;
        for (const auto& reply : rep_set) {
            if (reply->rep_event->ev_event != DBGEV_MQD_COMMUNICATORS ||
                reply->rep_event->ev_u.ev_u_mqd_communicators == nullptr)
            {
                continue;
            }

            const auto& comms = *reply->rep_event->ev_u.ev_u_mqd_communicators;

            for (const auto& comm : comms.communicators) {
                std::ostringstream oss;
                int start = comm.ranks[0];
                int prev = comm.ranks[0];

                for (const auto& rank : comm.ranks) {
                    if(rank == start) continue;
                    if(rank==prev+1){
                        prev=rank;
                        continue;
                    }
                    !oss.str().empty()? oss << ",": oss << reply->rep_di.c_str() << "{";
                    start == prev? oss << start : oss << start << ".." << prev;
                    start = rank;
                    prev = rank;
                }
                !oss.str().empty() ? oss << "," : oss << reply->rep_di.c_str() << "{";
                start == prev? oss << start << "}" : oss << start << ".." << prev << "}";
                auto& communicator = commMap[comm.comm.name];
                if(std::find(communicator.begin(), communicator.end(), oss.str()) == communicator.end()){
                    communicator.push_back(oss.str());
                }
            }

            not_shown += comms.not_shown;
        }
        for(const auto& entry : commMap){
            std::ostringstream oss;
            for(const auto& str:entry.second){
                if(!oss.str().empty()) oss << ","; 
                oss << str;
            }
            PrintOutMsg("%s:%s\n",entry.first.c_str(),oss.str().c_str());
        }

        if (not_shown > 0) {
            PrintOutMsg("... and %lu more communicators not shown. (Use -v or --verbose to see them)\n", not_shown);
        }
    }

    auto had_error = ProcessErrorInRepSet(rep_set);
    return had_error ? Command::Failure : Command::Success;
}

void print_pending_operation(const dbg_mqd_operation_t& p) {
    std::string tag = (int)p.tag_wild?"*":std::to_string((int)p.desired_tag)+"/"+std::to_string((int)p.actual_tag);
    PrintOutMsg("%s (%ld/%ld):   %10s %20s %10s(%#lx) %10ld/%ld %20ld/%ld %20ld/%ld  %20s\n", 
                p.comm.name.c_str(),(long)p.comm.rank,(long)p.comm.size,
                to_string(p.status).c_str(), to_string(p.origin_queue).c_str(), 
                p.system_buffer ? "System" : "User", p.buffer_addr,
                (long)p.desired_local_rank, (long)p.actual_local_rank, 
                (long)p.desired_global_rank, (long)p.actual_global_rank, 
                (long)p.desired_length, (long)p.actual_length,
                tag.c_str());

    std::ostringstream oss;
    for (const auto& txt : p.extra_text) {
        if(!oss.str().empty() && oss.str().back() != ',') oss << ",";
        for(char c: txt){
            if(std::isprint(static_cast<unsigned char>(c))){
                oss << c;
            }else{
                break;
            }
        }
    }
    PrintOutMsg("Extra Text: %s\n", oss.str().c_str());
}

int cmd_info_mpi_pending(rec_set_t* rec_set) {
    auto rep_set = DbgMQDGetPending(rec_set);

    {
        auto pager = cli_pager{};

        PrintOutMsg("%20s %20s %20s %30s %20s %20s %20s\n",
           "Status","Queue","Buffer","Local Rank(D/A)","Global Rank(D/A)","Length(D/A)","Tag(D/A)");
        for (const auto& reply : rep_set) {
            if (reply->rep_event->ev_event != DBGEV_MQD_OPERATIONS ||
                reply->rep_event->ev_u.ev_u_mqd_operations == nullptr)
            {
                continue;
            }

            const auto& ops = *reply->rep_event->ev_u.ev_u_mqd_operations;
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),"\n");

            for (const auto& op : ops.operations) {
                print_pending_operation(op);
            }
        }
    }

    auto had_error = ProcessErrorInRepSet(rep_set);
    return had_error ? Command::Failure : Command::Success;
}

} // anon namespace

void ConsoleCommandsFactory::insert_msgqueue_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "info mpi",
        .ptype = proc_required,
        .fn = nullptr,
        .help = nullptr,
        .desc = "Show information about the state of MPI internals.",
    });

    root.addSubCmd({
        .name = "info mpi communicators",
        .ptype = proc_required,
        .fn = cmd_info_mpi_communicators,
        .help = cmd_info_mpi_communicators_help,
        .desc = "List MPI communicators.",
    });

    root.addSubCmd({
        .name = "info mpi pending",
        .ptype = proc_required,
        .fn = cmd_info_mpi_pending,
        .help = cmd_info_mpi_pending_help,
        .desc = "List pending MPI messages.",
    });
}
