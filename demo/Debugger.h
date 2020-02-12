#pragma once

#include <string>
#include <random>

// Class to simulate the working of a gdb back-end connected to a real process
class Debugger {
public:
    Debugger(const std::string& type, size_t size, size_t numDiffs, double diffMult);

    void setParams(const std::string& type, size_t size, size_t numDiffs);

    // first version, we're simulating a 
    std::string getType() const;
    std::string getNumElems() const;
    std::string getSomeElements(size_t minIdx, size_t numElems);

    Debugger(const Debugger&) = delete;
    Debugger& operator=(const Debugger&) = delete;

private:
    bool isDifferent();
    double getDoubleVal();
    int getIntVal();
    
    double getDoubleDiff();
    int getIntDiff();

    std::string m_type;
    std::string m_sizeString;
    bool m_isDouble;
    bool m_hasDiffs;
    size_t m_size;
    double m_diffMult;

    std::minstd_rand m_rand;
    std::minstd_rand::result_type m_rand_cutoff;
};

    
  
