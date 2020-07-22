// Small test program to exercise the thread handling of the debugger.
// It's entire goal is just to fire off a bunch of threads and keep them
// occupied for 30 seconds so I can stop with the debugger.

#define USE_MPI
#ifdef USE_MPI
#include "mpi.h"
#endif

#include <iostream>
#include <thread>
#include <future>
#include <vector>

int main(int argc, char** argv) {

#ifdef USE_MPI
    int ierr = MPI_Init(&argc, &argv);
    if (ierr != MPI_SUCCESS) {
	    std::cout << stderr << "Error: MPI_init() failed: " << ierr;
    }
    int rank;
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (ierr != MPI_SUCCESS) {
        std::cout << stderr << "Error: MPI_init() failed: " << ierr;
    }
    int num_ranks;
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);
    if (ierr != MPI_SUCCESS) {
        std::cout << stderr << "Error: MPI_init() failed: " << ierr;
    }
#endif

    int numThreads = 10;
    if (argc >= 2) {
        try {
            numThreads = std::stoi(argv[1]);
        } catch (std::exception& err) {
            std::cerr << "usage " << argv[0] << " [numThreads]\n";
            return -1;
        }
    }

    std::vector<std::future<double>> futures(numThreads);
    std::condition_variable cond;
    std::mutex mutex;
    bool ready = false;

    // create the thread - they should all wait on the condition variable
    // I know async is just a filthy lie..
    for (int i=0; i<numThreads; ++i) {
        futures[i] = std::async(std::launch::async, [&cond,&mutex,&ready] {
                std::unique_lock<std::mutex> lock(mutex);
                cond.wait(lock, [&ready] { return ready; });

                double total = 0.0;
                for (int i=0; i<100; ++i) {
                    total += i;
                }

                return total;
            });
    }

    // make sure the threads have started
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // and start them off
    { std::lock_guard<std::mutex> lock(mutex); }

    ready = true;
    cond.notify_all();

    // and gather the results
    for (auto&& future : futures) {
        std::cout << "value is " << future.get() << "\n";
    }


#ifdef USE_MPI    
    ierr = MPI_Finalize();
    if (ierr != MPI_SUCCESS) {
        std::cout << stderr << "Error: MPI_Finalize() failed: " << ierr;
    }
#endif

    return 0;
}

