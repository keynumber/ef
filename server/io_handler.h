/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#ifndef __SERVER_IO_HANDLER_H_H___
#define __SERVER_IO_HANDLER_H_H___

#include <unordered_set>

#include "common/poller.h"

namespace ef {

class IoHandler {
public:
    IoHandler();
    virtual ~IoHandler();

    bool Initialize();
    bool HandleAccept();    // 外部client连接请求
    bool HanderClient();    // client读写请求
    bool HanderWorker();

    IoHandler(const IoHandler &) = delete;
    IoHandler(const IoHandler &&) = delete;
    IoHandler operator = (const IoHandler &) = delete;
    IoHandler operator = (const IoHandler &&) = delete;

private:
    Poller _poller;
    MemPool *pool;
    std::unordered_set<int>  *_fd_set;   // 判断文件描述符是否文件描述符
    FdInfo * _client_fd;                 // client fd

    char *buf;
};

} /* namespace ef */

#endif /* __SERVER_IO_HANDLER_H__ */
