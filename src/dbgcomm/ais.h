// Copyright 2022 Hewlett Packard Enterprise Development LP.
#pragma once

#include <string>
#include <sstream>
#include <map>
#include <memory>
#include <vector>
#include <functional>
#include <variant>
#include <bitset>

//===========================================================================
// The AIS (abstract information stream) is a means for taking an arbitrary
// C/C++/fortran data structure, breaking it into data primitives and meta-data
// sending it across a socket, and the interpreting it on the opposite side.
//
// In contrast to the former AIF, the key design feature is that this works
// entirely on the result as a *stream* of data, so even with gigantic arrays
// the memory footprint of the data processsing will not exceed that of a
// fixed buffering size.
//
// The main components of process are:
//
// Type : is a composable, nestable object for describing a C/C++/fortran data
//      structure.  Derived typed include primitives: ais::Integer, ais::Double etc.
//      and composites: ais::Array, ais::Struct etc.
//
// InterpretStream: a line of input from a debugger
//       It supports producing the output in chunks to limit memory usage
// Writer : the producer side of the socket communication
// Reader : the consumer side of the socket commnuncation
// WriteBuffer/ReadBuffer : socket communication in terms of primitives
// VStream : a stylized stream of the data form that can support multiple
//      operation
// Visitor : the basis for performing operations on the data after it's been
//      read from the socket.
//
// So the data flow:
// - dbgsrv_libmi2aif is asked to evaluate an expression
//    - it parses the type declaration to create -->
//      - ais::Type for the object
//      - an InterpretStream to represent the output from gdb
// - Type::parse consumes the InterpretStream, translating into a
//       stream of primitives going to ->
// - Writer
//       which encodes the type and sends the data to
// - WriterBuffer
//       which pushes it to a pipe ->
// - ReadBuffer
//       which reads primitives from the pipe ->
// - Reader
//      which recontructs the ais::Type for the data
// - Type::apply
//      which create a nested data representation (VStream) which goes to a
// - Visitor
//      to accept the annotated stream of data, this is the abstract parent of
// - Printer/Compare/Neg/Add etc.
//      which process 1 or more VStreams either as final consumer (Print,Compare)
//      or to output a modified stream of data.
//===============================================================================
namespace ais {

    class Type; class Array; class DecompArray;

// Error type used to throw ais exceptions
class Error : public std::runtime_error {
public:
    Error(const std::string& message);
    bool empty() { return false; }
};

class GdbError : public Error {
public:
    GdbError(const std::string& message) : Error(message) {}
};

// Abstract class for writing a stream of primitive data
class WriteBuffer {
public:
    WriteBuffer();
    virtual ~WriteBuffer();

    virtual void writeInt(int val) = 0;
    virtual void writeLong(long val) = 0;
    virtual void writeLongLong(long long val) = 0;
    virtual void writeFloat(float val) = 0;
    virtual void writeDouble(double val) = 0;
    virtual void writeLongDouble(long double val) = 0;
    virtual void writeString(const std::string& str) = 0;

    virtual void writePointer(const std::string& type, void* val) = 0;
    virtual void writeError(const std::string& str) = 0;
};

// Abstracrt class for read stream of primitive data
class ReadBuffer {
public:
    ReadBuffer();
    virtual ~ReadBuffer();

    virtual int readInt() = 0;
    virtual long readLong() = 0;
    virtual long long readLongLong() = 0;
    virtual float readFloat() = 0;
    virtual double readDouble() = 0;
    virtual long double readLongDouble() = 0;
    virtual std::string readString() = 0;
    virtual bool hasMoreData() = 0;
};

// Class for writing a stream of stuctured data objects
// The class does forward primitive writes to a WriteBuffer
// objects which lets it act as a single point of contact
// for the writing process, but it also caches type descriptions
// so they can be referred to with a single index.
class Writer {
public:
    Writer(std::shared_ptr<WriteBuffer> buffer) : m_buffer(buffer) {}

    // write a type definition into the stream
    void writeType(const Type& type);

    // get the index for a type 
    int getTypeId(const std::string& typeName);

    void resetTypes();

