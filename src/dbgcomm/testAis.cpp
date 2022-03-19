// Copyright 2022 Hewlett Packard Enterprise Development LP.

// This file defines mock-up version of a the key ais interactions
// which allows us to run debug data issues on a VM just by linking
// this object and ais.o.
#include "ais.h"
#include <sstream>
#include <iostream>
#include <iomanip>

// Dummy out dependencies.
class ThreadMsgParser {
private:
    std::string m_str;
    std::string m_thread;
public:
    ThreadMsgParser(const std::string& msg) : m_str(msg) {}
    ThreadMsgParser(std::string&& msg) : m_str(std::move(msg)) {}

    std::optional<std::pair<std::string,std::string>> nextResult();

    ThreadMsgParser(const ThreadMsgParser&) = delete;
    ThreadMsgParser& operator=(const ThreadMsgParser&) = delete;
};

std::optional<std::pair<std::string,std::string>> ThreadMsgParser::nextResult() {
        std::optional<std::pair<std::string,std::string>>  res;
        return res;
    }

class Buffer : public ais::WriteBuffer, public ais::ReadBuffer
{
public:
    void writeInt(int val) override;
    void writeLong(long val) override;
    void writeLongLong(long long val) override;
    void writeFloat(float val) override;
    void writeDouble(double val) override;
    void writeLongDouble(long double val) override;
    void writeString(const std::string& str) override;
    void writePointer(const std::string& str, void* ptr) override;

    int readInt() override;
    long readLong() override;
    long long readLongLong() override;
    float readFloat() override;
    double readDouble() override;
    long double readLongDouble() override;
    std::string readString() override;
    bool hasMoreData() override;

    void set(const std::string message);
private:
    std::istringstream& getIs();
    std::ostringstream m_os;
    std::unique_ptr<std::istringstream> m_is;
};

void Buffer::writeInt(int val) { m_os << " " << val; }
void Buffer::writeLong(long val) { m_os << " " << val; }
void Buffer::writeLongLong(long long val) { m_os << " " << val; }
void Buffer::writeFloat(float val) {
    m_os << " " << std::setprecision(std::numeric_limits<float>::max_digits10) << val; }
void Buffer::writeDouble(double val) {
    m_os << " " << std::setprecision(std::numeric_limits<double>::max_digits10) << val; }
void Buffer::writeLongDouble(long double val) {
    m_os << " " << std::setprecision(std::numeric_limits<long double>::max_digits10) << val; }
void Buffer::writeString(const std::string& val) { m_os << " " << std::quoted(val); }
void Buffer::writePointer(const std::string& type, void* val) {
    m_os << " unimplemented writePointer " << type << " " << val;
}

int Buffer::readInt() { int val; getIs() >> val; return val; }
long Buffer::readLong() { long val; getIs() >> val; return val; }
long long Buffer::readLongLong() { long long val; getIs() >> val; return val; }
float Buffer::readFloat() { float val; getIs() >> val; return val; }
double Buffer::readDouble() { double val; getIs() >> val; return val; }
long double Buffer::readLongDouble() { long double val; getIs() >> val; return val; }
std::string Buffer::readString() {
    std::string val; getIs() >> std::quoted(val); return val;
}
bool Buffer::hasMoreData() {
    auto& is = getIs();
    return is && (is.peek() != EOF);
}

std::istringstream& Buffer::getIs() {
    if (!m_is) {
        auto data = m_os.str();
        m_is.reset(new std::istringstream(data));
    }
    return *m_is;
}
void Buffer::set(std::string val) {
    m_is.reset(new std::istringstream(std::move(val)));
}

class IStream : public ais::InterpretStream
{
public:
    IStream(const std::vector<std::string>& s) : m_strs{s}, m_it(m_strs.begin()), m_pos(0) {}
    const char* get() override;
    void setPos(const char* pos) override;
    bool mayBeTruncated() override;
    bool isContinued() override;
private:
    std::vector<std::string> m_strs;
    std::vector<std::string>::iterator m_it;
    size_t m_pos;
};

const char* IStream::get() {
    if (m_pos >= m_it->size()) {
        if (++m_it == m_strs.end()) return nullptr;
        else m_pos = 0;
    }
    return &(*m_it)[m_pos];
}

void IStream::setPos(const char* pos) {
    m_pos = pos - &(*m_it)[0];
}

bool IStream::mayBeTruncated() { return false; };
bool IStream::isContinued() { return m_it + 1 != m_strs.end(); }

/*
// an array of ints
static void getTestInts(ais::Writer& writer)
{
    static int cnt = 0;

    auto elemType = std::make_shared<ais::Int>();
    auto arr = std::make_shared<ais::Array>(elemType,10);
    auto aif = ais::Any{arr, 0};

    auto gdbVal = std::stringstream{};
    gdbVal << "{";

    auto val = cnt++;
    for (int i=0; i<10; ++i, ++val) {
        gdbVal << ( i ? ", " : "") << val;
    }
    gdbVal << "}";

    auto is = IStream{gdbVal.str()};

    aif.parse(is, writer);
}
*/

int main(int argc, char* argv[])
{
    try {
        auto str = std::string{"[an output result from gdb]"};
        auto inp = std::vector<std::string>{str};
                               
        auto is = IStream{inp};

        auto buffer = std::make_shared<Buffer>();
        auto writer = ais::Writer{buffer};
        //getTestInts(writer);

        auto elemType = std::make_shared<ais::String>();
        auto vecType = std::make_shared<ais::Vector>(elemType);
        auto vecType2 = std::make_shared<ais::Vector>(vecType);
        auto aif = ais::Any{vecType2,0};

        aif.parse(is, writer);

        auto reader = std::make_unique<ais::BufferReader>(buffer);
        auto readerVS = ais::ReaderStream{std::move(reader)};
        ais::print(readerVS, "test:", std::cout);
        std::cout << std::endl;

        
    } catch (std::exception& err) {
        std::cerr << "caught exception " << err.what() << "\n";
    }
}
