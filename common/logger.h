/*
 * Author: number
 * Date  : Nov 3, 2015 */ 
#ifndef __COMMON_LOGGER_H_H___
#define __COMMON_LOGGER_H_H___

#include <stdarg.h>

namespace ef {

class Logger {
public:
    Logger();
    virtual ~Logger();

    static void Initialize(const char * file, const int max_file_size, const int max_file_num);
    static void Fatal(const char * file, int line, const char * format, ...) __attribute__ ((format (printf, 3, 4)));
    static void Err  (const char * file, int line, const char * format, ...) __attribute__ ((format (printf, 3, 4)));
    static void Warn (const char * file, int line, const char * format, ...) __attribute__ ((format (printf, 3, 4)));
    static void Info (const char * file, int line, const char * format, ...) __attribute__ ((format (printf, 3, 4)));
    static void Debug(const char * file, int line, const char * format, ...) __attribute__ ((format (printf, 3, 4)));
    static void Frame(const char * file, int line, const char * format, ...) __attribute__ ((format (printf, 3, 4)));

private:
    enum LogLevel {
        kLevelFatal    = 0,
        kLevelErr      = 1,
        kLevelWarn     = 2,
        kLevelInfo     = 3,
        kLevelDebug    = 4,
        kLevelFrame    = 5,
    };

private:
    static void Log(const char * file, const char * line, const LogLevel level,
             const char * format, va_list va);

private:
    static int _fd;
    static int _max_file_size;
    static int _max_file_num;
    static int _cur_file_index;
    static int _cur_file_size;
//    const char _file[256];
};

} /* namespace ef */

#endif /* __COMMON_LOGGER_H__ */
