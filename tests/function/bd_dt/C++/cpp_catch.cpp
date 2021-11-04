#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>

#include <stdexcept>
#include <string>

#include "mpi.h"

using namespace std::string_literals;

void load_and_execute(const char *name) {
    std::string path = BD_DT + "/c/lib" + name + ".so";
    
    void *handle = dlopen(path.c_str(), RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Failed to dlopen %s (%s).\n", path.c_str(), dlerror());
        return;
    }

    auto fn = (void (*)(void))(dlsym(handle, name));
    if (!fn) {
        fprintf(stderr, "Failed to look up %s (%s).\n", name, dlerror());
        return;
    }

    fn();
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        exit(0);
    }

    int myRank, numProcs;

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    // catch throw
    try {
        throw std::runtime_error("Catch me!");
    } catch (const std::runtime_error &e) {
        // catch catch
    }

    // catch load/unload
    load_and_execute("foo");
    load_and_execute("bar");

    MPI_Finalize();

    // catch fork
    if (fork()) {
        // continue in parent
    } else {
        // exit in child
        exit(0);
    }

    // catch vfork
    if (vfork()) {
        // continue in parent
    } else {
        // exit in child
        _exit(0);
    }

    // catch exec
    std::string path = BD_DT + "/C++/cpp_catch.x";
    char exit_arg[] = "exit";
    char * new_argv[] = {argv[0], exit_arg, NULL};
    execvp(path.c_str(), new_argv);
}
