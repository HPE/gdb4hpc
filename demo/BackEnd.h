#pragma once

#include <memory>

class Debugger; class IStream; class OStream;

class BackEnd
{
public:
    BackEnd(std::shared_ptr<Debugger> debugger, 
            std::shared_ptr<IStream> in,
            std::shared_ptr<OStream> out);

    void operator()();

    static void start(std::shared_ptr<Debugger> debugger, 
                      std::shared_ptr<IStream> in,
                      std::shared_ptr<OStream> out);

private:
    void dispatchLoop();
    void evalExpression();
    void evalSetParams();

    std::shared_ptr<Debugger> m_debugger;
    std::shared_ptr<IStream> m_in;
    std::shared_ptr<OStream> m_out;
};

    
