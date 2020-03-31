#pragma once

#include <memory>
class UnaryOp; class IStream;

//! Adapter class to pipe the input from a stream into 
//! an operator output.
class StreamReader {
public:
    StreamReader(std::shared_ptr<IStream> in);
    void pipeTo(std::shared_ptr<UnaryOp> to);
    
    // Start the read operation in a new thread.
    void start() const;
private:
    std::shared_ptr<IStream> m_in;
    std::shared_ptr<UnaryOp> m_to;
};

