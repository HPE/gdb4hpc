
#include "Op.h"

Op::Op(const std::string& name) : m_name(name) {}

bool Op::isTerminator() const { return false; }

bool Op::isSource() const { return false; }

bool Op::start() const { return false; }

