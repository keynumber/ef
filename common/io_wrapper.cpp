#include "io_wrapper.h"

#include <errno.h>

#include <unistd.h>

#include <string>

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

std::string safe_strerror(int errorno)
{
    const int buf_len = 256;
    char buf[buf_len];
#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
    strerror_r(errorno, buf, buf_len);
    return std::string(buf);
#else
    const char * error = strerror_r(errorno, buf, buf_len);
    return std::string(error);
#endif
}
