#pragma once

#include <memory>

class Debugger; class ReadDataStream; class WriteDataStream;

class BackEnd
{
public:
    BackEnd(std::shared_ptr<Debugger> debugger, 
            std::shared_ptr<ReadDataStream> in,
            std::shared_ptr<WriteDataStream> out);

    void operator()();

    static void start(std::shared_ptr<Debugger> debugger, 
                      std::shared_ptr<ReadDataStream> in,
                      std::shared_ptr<WriteDataStream> out);

private:
    void dispatchLoop();
    void evalExpression();

    std::shared_ptr<Debugger> m_debugger;
    std::shared_ptr<ReadDataStream> m_in;
    std::shared_ptr<WriteDataStream> m_out;
};

    
