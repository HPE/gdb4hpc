// Small test program to exercise the thread handling of the debugger.
// It's entire goal is just to fire off a bunch of threads and keep them
// occupied for 30 seconds so I can stop with the debugger.

#include "mpi.h"
#include <iostream>
#include <thread>
#include <future>
#include <vector>

int main(int argc, char** argv) {

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

    int numThreads = 10;
    if (argc >= 2) {
        try {
            numThreads = std::stoi(argv[1]);
        } catch (std::exception& err) {
            std::cerr << "usage " << argv[0] << " [numThreads]\n";
            return -1;
        }
    }

    std::vector<std::future<std::pair<int,double>>> futures(numThreads);
    std::condition_variable cond;
    std::mutex valMutex;
    double val = 0.0;
    bool valAvail = false;

    // start the processors
    for (int i=0; i<numThreads; ++i) {
        futures[i] = std::async(std::launch::async, [&cond,&valMutex,&val,&valAvail] {
                // the processors wait on the condition value cond and then 
                // just keep the running values for a distributed average computation. 
                // Keep reading values until a "poison pill" is read.   Use the valAvail
                // as value consumed acknowledgement.
                int numVals = 0;
                double sum = 0.0;
                while (true) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::unique_lock<std::mutex> lk(valMutex);
                    cond.wait(lk, [&]{return valAvail;});
                    if (val < 0.0) {
                        return std::make_pair(numVals,sum);
                    } else {
                        sum += val;
                        valAvail = false;
                        ++numVals;
                    }
                }
            });
    }

    // pump in values on the front end - I want this to run for about 30 seconds, and
    // with the sleep in the processors code, this can process numThreads values per second
    int numVals = 30 * numThreads;
    for (int i=0; i<numVals; ++i) {
        {
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::lock_guard<std::mutex> lk(valMutex);
            val = double(rand()) / RAND_MAX;
            valAvail = true;
        }
        cond.notify_one();
        // hot-wait for one of the process threads to see the value.
        while (valAvail) {}
    }

    // kill the sub-threads - any value less than 0 is a poison pill
    for (int i=0; i<numThreads; ++i) {
        {
            std::lock_guard<std::mutex> lk(valMutex);
            val = -1;
            valAvail = true;
        }
        cond.notify_one();
    }

    // and compute the combined averages - and why not?
    int receivedVals = 0;
    double sum = 0.0;
    for (auto&& f : futures) {
        auto val = f.get();
        receivedVals += val.first;
        sum += val.second;
    }

    double avg = sum / receivedVals;

    // if we didn't mess up the threading, we better have received as many values as were
    // sent.
    if (receivedVals != numVals) {
        std::cerr << "there's a fox in the henhouse!\n";
    }

    ierr = MPI_Finalize();
    if (ierr != MPI_SUCCESS) {
        std::cout << stderr << "Error: MPI_Finalize() failed: " << ierr;
    }


    return 0;
}

