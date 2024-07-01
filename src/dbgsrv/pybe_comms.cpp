// Copyright 2024 Hewlett Packard Enterprise Development LP.

#include "pybe_comms.h"

#include <cstdarg>
#include <cstdio>

#include "pybe_stl.h"

namespace pybe {

struct WireMsg {
    // A message for transfer to dbgsrv over the pybe socket
    enum class Type : int {
        Int = 1,
        String,

        Frame,
        Backtrace,
    };

    String msg;

    void push(int num);
    void push(const String& string);

    void push(const Frame& frame);
    void push(const Backtrace& frame);
};

void WireMsg::push(int num) {
    this->msg.append(to_string(static_cast<int>(Type::Int)).c_str());
    this->msg.append(" ");
    this->msg.append(to_string(num).c_str());
    this->msg.append(" ");
}

void WireMsg::push(const String& string) {
    this->msg.append(to_string(static_cast<int>(Type::String)).c_str());
    this->msg.append(" ");
    // note that this size push includes the type identifier for the int
    push(string.size());
    this->msg.append(string.c_str());
    this->msg.append(" ");
}

void WireMsg::push(const Frame& frame) {
    this->msg.append(to_string(static_cast<int>(Type::Frame)).c_str());
    this->msg.append(" ");

    push(frame.function_name);
    push(frame.file_name);
    push(frame.line_number);
}

void WireMsg::push(const Backtrace& bt) {
    this->msg.append(to_string(static_cast<int>(Type::Backtrace)).c_str());
    this->msg.append(" ");

    push(bt.frames.size());

    for (const auto& frame : bt.frames) {
        push(frame);
    }
}

void consume_whitespace(const char*& cursor) {
    while (::isspace(*cursor)) cursor++;
}

bool consume_type(const char*& cursor, WireMsg::Type type) {
    consume_whitespace(cursor);

    const auto type_str = to_string(static_cast<int>(type));
    if (::strncmp(cursor, type_str.c_str(), type_str.size()) == 0) {
        cursor += type_str.size();
        return true;
    } else {
        return false;
    }
}

Expected<int, std::nullptr_t> parse_int(const char*& cursor) {
    if (!consume_type(cursor, WireMsg::Type::Int)) return nullptr;

    // strtol consumes whitespace for us
    char* end = nullptr;
    auto value = static_cast<int>(::strtol(cursor, &end, 10));
    cursor = end;
    return value;
}

Expected<String, std::nullptr_t> parse_string(const char*& cursor) {
    if (!consume_type(cursor, WireMsg::Type::String)) return nullptr;

    consume_whitespace(cursor);

    if (auto maybe_len = parse_int(cursor)) {
        auto& len = *maybe_len;
        if (len < 0) return nullptr;

        cursor++; // we know there will only be one space between the
                  // len and the string - the string might start with whitespace
        auto result = String(cursor, len);
        cursor += len;
        return result;
    } else {
        return nullptr;
    }
}

String Frame::serialize() const {
    auto result = WireMsg{};
    result.push(*this);
    return std::move(result.msg);
}

Expected<Frame, std::nullptr_t> Frame::deserialize(const char*& cursor, size_t) {
    auto result = Frame{};

    // TODO bounds check with size_t param

    if (!consume_type(cursor, WireMsg::Type::Frame)) return nullptr;

    if (auto maybe_str = parse_string(cursor)) {
        result.function_name = *maybe_str;
    } else {
        return nullptr;
    }

    if (auto maybe_str = parse_string(cursor)) {
        result.file_name = *maybe_str;
    } else {
        return nullptr;
    }

    if (auto maybe_int = parse_int(cursor)) {
        result.line_number = *maybe_int;
    } else {
        return nullptr;
    }

    return result;
}

String Backtrace::serialize() const {
    auto result = WireMsg{};
    result.push(*this);
    return std::move(result.msg);
}

Expected<Backtrace, std::nullptr_t> Backtrace::deserialize(const char*& cursor, size_t len) {
    auto result = Backtrace{};

    // TODO bounds check with size_t param

    if (!consume_type(cursor, WireMsg::Type::Backtrace)) return nullptr;

    if (auto maybe_int = parse_int(cursor)) {
        auto& num_frames = *maybe_int;

        for (int i = 0; i < num_frames; i++) {
            if (auto maybe_frame = Frame::deserialize(cursor, len)) {
                result.frames.emplace_back(std::move(*maybe_frame));
            } else {
                return nullptr;
            }
        }

        return result;
    } else {
        return nullptr;
    }
}

bool Backtrace::operator==(const Backtrace& other) const {
    if (this->frames.size() != other.frames.size()) return false;
    for (size_t i = 0; i < this->frames.size(); i++) {
        if (this->frames[i] != other.frames[i]) return false;
    }
    return true;
}

void log(const char *format, ...) {
    // XXX: placeholder until proper logging solution is implemented.
    //      for now this will be seen as user application output.
    va_list arg_list;
    va_start(arg_list, format);
    vfprintf(stderr, format, arg_list);
    va_end(arg_list);
}

} // namespace pybe
