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
        // XXX: currently this output scales pretty poorly, even with as little as
        // 8 ranks (especially with -v, MPI makes a lot of internal unnamed communicators)
        auto pager = cli_pager{};

        long not_shown = 0;

        for (const auto& reply : rep_set) {
            if (reply->rep_event->ev_event != DBGEV_MQD_COMMUNICATORS ||
                reply->rep_event->ev_u.ev_u_mqd_communicators == nullptr)
            {
                continue;
            }

            const auto& comms = *reply->rep_event->ev_u.ev_u_mqd_communicators;
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "MPI Communicators: %lu\n", comms.communicators.size());

            for (const auto& comm : comms.communicators) {
                PrintOutMsg("- %s: rank %lu of %lu\n", comm.name.c_str(), comm.rank, comm.size);
            }

            not_shown += comms.not_shown;
        }

        if (not_shown > 0) {
            PrintOutMsg("... and %lu more communicators not shown. (Use -v or --verbose to see them)\n", not_shown);
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
}
