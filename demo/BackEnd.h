#pragma once

#include <memory>

class Debugger; class IStream; class OStream;

//! Simulate the gdb4hpc back-end process as a thread.
/*! The BackEnd connects a debugger simulator to the in and out
  data pipes simulating the command stream connection via MRNet.
  
  It runs as a separate stream serving commands from the input 
  stream, and runs in its own thread after being triggered by the
  start() function.
*/
class BackEnd
{
public:
    static void start(std::shared_ptr<Debugger> debugger, 
                      std::shared_ptr<IStream> in,
                      std::shared_ptr<OStream> out);

private:
    BackEnd(std::shared_ptr<Debugger> debugger, 
            std::shared_ptr<IStream> in,
            std::shared_ptr<OStream> out);

    void operator()();

    void dispatchLoop();
    void evalExpression();
    void evalSetParams();

    std::shared_ptr<Debugger> m_debugger;
    std::shared_ptr<IStream> m_in;
    std::shared_ptr<OStream> m_out;
};

    