    // writer primitives
    void writeInt(int val);
    void writeLong(long val);
    void writeLongLong(long long val);
    void writeFloat(float val);
    void writeDouble(double val);
    void writeLongDouble(long double val);
    void writeString(const std::string& str);
    void writePointer(const std::string& type, void* val);

    void writeError(const std::string& str);

    std::shared_ptr<WriteBuffer> getBuffer() const;

private:
    std::shared_ptr<WriteBuffer> m_buffer;
    std::map<std::string, int> m_typeNameToIdx;

};

// Stream for reading a set series of typed data objects.
// Types are handled via an ais::Type object, which can then 
// read the primitive data specific to that type and send if
// along for further processing.
// This is an abstract base class, the avialable implementations
// follow.
class Reader {
public:
    // Read and create the Type for the next object in the stream
    virtual std::shared_ptr<Type> readType() = 0;

    // Get the type from the index of a type that was
    // previously read.
    virtual std::shared_ptr<Type> getType(int idx) = 0;

    virtual int readInt() = 0;
    virtual long readLong() =  0;
    virtual long long readLongLong() = 0;
    virtual float readFloat() = 0;
    virtual double readDouble() = 0;
    virtual long double readLongDouble() = 0;
    virtual std::string readString() = 0;

    virtual bool hasMoreData() = 0;
    virtual ~Reader();
protected:
    static std::vector<std::shared_ptr<Type>> m_primitives;
    static std::vector<std::shared_ptr<Array>> m_arrayTypes;
};

// Reader class for reading a single data source.
class BufferReader : public Reader {
public:
    BufferReader(std::shared_ptr<ReadBuffer> buffer);

    std::shared_ptr<Type> readType() override;
    std::shared_ptr<Type> getType(int idx) override;

    int readInt() override;
    long readLong() override;
    long long readLongLong() override;
    float readFloat() override;
    double readDouble() override;
    long double readLongDouble() override;
    std::string readString() override;

    bool hasMoreData() override;

private:
    std::shared_ptr<ais::Array> getArrayType(const std::string& typeName) const;
    
    std::shared_ptr<ReadBuffer> m_buffer;
    std::vector<std::shared_ptr<Type>> m_types;

    std::map<std::string, std::shared_ptr<Type>> m_primitiveMap;
    std::map<std::string, std::shared_ptr<Array>> m_arrayTypeMap;
};

// Abstract class for managing a set of data streams in DecompReader
class DecompIterator {
public:
    virtual void begin(bool isFortran) = 0;
    virtual bool next() = 0;
    virtual int getDim() = 0;
    virtual ~DecompIterator() {}
    int streamIdx = 0;
    int len = 0;
    int startLevel = 0;
    int endLevel = 0;
};

// Reader class for combining a set data streams that represent
// a large conceptual array that has been partitioned across
// multiple ranks.
class DecompReader : public Reader {
public:
    DecompReader(std::vector<std::unique_ptr<Reader>>&& subReaders,
                 std::shared_ptr<DecompIterator> iter);

    std::shared_ptr<Type> readType() override;
    std::shared_ptr<Type> getType(int idx) override;

    int readInt() override;
    long readLong() override;
    long long readLongLong() override;
    float readFloat() override;
    double readDouble() override;
    long double readLongDouble() override;
    std::string readString() override;

    bool hasMoreData() override;

    void setSubIdx(int idx);
private:
    std::vector<std::unique_ptr<Reader>> m_subReaders;
    int m_subIdx;
    bool m_prologue;

    std::shared_ptr<DecompIterator> m_decompIter;
};


// The interpret stream is the result from MI that is used for the
// parse operation.   I might be able to make this just a pure
// character stream, but I'm not sure yet if some parsing operations
// might benefit from looking ahead, since the entire result will
// typically be available, and even for large objects a substantial
// slice should be available
class InterpretStream {
public:
    virtual ~InterpretStream() = default;
    virtual const char* get() = 0;
    virtual void setPos(const char* pos) = 0;
    virtual bool mayBeTruncated() = 0;
};

// Visitor class for applying an arbitrary operation to a data structure.
// The explicit types of the data objects being visited is made available in
// the call-back functions, it is presented as a nested set of of
// sequences of primivite data values.
class Visitor {
public:
    virtual void operator()(bool val);
    virtual void operator()(char val);
    virtual void operator()(int val);
    virtual void operator()(long val);
    virtual void operator()(long long val) = 0;
    virtual void operator()(float val);
    virtual void operator()(double val);
    virtual void operator()(long double val) = 0;
    virtual void operator()(const std::string& val) = 0;
    virtual void operator()(const Error& val) = 0;

