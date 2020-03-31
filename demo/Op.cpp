
#include "UnaryOp.h"

Op::Op(const std::string& name) : m_name(name) {}

bool Op::isTerminator() const { return false; }

bool Op::isSource() const { return false; }

bool Op::start() const { return false; }

void Op::done() {
    if (m_output)
        m_output->done();
}

void Op::pipeTo(std::shared_ptr<UnaryOp> dest) {
    m_output = dest;
}


