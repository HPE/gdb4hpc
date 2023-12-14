// exercise:
// - breakpoints
// - watchpoints
// - catchpoints
// and their ability to be set only on certain threads

#include <chrono>
#include <iostream>
#include <thread>
#include <utility>

#include "mpi.h"

int global = 0;

void loop_stuff() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for (int i = 0; i < 100; i++) {
        global += 1;
        global += i;
    }
}

void throw_stuff() {
    try {
        throw global;
    } catch (...) {
        global += 1;
        global += 2;
    }
}

void stuff() {
    loop_stuff();
    throw_stuff();
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    std::thread t2(stuff);
    stuff();

    t2.join();

    MPI_Finalize();
    return 0;
}