    // called when the nesting level of the data structure is
    // increased
    virtual void nest(const Type* t, int refIdx=-1);

    // called when a new object or compound is stated
    virtual void start(const Type* t);

    // reporting for non-data portions of the data structure.
    // This may be expanded, but is set up currently just for
    // defining the first index and total size of a collection
    virtual void addMetaInfo(const Type* t, std::pair<int,int> info);

    // called when an object is finished
    virtual void end();

    // called when the nesting level is being reduced
    virtual void unnest();

    // called when the data stream is done reading
    virtual void eof();

    virtual ~Visitor() = default;
};

// applies a (templated) function to every primitive type
template <class F>
class FVisitor : public Visitor {
public:
    FVisitor(F af) : f(af) {}
    void operator()(bool val) override { f(val); }
    void operator()(char val) override { f(val); }
    void operator()(int val) override { f(val); }
    void operator()(long val) override { f(val); }
    void operator()(long long val) override { f(val); }
    void operator()(float val) override { f(val); }
    void operator()(double val) override { f(val); }
    void operator()(long double val) override { f(val); }
    void operator()(const std::string& val) override { f(val); }
    void operator()(const ais::Error& val) override { f(val); }

    F& getF() { return f; }
private:
    F f;
};


struct Formatter {
    Formatter(const std::string& format);
    Formatter(const Formatter&) = default;
    enum Form { DEC, HEX, OCT, BIN, PADDED_BIN };

    Form form = DEC;
    bool csv = false;
    bool onePerLine = false;
    bool pretty = false;
    std::ostream* os = nullptr;
};

    
// Visitor class for applying the print operation to the data structure.
// The print operation is very specialized to the actual data types,
// and is not always stateless (e.g. print a comma before the element
// unless this is the first element).   So while printing a data
// stucture we maintain a set of Printer objects that mirrors the
// data structure nesting.
class PrinterMaster;
    
class Printer : public Visitor {
public:
    Printer(Printer& aparent);
    Printer(std::ostream& out); // for the master

    void operator()(bool val) override;
    void operator()(char val) override;
    void operator()(int val) override;
    void operator()(long val) override;
    void operator()(long long val) override;
    void operator()(float val) override;
    void operator()(double val) override;
    void operator()(long double val) override;
    void operator()(const std::string& val) override;
    void operator()(const ais::Error& val) override;
    
    void nest(const Type* t, int refIdx) override;
    void start(const Type* t) override;
    void addMetaInfo(const Type* t, std::pair<int,int> info) override;
    void end() override;
    void unnest();

    Formatter& format();

    virtual void startChild();
    virtual void endChild();

    virtual std::string getLabel();
    virtual void indent(); // add indenting when printing multi-line structures

    Printer* parent;
    std::unique_ptr<Printer> child;
    PrinterMaster& master;
    std::ostream& out;
};

// Visitor class that tracks the current index of within the data
// structure.   It works as a pass-through to another visitor.
class TrackingVisitor : public FVisitor<TrackingVisitor&> {
public:
    using Base = FVisitor<TrackingVisitor&>;
    TrackingVisitor(ais::Visitor* wrapped) : Base(*this), m_wrapped(wrapped) {}

    TrackingVisitor(const TrackingVisitor&) = delete;
    TrackingVisitor& operator=(const TrackingVisitor&) = delete;

    template<class T> void operator()(T x) { if (m_wrapped) (*m_wrapped)(x); }
    void nest(const Type* t, int refIdx) override;
    void start(const Type* t) override;
    void addMetaInfo(const Type* t, std::pair<int,int> info) override;
    void unnest() override;
    void end() override;
    void eof() override;

