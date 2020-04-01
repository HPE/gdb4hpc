// Copyright 2020 Cray Inc.

// Classes for defining a sequential stream of data accessed via the AIF interface.

#include "aif.h"
#include <memory>

class AIFStream {
public:
    enum steal_t { STEAL };
    
    // constructor for a single block of AIF data - takes ownership of aif if supplied
    AIFStream(AIF* aif);

    // this should be a temporary function to be replaced by real iteration
    AIF* joinAIF();

    AIF* joinAIF(steal_t);

private:
    void freeAIF(AIF* aif) { if (aif && !m_joinedIsStolen) AIFFree(aif); }
    void warning(const char* message);
    
    std::shared_ptr<AIF> m_joinedAIF;
    bool m_joinedIsStolen;
    bool m_joinedIsExposed;
};
