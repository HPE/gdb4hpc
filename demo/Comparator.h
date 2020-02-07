#pragma once

#include <string>
#include <memory>

class ReadDataStream; class WriteDataStream;

// demo/scratchpad for working out comparison operator stuff
class Comparator {
public:
    Comparator(std::shared_ptr<ReadDataStream> fromA, std::shared_ptr<WriteDataStream> toA,
               std::shared_ptr<ReadDataStream> fromB, std::shared_ptr<WriteDataStream> toB);
    
    void compare(const std::string& expression);

private:
    std::shared_ptr<ReadDataStream> m_fromA;
    std::shared_ptr<WriteDataStream> m_toA;
    std::shared_ptr<ReadDataStream> m_fromB;
    std::shared_ptr<WriteDataStream> m_toB;
};

