/*
 * Author: number
 * Date  : Nov 18, 2015
 */

#include "socket_buffer.h"

#include <cassert>
#include <cstring>

#include <unistd.h>

#include "mem_pool.h"
#include "io_wrapper.h"

namespace ef {

SocketBuffer::SocketBuffer(MemPool *pool, uint32_t block_size/* = DEFAULT_BUFFER_BLOCK_SIZE */)
    : _block_size(block_size)
    , _buf_size(0)
    , _mem_pool(pool)
{
    assert(_mem_pool);
}

SocketBuffer::~SocketBuffer()
{
    for (auto & it : _buf_list) {
        _mem_pool->Free(it.buf);
        it.buf = nullptr;
        it.read_index = 0;
        it.write_index = 0;
    }
}

void SocketBuffer::Clear()
{
    for (auto & it : _buf_list) {
        _mem_pool->Free(it.buf);
        it.buf = nullptr;
        it.read_index = 0;
        it.write_index = 0;
    }

    _buf_list.clear();
}

uint32_t SocketBuffer::Size() const
{
    return _buf_size;
}

bool SocketBuffer::Append(const char* buf, uint32_t len)
{
    if (_buf_list.empty()) {
        _buf_list.push_back(Block());
    }
    Block *blk = &_buf_list.back();

    uint32_t copy_len = len;
    const char *ptr = buf;
    while (copy_len > 0) {
        uint32_t cycle_len = copy_len > _block_size - blk->write_index ? _block_size - blk->write_index: copy_len;
        if (!blk->buf) {
            blk->buf = (char*)_mem_pool->Malloc(_block_size);
        }

        memcpy(blk->buf + blk->write_index, ptr, cycle_len);
        blk->write_index += cycle_len;

        copy_len -= cycle_len;
        ptr += cycle_len;

        if (copy_len > 0) {
            _buf_list.push_back(Block());
            blk = &_buf_list.back();
        }
    }

    _buf_size += len;
    return true;
}

uint32_t SocketBuffer::Get(char* buf, uint32_t len) const
{
    std::list<Block>::const_iterator it = _buf_list.begin();

    char *ptr = buf;
    uint32_t copy_len = len > _buf_size ? _buf_size : len;
    uint32_t left = copy_len;
    while (left > 0 && it != _buf_list.end() && it->buf != nullptr) {
        uint32_t cycle_len = left > it->write_index - it->read_index ? it->write_index - it->read_index : left;

        memcpy(ptr, it->buf + it->read_index, cycle_len);
        ptr += cycle_len;
        left -= cycle_len;

        ++it;
    }

    return copy_len;
}

uint32_t SocketBuffer::Take(char* buf, uint32_t len)
{
    std::list<Block>::iterator it = _buf_list.begin();
    std::list<Block>::iterator next;

    char *ptr = buf;
    uint32_t copy_len = len > _buf_size ? _buf_size : len;
    uint32_t left = copy_len;
    while (left > 0 && it != _buf_list.end() && it->buf != nullptr) {
        next = it;
        ++next;

        uint32_t cycle_len = left > it->write_index - it->read_index ? it->write_index - it->read_index : left;

        memcpy(ptr, it->buf + it->read_index, cycle_len);
        ptr += cycle_len;
        left -= cycle_len;

        it->read_index += cycle_len;
        if (it->write_index < _block_size) {
            _mem_pool->Free(it->buf);
            it->buf = nullptr;
            _buf_list.erase(it);
        }

        it = next;
    }

    _buf_size -= copy_len;
    return copy_len;
}

// TODO to test
int SocketBuffer::SendToSocket(int sock_fd, std::string *errmsg)
{
    std::list<Block>::iterator it = _buf_list.begin();
    std::list<Block>::iterator next;

    int total_send = 0;
    while (it != _buf_list.end() && it->buf != nullptr) {
        next = it;
        ++next;

        uint32_t to_send = it->write_index - it->read_index;
        int isend = safe_write(sock_fd, it->buf, to_send);
        if (isend < 0) {
            if (errmsg) {
                int errorno = errno;
                char buf[256];
                safe_strerror(errorno, buf, 256);
                *errmsg = buf;
            }
            return -1;
        }

        it->read_index += (uint32_t)isend;
        total_send += isend;

        if ((uint32_t)isend != to_send) {
            return total_send;
        }

        _mem_pool->Free(it->buf);
        it->buf = nullptr;
        _buf_list.erase(it);

        it = next;
    }

    return total_send;
}

} /* namespace ef */


