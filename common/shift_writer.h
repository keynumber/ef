/*
 * Author: number
 * Date  : Nov 3, 2015
 */

#ifndef __COMMON_SHIFT_WRITER_H_H___
#define __COMMON_SHIFT_WRITER_H_H___

#include <stdint.h>

#include <string>

namespace ef {

static const uint32_t kBuffLen = 128;
static const uint32_t kDefaultMaxFileSize = 20 * 1024 * 1024;
static const uint32_t kDefaultMaxFileNumber = 10;
static const char * kDefaultSuffix = ".log";

/** * 支持按照文件大小对文件及进行切分,
 */
class ShiftWriter {
public:
    ShiftWriter(const char * path,
                uint32_t max_file_size = kDefaultMaxFileSize,
                uint32_t max_file_number = kDefaultMaxFileNumber,
                const char *suffix = kDefaultSuffix);
    virtual ~ShiftWriter();

    int Initialize();
    int Write(void * buf, uint32_t len);

private:
    void Shift();

private:
    int _fd;
    std::string _path;
    std::string _suffix;
    uint32_t _max_file_size;
    uint32_t _max_file_num;
    uint32_t _cur_file_size;
};

} /* namespace ef */

#endif /* __COMMON_SHIFT_WRITER_H__ */
