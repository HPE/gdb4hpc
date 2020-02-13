#include "FrontEnd.h"
#include "IStream.h"
#include "OStream.h"
#include "Error.h"
#include "Compare.h"
#include "Connection.h"
#include "More.h"
#include "Collect.h"
#include "Value.h"
#include "OpBuilder.h"

#include <iostream>
#include <sstream>

FrontEnd::FrontEnd(std::shared_ptr<IStream> fromA, std::shared_ptr<OStream> toA,
                   std::shared_ptr<IStream> fromB, std::shared_ptr<OStream> toB)
    : m_fromA(fromA), m_toA(toA), m_fromB(fromB), m_toB(toB)
{
}

void FrontEnd::execute(const std::string& command, bool& isQuit)
{
    std::cerr << "executing " << command << "\n";

    isQuit = false;
    if (command.substr(0,2) == "p ") {
        OpBuilder opBuilder(*this);
        if (opBuilder.parse(command.substr(2))) {
            opBuilder.addDefaultTerminator();
            opBuilder.evaluate();
        } else {
            std::cerr << "could not parse expression in " << command;
        }
            
        return;
    } else if (command.substr(0,4) == "set ") {
        execSet(command.substr(4));
        return;
    } else if (command.substr(0,4) == "quit") {
        isQuit = true;
        return;
    }

    std::cerr << "did not recognized command in\"" << command << "\"\n";
    std::cerr << "supported commands are p|set|quit\n";
    /*
    Connection output;
    Compare compare(m_fromA, m_fromB, output.getWriter());

    More more(output.getReader(), 5);

    // send out the clowns
    m_toA->putInt(12);
    m_toB->putInt(12);

    more.wait();
    */
}

void FrontEnd::execPrint(const std::string& expression) {
    std::cerr << "print the value of " << expression << "\n";

    Connection output;

    Compare compare(m_fromA, m_fromB, output.getWriter());
    More more(output.getReader(), 30);

    m_toA->putInt(12);
    m_toB->putInt(12);
    more.wait();
    compare.detach();
}
void FrontEnd::execSet(const std::string& expression) {
    std::cerr << "set the debug parameters\n";
    
    std::istringstream str(expression);
    std::string typeStr; int num, numDiff;

    if (!(str >> typeStr >> num >> numDiff)) {
        std::cerr << "could not parse set parameters " << expression << "\n";
        return;
    }
    
    int typeId = 0;
    if (typeStr == "int")
        typeId = 0;
    else if (typeStr == "double")
        typeId = 1;
    else {
        std::cerr << "unrecognized type for set command: " << typeStr << "\n";
        return;
    }

    Collect collectA(m_fromA,2), collectB(m_fromB,2);

    int vals[4] = { 13, typeId, num, numDiff };
    m_toA->putInts(vals, 4);
    m_toB->putInts(vals, 4);

    collectA.wait();
    collectB.wait();

    collectA.detach();
    collectB.detach();

    if (collectA.getValues().empty()) {
        throw Error("did not get response from $a\n");
    }
    if (collectB.getValues().empty()) {
        throw Error("did not get response from $b\n");
    }

    if (collectA.getValues().size() != 1) {
        throw Error("got extraneous response from $a\n");
    }
    if (collectB.getValues().size() != 1) {
        throw Error("got extraneous response from $b\n");
    }
    
    Value expectedValue(int(0));
    if (collectA.getValues()[0] != expectedValue) {
        throw Error("got an error value from $a\n");
    }

    if (collectB.getValues()[0] != expectedValue) {
        throw Error("got an error value from $a\n");
    }
}



std::shared_ptr<OStream> FrontEnd::getCommandStreamForProcSet(const std::string& procSet) const
{
    if (procSet == "a")
        return m_toA;
    else if (procSet == "b")
        return m_toB;
    else {
        std::cerr << "unrecognized procset " << procSet << "\n";
        return nullptr;
    }
}
std::shared_ptr<IStream> FrontEnd::getResultStreamForProcSet(const std::string& procSet) const
{
    if (procSet == "a")
        return m_fromA;
    else if (procSet == "b")
        return m_fromB;
    else {
        std::cerr << "unrecognized procset " << procSet << "\n";
        return nullptr;
    }
}
