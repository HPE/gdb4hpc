// Copyright 2020 Cray Inc.

#ifndef _AIF_STREAM_H
#define _AIF_STREAM_H
// Classes for defining a sequential stream of data accessed via the AIF interface.

#include "aif.h"
#include "array.h"
#include <memory>

// This class represents a single data result from gdb which can be accessed as a series
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
        // the caller.   isFinal will contain true if there are no more slices after this
        // one.
        virtual AIF* getNextSlice(bool& isFinal) = 0;
    };

    enum steal_t { STEAL };

    // constructor for a single block of AIF data - takes ownership of aif if supplied
    AIFStream(AIF* aif);

    // construct a data array that can be accessed via sequential slices
    AIFStream(std::shared_ptr<Slices> slices);

    // Get the next slice of AIF data.   Set isFinal to true if this is the last
    // slice of the data object.   The returned AIF is owned by this object and
    // is guaranteed to exist until the next call to getNextSlice.
    // FIXME - oh just make this a shared pointer for god's sake.
    AIF* getNextSlice(bool& isFinal);

    // this should be a temporary function to be replaced by real iteration
    // The value is cached and is owned by the AIF stream object.
    AIF* joinAIF();

    // create the joined AIF and steal the ownership; i.e. the returned AIF object
    // can live after the AIF stream is destroyed and the caller is responsible for
    // freeing it.
    AIF* joinAIF(steal_t);
    
    // Get the dimensional shape of the entire data object if it is an array.   Otherwise,
    // returns nullptr.
    // The object is owned by this object.
    arrayDimension_t* getShape();

    ~AIFStream();

    AIFStream* clone();

    // Early version of exporting AIF operations via AIFStream.
    // ToDo: I'm letting this grow organically for now reserving the
    // possiblity of a cleaner/better approach later.
    int isZero(int& val);
    int toInt(int& val);
    int print(FILE* fp, int depth);

private:
    void joinAIFFromSlices();
    AIF* makeEmptyJoinedAIF(AIF* slice, size_t* elemSize);
    void copySlice(AIF* slice, size_t elemSize, char*& joinedIt);
    void warning(const char* message) const;
    
    AIF* m_joinedAIF;
    bool m_joinedIsStolen;
    bool m_joinedIsExposed;

    std::shared_ptr<Slices> m_slices;
    AIF* m_currentSlice;

};

#endif

