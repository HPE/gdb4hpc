#include "BackEnd.h"
#include "Error.h"
#include "IStream.h"
#include "OStream.h"
#include "Debugger.h"
#include "Value.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>

BackEnd::BackEnd(std::shared_ptr<Debugger> debugger, 
                 std::shared_ptr<IStream> in,
                 std::shared_ptr<OStream> out)
    : m_debugger(debugger), m_in(in), m_out(out)
{}

// thread main for the back-end
void BackEnd::operator()() 
{
    try {
        dispatchLoop();
    } catch (Error& err) {
        std::cerr << "caught error in back-end " << err.str() << "\n";
    } catch (...) {
        std::cerr << "caught unknown error in back-end\n";
    }
}

void BackEnd::dispatchLoop()
{
    while (true) {
        while (!m_in->hasData()) { // temporary - I'll thread this up later
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        int commandVal = m_in->getInt();

        if (commandVal == 12) 
            evalExpression();
        else if (commandVal == 13) {
            evalSetParams();
        }
    }
}

void BackEnd::evalExpression()
{
    int numElems = stoi(m_debugger->getNumElems());
    std::string type = m_debugger->getType();
    bool isDouble = (type == "double");

    // give the array type and dimension
    /*
    m_out->putInt((isDouble ? Stream::DOUBLE : Stream::INT));
    m_out->putInt(numElems);
    */

    const int QUERY_SIZE = 1000;
    for (int minIdx = 0; minIdx < numElems; minIdx += QUERY_SIZE) {
        int n = std::min(numElems-minIdx, QUERY_SIZE);
        std::string sval = m_debugger->getSomeElements(minIdx, n);
        
        std::istringstream ss(sval);
        if (isDouble) {
            double buf[QUERY_SIZE];
            std::copy_n(std::istream_iterator<double>(ss), n, buf);
            m_out->putDoubles(buf,n);
        } else {
            int buf[QUERY_SIZE];
            std::copy_n(std::istream_iterator<int>(ss), n, buf);
            m_out->putInts(buf,n);
        }
    }
    m_out->eof();
}

void BackEnd::evalSetParams()
{
    Value typeVal, sizeVal, diffVal;     
    int typeId, size, diff;
    int retVal = -1;
    if (!m_in->read(typeVal) || !m_in->read(sizeVal) || !m_in->read(diffVal)) {
        std::cerr << "could not read debugger parameters\n";
    } else if (!typeVal.to(typeId) || !sizeVal.to(size) || !diffVal.to(diff)) {
        std::cerr << "expected integral parameter values";
    } else if (typeId < 0 || typeId > 1) {
        std::cerr << "unexpected type " + std::to_string(typeId);
    } else {
        m_debugger->setParams( typeId == 0 ? "int" : "double", size, diff);
        retVal = 0;
    }

    m_out->putInt(retVal);
    m_out->eof();
        
}

void BackEnd::start(std::shared_ptr<Debugger> debugger, 
                    std::shared_ptr<IStream> in,
                    std::shared_ptr<OStream> out)
{
    std::thread beTA([debugger,in,out]() {
            auto beA = std::make_shared<BackEnd>(debugger, in, out);
            (*beA)();
        });

    beTA.detach();
}