    std::string getIdx() const;
    void incIdx();
private:
    ais::Visitor* m_wrapped = nullptr;
    std::vector<std::pair<const ais::Type*,int>> m_idcs;
};

// Base class for processing data types.   The member functions are the
// functions to be defined to allow it to read output from gdb, 
// break it into primitives to send through MRNet, read the values,
// and send it to further processing operators.
class Type {
public:
    virtual ~Type();

    // unique identifier for the type
    virtual std::string getTypeName() const = 0;

    // write type description to a data stream
    virtual int getTypeId(Writer& writer) const;

    // Parse gdb output and write to a data stream
    void parse(InterpretStream& in, Writer& out) const;

    // Read the object form a data stream and forward to
    // an operator
    void apply(Reader& reader, Visitor& f) const;

    // create a printer for human readable display
    virtual std::unique_ptr<Printer> makePrinter(Printer& parent) const;

    // specialization for printing the index within a compound type
    virtual void printIndex(int idx, std::ostream& out) const;

protected:
    // Parse gdb output and write to a data stream
    virtual void parseL(InterpretStream& in, Writer& out) const = 0;

    // Read the object form a data stream and forward to
    // an operator
    virtual void applyL(Reader& reader, Visitor& f) const = 0;

};

// Base class for a binary operator.   This can be use used to
// process a pair of VStream via a binary operator stream
// (makeBinaryOpStream).  Optionally, the operator can arrange
// to pass the output to another operator.
class BinaryOperator {
public:
    BinaryOperator();
    virtual ~BinaryOperator() = default;
    
    virtual void operator()(bool a, bool b);
    virtual void operator()(char a, char b);
    virtual void operator()(int a, int b);
    virtual void operator()(long a, long b);
    virtual void operator()(long long a, long long b) = 0;
    virtual void operator()(float a, float b);
    virtual void operator()(double a, double b);
    virtual void operator()(long double a, long double) = 0;
    virtual void operator()(const std::string& a, const std::string& b) = 0;
    virtual void operator()(const ais::Error& a, const ais::Error& b) = 0;

    // set the vistor to which the output of the operation can be
    // piped.
    void setOutput(Visitor* out) { m_out = out; }
protected:
    Visitor* m_out = nullptr;
};

// Glue class to create a concrete BinaryOperator from a lambda
// function
template<class BINARY_OP, class NON_ARITH_OP>
class FBinaryOperator :  public BinaryOperator {
public:
    FBinaryOperator(BINARY_OP op, NON_ARITH_OP naop) : m_op(op), m_naOp(naop) {}
    FBinaryOperator(const FBinaryOperator&) = delete;
    FBinaryOperator& operator=(const FBinaryOperator&) = delete;
    void operator()(bool a, bool b) override { f(a,b,m_op); }
    void operator()(char a, char b) override { f(a,b,m_op); }
    void operator()(int a, int b ) override { f(a,b,m_op); }
    void operator()(long a, long b) override { f(a,b,m_op); }
    void operator()(long long a, long long b) override { f(a,b,m_op); }
    void operator()(float a, float b) override { f(a,b,m_op); }
    void operator()(double a, double b) override { f(a,b,m_op); }
    void operator()(long double a, long double b) override { f(a,b,m_op); }
    void operator()(const std::string& a, const std::string& b) override {
        f(a,b,m_naOp);
    }
    void operator()(const ais::Error& a, const ais::Error& b) override {
        f(a.what(),b.what(),m_naOp);
    }

private:
    template <class T, class OP> inline void f(T a, T  b, OP& op) {
        using RT = typename std::invoke_result<decltype(op),T,T>::type;
        if constexpr (std::is_void<RT>::value) {
            op(a,b);
        } else if (m_out) {
            (*m_out)(op(a,b));
        } else {
            op(a,b);
        }
    }

    BINARY_OP m_op;
    NON_ARITH_OP m_naOp;
};

// read/writer for an arbitrary type.
// Note: The data strategy is set up to avoid repeating the type 
// information as much as possible.  So an array of 1000 ints will just
// write type "ArrayOfInts" (techniaclly: "[3" if 3 is the type index
// for 'int') instead of writing "int" for every single value.   Thus, each type
// assumes its type is available in the context.   So, when it is *not* known via
// context, the writer just needs to wrap the type with an ais::Any to set up the
// unmarshalling on the other end.
class Any : public Type {
public:
    Any();
    Any(std::shared_ptr<Type> obj, int idx);
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;
    int getTypeId(Writer& writer) const override;
private:
    std::shared_ptr<Type> m_obj;
    int m_idx;
};

// Place holder for a type that cannot be handled
class Unimplemented : public Type {
public:
    Unimplemented(const std::string& typeName);
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;

