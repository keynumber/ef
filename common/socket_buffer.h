/*
 * Author: number
 * Date  : Nov 18, 2015
 */

#ifndef __COMMON_SOCKET_BUFFER_H_H___
#define __COMMON_SOCKET_BUFFER_H_H___

#include <cstdint>
#include <list>

#define DEFAULT_BUFFER_BLOCK_SIZE 1024

namespace ef {

class MemPool;

class SocketBuffer {
public:
    SocketBuffer(MemPool *pool, uint32_t block_size = DEFAULT_BUFFER_BLOCK_SIZE);
    virtual ~SocketBuffer();

    uint32_t Size() const;
    void Clear();
    bool Append(const char *buf, uint32_t len);
    uint32_t Get(char *buf, uint32_t len) const;
    uint32_t Take(char *buf, uint32_t len);
    int SendToSocket(int sock_fd, std::string * errmsg = nullptr);

private:
    struct Block {
        char * buf = nullptr;
        uint32_t read_index = 0;
        uint32_t write_index = 0;
    };

private:
    std::list<Block> _buf_list;
    uint32_t _block_size;
    uint32_t _buf_size;

    MemPool *_mem_pool;
};

} /* namespace ef */

#endif /* __COMMON_SOCKET_BUFFER_H__ */
