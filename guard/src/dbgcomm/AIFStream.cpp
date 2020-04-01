// Copyright 2020 Cray Inc.

// Classes for defining a sequential stream of data accessed via the AIF interface.
#include "AIFStream.h"

AIFStream::AIFStream(AIF* aif) :
    m_joinedAIF(aif, [this](AIF* aif) { this->freeAIF(aif); }),
    m_joinedIsStolen(false), m_joinedIsExposed(false)
{}

// I am hoping for the joinAIF call be to temporary for when I'm pushing through
// the pieced out version.
AIF* AIFStream::joinAIF() {
    if (m_joinedAIF.get() && m_joinedIsStolen && m_joinedIsExposed) {
        warning("possible problematic lifetime in AIFStream\n");
    }
    m_joinedIsExposed = true;
    return m_joinedAIF.get();
}

AIF* AIFStream::joinAIF(steal_t) {
    if (m_joinedAIF.get() && m_joinedIsExposed) {
        warning("trying to steal an AIF that has already been exposed");
    }
    m_joinedIsExposed = m_joinedIsStolen = true;
    return m_joinedAIF.get();
}

void AIFStream::warning(const char* message) {
    fprintf(stderr, "AIFStream: %s\n", message);
}

