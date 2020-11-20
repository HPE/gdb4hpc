// Copyright 2020 Hewlett Packard Enterprise Development LP.

// Classes for defining a sequential stream of data accessed via the AIF interface.
#include "AIFStream.h"
#include <string.h>
#include <stdarg.h>

AIFStream::AIFStream(AIF* aif) :
    m_joinedAIF(aif), m_joinedIsStolen(false), m_joinedIsExposed(false),
    m_dataTruncatedLength{0},
    m_slices{nullptr}, m_currentSlice(nullptr) {}

AIFStream::AIFStream(std::shared_ptr<Slices> slices) :
    m_joinedAIF(nullptr), m_joinedIsStolen(false), m_joinedIsExposed(false),
    m_dataTruncatedLength{0},
    m_slices(slices), m_currentSlice(nullptr)
{}

AIFStream::~AIFStream(){
    if (m_joinedAIF && !m_joinedIsStolen)
        AIFFree(m_joinedAIF);
    if (m_currentSlice && m_slices.get())
        m_slices->freeSlice(m_currentSlice);
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
        AIF* slice = m_slices->getNextSlice(isFinal, m_dataTruncatedLength);
        if (m_currentSlice)
            m_slices->freeSlice(m_currentSlice);
        m_currentSlice = slice;
        return slice;
    } else {
        return nullptr;
    }
}

// Get the next slice of AIF data and return ownership of the AIF object.
AIF* AIFStream::getNextSlice(steal_t, bool& isFinal)
{
    AIF* slice = getNextSlice(isFinal);
    if (slice) {
        m_currentSlice = m_joinedAIF = nullptr;
    }
    return slice;
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

// creates a new aifstream and transfers ownership of the AIF retrieved from
// getNextSlice(STEAL).
// preserves truncation info in the new stream.
AIFStream* AIFStream::createStreamFromNextSlice(bool &isFinal) {
    // getNextSlice will set internal truncation data
    auto aif = getNextSlice(AIFStream::STEAL, isFinal);
    auto returnStream = new AIFStream(aif);
    returnStream->setTruncatedLength(m_dataTruncatedLength);
    return returnStream;
}

bool AIFStream::isTruncated() const {
    return m_dataTruncatedLength != 0;
}

size_t AIFStream::getTruncatedLength() const {
    return m_dataTruncatedLength;
}

void AIFStream::setTruncatedLength(size_t len) {
    m_dataTruncatedLength = len;
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

bool AIFStream::hasJoinAIF() const {
    return m_joinedAIF != nullptr;
}

// Create a single large AIF structure from the set of slices.
void AIFStream::joinAIFFromSlices() {
    AIF* joinedAIF = nullptr;
    bool hasError = false;
    bool isFinal;
    while (AIF* slice = m_slices->getNextSlice(isFinal, m_dataTruncatedLength)) {
        size_t offset, sliceSize; bool isFullArray;

        if (hasError) {
        } else if (!getSliceOffset(slice, offset, sliceSize, isFullArray)) {
            m_slices->freeSlice(slice);
            hasError = true;
        } else if (isFullArray) {
            if (joinedAIF)
                AIFFree(joinedAIF);
            joinedAIF = slice;
        } else if (joinedAIF || (joinedAIF = makeEmptyJoinedAIF(slice))) {
            const char* sliceData = AIF_DATA(slice);
            char* joinedData = AIF_DATA(joinedAIF);
            memcpy(joinedData+offset, sliceData, sliceSize);
        } else {
            fprintf(stderr, "could not create joined array\n");
            hasError = true;
        }

        if (slice != joinedAIF)
            m_slices->freeSlice(slice);
    }

    if (hasError) {
        fprintf(stderr, "could not reassemble array\n");
        if (joinedAIF)
            AIFFree(joinedAIF);
        joinedAIF = nullptr;
    }

    m_joinedAIF = joinedAIF;
}


// compute the offset in bytes to copy the data in \a slice into the the data for the
// the monolithic array
// \param slice the slice of interest
// \param offset will contrain on output the offset of the equivalent data of \a slice in the
//         full data object
// \param sliceSize will contain on output the size of the data in \a slice in bytes
// \param isFullArray will contain true on output if \a slice is the entire contents of the
//       the full result.
// \return true if \a slice is a legitimate subset (including equality) of the result array.
bool AIFStream::getSliceOffset(AIF* slice, size_t& offset, size_t& sliceSize, bool& isFullArray)
{
    arrayDimension_t* shape = getShape();
    if (!shape) {
        warning("no shape in joinAIFFromSlices");
        return false;
    }

    // index is initialized to the first element of \a slice
    AIFIndex* index = AIFArrayIndexInit(slice);
    offset = 0;
    isFullArray = false;

    size_t totalElems = 1;
    size_t elemSize = index->i_bsize;

    int dim = shape->dim;
    bool isOk = true;

    // compute the offset (in elements) of slice within the full array
    // and the total number of elements in the full array.
    for (int i=0; i<dim; ++i) {
        // slowest to fastest
        int d = (shape->isFortran ? dim-i : i+1);
        int min = getArrayDimensionRangeMin(shape,d);
        int max = getArrayDimensionRangeMax(shape,d);

        size_t len = (max-min+1);
        offset *= len;
        totalElems *= len;

        // AIFIndex is slowest to fastest
        int idx = index->i_index[i];
        if (idx < min || idx > max)
            isOk = false;
        else
            offset += (idx-min);
    }

    offset *= elemSize;

    sliceSize = AIF_LEN(slice);
    size_t arraySize = totalElems * elemSize;

    isFullArray = false;

    // check that its a valid subset
    if (offset + sliceSize > arraySize) {
        isOk = false;
    }

    AIFArrayIndexFree(index);
    if (!isOk) {
        DPRINT("slice copy is out of bounds\n");
    }
    return isOk;
}


// Create the data structure for the monolithic array.   Note that
// this will allocate all of the storage.  If the process will fail, this
// is the most likely place.
AIF* AIFStream::makeEmptyJoinedAIF(AIF* slice)
{
    arrayDimension_t* shape = getShape();
    if (!shape) {
        warning("no shape in joinAIFFromSlices");
        return nullptr;
    }

    bool isFortran = shape->isFortran;
    int dim = shape->dim;

    AIFIndex* index = AIFArrayIndexInit(slice);

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

