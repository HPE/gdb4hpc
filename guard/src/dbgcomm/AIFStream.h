// Copyright 2020 Cray Inc.

#ifndef _AIF_STREAM_H
#define _AIF_STREAM_H
// Classes for defining a sequential stream of data accessed via the AIF interface.

#include "aif.h"
#include "array.h"
#include <memory>

// This class represents a single data result from gdb which can be accesses as a series
// of discrete styled slices - at least for arrays.
class AIFStream {
public:
    // The Slices class for implementing the piecewise data access.   
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

    enum steal_t { STEAL };

    // constructor for a single block of AIF data - takes ownership of aif if supplied
    AIFStream(AIF* aif);

    // construct a data array that can be accessed via sequential slices
    AIFStream(std::shared_ptr<Slices> slices);

    // this should be a temporary function to be replaced by real iteration
    // The value is cached and is owned by the AIF stream object.
    AIF* joinAIF();

    // create the joined AIF and steal the ownership.  I.e. the returned AIF object
    // can live after the AIF stream is destroyed and the caller is responsible for
    // freeing it.
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

