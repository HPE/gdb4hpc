#include "More.h"
#include "IStream.h"
#include "Error.h"
#include <iostream>

More::More(std::shared_ptr<IStream> in, size_t numItems)
    : UnaryTerminator(in), m_maxItems(numItems), m_numPrinted(0), m_quit(false)
{}

void More::apply()
{
    std::any val;
    if (!getIn()->read(val)) {
        throw Error("unexpected EOF during read");
    }
    
    if (m_quit)
        return;

    if (val.type() == typeid(double)) {
        std::cout << std::any_cast<double>(val) << "\n";
    } else if (val.type() == typeid(int)) {
        std::cout << std::any_cast<int>(val) << "\n";
    } else {
        std::cout << "unrecognized type\n";
    }

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