    std::string getTypeName() const override;
private:
    std::string m_typeName;
};

class Bool : public Type {
public:
    Bool() {}
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;

    bool readVal(Reader& reader) const;
};
 

class Char : public Type {
public:
    Char() {}
    void parseL(InterpretStream& in, Writer& out) const override;
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;

    char readVal(Reader& reader) const;
};

class SignedChar : public Char {
public:
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    std::string getTypeName() const override;
};

class UnsignedChar : public Char {
public:
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    std::string getTypeName() const override;
};

class Short : public Type {
public:
    Short() {}
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;

    short readVal(Reader& reader) const;
};

class UnsignedShort : public Short {
public:
    UnsignedShort() {}
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    std::string getTypeName() const override;
};

class Int : public Type {
public:
    Int() {}
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;

    int readVal(Reader& reader) const;
};

class UnsignedInt : public Int {
public:
    UnsignedInt() {}
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    std::string getTypeName() const override;
};

class Long : public Type {
public:
    Long() {}
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;

    long readVal(Reader& reader) const;
};

class UnsignedLong : public Long {
public:
    UnsignedLong() {}
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    std::string getTypeName() const override;
};

class LongLong : public Type {
public:
    LongLong() {}
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;

    long long readVal(Reader& reader) const;
};

class UnsignedLongLong : public Long {
public:
    UnsignedLongLong() {}
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    std::string getTypeName() const override;
};

class Float : public Type {
public:
    Float() {}
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;
    float readVal(Reader& reader) const;
};

class Double : public Type {
public:
    Double() {}
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;

    double readVal(Reader& reader) const;
};

class LongDouble : public Type {
public:
    LongDouble() {}
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;

    long double readVal(Reader& reader) const;
};

class Complex : public Type {
public:
    Complex(std::shared_ptr<Type> elemType);
    void parseL(InterpretStream& in, Writer& out) const override;
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    void printIndex(int idx, std::ostream& out) const override;;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;
    const Type* getElemType() const;
    std::shared_ptr<Type> getElemTypePtr() { return m_elemType; }
private:
    std::shared_ptr<Type> m_elemType;
};

class String : public Type {
public:
    String() {}
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;

    std::string readVal(Reader& reader) const;
};

// A string that should be displayed without quotes
class Literal : public String {
public:
    Literal() {}
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    std::string getTypeName() const override;
};

class FtnString : public String {
public:
    FtnString(int len=0) : m_len(len) {}
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    std::string getTypeName() const override;
    int getLen() const { return m_len; }
private:
    int m_len;
};

class Enum : public Literal {
public:
    Enum();
    Enum(const std::string& name);
    void parseL(InterpretStream& in, Writer& out) const override;
    std::string getTypeName() const override;
private:
    std::string m_name;
};

// Fortran LOGICAL type
class Logical : public Type {
public:
    Logical() {}
    void parseL(InterpretStream& in, Writer& out) const override;
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;

    bool readVal(Reader& reader) const;
};

class MultiThreadResult : public String {
public:
    MultiThreadResult() {}
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;
};

// A function pointer
class Function : public Literal {
public:
    Function();
    void parseL(InterpretStream& in, Writer& out) const override;
    std::string getTypeName() const override;
};

// define a filter to allow writing a sub-set of an array elements
class ArrayFilter {
public:
    ArrayFilter();
    virtual ~ArrayFilter();

    ArrayFilter(const ArrayFilter&) = delete;
    ArrayFilter& operator=(const ArrayFilter&) = delete;