#if 0

#include <iostream>
#include <cstring>
using namespace std;

const char * str = "abcdefghijklmnopqrstuvwxyz";

void test_get() {
    ef::MemPool pool;
    ef::SocketBuffer buffer(&pool, 5);
    uint32_t len = strlen(str);
    buffer.Append(str, len);

    cout << "buffer size after append: " << buffer.Size() << endl;

    buffer.Append(str, len);
    cout << "buffer size after append: " << buffer.Size() << endl;

    char buf[1024];
    memset(buf, 0, sizeof(buf));
    uint32_t size = buffer.Get(buf, 3);
    cout << "Get 3 bytes, returned get " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Get(buf, 5);
    cout << "Get 5 bytes, returned get " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Get(buf, 7);
    cout << "Get 7 bytes, returned get " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Get(buf, 10);
    cout << "Get 10 bytes, returned get " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Get(buf, 13);
    cout << "Get 13 bytes, returned get " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Get(buf, 33);
    cout << "Get 33 bytes, returned get " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Get(buf, 1024);
    cout << "Get 1024 bytes, returned get " << size  << " content : " << buf << endl;
}

void test_take() {
    ef::MemPool pool;
    ef::SocketBuffer buffer(&pool, 5);
    uint32_t len = strlen(str);
    buffer.Append(str, len);

    cout << "buffer size after append: " << buffer.Size() << endl;

    buffer.Append(str, len);
    cout << "buffer size after append: " << buffer.Size() << endl;

    char buf[1024];
    memset(buf, 0, sizeof(buf));
    uint32_t size = buffer.Take(buf, 3);
    cout << "Take 3 bytes, returned take " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Take(buf, 5);
    cout << "Take 5 bytes, returned take " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Take(buf, 7);
    cout << "Take 7 bytes, returned take " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Take(buf, 10);
    cout << "Take 10 bytes, returned take " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Take(buf, 13);
    cout << "Take 13 bytes, returned take " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Take(buf, 33);
    cout << "Take 33 bytes, returned take " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Take(buf, 1024);
    cout << "Take 1024 bytes, returned take " << size  << " content : " << buf << endl;
}

void test_append_get_and_take() {
    ef::MemPool pool;
    ef::SocketBuffer buffer(&pool, 5);
    uint32_t len = strlen(str);
    buffer.Append(str, len);

    cout << "buffer size after append: " << buffer.Size() << endl;

    buffer.Append(str, len);
    cout << "buffer size after append: " << buffer.Size() << endl;

    char buf[1024];
    memset(buf, 0, sizeof(buf));
    uint32_t size = buffer.Take(buf, 3);
    cout << "Take 3 bytes, returned take" << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Get(buf, 20);
    cout << "Get 20 bytes, returned get " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Take(buf, 13);
    cout << "Take 13 bytes, returned take " << size  << " content : " << buf << endl;

    buffer.Append(str, len);
    cout << "buffer size after append: " << buffer.Size() << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Get(buf, 300);
    cout << "Get 300 bytes, returned get " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Take(buf, 33);
    cout << "Take 33 bytes, returned take " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Get(buf, 30);
    cout << "Get 30 bytes, returned get " << size  << " content : " << buf << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Take(buf, 1024);
    cout << "Take 1024 bytes, returned take " << size  << " content : " << buf << endl;

    buffer.Append(str, len);
    cout << "buffer size after append: " << buffer.Size() << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Take(buf, 10);
    cout << "Take 10 bytes, returned take " << size  << " content : " << buf << endl;

    cout << "buffer size after append: " << buffer.Size() << endl;

    memset(buf, 0, sizeof(buf));
    size = buffer.Get(buf, 300);
    cout << "Get 300 bytes, returned get " << size  << " content : " << buf << endl;

    cout << "buffer size after append: " << buffer.Size() << endl;
}

int main(int argc, char **argv)
{
    cout << "test append/get " << endl;
    test_get();

    cout << endl << endl;
    cout << "test append/get " << endl;
    test_take();

    cout << endl << endl;
    cout << "test append/get/take " << endl;
    test_append_get_and_take();
    return 0;
}

#endif
