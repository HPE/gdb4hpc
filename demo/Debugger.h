#pragma once

#include <string>
#include <random>

//! Class to simulate the working of a gdb back-end connected to a real process
/*! Currently it pretty much is a generator function for simulationg a large array
  tied with a string based approach that looks enough like MI to get the idea.
*/
class Debugger {
public:
    Debugger(const std::string& type, size_t size, size_t numDiffs, double diffMult);

    // first version, we're simulating an array of type ("int" or "double") with size
    // elements - approximately numDiff of which deviate from the main value by 
    // diffMult
    void setParams(const std::string& type, size_t size, size_t numDiffs);

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

    
  
