#include "Debugger.h"

#include <sstream>

Debugger::Debugger(const std::string& type, size_t size, size_t numDiffs, double diffMult)
    : m_diffMult(diffMult)
{
    setParams(type, size, numDiffs);
}

void Debugger::setParams(const std::string& type, size_t size, size_t numDiffs)
{
    m_type = type;
    m_sizeString = std::to_string(size);
    m_isDouble = (type == "double");
    m_hasDiffs = (numDiffs>0);
    m_size = size;
    m_rand_cutoff = m_rand.max();

    if (numDiffs > 0) {
        double bc = double(numDiffs) / size;
        m_rand_cutoff = std::minstd_rand::result_type(m_rand.min() * bc + m_rand.max() * (1-bc));
    }
}

// first version, we're simulating a 
std::string Debugger::getType() const { return m_type; }
std::string Debugger::getNumElems() const { return m_sizeString; }
std::string Debugger::getSomeElements(size_t minIdx, size_t numElems)
{
    if (minIdx == 0)
        m_rand.seed(0);

    bool first = true;
    std::ostringstream os;
    if (m_isDouble) {
        for (int i=0; i<numElems; ++i) {
            double d = getDoubleVal();
            if (isDifferent())
                d += getDoubleDiff();
            if (i)
                os << " ";
            os << d;
        }
    } else {
        for (int i=0; i<numElems; ++i) {
            int val = getIntVal();
            if (isDifferent())
                val += getIntDiff();
            if (i)
                os << " ";
            os << val;
        }
    }

    return os.str();
}

bool Debugger::isDifferent()
{
    return (m_hasDiffs && m_rand() > m_rand_cutoff);
}

double Debugger::getDoubleVal()
{
    return 100.0 * m_rand() / m_rand.max();
}

double Debugger::getDoubleDiff()
{
    return m_diffMult * 20.0 * m_rand() / m_rand.max() - 10.0;
}

int Debugger::getIntVal()
{
    return int(getDoubleVal());
}

int Debugger::getIntDiff()
{
    return int(getDoubleDiff());
}

