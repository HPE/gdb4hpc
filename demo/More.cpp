#include "More.h"
#include "IStream.h"
#include "Error.h"
#include "Value.h"
#include "OpFactory.h"
#include "OpBuilder.h"
#include <iostream>

static OpFactory factory("more", More::make);

More::More(std::shared_ptr<IStream> in, size_t numItems)
    : UnaryTerminator("more", in), m_maxItems(numItems), m_numPrinted(0), m_quit(false)
{}

std::shared_ptr<Op> More::make(OpBuilder& builder)
{
    if (builder.getNumIStreams() != 1) {
        std::cerr << "$more is a terminating operation\n";
        return nullptr;
    }

    int numItems = 50;
    std::string args = builder.getOpArgs();
    if (!args.empty()) {
        try {
            numItems = std::stoi(args);
        } catch (std::exception) {
            std::cerr << "expecting an integer argument for $more\n";
        }
        numItems = std::max(0,numItems);
    }

    return std::make_shared<More>(builder.getNthIStream(0), numItems);
}

void More::apply()
{
    Value val;
    if (!getIn()->read(val)) {
        notifyDone();
        return;
    }
    
    if (m_quit)
        return;

    std::cout << val << "\n";

    if (++m_numPrinted == m_maxItems) {
        std::cerr << "----- more? -----\n";
        
        std::string resp;
        std::cin >> resp;
        
        if (resp.size() && resp[0] == 'q') {
            //getIn()->close();
            m_quit = true;
        } else {
            m_numPrinted = 0;
        }
    }
}
