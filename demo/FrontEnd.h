#pragma once

#include <string>
#include <memory>

class IStream; class OStream;

// demo/scratchpad for working out comparison operator stuff
class FrontEnd {
public:
    FrontEnd(std::shared_ptr<IStream> fromA, std::shared_ptr<OStream> toA,
             std::shared_ptr<IStream> fromB, std::shared_ptr<OStream> toB);
    
    void execute(const std::string& command, bool& isQuit);

    std::shared_ptr<OStream> getCommandStreamForProcSet(const std::string& procSet) const;
    std::shared_ptr<IStream> getResultStreamForProcSet(const std::string& procSet) const;

private:
    void execPrint(const std::string& expression);
    void execSet(const std::string& params);
    
    std::shared_ptr<IStream> m_fromA;
    std::shared_ptr<OStream> m_toA;
    std::shared_ptr<IStream> m_fromB;
    std::shared_ptr<OStream> m_toB;
};

