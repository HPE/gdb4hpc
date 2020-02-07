#include <string>
#include <iostream>
#include <thread>

#include "Debugger.h"
#include "Comparator.h"
#include "OneWayConnection.h"
#include "BackEnd.h"
#include "Dumper.h"

static void usage(const std::string& exec);

int main(int argc, char* argv[])
{
    if (argc < 4) {
        usage(argv[0]);
        return -1;
    }

    std::string type = argv[1];
    if (type != "double" && type != "int") {
        usage(argv[0]);
        return -1;
    }

    int size = atoi(argv[2]);
    if (size <= 0) {
        usage(argv[0]);
        return -1;
    }

    int diffs = atoi(argv[3]);
    
    auto debA = std::make_shared<Debugger>(type, size, diffs, 1.0);
    auto debB = std::make_shared<Debugger>(type, size, diffs, -1.0);
    //Comparator comparator(a, b);

    auto toA = OneWayConnection::make();
    auto fromA = OneWayConnection::make();
    
    BackEnd::start(debA, toA->getReader(), fromA->getWriter());

    if (false) {
      auto feA = std::make_shared<Dumper>(fromA->getReader(), toA->getWriter());
      feA->dumpVals();
    } else {
        auto toB = OneWayConnection::make();
        auto fromB = OneWayConnection::make();

        BackEnd::start(debB, toB->getReader(), fromB->getWriter());
        
        Comparator compare(fromA->getReader(), toA->getWriter(), 
                           fromB->getReader(), toB->getWriter());
        
        compare.compare("shazbot");
    }

    //comparator.compare("a == b");
}

static void usage(const std::string& exec)
{
    std::cerr << "usage " << exec << " double|int size differences\n";
}
