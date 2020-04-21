// Copyright 2020 Cray Inc.

// Classes for defining a sequential stream of data accessed via the AIF interface.
#include "AIFStream.h"
#include <string.h>
#include <stdarg.h>

AIFStream::AIFStream(AIF* aif) :
    m_joinedAIF(aif), m_joinedIsStolen(false), m_joinedIsExposed(false),
    m_currentSlice(nullptr) {}

AIFStream::AIFStream(std::shared_ptr<Slices> slices) :
    m_joinedAIF(nullptr), m_joinedIsStolen(false), m_joinedIsExposed(false), m_slices(slices),
    m_currentSlice(nullptr)
{}

AIFStream::~AIFStream(){
    if (m_joinedAIF && !m_joinedIsStolen)
        AIFFree(m_joinedAIF);
    if (m_currentSlice)
        AIFFree(m_currentSlice);
}

// Get the next slice of AIF data.   Set isFinal to true if this is the last
// slice of the data object.   The returned AIF is owned by this object and
// is guaranteed to exist until the next call to getNextSlice.
// FIXME - oh just make this a shared pointer for god's sake.
AIF* AIFStream::getNextSlice(bool& isFinal)
{
    isFinal = true;
    if (m_joinedIsExposed) {
        return nullptr;
    } else if (m_joinedAIF) {
        return m_joinedAIF;
    } else if (m_slices) {
        AIF* slice = m_slices->getNextSlice(isFinal);
        if (m_currentSlice)
            AIFFree(m_currentSlice);
        m_currentSlice = slice;
        return slice;
    } else {
        return nullptr;
    }
}


// I am hoping for the joinAIF call be to temporary for when I'm pushing through
// the pieced out version.  This allows us to improve the performance in
// step-wise fashion instead of having to implement everything at once.
// This version maintains ownership of AIF object.
AIF* AIFStream::joinAIF() {
    if (m_joinedAIF && m_joinedIsStolen && m_joinedIsExposed) {
        warning("possible problematic lifetime in AIFStream\n");
    }
    if (!m_joinedAIF && m_slices.get()) {
        joinAIFFromSlices();
    }
        
    m_joinedIsExposed = true;
    return m_joinedAIF;
}

// This version retrieves the monolithic AIF data structure, and cedes
// its ownership to the caller.
AIF* AIFStream::joinAIF(steal_t) {
    if (m_joinedAIF && m_joinedIsExposed) {
        warning("trying to steal an AIF that has already been exposed");
    }

    m_joinedIsStolen = true;
    return joinAIF();
}

// Create a single large AIF structure from the set of slices.
void AIFStream::joinAIFFromSlices() {
    AIF* joinedAIF = nullptr;
    char* joinedIt = nullptr;
    size_t elemSize = 0;
    bool isFirst = true;

    bool isFinal;
    while (AIF* slice = m_slices->getNextSlice(isFinal)) {
        if (isFirst) {
            // first slice, it might also be the last so put off creating a new
            // data structure
            joinedAIF = slice;
            isFirst = false;
        } else {
            if (!joinedIt) {
                // this is the *second* slice, now we know we have to allocate a
                // new AIF object
                AIF* slice0 = joinedAIF;
                joinedAIF = makeEmptyJoinedAIF(slice, &elemSize);
                joinedIt = AIF_DATA(joinedAIF);
                copySlice(slice0, elemSize, joinedIt);  // joinedIt is updated
                AIFFree(slice0);
            }

            copySlice(slice, elemSize, joinedIt); // joinedIt is updated
            AIFFree(slice);
        }
    }

    m_joinedAIF = joinedAIF;
}

// copy one slice of data into the amalgamated structure.
// \param slice is the AIF slice object to copy
// \param elemSize is the size in bytes of each element of the array
// \param joinedIt I/O is the next location in the monolithic data object
void AIFStream::copySlice(AIF* slice, size_t elemSize, char*& joinedIt)
{
    // AIF doesn't export any real useful ways to copy a bunch of values
    // so we'll tread the well worn path and just use binary copy
    const char* sliceData = AIF_DATA(slice);
    int sliceElems = AIFArraySize(slice);
    size_t sliceSize = sliceElems * elemSize;

    memcpy(joinedIt, sliceData, sliceSize);
    joinedIt += sliceSize;

}

// Create the data structure for the monolithic array.   Note that
// this will allocate all of the storage.  If the process will fail, this
// is the most likely place.
AIF* AIFStream::makeEmptyJoinedAIF(AIF* slice, size_t* elemSize)
{
    arrayDimension_t* shape = getShape();
    if (!shape) {
        warning("no shape in joinAIFFromSlices");
        return nullptr;
    }

    bool isFortran = shape->isFortran;
    int dim = shape->dim;

    AIFIndex* index = AIFArrayIndexInit(slice);
    *elemSize = index->i_bsize;

    // Actually an element here, but AIF uses it as an exemplar to create the containing
    // array.
    AIF* array = AIFArrayElement(slice,index);

    for (int i=0; array && i<dim; ++i) {
        // need to work form innermost to outermost.  Note that arrayDimension used i=1.
        int d = (isFortran ? i+1 : dim-i);
        int minIdx = getArrayDimensionRangeMin(shape, d);
        int maxIdx = getArrayDimensionRangeMax(shape, d);

        AIF* nextArray = (isFortran ? EmptyFtnArrayToAIF(minIdx, maxIdx, array) :
                          EmptyArrayToAIF(minIdx, maxIdx, array));

        AIFFree(array);
        array = nextArray;
    }

    AIFArrayIndexFree(index);
    return array;
}

arrayDimension_t* AIFStream::getShape() {
    if (m_slices) {
        return m_slices->getFullShape();
    }
    return nullptr;
}

// AIF translations.
int AIFStream::isZero(int& val) { return AIFIsZero(joinAIF(), &val); }
int AIFStream::toInt(int& val) { return AIFToInt(joinAIF(), &val); }
int AIFStream::print(FILE* fp, int depth) { return AIFPrint(fp, depth, joinAIF()); }
AIFStream* AIFStream::clone() { return new AIFStream(joinAIF()); }


void AIFStream::warning(const char* message) const {
    //fprintf(stderr, "AIFStream: %s\n", message);
}

