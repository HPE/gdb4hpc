#pragma once

#include <memory>
class UnaryOp; class IStream;

class StreamReader {
public:
    StreamReader(std::shared_ptr<IStream> in);
    void pipeTo(std::shared_ptr<UnaryOp> to);
    
    void start() const;
private:
    std::shared_ptr<IStream> m_in;
    std::shared_ptr<UnaryOp> m_to;
};

