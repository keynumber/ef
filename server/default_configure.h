/*
 * Author: number
 * Date  : Nov 21, 2015
 */

#ifndef __SERVER_DEFAULT_CONFIGURE_H_H___
#define __SERVER_DEFAULT_CONFIGURE_H_H___

#include <cstdint>

namespace ef {

struct DefaultConf {
    static const int kListenBackLog = 1024;

    static const uint32_t kMaxLogFileSize            = 50 * 1024 * 1024;
    static const uint32_t kMaxLogFileNum             = 10;

    static const uint32_t kMaxListenSocket           = 128;
    static const uint32_t kMaxClientNum              = 10000;

    static const uint32_t kBufferBlockSize           = 1024;
    static const uint32_t kReadBufferSize            = 1024 * 1024;
};

} /* namespace ef*/



#endif /* __SERVER_DEFAULT_CONFIGURE_H__ */