    // Type the array will emulate
    virtual std::shared_ptr<Type> getElemType() const = 0;
    virtual int getLen() = 0;
    virtual bool doWrite() = 0;
};
      

// C array class.
// This is also the base class for all homogeneous data objects.
// The array can wrap any element type, including another array which is
// how multi-dimension arrays are handled, with the slowest changing index outermost.
class Array : public Type {
public:
    Array();
    Array(std::shared_ptr<Type> elemType, int len=0, int idx0=0);  // len only used on write side
    void parseL(InterpretStream& in, Writer& out) const override;
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    void printIndex(int idx, std::ostream& out) const override;
    std::string getTypeName() const override;
    int getTypeId(Writer& writer) const override;

    // Special functions for containers

    // Create container of this type wrapping an element type
    virtual std::shared_ptr<Array> makeTyped(std::shared_ptr<ais::Type> elemType) const;

    // Visit the next element in the sequence.  ApplyOneIdx gives the location
    // within the structure, and is update on output.
    // -- this form is used when processing decomposition arrays --
    struct ApplyOneIdx { int idx; int len; bool mayBeTruncated; };
    virtual void applyOne(Reader& reader, std::vector<ApplyOneIdx>& location, int level,
                          const std::function<void(const Type&,Reader&)>& func);

    // Return true if the array is fortran - default false
    virtual bool isFortran() const;

    // element type
    std::shared_ptr<Type> getElemTypePtr() { return m_elemType; }
    const Type* getElemType() const { return m_elemType.get(); }
    int getLen() const { return m_numElems; }
    void setBounds(int len, int idx0) { m_numElems = len; m_idx0 = idx0; }

    void setFilter(std::shared_ptr<ArrayFilter> filter);
        
protected:
    int getElemTypeId(ais::Writer& writer) const;

    std::shared_ptr<Type> m_elemType;
    int m_numElems;
    int m_idx0 = 0;
    std::shared_ptr<ArrayFilter> m_filter;
};

// Fortran array - multi dimension arrays are handled by nesting FntArray types
class FtnArray : public Array {
public:
    FtnArray();
    FtnArray(std::shared_ptr<Type> elemType, int len=0);
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    void printIndex(int idx, std::ostream& out) const override;
    std::shared_ptr<Array> makeTyped(std::shared_ptr<ais::Type> elemType) const override;
    std::string getTypeName() const override;
    int getTypeId(Writer& writer) const override;
    bool isFortran() const override;
};

// std::vector    
class Vector : public Array {
public:
    Vector();
    Vector(std::shared_ptr<Type> elemType, int len=0, int capacity=0);
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;
    int getTypeId(Writer& writer) const override;

    std::shared_ptr<Array> makeTyped(std::shared_ptr<ais::Type> elemType) const override;
    
