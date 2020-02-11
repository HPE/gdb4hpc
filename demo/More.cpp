#include "More.h"
#include "IStream.h"
#include "Error.h"
#include "Value.h"
#include <iostream>

More::More(std::shared_ptr<IStream> in, size_t numItems)
    : UnaryTerminator(in), m_maxItems(numItems), m_numPrinted(0), m_quit(false)
{}

void More::apply()
{
    Value val;
    if (!getIn()->read(val)) {
        throw Error("unexpected EOF during read");
    }
    
    if (m_quit)
        return;

    std::cout << val << "\n";

    if (++m_numPrinted == m_maxItems) {
        std::cerr << "----- more? -----\n";
        
        std::string resp;
        std::cin >> resp;
        
        if (resp.size() && resp[0] == 'q') {
            getIn()->close();
        } else {
            m_numPrinted = 0;
        }
    }
}
