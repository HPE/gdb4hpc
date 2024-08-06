// Copyright 2024 Hewlett Packard Enterprise Development LP.

#include "pybe_stl.h"

#include <cstddef>

namespace pybe {

Expected<Thread, std::nullptr_t> Thread::make
    (void* (*start_fn)(void *), void* start_arg)
{
    auto attr = ::pthread_attr_t{};
    if (::pthread_attr_init(&attr)) {
        // TODO return errno or strerror
        return nullptr;
    }

    auto pthread = ::pthread_t{};
    if (::pthread_create(&pthread, &attr, start_fn, start_arg)) {
        // TODO return errno or strerror
        return nullptr;
    }

    return Thread(pthread, attr);
}

Thread::Thread(::pthread_t id, ::pthread_attr_t attrs)
    : m_id{id}
    , m_attrs{attrs}
{}

Thread::Thread(Thread&& other)
    : m_id{other.m_id}
    , m_attrs{other.m_attrs}
{
    other.m_id = 0;
    // TODO: confirm that it's ok to leave other.m_attrs untouched
}

Thread::~Thread() {
    // our ID might be 0 after a std::move
    if (m_id != 0) {
        if (!m_detached) {
            // mimicking std::thread.
            // TODO support return values
            ::pthread_join(m_id, nullptr);
        }
    }
}

[[ nodiscard ]]
int Thread::detach() {
    int rc = ::pthread_detach(m_id);
    if (rc == 0) m_detached = true;
    return rc;
}

String to_string(int num) {
    auto result = String{};

    auto len = ::snprintf(nullptr, 0, "%d", num) + 1;
    auto buf = new char[len];
    ::snprintf(buf, len, "%d", num);
    // XXX: is there a way to do this without the copy?
    result.append(buf);
    delete[] buf;

    return result;
}

Expected<LogFile, std::nullptr_t> LogFile::open(StringView dir_path) {
    auto result = LogFile{};

    auto filename = String{dir_path.begin(), dir_path.size()} + "/pybe_";
    filename.append(to_string(::getpid()).c_str());
    filename.append(".log");

    result.m_file = ::fopen(filename.c_str(), "w");
    if (!result.m_file) result.m_file = ::fopen(filename.c_str(), "a");
    if (!result.m_file) return nullptr;

    return result;
}

LogFile::LogFile(LogFile&& expiring)
    : m_file{nullptr}
{
    std::swap(m_file, expiring.m_file);
}

LogFile::~LogFile() {
    if (m_file) {
        ::fclose(m_file);
    }
}

void LogFile::write(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    write_v(fmt, args);
    va_end(args);
}

void LogFile::write_v(const char* fmt, va_list args) {
    ::vfprintf(m_file, fmt, args);
    ::fflush(m_file);
}

void log(const char *format, ...) {
    static auto logfile = []() -> Expected<LogFile, std::nullptr_t> {
        // XXX: should be using an env var macro defined in a header
        if (const auto* path = ::getenv("CRAY_DBG_LOG_DIR")) {
            auto log = LogFile::open(path);
            if (!log) {
                ::fprintf(stderr, "Warning: Failed to open pybe log file\n");
            }
            log->write("pybe log start\n");
            return std::move(*log);
        }
        return nullptr;
    }();

    if (logfile) {
        va_list arg_list;
        va_start(arg_list, format);
        logfile->write_v(format, arg_list);
        va_end(arg_list);
    }
}

} // namespace pybe

