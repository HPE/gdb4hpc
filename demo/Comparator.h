#pragma once

#include <string>
#include <memory>

class IStream; class OStream;

// demo/scratchpad for working out comparison operator stuff
class Comparator {
public:
    Comparator(std::shared_ptr<IStream> fromA, std::shared_ptr<OStream> toA,
               std::shared_ptr<IStream> fromB, std::shared_ptr<OStream> toB);
    
    void compare(const std::string& expression);

private:
    std::shared_ptr<IStream> m_fromA;
    std::shared_ptr<OStream> m_toA;
    std::shared_ptr<IStream> m_fromB;
    std::shared_ptr<OStream> m_toB;
};

