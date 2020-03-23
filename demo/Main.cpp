#include <string>
#include <iostream>
#include <vector>

#include "Debugger.h"
#include "AsyncConnection.h"
#include "BackEnd.h"
#include "FrontEnd.h"

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

    auto toA = AsyncConnection::make();
    auto fromA = AsyncConnection::make();

    BackEnd::start(debA, toA->getReader(), fromA->getWriter());

    auto toB = AsyncConnection::make();
    auto fromB = AsyncConnection::make();

    BackEnd::start(debB, toB->getReader(), fromB->getWriter());

    //fromA->start();
    //fromB->start();

    std::vector<std::string> userCommands = { 
        "p $a::a",
        "set int 40 0",
        "p $a::a $| $more(10)",
        "set double 500 100",
        "p $a::a - $b::a $| $range()",
        "p $a::a - $b::a $| $gt(30.0) $| $more(10)",
        "quit"
    };

    FrontEnd frontEnd(fromA->getReader(), toA->getWriter(),
                      fromB->getReader(), toB->getWriter());

    bool doQuit = false;
    for (std::string userCommand : userCommands) {
        std::cout << "execute " << userCommand << "?";
        std::string resp;
        std::cin >> resp;
        if (!resp.empty() && resp[0] == 'n')
            continue;

        frontEnd.execute(userCommand, doQuit);
        if (doQuit)
            break;
    }
}

static void usage(const std::string& exec)
{
    std::cerr << "usage " << exec << " double|int size differences\n";
}
