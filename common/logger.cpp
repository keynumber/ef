/*
 * Author: number
 * Date  : Nov 3, 2015
 */

#include "logger.h"

#include <stdio.h>
#include <string.h>

#include <sys/time.h>

namespace ef {

static LogLevel _log_filter = kLevelDebug;
static ShiftWriter _writer;

static const int kMaxLogSize = 2048;
static const int kMaxPathLength = 256;
static const int kBufSize = kMaxLogSize + kMaxPathLength + 100;

Logger::Logger(const char* file, int line)
    : _file(file)
    , _line(line)
{
}

Logger::~Logger()
{
}

int Logger::Initialize(const char* path, const uint32_t max_file_size,
                       const uint32_t max_file_num, const LogLevel filter)
{
    int ret = 0;
    if ((ret = _writer.Initialize(path, max_file_size, max_file_num, ".log"))) {
        return ret;
    }
    _log_filter = filter;
    return 0;
}

void Logger::SetLogFilter(LogLevel filter)
{
    _log_filter = filter;
}

void Logger::Log(const LogLevel level, const char* format, va_list va)
{
    if (_log_filter < level) {
        return;
    }

    char buf[kBufSize];

    char * ptr = buf;

    timeval now;
    gettimeofday(&now, nullptr);
    struct tm tm;
    localtime_r(&now.tv_sec, &tm);
    int write_size = snprintf(ptr, static_cast<size_t>(kBufSize-(ptr-buf)), "%02d-%02d %02d:%02d:%02d.%06ld",
            tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
            static_cast<long>(now.tv_usec));
    ptr += write_size;
    // replace 0 with ' '
    *ptr = ' ';
    ++ ptr;

    switch (level) {
        case kLevelFatal: memcpy(ptr, "FATAL ", 6); ptr += 6; break;
        case kLevelErr  : memcpy(ptr, "ERROR ", 6); ptr += 6; break;
        case kLevelWarn : memcpy(ptr, "WARN  ", 6); ptr += 6; break;
        case kLevelInfo : memcpy(ptr, "INFO  ", 6); ptr += 6; break;
        case kLevelDebug: memcpy(ptr, "DEBUG ", 6); ptr += 6; break;
        default         : memcpy(ptr, "OTHER ", 6); ptr += 6; break;
    }

    write_size = snprintf(ptr, static_cast<size_t>(kBufSize-(ptr-buf)), "[%s:%d] ", _file, _line);
    if (write_size > kMaxPathLength) {
        write_size = kMaxPathLength;
    }
    ptr += write_size;

    write_size = vsnprintf(ptr, kMaxLogSize, format, va);
    if (write_size > kMaxLogSize) {
        write_size = kMaxLogSize;
    }
    ptr += write_size;
    *ptr = 0;

    _writer.Write(buf, ptr-buf);
}

#define LOGGER_DEF(name)                            \
void Logger::name(const char * format, ...)         \
{                                                   \
    va_list va;                                     \
    va_start(va, format);                           \
    Log(kLevel##name, format, va);                  \
}


LOGGER_DEF(Fatal)
LOGGER_DEF(Err  )
LOGGER_DEF(Warn )
LOGGER_DEF(Info )
LOGGER_DEF(Debug)

} /* namespace ef */
