#include "io_wrapper.h"

#include <errno.h>

#include <unistd.h>

#include <string.h>

#include "macro.h"

ssize_t safe_read(int fd, void *buf, size_t count)
{
    ssize_t ret;
    do {
        ret = read(fd, buf, count);
    } while (unlikely(ret < 0 && errno == EINTR));
    return ret;
}

ssize_t safe_write(int fd, const void *buf, size_t count)
{
    ssize_t ret;
    do {
        ret = write(fd, buf, count);
    } while (unlikely(ret < 0 && errno == EINTR));
    return ret;
}

int safe_close(int fd)
{
    int ret;
    do {
        ret = close(fd);
    } while (unlikely(ret < 0 && errno == EINTR));
    return ret;
}

char * safe_strerror(int errorno, char *buf, size_t buflen) {
#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
    strerror_r(errorno, buf, buflen);
#else
    const char * error = strerror_r(errorno, buf, buflen);
    if (error != buf) {
        int to_copy = strlen(error) + 1;
        int copy_len = to_copy > buflen ? buflen : to_copy;
        memcpy(buf, error, copy_len);
        buf[buflen-1] = 0;
    }
#endif
    return buf;
}


