// Copyright 2020 Cray Inc.

#ifndef _AIF_STREAM_H
#define _AIF_STREAM_H
// Classes for defining a sequential stream of data accessed via the AIF interface.

#include "aif.h"
#include "array.h"
#include <memory>


class AIFStream {
public:
    enum steal_t { STEAL };

    class Slices {
    public:
        Slices() = default;
        Slices(const Slices&) = delete;
        Slices& operator=(const Slices&) = delete;

        virtual ~Slices() = default;

        // get the shape of the full array - the result is owned by this object
        virtual arrayDimension_t* getFullShape() const = 0;

        // return the next partition of data, the result must be freed by
        // the caller
        virtual AIF* getNextSlice() = 0;
    };

    // constructor for a single block of AIF data - takes ownership of aif if supplied
    AIFStream(AIF* aif);

    // construct a data array that can be accessed via sequential slices
    AIFStream(std::shared_ptr<Slices> slices);

    // this should be a temporary function to be replaced by real iteration
    AIF* joinAIF();

    AIF* joinAIF(steal_t);

    // Get the dimensional shape of the entire data object if it is an array.   Otherwise,
    // returns nullptr.
    // The object is owned by this object.
    arrayDimension_t* getShape();

private:
    void joinAIFFromSlices();
    void freeAIF(AIF* aif) { if (aif && !m_joinedIsStolen) AIFFree(aif); }
    AIF* makeEmptyJoinedAIF(AIF* slice, size_t* elemSize);
    void copySlice(AIF* slice, size_t elemSize, char*& joinedIt);
    void warning(const char* message);
    
    std::shared_ptr<AIF> m_joinedAIF;
    bool m_joinedIsStolen;
    bool m_joinedIsExposed;

    std::shared_ptr<Slices> m_slices;
};

#endif

