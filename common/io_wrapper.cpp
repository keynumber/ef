#include "io_wrapper.h"

#include <errno.h>

#include <unistd.h>

ssize_t safe_read(int fd, void *buf, size_t count)
{
    ssize_t ret;
    do {
        ret = read(fd, buf, count);
    } while (ret < 0 && errno == EINTR);
    return ret;
}

ssize_t safe_write(int fd, const void *buf, size_t count)
{
    ssize_t ret;
    do {
        ret = write(fd, buf, count);
    } while (ret < 0 && errno == EINTR);
    return ret;
}

int safe_close(int fd)
{
    int ret;
    do {
        ret = close(fd);
    } while (ret < 0 && errno == EINTR);
    return ret;
}

