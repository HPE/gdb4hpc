// Copyright 2020 Hewlett Packard Enterprise Development LP.

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
        // the caller.
        // flags:
        //   - isFinal will contain true if there are no more slices after this one.
        //   - isTruncated will be true if for some reason this slice should be
        //       treated as incomplete, e.g. an array truncated by set print elements.
        //   - truncatedLength is the true length of the slice's data in number of
        //       elements.
        virtual AIF* getNextSlice(bool& isFinal) = 0;
        virtual AIF* getNextSlice(bool& isFinal, unsigned int& truncatedLength) = 0;

        // free (or not) a slice after it is done being processed
        virtual void freeSlice(AIF* aif) = 0;
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

    // Get the next slice of AIF data and return ownership of AIF object.
    // return nullptr if there are no new slices.
    AIF* getNextSlice(steal_t, bool& isFinal);

    // this should be a temporary function to be replaced by real iteration
    // The value is cached and is owned by the AIF stream object.
    AIF* joinAIF();

    // create the joined AIF and steal the ownership; i.e. the returned AIF object
    // can live after the AIF stream is destroyed and the caller is responsible for
    // freeing it.
    AIF* joinAIF(steal_t);

    // returns true if joinAIF is not nullptr. Distinct from an outside call to
    // if (joinAIF() == nullptr) since joinAIF has side effects
    bool hasJoinAIF() const;

    // creates a new AIF stream from the AIF that comes from a call to getNextSlice(STEAL).
    // the new stream will have the truncation information related to the slice.
    // this transfes ownership of the AIF returned from getNextSlice(STEAL) to
    // the new stream.
    AIFStream* createStreamFromNextSlice(bool &isFinal);

    // functions about data truncation. sometimes, especially in the case of arrays
    // and set print elements, data can be "truncated", but by "truncated" we mean
    // only partially filled, i.e., an array with some false zeros filling the last
    // slots. so we need to keep track of how much of the data is real.
    bool isTruncated() const;
    unsigned int getTruncatedLength() const;
    void setTruncatedLength(unsigned int length);

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
    AIF* makeEmptyJoinedAIF(AIF* slice);
    bool getSliceOffset(AIF* slice, size_t& offset, size_t& sliceSize, bool& isFullArray);
    void warning(const char* message) const;

    AIF* m_joinedAIF;
    bool m_joinedIsStolen;
    bool m_joinedIsExposed;

    unsigned int m_dataTruncatedLength;

    std::shared_ptr<Slices> m_slices;
    AIF* m_currentSlice;

};

#endif

