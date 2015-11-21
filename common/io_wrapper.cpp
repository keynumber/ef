#include "io_wrapper.h"

#include <string.h>

namespace ef {

char * safe_strerror(int errorno, char *buf, size_t buflen) {
#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
    strerror_r(errorno, buf, buflen);
#else
    const char * error = strerror_r(errorno, buf, buflen);
    if (error != buf) {
        size_t to_copy = strlen(error) + 1;
        size_t copy_len = to_copy > buflen ? buflen : to_copy;
        memcpy(buf, error, copy_len);
        buf[buflen-1] = 0;
    }
#endif
    return buf;
}

} /* namespace ef */

