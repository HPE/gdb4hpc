#include "More.h"
#include "IStream.h"
#include "Error.h"
#include "Value.h"
#include "OpFactory.h"
#include "OpBuilder.h"
#include <iostream>

static OpFactory factory("more", More::make);

More::More(size_t numItems)
    : UnaryTerminator("more"), m_maxItems(numItems), m_numPrinted(0), m_quit(false)
{}

std::shared_ptr<Op> More::make(OpBuilder& builder)
{
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

    return std::make_shared<More>(numItems);
}

void More::apply(const Value& val)
{
    if (m_quit)
        return;

    std::cout << val << "\n";

    if (++m_numPrinted == m_maxItems) {
        std::cerr << "----- more? -----\n";
        
        std::string resp;
        std::cin >> resp;
        
        if (resp.size() && resp[0] == 'q') {
            // lookat- can this notify waiters??
            m_quit = true;
        } else {
            m_numPrinted = 0;
        }
    }
}
