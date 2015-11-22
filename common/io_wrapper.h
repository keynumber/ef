/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#ifndef __COMMON_IO_WRAPPER_H_H___
#define __COMMON_IO_WRAPPER_H_H___

#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include "macro.h"

namespace ef {

inline ssize_t safe_read(int fd, void *buf, size_t count)
{
    ssize_t ret;
    do {
        ret = read(fd, buf, count);
    } while (unlikely(ret < 0 && errno == EINTR));
    return ret;
}

inline ssize_t safe_write(int fd, const void *buf, size_t count)
{
    ssize_t ret;
    do {
        ret = write(fd, buf, count);
    } while (unlikely(ret < 0 && errno == EINTR));
    return ret;
}

inline int set_nonblock(int fd)
{
    int ret = 0;
    do {
        ret = fcntl(fd, F_GETFL);
    } while (unlikely(ret <0 && errno == EINTR));

    if (ret < 0) {
        return -1;
    }

    do {
        ret = fcntl(fd, F_SETFL, ret | O_NONBLOCK | O_NDELAY);
    } while (unlikely(ret <0 && errno == EINTR));

    return ret;
}

inline int safe_close(int fd)
{
    int ret;
    do {
        ret = close(fd);
    } while (unlikely(ret < 0 && errno == EINTR));
    return ret;
}

char * safe_strerror(int errorno, char *buf, size_t buflen);

} /* namespace ef */

#endif /* __COMMON_IO_WRAPPER_H_H___*/
