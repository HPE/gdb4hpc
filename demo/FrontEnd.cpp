#include "FrontEnd.h"
#include "IStream.h"
#include "OStream.h"
#include "Error.h"
#include "Compare.h"
#include "More.h"
#include "Collect.h"
#include "Value.h"
#include "OpBuilder.h"
#include "StreamReader.h"

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
        std::shared_ptr<UnaryOp> head; std::shared_ptr<Op> tail;
        if (opBuilder.parse(command.substr(2), head, tail)) {
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

    StreamReader fromA(m_fromA), fromB(m_fromB);

    auto collectA = std::make_shared<Collect>(2);
    auto collectB = std::make_shared<Collect>(2);

    fromA.pipeTo(collectA);
    fromB.pipeTo(collectB);

    int vals[4] = { 13, typeId, num, numDiff };
    m_toA->putInts(vals, 4);
    m_toB->putInts(vals, 4);
    
    fromA.start();
    fromB.start();

    collectA->wait();
    collectB->wait();

    auto valsA = collectA->getValues();
    auto valsB = collectB->getValues();

    if (valsA.empty()) {
        throw Error("did not get response from $a\n");
    }
    if (valsB.empty()) {
        throw Error("did not get response from $b\n");
    }

    if (valsA.size() != 1) {
        throw Error("got extraneous response from $a\n");
    }
    if (valsB.size() != 1) {
        throw Error("got extraneous response from $b\n");
    }
    
    Value expectedValue(int(0));
    if (valsA[0] != expectedValue) {
        throw Error("got an error value from $a\n");
    }

    if (valsB[0] != expectedValue) {
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
