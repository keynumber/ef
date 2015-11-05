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

    static int Initialize(const char * path, const int max_file_size,
                          const int max_file_num, const LogLevel filter);
    static void SetLogFilter(LogLevel filter);
    void Fatal(const char * format, ...) __attribute__ ((format (printf, 2, 3)));
    void Err  (const char * format, ...) __attribute__ ((format (printf, 2, 3)));
    void Warn (const char * format, ...) __attribute__ ((format (printf, 2, 3)));
    void Info (const char * format, ...) __attribute__ ((format (printf, 2, 3)));
    void Debug(const char * format, ...) __attribute__ ((format (printf, 2, 3)));


private:
    void Log(const LogLevel level, const char * format, va_list va);

private:
    const char * _file;
    int _line;
};

} /* namespace ef */

#endif /* __COMMON_LOGGER_H__ */
