/*
 * Author: number
 * Date  : Nov 3, 2015
 */

#ifndef __COMMON_LOGGER_H_H___
#define __COMMON_LOGGER_H_H___

#include <stdarg.h>

#include "shift_writer.h"

namespace ef {

enum LogLevel {
    kLevelFatal    = 0,
    kLevelErr      = 1,
    kLevelWarn     = 2,
    kLevelInfo     = 3,
    kLevelDebug    = 4,
};

class Logger {
public:
    Logger(const char *file, int line);
    virtual ~Logger();

    static bool Initialize(const char * path, const uint32_t max_file_size,
                          const uint32_t max_file_num, const LogLevel level);
    static bool Initialize(const char * path, const uint32_t max_file_size,
                          const uint32_t max_file_num, const char * level);
    static const std::string & GetErrMsg();

    static void SetLogFilter(LogLevel filter);
    void Fatal(const char * format, ...) __attribute__ ((format (printf, 2, 3)));
    void Err  (const char * format, ...) __attribute__ ((format (printf, 2, 3)));
    void Warn (const char * format, ...) __attribute__ ((format (printf, 2, 3)));
    void Info (const char * format, ...) __attribute__ ((format (printf, 2, 3)));
    void Debug(const char * format, ...) __attribute__ ((format (printf, 2, 3)));


private:
    void Log(const LogLevel level, const char * format, va_list va);
    static LogLevel GetLevel(const char * level);

private:
    const char * _file;
    int _line;
};


#define LOG ef::Logger(__FILE__, __LINE__)

} /* namespace ef */

#endif /* __COMMON_LOGGER_H__ */
