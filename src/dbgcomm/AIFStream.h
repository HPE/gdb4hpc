// Copyright 2020-2022 Hewlett Packard Enterprise Development LP.

#ifndef _AIF_STREAM_H
#define _AIF_STREAM_H
// Classes for defining a sequential stream of data accessed via the AIF interface.

#include <memory>
#include <functional>

namespace ais {
    class Reader; class Writer; class DecompIterator; class Difference; class VStream;
}
struct g_block_map_def_t;

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

        // return the next partition of data, the result must be freed by
        // the caller.
        // - isFinal will contain true if there are no more slices after this one.
        virtual std::string getNextSlice(bool& isFinal) = 0;

        // defaults to throwing an error
        virtual std::shared_ptr<Slices> clone() const;
    };

    class Difference {
    public:
        class Impl;

        Difference();
        ~Difference();
        std::string getResult();
        AIFStream* getResultAIF();

        Impl& getAis();
        
    private:
        std::unique_ptr<Impl> m_impl;
        
    };

    // construct a data array that can be accessed via sequential slices
    AIFStream(std::shared_ptr<Slices> slices);

    // constructor for a single block of data
    AIFStream(std::string&& data, bool isPartial);

    // build from a set of cached slices
    AIFStream(std::vector<std::string>&& data);

    // build from a blockmap connecting multiple streams, blockMap defines the
    // ordering of the ranks.  The elements of data are consumes in the
    // process
    AIFStream(std::shared_ptr<ais::DecompIterator> blockmap,
              const std::vector<AIFStream*>& data);

    // build from a operator 
    AIFStream(std::shared_ptr<ais::VStream> vstream);

    // make rhs empty
    AIFStream(AIFStream&& rhs) = default;

    // Get the next slice of AIF data.   Set isFinal to true if this is the last
    // slice of the data object.  
    std::string getNextSlice(bool& isFinal);

    // create an encapsulated version of this stream
    AIFStream* join();

    // creates a new AIF stream from the AIF that comes from a call to getNextSlice(STEAL).
    // the new stream will have the truncation information related to the slice.
    // this transfes ownership of the AIF returned from getNextSlice(STEAL) to
    // the new stream.
    AIFStream* createStreamFromNextSlice(bool &isFinal);

    ~AIFStream();

    // Copy the state of an AIF stream.  The operation is only well-defined before
    // any data is extracted
    AIFStream* clone();

    // Get the vstream object which allows any ais::Vistor based operation - like
    // compare or print to be applied the data
    ais::VStream& getVStream();
    std::shared_ptr<ais::VStream> getVStreamPtr();

    // AIF style operations on the stream, typically for return values
    int isZero(int& val);
    int toInt(int& val);

    // Print the data in human readable form
    int print(FILE* fp);
    int print(FILE* fp, const std::string& label);
    void print(std::ostream & out);

    // Create an AIFStream representing a single value
    static AIFStream* fromInt(int val);
    static AIFStream* fromDouble(double val);
    static AIFStream* fromString(const char* val);
    static AIFStream* fromString(const std::string& val);

    // fromLiteral differs from fromString mainly that the result will not be 
    // printed with quote marks
    static AIFStream* fromLiteral(const std::string& val);
    static AIFStream* fromMultiThreadResult(const char* val);

    // create a stream by operating on 1 or 2 existing streams
    // the input streams are consumed
    static AIFStream* add(AIFStream* a, AIFStream* b);
    static AIFStream* sub(AIFStream* a, AIFStream* b);
    static AIFStream* mul(AIFStream* a, AIFStream* b);
    static AIFStream* div(AIFStream* a, AIFStream* b);
    static AIFStream* rem(AIFStream* a, AIFStream* b);
    static AIFStream* And(AIFStream* a, AIFStream* b);
    static AIFStream* Or(AIFStream* a, AIFStream* b);
    static AIFStream* neg(AIFStream* a);

    // Compare two streams of data for equality, or near equality
    // *res is 0 if equal, 1 if a > b, 2 a >~ n, -1 a < b, -2 a <~ n
    // ~ means the difference is less than *hi if it's passed in
    // return 0 on success.   
    static int compare(AIFStream* a, AIFStream* b, int* res, Difference& diff,
                       double* lo, double* hi, std::string& errMsg);

private:
    std::shared_ptr<Slices> getSlices() { return m_slices; }

    std::shared_ptr<Slices> m_slices;

    std::unique_ptr<ais::Reader> m_reader;
    std::shared_ptr<ais::VStream> m_vstream;
};

#endif

