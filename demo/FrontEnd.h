#pragma once

#include <string>
#include <memory>

class IStream; class OStream;

//! Simulate the working of a tinker-toy gdb4hpc front end.
/*! We are only handling a couple of operations "set", "print", and
  "quit" to explore the basic data interaction in isolation.
*/
class FrontEnd {
public:
    // Set up the front-end with connections to and from two back-ends.
    // So is the command stream going to the first back-end, and fromA is the
    // data stream from that back-end.   
    FrontEnd(std::shared_ptr<IStream> fromA, std::shared_ptr<OStream> toA,
             std::shared_ptr<IStream> fromB, std::shared_ptr<OStream> toB);
    
    // Execute a user level command.   We are handling only a couple of 
    // operations "set", "print" and "quit" to explore the
    void execute(const std::string& command, bool& isQuit);

    std::shared_ptr<OStream> getCommandStreamForProcSet(const std::string& procSet) const;
    std::shared_ptr<IStream> getResultStreamForProcSet(const std::string& procSet) const;

private:
    void execSet(const std::string& params);
    
    std::shared_ptr<IStream> m_fromA;
    std::shared_ptr<OStream> m_toA;
    std::shared_ptr<IStream> m_fromB;
    std::shared_ptr<OStream> m_toB;
};

