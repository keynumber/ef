/*
 * Author: number
 * Date  : Nov 3, 2015
 */

#include "shift_writer.h"

#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

#include "io_wrapper.h"

namespace ef {

ShiftWriter::ShiftWriter()
    : _fd(-1)
    , _max_file_size(0)
    , _max_file_num(0)
    , _cur_file_size(0)
{
}

ShiftWriter::~ShiftWriter()
{
    if (_fd >= 0) {
        safe_close(_fd);
        _fd = -1;
    }
}

bool ShiftWriter::Initialize(const char * path,
                            uint32_t max_file_size,
                            uint32_t max_file_number,
                            const char *suffix)
{
    _path = path;
    _max_file_num = max_file_number;
    _max_file_size = max_file_size;
    _suffix = suffix;

    // to test whether the path is valid
    char file[256];
    snprintf(file, 256, "%s%d%s", _path.c_str(), 0, _suffix.c_str());
    _fd = open(file,
            O_CREAT | O_APPEND | O_WRONLY | O_NOFOLLOW | O_NOCTTY,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (_fd < 0) {
        int errorno = errno;
        char buf[256];
        safe_strerror(errorno, buf, 256);
        _errmsg = buf;
        return false;
    }

    return true;
}

int ShiftWriter::Write(void* buf, uint32_t len)
{
    if (_cur_file_size > _max_file_size) {
        Shift();
    }

    if (_fd < 0) {
        char file[256];
        snprintf(file, 256, "%s%d%s", _path.c_str(), 0, _suffix.c_str());
        _fd = open(file,
                O_CREAT | O_APPEND | O_WRONLY | O_NOFOLLOW | O_NOCTTY,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (_fd < 0) {
            int errorno = errno;
            char buf[256];
            safe_strerror(errorno, buf, 256);
            _errmsg = buf;
            return -1;
        }
    }

    int ret = safe_write(_fd, buf, len);

    _cur_file_size += len;

    return ret;
}

void ShiftWriter::Shift()
{
    if (_fd >= 0) {
        safe_close(_fd);
        _fd = -1;
    }

    char new_file[256];
    char old_file[256];
    for (int i = (int)_max_file_num - 2; i >= 0; --i) {
        snprintf(old_file, 256, "%s%d%s", _path.c_str(), i, _suffix.c_str());
        if (access(old_file, F_OK) != 0) {
            continue;
        }

        snprintf(new_file, 256, "%s%d%s", _path.c_str(), i+1, _suffix.c_str());
        rename(old_file, new_file);
    }

    _cur_file_size = 0;
}

const std::string & ShiftWriter::GetErrMsg() const
{
    return _errmsg;
}

} /* namespace ef */

#if 0

#include <string.h>

int main(int argc, char *argv[])
{
    ef::ShiftWriter writer;
    int ret = 0;
    if (ret = writer.Initialize("/home/number/test_shift_write", 500, 5, ".log")) {
        printf("writer initialize failed, errmsg: %s\n", writer.GetErrMsg().c_str());
        return -1;
    }

    char buf[1024];
    for (int i=0; i<100; i++) {
        snprintf(buf, 1024, "this is just a test for ShiftWriter, line number %d\n", i);
        writer.Write(buf, strlen(buf));
    }
    return 0;
}

#endif
