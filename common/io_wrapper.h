#include <sys/types.h>

ssize_t safe_read(int fd, void *buf, size_t count);
ssize_t safe_write(int fd, const void *buf, size_t count);
int     safe_close(int fd);

char * safe_strerror(int errorno, char *buf, size_t buflen);
