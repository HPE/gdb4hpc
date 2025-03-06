// Copyright 2024-2025 Hewlett Packard Enterprise Development LP.

#include "client.h"

namespace {

int cmd_maint_mqd_foo(rec_set_t* rec_set) {
    // (this function doesn't do anything useful yet)

    auto rep_set = DbgMQDFoo(rec_set);
    auto had_error = false;
    had_error = ProcessErrorInRepSet(rep_set);
    return had_error ? Command::Failure : Command::Success;
}

} // anon namespace

void ConsoleCommandsFactory::insert_msgqueue_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "maint mqd foo", .ptype = proc_required, .fn = cmd_maint_mqd_foo,
        .help = nullptr, .desc = nullptr});
}
