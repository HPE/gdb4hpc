# AIS Abstract Information Stream

## Goals

AIS is a means for describing at runtime an arbitrary
C/C++/fortran/rust data structure that can be parsed and transmitted
across a socket, then processed.  The goals:

- The protocol is not aimed at persistence, so there is no provisions
  for versioning.
- The protocol need to be light weight, for a particular example, if
  transmitting an array, the type meta data should be transmitted
  exactly once, not once for each element.
- The protocol does not copy data elements, the sending side writes
  directly to the raw buffer, and the reading side processes directly
  from the buffer.
- The data model is a single use stream, there is no need or provision
  for random access or multiple passes.
- On the read side, it should be able to support printing and
  comparison operations and simple expressions by defining appropriate
  visitor objects.

## The ais::Type object

An object derived from `ais::Type` describes how to handle the
streaming of that specific type.   An instance does represent a value
of that type, just the handling.   For example:
```
class Float : public Type {
public:
    Float() {}
    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;
    float readVal(Reader& reader) const;
};
```

Notice that every method is const.   `parseL` expects the next thing
available in `in` to be a float value in human readable form, it will
it then send it to `out` in stream protocol form.

`applyL` expect the next thing in `reader` to the a float in steam
protocol form, which it will input and then call `f(val)` for the
visitor to process.

`getTypeName` just provides the meta data to describe the type for the
stream protocol.   In this case, the function just returns "float".

## Compound types

AIS types can be defined in terms of other AIS types.   For example:

```
class Struct : public Type {
public:
    Struct(const std::string& name);
    Struct(ais::Reader& reader, const std::string& defString);

    // add a member to the definition
    void addMember(const std::string& memberName,
                   std::shared_ptr<Type> member)

    void parseL(InterpretStream& in, Writer& out) const override;
    void applyL(Reader& reader, Visitor& f) const override;
    std::string getTypeName() const override;
    int getTypeId(Writer& out) const override;
}
```

The writer side uses the first constructor then builds up the
definition with the `addMember` method.   The second constructor
creates one based on type string in the protocol.

## Data protocol

The stream is a sequence of records, each of which starts with an
index.   If the index equal to the number of types already defined,
then it is followed by a type description, otherwise it's immediately
followed by the contents matching for that type.

Thus data for sending an array of three floats would be:
```
0 float
1 [0 
1 3 0 0 1.1 2.2 3.3
```
i.e.
- type 0 is a float
- type 1 is a array of type 0
- data element of type 1 - length:3, index0:0, mayBeTruncated:0, values

Each type defines it's own layout.   The `mayBeTruncated` is a choice
for the array implementation.  In some cases, gdb may not print the
entire array, so in those cases it uses a terminator.

There were plans to move to binary form, but the original ascii
approach has been performant enough so far and can make debugging
easier.

## The parseL function

`parseL` has to read whatever gdb prints for a given type.   The
InterpreterStream interface is based on particularly primitive
assumptions:

```
void ais::Float::parseL(InterpretStream& in, Writer& out) const
{
    const char* str = in.get();

    char* next = nullptr;
    auto val = strtof(str, &next);
    in.setPos(next);

    if (next == str) {
        out.writeError("could not evaluate");
    } else {
        out.writeFloat(val);
    }
}
```
Note the reliance on manually updating the read position.

For `float` there's not ambiguity from gdb, but the parser for
ais::String has to deal with gdb sometimes printing `"the value"` and
sometimes printing something like `{std::basic_string<char> = "the
value"}`.  That logic is pulled into ais in preference of maintaining
two full type hierarchies.

## the applyL function and visitors

The key of the streaming based approach is that the developer creates
a visitor object that is able to consume the data structure as a
sequence of primitives interspersed with meta-information calls to
describe the structure of the data.   Using this, one can write a
function to compare the values of two data streams without having to
fully understand the specifics.   Thus the compare function can easily
handle `std::pair<float,float>` vs. `struct X { float a; float b };`
vs even `double[2]` since they all map to the same sequence.

The more challenging `Visitor` to implement can actually be the pretty
printer.  These tend to have state based logic like "don't print a
comma before the first element" or "print ... if the array is
truncated".   So there is a `Printer` class and a `makePrinter`
virtual function in `ais::Type`.

## Adding a new type

At this point, there are a wide spectrum of types already defined.
Typically, by far the easiest way to develop the new one is to model
it on something similar.