    void applyOne(Reader& reader, std::vector<ApplyOneIdx>& location, int level,
                  const std::function<void(const Type&,Reader&)>& func);

private:
    int m_capacity;
};

// C++ std::deque    
class Deque : public Array {
public:
    Deque();
    Deque(std::shared_ptr<Type> elemType);
    void parseL(InterpretStream& in, Writer& out) const override;
    std::shared_ptr<Array> makeTyped(std::shared_ptr<ais::Type> elemType) const override;
    std::string getTypeName() const override;
    int getTypeId(Writer& writer) const override;
};

// C++ std::queue
class Queue : public Deque {
public:
    Queue();
    Queue(std::shared_ptr<Type> elemType);
    void parseL(InterpretStream& in, Writer& out) const override;
    std::shared_ptr<Array> makeTyped(std::shared_ptr<ais::Type> elemType) const override;
    std::string getTypeName() const override;
    int getTypeId(Writer& writer) const override;
};

// C++ std::stack    
class Stack : public Array {
public:
    Stack();
    Stack(std::shared_ptr<Type> elemType);
    void parseL(InterpretStream& in, Writer& out) const override;
    std::shared_ptr<Array> makeTyped(std::shared_ptr<ais::Type> elemType) const override;
    std::string getTypeName() const override;
    int getTypeId(Writer& writer) const override;
};

// C++ std::priority_queue
class PriorityQueue: public Vector {
public:
    PriorityQueue();
    PriorityQueue(std::shared_ptr<Type> elemType);
    void parseL(InterpretStream& in, Writer& out) const override;
    std::shared_ptr<Array> makeTyped(std::shared_ptr<ais::Type> elemType) const override;
    std::string getTypeName() const override;
    int getTypeId(Writer& writer) const override;
};

// C++ std::array    
class StdArray : public Array {
public:
    StdArray();
    StdArray(std::shared_ptr<Type> elemType, int len=0);
    void parseL(InterpretStream& in, Writer& out) const override;
    std::shared_ptr<Array> makeTyped(std::shared_ptr<ais::Type> elemType) const override;
    std::string getTypeName() const override;
    int getTypeId(Writer& writer) const override;
};
    
// special handlig for char[] in C/C++
class CharArray : public Type {
public:
    CharArray();
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    std::string getTypeName() const override;
private:
    static const std::string nullString;
    std::string readVal(Reader& reader) const;
};

// An array decomposed into pieces stored on multiple ranks.
class DecompArray : public Type {
public:
    // array is the underlying array type, decompIter defines the
    // mapping from locations to the rank it comes from
    DecompArray(std::shared_ptr<ais::Array> array,
                std::shared_ptr<ais::DecompIterator> decompIter);
    void parseL(InterpretStream& in, Writer& out) const override;
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    void printIndex(int idx, std::ostream& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;

protected:
    std::shared_ptr<ais::Array> m_array;
    std::shared_ptr<ais::Type> m_arrayOfArray;
    std::shared_ptr<ais::DecompIterator> m_decompIter;
    char m_openChar;
    char m_closeChar;
};

// wrapper to make a single object act like an array of length 1
class Array1 : public Array {
public:
    Array1(std::shared_ptr<Type> elemType);
    void applyL(Reader& reader, Visitor& f) const override;
    void applyOne(Reader& reader, std::vector<ApplyOneIdx>& location, int level,
                  const std::function<void(const Type&,Reader&)>& func);
};
 
// Structure container    
class Struct : public Type {
public:
    Struct(const std::string& name);
    Struct(ais::Reader& reader, const std::string& defString);

    // add a member to the definition
    void addMember(const std::string& memberName, std::shared_ptr<Type> member);

    void parseL(InterpretStream& in, Writer& out) const override;
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    void printIndex(int idx, std::ostream& out) const override;
    std::string getTypeName() const override;
    int getTypeId(Writer& out) const override;
private:    
    std::string m_name;
    std::vector<std::pair<std::string, std::shared_ptr<Type>>> m_members;
};

class Union : public Struct {
public:
    Union(const std::string& name);
    void parseL(InterpretStream& in, Writer& out) const override;
};

// Pointer type    
class Pointer : public Type {
public:
    Pointer(std::string derefType);
    void parseL(InterpretStream& in, Writer& out) const override;
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;
private:
    std::string m_derefType;
};

// std::unique_ptr or std::shared_ptr    
class SmartPointer : public Type {
public:
    SmartPointer(std::string pointerType, std::string derefType);
    void parseL(InterpretStream& in, Writer& out) const override;
    std::unique_ptr<Printer> makePrinter(Printer& parent) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;
private:
    std::string m_pointerType;
    std::string m_derefType;
};

// Possible results from comparing two primitives
enum Comparison { LT, WARN_LT, EQ, WARN_GT, GT, NOT_COMPARABLE };

// Base class for any object that can be visited
// This can be either being read from a data stream, or procesed through
// some chain of operations.
class VStream {
public:
    virtual void apply(Visitor& v) = 0;
};

// Class that allow applying a Visitor to a read data stream
class ReaderStream : public VStream {
public:
    ReaderStream(std::unique_ptr<ais::Reader>&& rdr);
    void apply(Visitor& v) override;
    std::unique_ptr<ais::Reader> moveReader() { return std::move(m_rdr); }
private:
    std::unique_ptr<ais::Reader> m_rdr;
};

// Function adapter to apply a unary function and pass the result to
// a visitor
template<class UNARY_OP>
class OpVisitorFunc {
 public:
    OpVisitorFunc(UNARY_OP op, Visitor& out) : m_op(op), m_out(out) {}
    template<class T> void operator()(T x) { m_out(m_op(x)); }
private:
    UNARY_OP m_op;
    Visitor& m_out;
};

// Operator to transform a stream by applying a unary operator.
// An instance of this class can be passed to VStream::apply, and it
// transform each value by UNARY_OP and passes the result to another visitor.
// The control calls (nest,start,etc.) are passed along unchanged.    
template<class UNARY_OP>
class OpVisitor : public FVisitor<OpVisitorFunc<UNARY_OP>> {
public:
    OpVisitor(UNARY_OP op, Visitor& out) :
        FVisitor<OpVisitorFunc<UNARY_OP>>(OpVisitorFunc(op,out)), m_out(out) {}
    void nest(const Type* t, int refIdx) override { m_out.nest(t); }
    void start(const Type* t) override { m_out.start(t); }
    void addMetaInfo(const Type* t, std::pair<int,int> info) override {m_out.addMetaInfo(t,info);}
    void end() override { m_out.end(); }
    void unnest() override { m_out.unnest(); }
    void eof() override { m_out.eof(); }
private:
    Visitor& m_out;
};

// Streamable object based on a unary transformation of another stream.
template <class UNARY_OP>
class OpStream : public VStream {
public:    
    OpStream(std::shared_ptr<VStream> input, UNARY_OP op) : m_in(input), m_op(op) {}

    void apply(Visitor& v) override {
        OpVisitor<UNARY_OP> opV{m_op, v};
        m_in->apply(opV);
    }
private:
    std::shared_ptr<VStream> m_in;
    UNARY_OP m_op;
};

template<class UNARY_OP>
std::shared_ptr<OpStream<UNARY_OP>> makeUnaryOpStream(
    std::shared_ptr<VStream> input,
    UNARY_OP op) {
    return std::make_shared<OpStream<UNARY_OP>>(input, op);
}
    
// Make a stream that wraps a single primitive value
template<class TYPE, class VAL>
class SingleValueStream : public VStream {
public:
    static std::shared_ptr<SingleValueStream<TYPE,VAL>> make(VAL aval) {
        return std::make_shared<SingleValueStream<TYPE,VAL>>(aval);
    }
    SingleValueStream(VAL aval) : val(aval) {}
    void apply(Visitor& v) override {
        v.nest(&t,-1);
        v.start(&t);
        v(val);
        v.end();
        v.unnest();
    }
private:
    TYPE t;
    VAL val;
};


// Base class for accumulating the differences during ais::compare
class Difference {
public:
    virtual void add(const std::string& idx, long int a, long int  b) = 0;
    virtual void add(const std::string& idx, long double a, long double b) = 0;
    virtual void add(const std::string& idx, const std::string& a, const std::string& b) = 0;
    virtual std::string getResult() = 0;
    virtual ~Difference() = default;
};

// compare the contents of two data streams
std::bitset<6> compare(VStream& a, VStream& b, Difference& diff,
                       const double eqTol, const double warnTol);

// print a data stream, prepend with label if given.
// (note, label is passed in mainly because it needs to be interpolated into
//  MultiTheadOutput).
void print(ais::VStream& from, const std::string& label, const std::string& format, std::ostream& stream);

std::shared_ptr<ais::VStream> makeBinaryOpStream(
    std::shared_ptr<ais::VStream> a,
    std::shared_ptr<ais::VStream> b,
    std::unique_ptr<ais::BinaryOperator>&& op);

} // namespace ais

ais::Formatter& operator<<(std::ostream& s, ais::Formatter& f);

template <class T>
std::ostream& operator<<(ais::Formatter& f, T val) {
    switch (f.form) {
    default:
    case ais::Formatter::DEC:
        return *f.os << val;
    case ais::Formatter::HEX:
        return *f.os << std::hex << std::showbase << val << std::dec << std::noshowbase;
    case ais::Formatter::OCT:
        return *f.os << std::oct << std::showbase << val << std::dec << std::noshowbase;
    case ais::Formatter::BIN: {
        typename std::make_unsigned<T>::type m;
        for (m = 1; val & ~(2*m-1); m <<= 1) {}
        for ( ; m ; m >>= 1) { *f.os << ((val & m) ? 1 : 0); };
        return *f.os;
    }
    case ais::Formatter::PADDED_BIN: {
        typename std::make_unsigned<T>::type m = ~0;
        for (m = m ^ (m>>1); m ; m >>= 1) { *f.os << ((val & m) ? 1 : 0); };
        return *f.os;
    }
    }
}
